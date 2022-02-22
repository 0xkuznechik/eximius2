#Overview
*"An SSTable is a simple abstraction to efficiently store large numbers of
key-value pairs while optimizing for high throughput, sequential read/write
workloads."*

In its simplest form, my implementation of the SSTable will be two basic data
structures:
* an index dictionary (k=key, v=offset)
* actual SSTable file with a sorted set of key-value pairs written to an
immutable blob.

![](https://www.igvita.com/posts/12/xsstable.png.pagespeed.ic.IkMoqaKZX9.webp)

Keys are sorted when they are processed via i/o. This makes algorithms on sorted
arrays available to the user.

#Design Doc
## Overview
I aim to implement an intentionally reductive and overly simplified version of
the SSTable data model.

Data is streamed to the SSTable API, which listens and sorts inputs in-memory.
Once the stream is closed by the client, SSTable will write the in-memory data
into disk and finalize the SSTable file interface for future client reads.

Optimizing the behavior underneath this basic interface will be done in stages.
Getting the basic abstraction working is most important; then, implementing
known methods of improving read/write (e.g. compaction) will be done later.

### Background
### Requirements

* User can call SSTable API, provide a stream of data (or a batch), and expect
an .sstable file. Options must be provided.
* .sstable files can be loaded and read using the SSTable API. Clients are able
to look up a key and expect the value returned.

### Out-of-scope

## Design Ideas
### Workflow
Client creates an SSTable object, (optionally) passes in filename and delimiter.
	`SSTable new_sstable("myfile");`

Calls Run() or Run(const void* data).
	`new_sstable.Run()`
	Run() will read the file or start processing the data stream.
	Delimits Key-Value pairs; each record is separated by \n.
	When File or data stream hits null (\0), then completes.
	It will also update the output filename(s) for getter access.

Checks for the .sstable filename.
	`new_sstable.GetOutputFileName()`

### Technologies
### Interface
### Data Structure

For now, the primary data structure will be a std::map, where K and V are
defined in the data stream. The client can either define their own delimiter or
default to ",".

### Algorithms
### Alternatives Considered

## Implementation
### Memory

#HERE
- Data blocks are entries in sorted virtual page table.
- A regular background process goes through and organizes the data into truly
	contiugous physical memory entries (?)

### Timeline

**Basic User Interface**

Use standard C++ std::map data structure to build key-offset index.

The ordered blob will be represented as a sequence of chunks, composed of string
keys and raw bytes.

### Testing
### Security

## Resources
- [description of leveldb and sstables](https://www.igvita.com/2012/02/06/sstable-and-log-structured-storage-leveldb/)

## Notes
Will probably need to use mmap for better efficiency.
