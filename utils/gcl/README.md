# Overview
A simplified version of Google's GCL.

# Design Doc
## Overview
An interpreter for an uber-simplified version of GCL. This package reads and
parses a .gcl file, interpreting it into a compiled object API for programmatic
use and manipulation by the user for their own application.

### Background
A general config language with stricter syntax and grammar (vs a simple JSON or
even XML) is generically useful for defining and manipulating data using common
data structures and patterns. For example, being able to define lists and
maps/dictionaries in a config file, and being able to interpret that file into
an object with the usual behaviors (e.g. lists having iterators) normalizes the
interface pattern for any application using GCL.

### Requirements
- Data structures to be implemented:
    - Simple values (e.g. "10", "x").
    - Lists (e.g. [1,2,3]).
    - Tuples (e.g. { a = 1 b = 2 }); essentially a dictionary.
- Unit tests that demonstrate and prove promised interpretation of GCL.
- For now: a C++ object class as the API output for users. Future languages may
  be used, but for now I'm focusing on C++. The output will be read only.

### Out-of-scope
- Does **not** take a provided object and reverse-interpret it into a .gcl
  config file. While this is possible and a good candidate for future work, I'm
  limiting the scope for v0.1 to a workable version.
- Lambda functions. 
- Imports.
- Asserts.
- Any data structure or behavior not mentioned in "Requirements", above.

## Design Ideas
This is admittedly an educational exercise for me to implement C++ best
practices, a la the Effective C++ series, and design patterns from GoF.

What this library does:
- Reads a .gcl file and tokenizes it.
- With the tokens, creates an abstract syntax tree (AST).
- With the AST, returns a C++ class with a clearly defined API.

For reading a file and tokenizing, I plan to follow most of Bob Nystrom's
excellent [Crafting
Interpreters](https://craftinginterpreters.com/scanning.html#lexemes-and-tokens)
describes.

Creating a complex AST is most likely unnecessary for our simple GCL
implementation. Our most sophisticated use case would be nested tuples and
lists. Regardless, the Interpreter pattern (GoF) and [Crafting
Interpreters](https://craftinginterpreters.com/representing-code.html) offer
fine patterns for creating our AST. Tokens will be recursively matched to
specific patterns (details below), eventually returning an AbstractSyntaxTree
object our library can work with to reliably create our final API object.

A variety of Creational Patterns (GoF) will be used to build our final API
object. Specifically, I plan to use:
- Factory Method: Define specific types of objects; let subclasses (Prototype,
  below) instantiate.
- Prototype: Create specific types of objects and define their API interfaces;
  these will be cloned for each instance of the parsed objects.
- Builder: Used to construct each Prototype.
- Singleton: Only one copy of the final API object should exist.

Note that I have yet to thoroughly read these chapters. I'm proposing them now
as a placeholder.

### Technologies
This is intended to be as simple of a library of possible. As a result, the only
technology requirements for the library will be C++ and a build library
(Makefile). Build binaries will be published as a command line tool for creating
a compiled C++ object that can be linked for the user's purposes. Alternatively,
the source can be copied or referenced, and used as a part of the user's own C++
library or binary package, for their own application.

### Workflow
#TODO(blahzy)

### Data Structure
#TODO(blahzy)
The final C++ class will have some standard API getters (no setters).

### Algorithms
#TODO(blahzy)

### Alternatives Considered
Any host of semi-structured data types or popular config languages work pretty
well. Both JSON or XML have robust libraries available for a variety of
languages. However, JSON does not have a native tuple concept. XML also tends to
be difficult to read and overly complex, visually, for my personal needs. The
mature Google GCL definition and implementation is (AFAIK) a Turing-complete
language, with lambdas, functions, inheritance, and asserts. While this
functionality is not intended to be implemented in this v0.1 of my GCL, having a
simple, ground up language makes this a possibility. Trying to enrich JSON or
XML is certainly possible, but it seems like as much, if not less, work to just
implement a simple config language from the ground up.

For my own internal representation, or as a general structed data solution, I
would defer to Google Protocol Buffers (see my primary motivations and framework
at [my homepage](https://blahzy.github.io/). However, I explicitly want this
library to be independent and simple, so I eschewed using protos in favor of
having C++ data structures and classes do the job.

---
## Implementation
#TODO(blahzy)

### Timeline
#TODO(blahzy)

### Testing
#TODO(blahzy)
Given the atomicity of this library (i.e. I intend to keep it as self-sufficient
as possible), unit testing should focus on behavior.

**Reading the file**
- Basic input data, provided in a test.gcl file, can be read.
- Malicious input data is rejected. This input could be raw bytes or machine
  code.
- Overflow data is detected and rejected. For example, a file with an enormous
  size, or with too much nesting, will be rejected.

**Parsing the file**
- Basic tokens can be identified and represented.
- Nested tokens can be parsed.

**Interpreting the tokens into an AST**
- 

**AST can be converted to C++ class API**

### Security
The final output will be read only. No mutability will be explicitly provided
for the final C++ class.

Additionally, reading the input gcl file will be done as functionally as
possible. I intend to avoid storing it in memory any longer than is needed.

All data being read will also be checked for basic issues, such as overflow;
this should go without saying, but this best practice is worth explicitly
calling out in the design doc.

## Resources
- [GCL Viewer](https://pure.tue.nl/ws/portalfiles/portal/46927079/638953-1.pdf)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [GoF - Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns)

## Notes
This doc may change over time, since I'm not really getting any rigorous review
before staritng implementation. It's intended to be updated if I discover an
egregiously bad or incorrect design decision I've proposed. Ideally, once a
version of the tool is published, this doc will be archived and submitted as a
snapshot of this initial version's design.
