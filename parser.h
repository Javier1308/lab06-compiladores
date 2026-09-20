#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"    // Incluye la definición del escáner (provee tokens al parser)
#include "ast.h"        // Incluye las definiciones para construir el Árbol de Sintaxis Abstracta (AST)

class Parser {
private:
    Scanner* scanner;       // Puntero al escáner, de donde se leen los tokens
    Token *current, *previous; // Punteros al token actual y al anterior
    bool match(Token::Type ttype);   // Verifica si el token actual coincide con un tipo esperado y avanza si es así
    bool check(Token::Type ttype);   // Comprueba si el token actual es de cierto tipo, sin avanzar
    bool advance();                  // Avanza al siguiente token
    bool isAtEnd();                  // Comprueba si ya se llegó al final de la entrada
public:
    Parser(Scanner* scanner);
    Program* parseProgram();             // Punto de entrada: analiza un programa completo
    Stm* parseStm();
    list<Stm*> parseBody();
    Case* parseCase();
    Case* parseDefaultCase();
    Exp* parseCE();                   // Regla gramatical CExp / LOrExp
    Exp* parseAndExp();               // Regla gramatical LAndExp
    Exp* parseRelExp();               // Regla gramatical RelExp
    Exp* parseExpr();                 // Regla gramatical Expr
    Exp* parseE();                    // Regla gramatical Term
    Exp* parseT();                    // Regla gramatical PowExp
    Exp* parseF();                    // Regla gramatical Factor
};

#endif // PARSER_H
