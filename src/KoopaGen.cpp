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

    // dycast<BaseAST*>(expression.get()) to UnaryExpressionAST*
    auto expr_ast = dynamic_cast<AddExpressionAST*>(expression.get());
    
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

std::string UnaryExpressionAST::GenKoopa() const {
    std::string s;
    if (op == '\0') {

        auto primary_exp_ast = dynamic_cast<PrimaryExpAST*>(primary_exp.get());
        primary_exp_ast->temp_counter = temp_counter;
        
        std::string exp = primary_exp_ast->GenKoopa();
        s += exp;
        temp_counter = primary_exp_ast->temp_counter;
        koopa_result = primary_exp_ast->koopa_result;

    } else {

        auto expr_ast = dynamic_cast<UnaryExpressionAST*>(expression.get());
        expr_ast->temp_counter = temp_counter;

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

        auto expr_ast = dynamic_cast<UnaryExpressionAST*>(expression.get());
        expr_ast->temp_counter = temp_counter;

        std::string s;
        std::string exp = expr_ast->GenKoopa();
        s += exp;
        temp_counter = expr_ast->temp_counter;
        koopa_result = expr_ast->koopa_result;
        return s;
    }
}


std::string AddExpressionAST::GenKoopa() const {
    std::string s;
    if (op == '\0') {
        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());

        std::string exp = mul_exp_ast->GenKoopa();
        s += exp;

        temp_counter = mul_exp_ast->temp_counter;
        koopa_result = mul_exp_ast->koopa_result;

        return s;

    } else {

        auto add_exp_ast = dynamic_cast<AddExpressionAST*>(AddExpression.get());
        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());

        std::string exp1 = add_exp_ast->GenKoopa();
        s += exp1;

        mul_exp_ast->temp_counter = add_exp_ast->temp_counter;

        left_result = add_exp_ast->koopa_result;

        std::string exp2 = mul_exp_ast->GenKoopa();
        s += exp2;
        temp_counter = mul_exp_ast->temp_counter;
        right_result = mul_exp_ast->koopa_result;

        std::string index = std::to_string(temp_counter);
        std::string left_index,right_index;

        if (temp_counter == 0) {
            left_index = left_result;
            right_index = right_result;
        } else if (add_exp_ast->temp_counter == 0) {
            left_index = left_result;
            right_index = "%" + std::to_string(temp_counter - 1);
        } else if (mul_exp_ast->temp_counter == add_exp_ast->temp_counter) {
            left_index = "%" + std::to_string(add_exp_ast->temp_counter - 1);
            right_index = right_result;
        } else {
            left_index = "%" + std::to_string(add_exp_ast->temp_counter - 1);
            right_index = "%" + std::to_string(temp_counter - 1);
        }

        if (op == '+') {
            s += "  %" + index + " = add " + left_index + ", " + right_index + "\n";
            temp_counter++;
        } else if (op == '-') {
            s += "  %" + index + " = sub " + left_index + ", " + right_index + "\n";
            temp_counter++;
        }

        return s;
    }
}

std::string MulExpressionAST::GenKoopa() const {

    std::string s;

    if (op == '\0') {
        auto unary_exp_ast = dynamic_cast<UnaryExpressionAST*>(UnaryExpression.get());

        unary_exp_ast->temp_counter = temp_counter;

        s += unary_exp_ast->GenKoopa();
        temp_counter = unary_exp_ast->temp_counter;
        koopa_result = unary_exp_ast->koopa_result;

        return s;
    } else {

        std::string left_index,right_index;

        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());
        auto unary_exp_ast = dynamic_cast<UnaryExpressionAST*>(UnaryExpression.get());

        mul_exp_ast->temp_counter = temp_counter;

        s += mul_exp_ast->GenKoopa();

        left_result = mul_exp_ast->koopa_result;
        unary_exp_ast->temp_counter = mul_exp_ast->temp_counter;

        if (mul_exp_ast->temp_counter == temp_counter) {
            left_index = left_result;
        } else {
            left_index = "%" + std::to_string(mul_exp_ast->temp_counter - 1);
        }

        s += unary_exp_ast->GenKoopa();

        temp_counter = unary_exp_ast->temp_counter;
        right_result = unary_exp_ast->koopa_result;

        if (unary_exp_ast->temp_counter == mul_exp_ast->temp_counter) {
            right_index = right_result;
        } else {
            right_index = "%" + std::to_string(temp_counter - 1);
        }
        
        std::string index = std::to_string(temp_counter);

        if (op == '*') {
            s += "  %" + index + " = mul " + left_index + ", " + right_index + "\n";
            temp_counter++;
        } else if (op == '/') {
            s += "  %" + index + " = div " + left_index + ", " + right_index + "\n";
            temp_counter++;
        } else if (op == '%') {
            s += "  %" + index + " = mod " + left_index + ", " + right_index + "\n";
            temp_counter++;
        }

        return s;
    }
}