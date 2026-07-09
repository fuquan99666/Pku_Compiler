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