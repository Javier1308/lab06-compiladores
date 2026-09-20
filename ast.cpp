#include "ast.h"
#include <iostream>

using namespace std;

// ------------------ Exp ------------------
Exp::~Exp() {}
Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        case LT_OP:    return "<";
        case GT_OP:    return ">";
        case LE_OP:    return "<=";
        case GE_OP:    return ">=";
        case EQ_OP:    return "==";
        case NEQ_OP:   return "!=";
        case AND_OP:   return "and";
        case OR_OP:    return "or";
        default:       return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}


BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}

// ------------------ BoolExp ------------------
BoolExp::BoolExp(bool v) : value(v) {}

BoolExp::~BoolExp() {}


// ------------------ SqrtExp ------------------
SqrtExp::SqrtExp(Exp* v) : value(v) {}

SqrtExp::~SqrtExp() {}

IdExp::IdExp(string v) : value(v) {}

IdExp::~IdExp() {}
