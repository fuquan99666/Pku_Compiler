#include "AST.h"

void CompUnitAST::Dump() const{
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }" ;
    std::cout << std::endl;
}



void FuncDefAST::Dump() const{
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }" ;
}

void FuncTypeAST::Dump() const{
    std::cout << "FuncTypeAST { " << type << " }" ;
}


void BlockAST::Dump() const{
    std::cout << "BlockAST { ";
    stmt->Dump();
    std::cout << " }" ;
}

void StmtAST::Dump() const{
    std::cout << "StmtAST { return ";
    expression->Dump();
    std::cout << " }" ;
}


void PrimaryExpAST::Dump() const{
    if (is_number) {
        std::cout << "PrimaryExpAST { number: " << number << " }";
    } else {
        std::cout << "PrimaryExpAST { ( ";
        expression->Dump();
        std::cout << " ) }";
    }
}


void UnaryExpressionAST::Dump() const{
    if (op == '\0') {
        std::cout << "UnaryExpressionAST { ";
        primary_exp->Dump();
            std::cout << " }";
        }
    else {
        std::cout << "UnaryExpressionAST { ";
        std::cout << "op: " << op << ", ";
        expression->Dump();
        std::cout << " }";
    }
}


void AddExpressionAST::Dump() const{
    if (op == '\0') {
        std::cout << "AddExpressionAST { ";
        MulExpression->Dump();
        std::cout << " }";
    } else {
        std::cout << "AddExpressionAST { ";
        std::cout << "op: " << op << ", ";
        AddExpression->Dump();
        std::cout << ", ";
        MulExpression->Dump();
        std::cout << " }";
    }
}

void MulExpressionAST::Dump() const{
    if (op == '\0') {
        std::cout << "MulExpressionAST { ";
        UnaryExpression->Dump();
        std::cout << " }";
    } else {
        std::cout << "MulExpressionAST { ";
        std::cout << "op: " << op << ", ";
        MulExpression->Dump();
        std::cout << ", ";
        UnaryExpression->Dump();
        std::cout << " }";
    }
}

void RelationalExpressionAST::Dump() const {
    if (op == "\0") {
        std::cout << "RelationalExpressionAST { ";
        AddExp->Dump();
        std::cout << " }";
    } else {
        std::cout << "RelationalExpressionAST { ";
        std::cout << "op: " << op << ", ";
        RelExp->Dump();
        std::cout << ", ";
        AddExp->Dump();
        std::cout << " }";
    }
}

void EqualityExpressionAST::Dump() const {
    if (op == "\0") {
        std::cout << "EqualityExpressionAST { ";
        RelExp->Dump();
        std::cout << " }";
    } else {
        std::cout << "EqualityExpressionAST { ";
        std::cout << "op: " << op << ", ";
        EqExp->Dump();
        std::cout << ", ";
        RelExp->Dump();
        std::cout << " }";
    }
}

void LogicalAndExpressionAST::Dump() const {
    if (op == "\0") {
        std::cout << "LogicalAndExpressionAST { ";
        EqExp->Dump();
        std::cout << " }";
    } else {
        std::cout << "LogicalAndExpressionAST { ";
        std::cout << "op: " << op << ", ";
        AndExp->Dump();
        std::cout << ", ";
        EqExp->Dump();
        std::cout << " }";
    }
}

void LogicalOrExpressionAST::Dump() const {
    if (op == "\0") {
        std::cout << "LogicalOrExpressionAST { ";
        AndExp->Dump();
        std::cout << " }";
    } else {
        std::cout << "LogicalOrExpressionAST { ";
        std::cout << "op: " << op << ", ";
        OrExp->Dump();
        std::cout << ", ";
        AndExp->Dump();
        std::cout << " }";
    }
}