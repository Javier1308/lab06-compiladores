# lab06-compiladores

## Gramática Actualizada

```
Program      ::= StmtList

StmtList     ::= Stmt { ';' Stmt }*

Body         ::= Stmt { ';' Stmt }*

Stmt         ::= Id '=' CExp
               | 'print' '(' CExp ')'
               | 'if' CExp 'then' Body { 'elif' CExp 'then' Body }* [ 'else' Body ] 'endif'
               | 'do' Body 'while' CExp
               | 'while' CExp 'do' Body 'endwhile'
               | 'switch' CExp { 'case' CExp Body [ 'break' ] }* [ 'default' Body ] 'endswitch'
               | 'break'

CExp         ::= LOrExp

LOrExp       ::= LAndExp { 'or' LAndExp }*

LAndExp      ::= RelExp { 'and' RelExp }*

RelExp       ::= Expr { ('<' | '>' | '<=' | '>=' | '==' | '!=') Expr }*

Expr         ::= Term { ('+' | '-') Term }*

Term         ::= PowExp { ('*' | '/') PowExp }*

PowExp       ::= Factor [ '**' Factor ]

Factor       ::= Number
               | 'true'
               | 'false'
               | '(' CExp ')'
               | 'sqrt' '(' CExp ')'
               | Id
```

## Estructura del proyecto

```
scanner.h/.cpp     Analizador léxico (reconocimiento de palabras clave, operadores y literales)
token.h/.cpp       Definición de tipos de Token y sobrecarga de operadores
parser.h/.cpp      Analizador sintáctico descendente recursivo con precedencia
ast.h/.cpp         Jerarquía de clases del AST (Expresiones, Sentencias, Switch, etc.)
visitor.h/.cpp     Patrón Visitor: PrintVisitor (código reconstruido) y EVALVisitor (intérprete)
main.cpp           Punto de entrada: escaneo, parseo, impresión y evaluación
inputs/            10 programas de prueba con switch, booleanos, operadores lógicos y relacionales
outputs/           Tokens y salidas de ejecución (.txt)
run_all_inputs.py  Script automatizado de compilación y ejecución de todos los inputs
```

## Compilación y ejecución manual

```bash
g++ main.cpp scanner.cpp token.cpp parser.cpp ast.cpp visitor.cpp -o a.out
./a.out inputs/input1.txt
```

## Ejecución de todas las pruebas

Para compilar y ejecutar automáticamente los 10 inputs de prueba, generando salidas y listados de tokens:

```bash
python3 run_all_inputs.py
```
