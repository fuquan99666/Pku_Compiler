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
    s += "  ret " + std::to_string(number) + "\n";
    return s;
}