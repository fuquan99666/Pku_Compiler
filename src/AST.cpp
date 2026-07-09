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


void ExpressionAST::Dump() const{
    if (op == '\0') {
        std::cout << "ExpressionAST { ";
        primary_exp->Dump();
            std::cout << " }";
        }
    else {
        std::cout << "ExpressionAST { ";
        std::cout << "op: " << op << ", ";
        expression->Dump();
        std::cout << " }";
    }
}