#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    Program* programa = new Program();
    programa->cuerpo.push_back(parseStm());
    while(match(Token::SEMICOLON)){
        programa->cuerpo.push_back(parseStm());
    }
    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Stm* Parser::parseStm(){
    if (match(Token::ID))
    {
        AssignStatement* stm = new AssignStatement();
        stm->variable = previous->text;
        match(Token::ASSIGN);
        stm->valor =  parseCE();
        return stm;
    }
    else if (match(Token::PRINT))
    {
        PrintStatement* stm = new PrintStatement();
        match(Token::LPAREN);
        stm->valor =  parseCE();
        match(Token::RPAREN);

        return stm;
    }
    else if (match(Token::SWITCH))
    {
        SwitchStatement* stm = new SwitchStatement();
        stm->condition = parseCE();
        while (check(Token::CASE)) {
            stm->cases.push_back(parseCase());
        }
        if (check(Token::DEFAULT)) {
            stm->defaultCase = parseDefaultCase();
        }
        match(Token::ENDSWITCH);
        return stm;
    }
    else if (match(Token::IF))
    {
        IfStatement* stm = new IfStatement();
        stm->condition = parseCE();
        match(Token::THEN);
        stm->thenBody = parseBody();

        IfStatement* actual = stm;
        while (match(Token::ELIF)) {
            IfStatement* rama = new IfStatement();
            rama->condition = parseCE();
            match(Token::THEN);
            rama->thenBody = parseBody();
            actual->elseIf = rama;
            actual = rama;
        }
        if (match(Token::ELSE)) {
            actual->elseBody = parseBody();
        }
        match(Token::ENDIF);
        return stm;
    }
    else if (match(Token::DO))
    {
        DoWhileStatement* stm = new DoWhileStatement();
        stm->body = parseBody();
        match(Token::WHILE);
        stm->condition = parseCE();
        return stm;
    }
    else if (match(Token::WHILE))
    {
        WhileStatement* stm = new WhileStatement();
        stm->condition = parseCE();
        match(Token::DO);
        stm->body = parseBody();
        match(Token::ENDWHILE);
        return stm;
    }
    else if (match(Token::BREAK))
    {
        return new BreakStatement();
    }

    else {
        throw runtime_error("Error sintáctico");
    }
}

Case* Parser::parseCase() {
    Case* c = new Case();
    match(Token::CASE);
    c->valor = parseCE();
    c->cuerpo = parseBody();
    return c;
}

Case* Parser::parseDefaultCase() {
    Case* c = new Case();
    match(Token::DEFAULT);
    c->cuerpo = parseBody();
    return c;
}

list<Stm*> Parser::parseBody() {
    list<Stm*> body;
    body.push_back(parseStm());
    while (match(Token::SEMICOLON)) {
        body.push_back(parseStm());
    }
    return body;
}

Exp* Parser::parseCE() {
    Exp* l = parseAndExp();
    while (match(Token::OR)) {
        Exp* r = parseAndExp();
        l = new BinaryExp(l, r, OR_OP);
    }
    return l;
}

Exp* Parser::parseAndExp() {
    Exp* l = parseRelExp();
    while (match(Token::AND)) {
        Exp* r = parseRelExp();
        l = new BinaryExp(l, r, AND_OP);
    }
    return l;
}

Exp* Parser::parseRelExp() {
    Exp* l = parseExpr();
    while (match(Token::LT) || match(Token::GT) || match(Token::LE) ||
           match(Token::GE) || match(Token::EQ) || match(Token::NEQ)) {
        BinaryOp op;
        switch (previous->type) {
            case Token::LT:  op = LT_OP;  break;
            case Token::GT:  op = GT_OP;  break;
            case Token::LE:  op = LE_OP;  break;
            case Token::GE:  op = GE_OP;  break;
            case Token::EQ:  op = EQ_OP;  break;
            default:         op = NEQ_OP; break;
        }
        Exp* r = parseExpr();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseExpr() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}



Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}


Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::TRUE)) {
        return new BoolExp(true);
    }
    else if (match(Token::FALSE)) {
        return new BoolExp(false);
    }
    else if (match(Token::ID)) {
        string va  = previous->text;
        return new IdExp(va);
    }
    else if (match(Token::LPAREN))
    {
        e = parseCE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}
