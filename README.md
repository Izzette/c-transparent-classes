# `libctc`: C Transparent Classes

Simple library and runtime simplifying and standardizing C object-oriented
patterns.

## Features:

### DONE:

* Classes and interfaces
  * Initializers and destructors.
* Type-polymorphism
* Runtime "is-a" relationships
* Multiple implementation-inheritance

### BACKLOG:

* Compile-time class and interface initialization.<br/>
  This library relies on, for performance and ease of writing, several managed
  attributes of class and interface structures.
  Unfortunately, this requires doing some setup at runtime before first-use or
  lazily at first-use.
  For extremely large projects this will result in unacceptable startup time.
  However, no-such projects as of yet exist.
  This must be implemented by a tool to re-write these attributes on binaries at
  compile-time.
  The implementation will be by necessity executable-format specific.
* Tests.<br/>
  Need I say more?

### WON'T DO:

* Class inheritance and abstract classes<br/>
  Class inheritance, and in-particular multiple class-inheritance produces
  complex intra-project dependency problems.
  Excluding these features avoids the diamond problem and others.
  Many of the advantages of class inheritance can be obtained using delegation
  patterns.
  Although delegation patterns are not explicitly codified in this library,
  they're easy to implement albeit requiring an unfortunate amount of
  boiler-plate code.
  These are neither simple to implement nor are they transparent, as such they
  are out of scope for this project.
* Fancy macro syntax<br/>
  The [cello project](http://libcello.org/) by
  [Daniel Holden](http://www.theorangeduck.com/) implements many-and-more of the
  features here, in particular some macro hacks to use syntax familiar to
  many programmers of higher-level languages.
  While I applaud Daniel Holden for this first-of-a-kind and extremely
  interesting project, some of the design choices hurt transparency and
  performance.
  The traits have been critical for C's apparent immortality and I have no
  desire weaken them with this project.
  For functional code, I will—wherever possible—utilise existing and
  well-known C syntax and programming patterns.
* Compile-time type checking<br/>
  I'm not sure this is possible, but if it is I expect it would be too
  complicated.

## Check out the [examples](examples/)

Included in the examples directory are two examples:

* `hello`: a simple Hello World program with unnecessary indirection.
* `list`: An example linked-list and re-sizable array implementation
   demonstrating type polymorphism.

## License

```text
c-transparent-classes  Copyright (C) 2020  Isabelle Erin COWAN-BERGMAN
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
