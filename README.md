# Mini-C-Compiler-Construction

Complier Construction for Mini C Programming Language

## MiniC

* `MiniC` is a simple **subset** of `the standard C language`
  * It does **NOT** include:
    * arrays
    * structs
    * unions
    * files
    * sets
    * switch statements
    * do statements
    * or or many of the low level operators
* The only data types permitted:
  * `int`
  * `float`
* A complete grammar for MiniC is shown below link
  * similar to the `yacc` grammar  
* Here we use the convention that symbols beginning with
  * upper-case letters:
    * **nonterminals**
  * and all other symbols:
    * **terminals** (i.e., lexical tokens)
* As in BNF, the vertical bar `|` to indicate _alternate definitions_ for a **nonterminal**

Here is the grammar of `MiniC` -> [click here!](mini_c.gr)

