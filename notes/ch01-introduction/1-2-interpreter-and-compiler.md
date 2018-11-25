# 1.2. Interpreter and Compiler

## Compiler

> “A compiler  is  a computer program  which  translates programs written in  a particular high-level programming language  into executable code for a specific target computer.”

* Source Programs -> **COMPILER** -> Object Programs (Assembly Language, Machine Language)

e.g. C compiler on SPARC (Scalable Processor Architecture)

> C compiler for SPARC gets C program codes and generates executable-code for the SPARC machine

## Compiler Structure

* "Source Programs" -> **Front-end** -> "Intermediate Code" -> **Back-end** -> "Object Programs"
  * Front-end: language dependent part
  * Back-end: machine dependent part

## Cross-Compiler

“A cross-compiler is  a  program  which  is  to run  on machine A and produce target code for another machine B.”

* "Source Programs" -> **Compiler on Machine A** -> "Target Code for Machine B"

* Execution : down-loading or interpretation
* Bootstrapping

## Interpreter

“An interpreter transforms a program directly into a sequence of machine actions and produces the results of the program.”

* "Source Programs" -> **Interpreter**
  * data -> **Interpreter** -> result

* Compiler : Operational System
* Interpreter : Developing System or Educational System

## Preprocessor

for the language extension

* "Source Programs" -> **Preprocessor** -> "**Extended** Source Programs" -> Translator -> "Target Programs"

* Macro substitution
* Conditional compilation
* Inclusion of files

## The Structure of Conventional Compiler


* Source Program -> **Lexical Analyzer** -> Token
  * Token -> **Syntax Analyzer** -> Tree
    * Tree -> **ICG (Intermediate Code Generator)** -> IC (Intermedicate Code)
      * IC -> **Code Optimiser** -> Optimised Code
        * Optimised Code -> **Target Code Generator** -> Object Programs

## 1. Lexical Analyzer (Scanner)

Convert source program to integers which are easy and efficient for handling in the compiler

* "Source Programs" -> **Lexical Analyzer** -> "A Sequence of **Tokens**"

e.g. a simple code

```c
if (a > 10) ...
```

the Lexical Analyzer will do:

```note
Token        : if   (   a   >   10   ) 
Token Number : 32   7   4   25  5    8
```

so `if`, `(`, `a`, `>`, `18`, `)` will be `32`, `7`, `4`, `25`, `5`, `8`, respectively and these numbers are called as **Token Number**

## 2. Syntax Analzer (Parser)

* Syntax Checking, Tree Generation

"A Sequence of Tokens" -> **Syntax Analzer (Parser)** -> "Tree (error messages OR syntatic structure)"

* OUTPUT (of the Parser)
  * Incorrect: ignite error messages
  * Correct: generate a program structure (tree-shaped)

e.g. a program structure

the simple code below:

```c
if (a > 10) a = 1;
```

if there is no error, then the scanner will create a tree-shaped program structure

```note
        if
      /     \
    >         =
  /   \      /  \
a      10   a     1
```

## 3. Intermediate Code Generator

* **Semantic Checking**
* ICG (Intermediate Code Generator)

e.g. if `a` is `int` type, we get a **semantic error**

```c
if (a > 10) a = 1.0;
```

e.g. a simple code with the **tree** and `Ucode`

```c
a = b + 1
```

Tree: 

```note
    =
  /   \
a       +
      /   \
    b       1
```

Ucode: - variable reference: (base, offset)

```ucode
lod   1 2
ldc   1
add
str   1 1
```

## 4. Code Optimizer

* **Optional Phase**
* Find inefficient code and update the code as more efficient code
* Meaning of optimization
  * major part: improve runnning time
  * minor part: reduce code size
  * example

    ```note
    LDC   R1, 1
    LDC   R1, 1   // (X)
    ```

* Criteria of Optimization
  * preserve the meanings of programs
  * speed up on average
  * be worth for the effort

* Local optimization
  * through _local inspection_, find out "inefficient code" and update the code as "more efficient code"

  1. constant folding
  2. eliminating redundant load, store instruction (see the above example)
  3. algebraic simplification
  4. strength reduction

* Global optimization
  * employ flow analysis technique
    1. common subexpression
    2. moving loop invariants
    3. removing unreachable codes

## 5. Target Code Generator

* Generate "Machine Instruction" from "IC"

* "IC" -> **Target Code Generator** -> "Target Code"

* Code generation tasks
  1. instruction selection and generation
  2. register management
  3. storage allocation
  4. code optimization (machine-dependant optimization)

## 6. Error Recovery

* Error Recovery
  * Error Recovery: fixing errors with affecting other sentences
  * Error Repair: recovering when error ignited

* Error handling
  * Error detection
  * Error recovery
  * Error reporting
  * Error repair

* Errors
  * Syntax Error
  * Semantic Error
  * Run-time Error






