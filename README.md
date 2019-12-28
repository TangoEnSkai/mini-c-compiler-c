# Mini-C Compiler Construction in C

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

### A MiniC Program to Compute the Cosine Function

```c
int main ()
{
  float cos, x, n, term, eps, alt;
  // compute the cosine of x to within tolerance eps
  // use an alternating series

  x = 3.14159;
  eps = 0.1;
  n = 1;
  cos = 1;
  term = 1;
  alt = -1;
  
  while (term>eps)
  {
    term = term * x * x / n / (n+1);
    cos = cos + alt * term;
    alt = -alt;
    n = n + 2;
  }
}
```
