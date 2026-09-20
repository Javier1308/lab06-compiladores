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
        if (isAtEnd()) break;
        programa->cuerpo.push_back(parseStm());
    }
    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Body* Parser::parseBody(){
    Body* body = new Body();
    body->list_stm.push_back(parseStm());
    while(match(Token::SEMICOLON)){
        if (check(Token::ENDIF) || check(Token::ELIF) || check(Token::ELSE) || check(Token::WHILE) || check(Token::ENDWHILE)
            || check(Token::CASE) || check(Token::DEFAULT) || check(Token::ENDSWITCH)) {
            break;
        }
        body->list_stm.push_back(parseStm());
    }
    return body;
}

Case* Parser::parseCase() {
    match(Token::CASE);
    Exp* valor = parseCExp();
    Body* cuerpo = parseBody();
    return new Case(valor, cuerpo);
}

Stm* Parser::parseStm(){
    if (match(Token::ID))
    {
        AssignStatement* stm = new AssignStatement();
        stm->variable = previous->text;
        match(Token::ASSIGN);
        stm->valor =  parseCExp();
        return stm;
    }
    else if (match(Token::PRINT))
    {
        PrintStatement* stm = new PrintStatement();
        match(Token::LPAREN);
        stm->valor =  parseCExp();
        match(Token::RPAREN);

        return stm;
    }
    else if (match(Token::BREAK))
    {
        return new BreakStatement();
    }
    else if (match(Token::IF))
    {
        IfStatement* stm = new IfStatement();
        stm->condition = parseCExp();
        if (!match(Token::THEN)) {
            throw runtime_error("Error sintáctico: se esperaba 'then'");
        }
        stm->ifbody = parseBody();
        while (match(Token::ELIF)) {
            Exp* elifCond = parseCExp();
            if (!match(Token::THEN)) {
                throw runtime_error("Error sintáctico: se esperaba 'then'");
            }
            Body* elifBody = parseBody();
            stm->elif_list.push_back(new ElifStatement(elifCond, elifBody));
        }
        if (match(Token::ELSE)) {
            stm->elsebody = parseBody();
        }
        if (!match(Token::ENDIF)) {
            throw runtime_error("Error sintáctico: se esperaba 'endif'");
        }
        return stm;
    }
    else if (match(Token::DO))
    {
        DoWhileStatement* stm = new DoWhileStatement();
        stm->body = parseBody();
        if (!match(Token::WHILE)) {
            throw runtime_error("Error sintáctico: se esperaba 'while'");
        }
        stm->condition = parseCExp();
        return stm;
    }
    else if (match(Token::WHILE))
    {
        WhileStatement* stm = new WhileStatement();
        stm->condition = parseCExp();
        if (!match(Token::DO)) {
            throw runtime_error("Error sintáctico: se esperaba 'do'");
        }
        stm->body = parseBody();
        if (!match(Token::ENDWHILE)) {
            throw runtime_error("Error sintáctico: se esperaba 'endwhile'");
        }
        return stm;
    }
    else if (match(Token::SWITCH))
    {
        SwitchStatement* stm = new SwitchStatement();
        stm->condition = parseCExp();
        while (check(Token::CASE)) {
            stm->cases.push_back(parseCase());
        }
        if (match(Token::DEFAULT)) {
            stm->defaultBody = parseBody();
        }
        if (!match(Token::ENDSWITCH)) {
            throw runtime_error("Error sintáctico: se esperaba 'endswitch'");
        }
        return stm;
    }
    else {
        throw runtime_error("Error sintáctico");
    }

}

Exp* Parser::parseCExp() {
    Exp* l = parseLAndExp();
    while (match(Token::OR)) {
        Exp* r = parseLAndExp();
        l = new BinaryExp(l, r, OR_OP);
    }
    return l;
}

Exp* Parser::parseLAndExp() {
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
    Exp* l = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseTerm();
        l = new BinaryExp(l, r, op);
    }
    return l;
}



Exp* Parser::parseTerm() {
    Exp* l = parsePowExp();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parsePowExp();
        l = new BinaryExp(l, r, op);
    }
    return l;
}


Exp* Parser::parsePowExp() {
    Exp* l = parseFactor();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseFactor();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseFactor() {
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
    else if (match(Token::LPAREN))
    {
        e = parseCExp();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {
        match(Token::LPAREN);
        e = parseCExp();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else if (match(Token::ID)) {
        string va  = previous->text;
        return new IdExp(va);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}
