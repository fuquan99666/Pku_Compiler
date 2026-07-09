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

/*

fun @main(): i32 {
  %entry:
    %0 = eq 6, 0
    %1 = sub 0, %0
    %2 = sub 0, %1
    ret %2
}

*/


std::string StmtAST::GenKoopa() const {
    std::string s;

    // dycast<BaseAST*>(expression.get()) to ExpressionAST*
    auto expr_ast = dynamic_cast<ExpressionAST*>(expression.get());
    
    std::string exp = expr_ast->GenKoopa();
    s += exp;

    if (expr_ast->temp_counter == 0) {
        s += "  ret " + expr_ast->koopa_result;
    } else {
        s += "  ret %" + std::to_string(expr_ast->temp_counter - 1);
    }

    s += "\n";

    return s;
}

std::string ExpressionAST::GenKoopa() const {
    std::string s;
    if (op == '\0') {

        auto primary_exp_ast = dynamic_cast<PrimaryExpAST*>(primary_exp.get());
        
        std::string exp = primary_exp_ast->GenKoopa();
        s += exp;
        temp_counter = primary_exp_ast->temp_counter;
        koopa_result = primary_exp_ast->koopa_result;

    } else {

        auto expr_ast = dynamic_cast<ExpressionAST*>(expression.get());

        std::string exp = expr_ast->GenKoopa();
        temp_counter = expr_ast->temp_counter;
        koopa_result = expr_ast->koopa_result;

        s += exp;

        std::string index = std::to_string(temp_counter);
        std::string pre_index;

        if (temp_counter == 0) {
            pre_index = koopa_result;
        } else {
            pre_index = "%" + std::to_string(temp_counter - 1);
        }


        if (op == '+') {
            // do nothing
        } else if (op == '-') {
            s += "  %" + index + " = sub 0, " + pre_index + "\n";
            temp_counter++;
        } else if (op == '!') {
            s += "  %" + index + " = eq " + pre_index + ", 0\n";
            temp_counter++;
        }

    }
    return s;
}

std::string PrimaryExpAST::GenKoopa() const {
    if (is_number) {

        koopa_result = std::to_string(number);

        return "";
    } else {

        auto expr_ast = dynamic_cast<ExpressionAST*>(expression.get());

        std::string s;
        std::string exp = expr_ast->GenKoopa();
        s += exp;
        temp_counter = expr_ast->temp_counter;
        return s;
    }
}