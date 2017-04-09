# petit-compiler
PLD-COMP H4414

# Building
All object files can be found in `obj`, and binaries (i.e. tests and the compiler) in `bin`.


To build the compiler use `make`.
To run tests use `make tests`. Please note that these currently only test the front-end (up to generation of the AST).


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

# Implemented features
## Syntax analysis

+ Function declarations
+ Function definitions
+ Global variable declaration/definitions
+ Mixed variable definitions and declarations (`char a, b = 'b', c, d ...`)
+ Array declarations
+ Function calls
+ All C binary operators except bitwise operators
+ All C unary operators
+ Flow control: if, while, for
+ Nested blocks

## Semantic analysis

+ Scope verification
    + Redefinitions
	+ Use without declaring
	+ Allows identically named variables in nested scopes
+ Type checking in all expressions
+ Return statement verification
	+ Missing return
	+ Return wrong type

## Assembly generation

+ Basic arithmetic expressions
+ Return from function (with offset problems)

# Unimplemented features

+ Forward declarations of functions
+ Array semantics
+ Optimisation
+ Assembly generation
	+ Function calls
	+ Flow control
	+ Correct type sizes and alignment
