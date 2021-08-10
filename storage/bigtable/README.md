# Overview
Bigtable was a highly impactful paper and application within Google. My old
manager described it as the "crown jewel" of large, distributed db technologies.
It is still being used today in Google for production and mission critical
systems.

The paper itself is highly detailed and a pleasure to read. I am implementing my
own version of the system as an educational exercise.

# Design Doc
This is more or less my own regurgitation of the BigTable paper, with a few
implementation notes. It's for me to wrap my own mind around the details of the
system and how it may be implemented.

## Overview
### Background
### Requirements
### Out-of-scope

## Design Ideas

### Interface
The BigTable paper describes the basic API for the service. It will largely be
copied in my implementation.

[gcloud's BigTable service](https://cloud.google.com/bigtable/) also describes
a CLI tool for interacting with your BigTable.

**API**

Basic operations for initial POC:
* open a table
* define a row mutation, apply it (write) to table
* scan a table::row(::columnfamily::column)

These are purposefully very generic for now. Details can be found in the actual
implementation.

Note that reading a row is essentially scanning a single row.

**CLI**

The CLI should largely mirror the programmatic API's contract. However, it is
meant to be transactional, i.e. the user must perform an action on the table:
* read a row
* scan a table
* delete a row
* delete a table

Users are also able to filter and limit based on row values and column families.

Again, this is purposefully generic and reductive. I'm aiming for a basic poc
with basic functionality.

### Architecture
As described in the paper, BigTable has three main components:
* client library
* single master server
* tablet servers

The master and tablet servers make up an entire BigTable cluster.

#### Master server
Responsibilities:
* assign tablets to tablet servers
* detect addition and expiration of tablet servers
* load balancer for tablet server
* gc of gfs files
* handles schema changes (e.g. table and column family creations)

In short, the master server tracks all tablets and tablet servers. It makes
sure that unassigned tablets have a tablet server. The master relies on Chubby
to track tablet servers and their locations. It enforces this scheme by
regularly pings each tablet server for the status of its lock on an assigned
Chubby file. If the lock has been released, the master finds an unassigned
tablet server and assigns it to the Chubby file. If the master cannot reach the
tablet server, then the master assumes:
* the tablet server is down;
* the tablet server cannot communicate with Chubby;
* the master cannot communicate with the tablet server;
* Chubby itself is down.

The master can test these assumptions by acquiring a Chubby lock on the file in
question. If it can acquire a lock, it assumes that the tablet server has issues
and simply releases its lock and re-assigns the Chubby file to an unassigned
tablet server. It deletes the problematic tablet from its server file to
ensure that the server won't come back alive and start serving again. It will
then re-assign tables from that tablet server to the new tablet server.

However, if it cannot acquire a lock and cannot contact the tablet server, more
troubleshooting may be necessary. In general, the master will kill itself if its
Chubby session expires. I may investigate this strategy as a means of checking
if this particular master has an issue that a new master may not have re: trying
to reach an unresponsive tablet server that ostensibly still has its Chubby
lock.

How does the master keep track of tablets, tablet servers, and tables?

When it starts up, the master must acquire a special lock (see Chubby section
below). It scans the Chubby directory for a list of tablet servers, and pings
each tablet server to acquire a list of assigned tablets. Concurrently, the
master asks Chubby for the root METADATA (see below) for a complete list of
tablets. Once it has a list of tablet servers and tablets, it diffs the two to
discover unassigned tablets. If a tablet is unassigned, it will assign it to
a tablet server.

#### Tablet servers
Responsibilities:
* process read/write requests to tablets
* split tablets that have become too big

Note that clients make direct requests to tablet servers. They should rarely
talk to the master.

#### Chubby
Chubby is BigTable's escrow service.

BigTable's Chubby directory looks like this:
.
|___ SpecialDirectoryName
    |___ MasterServerLock
    |___ TableServer1Lock
    |___ TableServer2Lock
    |___ TableServer3Lock
		...
    |___ TableServerNLock

A machine that wants to or is the master must acquire the MasterServerLock,
which is unique and ensures there is only one master server.

A machine that wants to be a specific tablet server must acquire that specific
lock. For example, if a machine wants to be TabletServer1, it needs to acquire
TabletServer1Lock.

The machine with MasterServerLock can scan the contents of SpecialDirectoryName
for a list of tablet servers.

#### METADATA table

A single BigTable table may span multiple tablets. When a client makes a
request, BigTable will have to look up the location of the tablets that compose
the client's table. BigTable uses a special METADATA table to accomplish this.

BigTable asks Chubby for the location of the METADATA table's root tablet. This
root table contains the locations of all tablets, which are themeslves METADATA
tablets. Each of these METADATA tablets contain the locations of all the user
tables. Each METADATA's row is structured as:
*TabletLocation*::*TableName*::*EndRowKey*

The *TableName*::*EndRowKey* is the unique identifier for each tablet.

In short, Bigtable constructs a three-level hierarchy:
* the root METADATA tablet/table (it is a single tablet) contains a list of all
tablets, which are uniquely identified by their location and
tablename-endrowkey
* the next level contain the locations of user tablets
* the final level are the user tables, potentially split across multiple tablets 

A client will keep the tablet location of its requested tables cached. It can
then look up the location, look up the table name, and perform a binary search
for the specific row or range of rows.

To illustrate this request flow: a client requests to read FooTable. If this is
the first request, it must now ask BigTable for the tablet location for
FooTable.

BigTable asks Chubby for the root METADATA table's location. It reads the
single METADATA tablet:

```
RootTabletLocation::METADATA::FinalTabletLocation
Tablet1Location::BazTable::BazTableM
Tablet4Location::GrokTable::GrokTableN
...
TabletNLocation::NTable::NTableKey
```
Since we use SSTables as the primary data store for all BigTable tables, we can
perform binary search to find the table name in the root METADATA. Let's
pretend FooTable is sandwiched between BazTable and GrokTable, so the root
METADATA row is: Tablet1Location::BazTable::BazTableM

BigTable goes to Tablet1Location. It reads the METADATA of the Tablet1:

```
BazTablet1Location::BazTable::BazRowX
BazTablet2Location::BazTable::BazRowY
BazTablet3Location::BazTable::BazRowZ
...
```

We can now read the contents of BazTable by looking up its respective tablet
locations and scanning or binary searching for a specific row. BigTable will
also return Tablet1Location, which is cached by the client. It can now perform
scans and writes by directly looking up the FooTable file at Tablet1Location. If
the tablet is no longer at Tablet1Location, it can perform the search again.

A few notes:

The root tablet is unique - it is never split. This is done to limit the
described hierarchy to three levels (root METADATA table -> other METADATA
tablets -> user tables). The first entry of the root METADATA table is the root
itself.

Each METADATA table row is 1 KiB and each METADATA tablet is 128 MiB. This
allows us to store 2e34 tablets. Each tablet is approximately 100-200 MiB.

### Operations
This section describes how BigTable records the changes in its underlying data.
Interestingly enough, BigTable seems to implement a slow changing data model
(or whatever it was called) - a concept my old manager introduced to me. Updates
and changes are logged, and with the net result of the updates netted out to
produce the state of a table. An obvious optimization is to archive the changes
and to compact the log to the current state of the data.

#### Write and MemTables
Clients pushing updates to BigTable talk to the tablet server. Updates are
stored in a sorted buffer, which BigTable refers to as a MemTable. Older updates
are stored in SSTables on disk.

This scheme allows for recovery. A tablet server that needs to recover a tablet
reads its METADATA table to locate the appropriate SSTables that make up the
missing tablet. These SSTables have pointers that contain commit logs, which the
table server can apply to recreate the latest state of the tablet.

For all intents and purposes, updates are essentially writes.

#### Scans
A client's scan request is directly made to a tablet server. Note: if the tablet
server can't be reached, BigTable will find the appropriate one, see process
above.

BigTable will merge the commit logs into a view. This response is sent back to
the client. Since the MemTable and SSTables are lexicographically sorted, the
merge should be efficient.

#### Compaction
When the MemTable reaches a certain size, it is frozen and written to disk as
an SSTable. This is defined by BigTable as a minor compaction.

BigTable will periodically read the MemTable and SSTables and perform a major
compaction - essentially merging all the data into a single, ostensibly slimmer
SSTable.

### Technologies
The actual BigTable system relies on the Google stack.

**filesystem**

The real BigTable used to depend on GFS.

Since I'm implementing
[my own version of this](https://github.com/chunkblob/gfs), I'll use that.

The fs will store log and data files.

**cluster management**

I'm separating this from the fs comments above, since I may try to use a local
fs as a poc for this BigTable.

Also, I am considering designing and writing a generic cluster management system
for fun/education as well.

**file format**

Google uses the SSTable file format. Google Cloud links to
[this description](https://www.igvita.com/2012/02/06/sstable-and-log-structured-storage-leveldb/).

**cli scaffolding**

A C++ framework for cleanly defining and implementing a CLI interface. I must
admit that I'll be taking prodigious inspiration from internal scaffolding I've
seen at my work.

**distributed lock system**

The paper describes BigTable's use of Chubby as a means of abstracting away
where the actual BigTable lives. For the first POC, I won't include this, as
the general BigTable abstraction and a working poc is more important for
educational purposes.

**testing**

GUnit will be used for unit testing.

### Data Structure

Conceptually, a BigTable is a multi-dimensional map.

**Map**

Primary data structure for BigTable.

Key=(row:string, column:string, time:int64)
Value=string

See below for details on keys.

**Row**

Key=64KiB string

Row read/write is atomic, regardless of columns being written or read.

**Column**

Grouped and defined by user.

Column family data are of the same type (note: not strictly the case in the
actual Bigtable paper, but this will make it easier for now).

Column indexes are organized underneath a family.

User must create and define a column family before using it. 

**Timestamp**

int64, can be assigned by BigTable (default), or by user.

**Value**

User-provided string.

However, various objects will be used as abstractions to implement this
conceptual data structure.

### Algorithms
### Alternatives Considered

## Implementation
### Timeline
A number of external dependencies exist for this deisign to be implemented as
described. They are described in Technologies (above).

TODO implementations:
* cli scaffolding tool
* sstable
* gfs
* chubby
* borg

These TODOs can be worked on concurrently. See my other github repos for their
respective statuses and implementations.

More importantly, the initial plan for this BigTable implementation, in stages:

**Setup**

1. Basic library objects and API interfaces defined.
1. Unit test file. All merges should have a fully functional set of unit tests.
These tests must pass for new changes and features implemented in the change.

**Interface**

1. Create, Delete, and Scan a Table.
1. Create, Delete, and Scan a Row.
1. Define and Update a ColumnFamily. Add and Remove a Column.

These are pure abstractions that simple and that work. Utilizing GFS, etc.,
which provides the reliability and performance that makes BigTable unique, will
be done at a later stage. Implementing the distributed system components will
also be done later.

Specifically, I will use the basic File API to store blobs of data on my local
disk/mem, with no redundancy or distribution.

This stage's goal is limited to getting the interface working. Detailed
implementation of some of BigTable's magic sauce, namely its use of distributed
technologies, will be done later.

**Architecture**

1. Implement METADATA, Tablet, TableServer, MasterServer

This can probably be concurrently implemented with the basic interface.

I may not have Chubby set up yet, so a mocked escrow service (probably just a
hacky interface with flags set up) will be used.

My goal here is to implement the parser for the BigTable rows, and have a
working mocked version of the distributed system components.

**CLI**

1. Implement BigTable CLI. (This may require a separate design doc.)

**Scaling and Reliability**

At this point, the user interface should be working. Ideally, there will be few
changes in the interface (API or CLI).

1. Migrate to sstable.

### Testing
### Security
The BigTable paper ties access on various levels. This is a TODO for the future,
as this system is NOT intended to be used in production.

## Resources
- [Bigtable
  paper](https://static.googleusercontent.com/media/research.google.com/en//archive/bigtable-osdi06.pdf)

## Notes
