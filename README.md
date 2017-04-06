# petit-compiler
PLD-COMP H4414

# Building
All object files can be found in `obj`, and binaries (i.e. tests and the compiler) in `bin`.


To build the compiler use `make`.
To run tests use `make tests`.


# Usage
```
   Usage:
       ./c <options> <file>

   Options:
       -a          Static analysis.
       -o          Enable optimisation. <NOT IMPLEMENTED>
       -c          Compile to <file>.asm, or the path specified by `--outfile` if given.
       --outfile   Specify the output path for compilation.
       -t          Generate a Graphviz dot graph of the AST in <file>.dot, or the path specified by `--astfile` if given.
       --astfile   Specify the output path for AST graph
```
