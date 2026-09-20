#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"


using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BreakStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SwitchStatement::accept(Visitor* visitor) {
    return visitor-> visit(this);
}

int IfStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int DoWhileStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int WhileStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}


///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    cout << (exp->value ? "true" : "false");
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}

int PrintVisitor::visit(Program* p) {
    cout << "PROGRAMA" << endl;
    for (auto i: p->cuerpo)
    {
        i->accept(this);
    }

    return 0;
}

int PrintVisitor::visit(PrintStatement* p) {
    cout << "print(" ;
    p->valor->accept(this);
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(AssignStatement* p) {
    cout << p->variable << "=";
    p->valor->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(BreakStatement* p) {
    cout << "break" << endl;
    return 0;
}

int PrintVisitor::visit(SwitchStatement* p) {
    cout << "switch ";
    p->condition->accept(this);
    cout << endl;
    for (auto c : p->cases) {
        cout << "case ";
        c->valor->accept(this);
        cout << endl;
        for (auto s : c->cuerpo) s->accept(this);
    }
    if (p->defaultCase) {
        cout << "default" << endl;
        for (auto s : p->defaultCase->cuerpo) s->accept(this);
    }
    cout << "endswitch" << endl;
    return 0;
}

int PrintVisitor::visit(IfStatement* p) {
    cout << "if ";
    p->condition->accept(this);
    cout << " then" << endl;
    for (auto s : p->thenBody) s->accept(this);
    if (p->elseIf) {
        cout << "elif" << endl;
        p->elseIf->accept(this);
    } else if (!p->elseBody.empty()) {
        cout << "else" << endl;
        for (auto s : p->elseBody) s->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(DoWhileStatement* p) {
    cout << "do" << endl;
    for (auto s : p->body) s->accept(this);
    cout << "while ";
    p->condition->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(WhileStatement* p) {
    cout << "while ";
    p->condition->accept(this);
    cout << " do" << endl;
    for (auto s : p->body) s->accept(this);
    cout << "endwhile" << endl;
    return 0;
}

int PrintVisitor::visit(IdExp* p) {
    cout << p->value ;
    return 0;
}



void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl;
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        case LT_OP:
            result = v1 < v2;
            break;
        case GT_OP:
            result = v1 > v2;
            break;
        case LE_OP:
            result = v1 <= v2;
            break;
        case GE_OP:
            result = v1 >= v2;
            break;
        case EQ_OP:
            result = v1 == v2;
            break;
        case NEQ_OP:
            result = v1 != v2;
            break;
        case AND_OP:
            result = v1 && v2;
            break;
        case OR_OP:
            result = v1 || v2;
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(BoolExp* exp) {
    return exp->value ? 1 : 0;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
}

void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:" << endl;
        programa->accept(this);
    }
    return;

}

int EVALVisitor::visit(Program* p) {
    for (auto i:p->cuerpo)
    {
       i->accept(this);
    }

    return 0;
}

int EVALVisitor::visit(PrintStatement* p) {
    cout << p->valor->accept(this) << endl;
    return 0;
}

int EVALVisitor::visit(AssignStatement* p) {
    memoria[p->variable] = p->valor->accept(this);
    return 0;
}

int EVALVisitor::visit(BreakStatement* p) {
    return 0;
}

int EVALVisitor::visit(SwitchStatement* p) {
    int val = p->condition->accept(this);
    for (auto c : p->cases) {
        if (c->valor->accept(this) == val) {
            for (auto s : c->cuerpo) {
                s->accept(this);
            }
            return 0;
        }
    }
    if (p->defaultCase) {
        for (auto s : p->defaultCase->cuerpo) s->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(IfStatement* p) {
    int val = p->condition->accept(this);
    if (val) {
        for (auto s : p->thenBody) s->accept(this);
    } else if (p->elseIf) {
        p->elseIf->accept(this);
    } else {
        for (auto s : p->elseBody) s->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(DoWhileStatement* p) {
    do {
        for (auto s : p->body) s->accept(this);
    } while (p->condition->accept(this));
    return 0;
}

int EVALVisitor::visit(WhileStatement* p) {
    while (p->condition->accept(this)) {
        for (auto s : p->body) s->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(IdExp* p) {
    return memoria[p->value];
}
