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
```gist.
# Available features
- Syntax analysis
   Function declaration
   Function definition
   Global variables
   Arrays
   Function calls
   Binary operations
   Unary operations
   Multiple definitions (int a,b,c = ...)
   Conditional structures : if, while, for
   Nested blocks
   
- Semantic analysis
   Scope solving
   Variables and functions redefinitions
   Undefined variables and functions
   Mismatching arguments (number / type) for function calls
   Non compatible types for expressions
   
- Assembly generation
   Basic arithmetic expressions and return (offset problems)
   
# Features not covered
- Semantic analysis
   Forward declarations of functions
   Arrays
- Optimization
   No optimization at all
- Assembly generation
   Function calls
   Conditional structures
   Arrays
