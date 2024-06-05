# Custom Interpreted Language Written In C

## A fully functional interpreter for a custom programming language, written in C.

The code does the following:

* Receives a file containing C code

* Interprets line by line, keeping a realtime and updated stack of initialized variables and their values

* Computes binary operations, arithmetic operations, variable initializations, assignments, and reassignments

* Exits after all C code has been interpreted, allowing the user to see final values stored in variables initialized and reassigned over runtime

## Example and visual representation of abstract syntax tree 

An example would be: given the three lines of C code 'int y = 3+(5-7*3);\n int x = (y+8)*3;\n y = x+y;', the interpreter would end with the values -28, -15 stored in the variables y and x respectively.

The abstract syntax tree for 'int x = y + (z = x + y) - z;':

      [=]
      
      / \
      
     x   [-]
     
         / \
         
       [+]  z
       
       / \
       
      y   [=]
      
          / \
          
         z   [+]
         
             / \
             
            x   y

The context free grammer rules used:

A ::= C | F=C

C ::= B | C | B | C && B

B ::= E | E > E | E < E | E >= E | E <= E | E == E

E ::= T | E + F | E - F

T ::= F | T * F | T/F

F ::= <var> | <const> | (A)

