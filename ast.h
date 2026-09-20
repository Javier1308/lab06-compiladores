#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

class Visitor;

// Operadores binarios soportados
enum BinaryOp {
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    POW_OP,
    LT_OP,
    GT_OP,
    LE_OP,
    GE_OP,
    EQ_OP,
    NEQ_OP,
    AND_OP,
    OR_OP
};

// Clase abstracta Exp
class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();

};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

// Literal booleano
class BoolExp : public Exp {
public:
    bool value;
    int accept(Visitor* visitor);
    BoolExp(bool v);
    ~BoolExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    int accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
};

class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

class Stm {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm{
public:
    string variable;
    Exp* valor;
    int  accept(Visitor* visitor);
    AssignStatement(){};
    ~AssignStatement(){};
}
;

class PrintStatement : public Stm{
public:
    Exp* valor;
    int  accept(Visitor* visitor);
    PrintStatement(){};
    ~PrintStatement(){};
}
;

class BreakStatement : public Stm {
public:
    int accept(Visitor* visitor);
    BreakStatement(){};
    ~BreakStatement(){};
};

class Body : public Stm {
public:
    list<Stm*> list_stm;
    int accept(Visitor* visitor);
    Body(){};
    ~Body(){};
};

class ElifStatement : public Stm {
public:
    Exp* condition;
    Body* elifbody;
    int accept(Visitor* visitor);
    ElifStatement(Exp* cond, Body* body);
    ~ElifStatement(){};
};

class IfStatement : public Stm {
public:
    Exp* condition;
    Body* ifbody;
    list<ElifStatement*> elif_list;
    Body* elsebody;
    int accept(Visitor* visitor);
    IfStatement();
    IfStatement(Exp* e);
    ~IfStatement(){};
};

class DoWhileStatement : public Stm {
public:
    Body* body;
    Exp* condition;
    int accept(Visitor* visitor);
    DoWhileStatement();
    DoWhileStatement(Body* b, Exp* c);
    ~DoWhileStatement(){};
};

class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* body;
    int accept(Visitor* visitor);
    WhileStatement();
    WhileStatement(Exp* c, Body* b);
    ~WhileStatement(){};
};

class Case {
public:
    Exp* valor;
    Body* cuerpo;
    Case(Exp* v, Body* b);
    ~Case(){};
};

class SwitchStatement : public Stm {
public:
    Exp* condition;
    list<Case*> cases;
    Body* defaultBody;
    int accept(Visitor* visitor);
    SwitchStatement();
    ~SwitchStatement(){};
};


class Program {
public:
    list<Stm*> cuerpo;
    Program(){};
    ~Program(){};
    int accept(Visitor* visitor);
}
;

#endif // AST_H
