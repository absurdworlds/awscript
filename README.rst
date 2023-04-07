About
-----
This is a programming language that belongs to the curly-bracey family.
It is designed for expressiveness and simplicity.



Status
------
There is a roadmap:

1. Implement a basic subset that allows to write a self-hosting compiler.
2. Write a self-hosting compiler that implements the "full" language.
3. Implement a "safe" subset of the language.

Currently it is at the stage 1 of the roadmap.

Building
--------

Prerequisites
=============

To build, you'll need the latest versions of awlib_ (no version) and LLVM_ (version 16.0.0 as of writing),
C++20-capable compiler and CMake 3.19+.

Instructions
============

1. Build awlib:``
git clone git@github.com:absurdworlds/awlib.git
cmake -DCMAKE_INSTALL_PREFIX=aw_prefix -S awlib -B build-awlib
cmake --install build-awlib
``

2. Build LLVM (skip this step if you have recent enough version of LLVM provided by your system):``
git clone git@github.com:llvm/llvm-project.git
cmake -DCMAKE_INSTALL_PREFIX=aw_prefix -S llvm-project/llvm -B build-llvm
cmake --install build-llvm
``

3. Build awscript.``
# Add -DAWSCRIPT_USE_SYSTEM_LLVM to use the system-provided LLVM.
cmake -DCMAKE_PREFIX_PATH=aw_prefix -S awlang -B build-awlang
cmake --build build-awlang
``

.. _awlib: https://github.com/absurdworlds/awlib
.. _LLVM: https://llvm-project.org


Overview
--------

Syntax
======
As previously mentioned, the language uses curly braces to indicate blocks of code and other structures.

The language doesn't have any reserved keywords[*]_, all keywords are context-specific.
Writing an example and want to name your class a `class`? Sure!
Want to name your variable `if`? Sure, just remember that it might make your code harder to read and use.

.. [*] Subject to change. Some keywords may confuse readers of the code when used as identifiers, and/or hard to use. E.g. `true` and `false`.