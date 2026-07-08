#include "AST.h"


std::string CompUnitAST::GenKoopa() const {
    return func_def->GenKoopa();
}

std::string FuncDefAST::GenKoopa() const {
    std::string s;
    s += "fun @" + ident + "(): i32 {\n";
    s += block->GenKoopa();
    s += "}\n";
    return s;
}

std::string FuncTypeAST::GenKoopa() const {
    return type;
}

std::string BlockAST::GenKoopa() const {
    std::string s;
    s += "%entry:\n";
    s += stmt->GenKoopa();
    return s;
}


std::string StmtAST::GenKoopa() const {
    std::string s;
    s += "  ret " + expression->GenKoopa() + "\n";
    return s;
}

std::string ExpressionAST::GenKoopa() const {
    std::string s;
    if (op == '\0') {
        s += primary_exp->GenKoopa();
    } else {
        s += " " + std::string(1, op) + expression->GenKoopa();
    }
    return s;
}

std::string PrimaryExpAST::GenKoopa() const {
    if (is_number) {
        return std::to_string(number);
    } else {
        return "(" + expression->GenKoopa() + ")";
    }
}