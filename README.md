# Compiler with C\C++ using flex&bison
Compiler (Front End - convert our programming language(source code) to three address code(IR CODE) ).

The part of the compiler which convert source code to an Intermediate code/representation (IR).
A three-address code compiler using AST for Mr.
Gadi Pessah's Compilation course @ Afeka College

* Examples of inputs-outputs in input&output_examples folder.
* Made in Ubuntu 18.04 and gcc conpiler, by flex&bison tools.

It recognizes the following syntax commands:
'+'
'-'
'*'
'/'
'<'
'>'
'<='
'>='
'=='
'!='
'('
')'
'='
';'
':'
'{'
'}'
'if'
'else'
'while'
'int'
'float'
'or'
'and'
'not'
'=>'
'switch'
'case'
'default'
'break'
'read'
'do'



Authors:
* Gadi Pessah - Initial work
* Shahar Hikri - Updates according to requests
* Omer Ratsabi - Updates according to requests

- Info about our updates (in Hebrew) in file: AST_exercise_2021B.doc (it was a task for compilation course).

after after compilation (by 'make') - run 'myprog input_src_code_file_name'
