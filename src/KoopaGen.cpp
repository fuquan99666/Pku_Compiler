#include "AST.h"


std::string CompUnitAST::GenKoopa(KoopaContext& ctx) const {
    ctx.reset(); 
    return func_def->GenKoopa(ctx);
}

std::string FuncDefAST::GenKoopa(KoopaContext& ctx) const {
    ctx.reset();
    std::string s;
    s += "fun @" + ident + "(): i32 {\n";
    s += block->GenKoopa(ctx);
    s += "}\n";
    return s;
}

std::string FuncTypeAST::GenKoopa(KoopaContext& ctx) const {
    return type;
}

std::string BlockAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;
    s += "%entry:\n";
    s += stmt->GenKoopa(ctx);
    return s;
}


std::string StmtAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;

    // dycast<BaseAST*>(expression.get()) to UnaryExpressionAST*
    auto expr_ast = dynamic_cast<ExpAST*>(expression.get());
    
    std::string exp = expr_ast->GenKoopa(ctx);
    s += exp;

    // 使用 KoopaContext 获取最后一个临时变量或直接结果
    s += "  ret " + ctx.getLastValue() + "\n";  

    s += "\n";

    return s;
}

std::string ExpAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;

    auto expr_ast = dynamic_cast<LogicalOrExpressionAST*>(expression.get());
    
    std::string exp = expr_ast->GenKoopa(ctx);
    s += exp;

    return s;
}

std::string UnaryExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;
    if (op == '\0') {

        auto primary_exp_ast = dynamic_cast<PrimaryExpAST*>(primary_exp.get());
        
        std::string exp = primary_exp_ast->GenKoopa(ctx);
        s += exp;

    } else {

        auto expr_ast = dynamic_cast<UnaryExpressionAST*>(expression.get());

        std::string exp = expr_ast->GenKoopa(ctx);

        std::string pre_index = ctx.result;

        s += exp;

        std::string index;
        if (op != '+') {
            index = ctx.newTemp();
            ctx.result = index;
        }

        if (op == '-') {
            s += "  " + index + " = sub 0, " + pre_index + "\n";
        } else if (op == '!') {
            s += "  " + index + " = eq " + pre_index + ", 0\n";
        }

    }
    return s;
}

std::string PrimaryExpAST::GenKoopa(KoopaContext& ctx) const {
    if (is_number) {
        ctx.result = std::to_string(number);
        return "";
    } else {

        auto expr_ast = dynamic_cast<ExpAST*>(expression.get());

        std::string s;
        std::string exp = expr_ast->GenKoopa(ctx);
        s += exp;
        return s;
    }
}


std::string AddExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;
    if (op == '\0') {
        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());

        std::string exp = mul_exp_ast->GenKoopa(ctx);
        s += exp;

        return s;

    } else {

        auto add_exp_ast = dynamic_cast<AddExpressionAST*>(AddExpression.get());
        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());

        std::string exp1 = add_exp_ast->GenKoopa(ctx);
        s += exp1;

        left_result = ctx.result;

        std::string exp2 = mul_exp_ast->GenKoopa(ctx);
        s += exp2;
        right_result = ctx.result;

        std::string index = ctx.newTemp();

        if (op == '+') {
            s += "  " + index + " = add " + left_result+ ", " + right_result + "\n";
        } else if (op == '-') {
            s += "  " + index + " = sub " + left_result + ", " + right_result + "\n";
        }

        ctx.result = index;

        return s;
    }
}

std::string MulExpressionAST::GenKoopa(KoopaContext& ctx) const {

    std::string s;

    if (op == '\0') {
        auto unary_exp_ast = dynamic_cast<UnaryExpressionAST*>(UnaryExpression.get());

        s += unary_exp_ast->GenKoopa(ctx);

        return s;
    } else {

        auto mul_exp_ast = dynamic_cast<MulExpressionAST*>(MulExpression.get());
        auto unary_exp_ast = dynamic_cast<UnaryExpressionAST*>(UnaryExpression.get());

        s += mul_exp_ast->GenKoopa(ctx);

        left_result = ctx.result;

        s += unary_exp_ast->GenKoopa(ctx);

        right_result = ctx.result;
        
        std::string index = ctx.newTemp();

        if (op == '*') {
            s += "  " + index + " = mul " + left_result + ", " + right_result + "\n";
        } else if (op == '/') {
            s += "  " + index + " = div " + left_result + ", " + right_result + "\n";
        } else if (op == '%') {
            s += "  " + index + " = mod " + left_result + ", " + right_result + "\n";
        }

        ctx.result = index;
        return s;
    }
}

std::string RelationalExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;

    if (op == "\0") {
        auto add_exp_ast = dynamic_cast<AddExpressionAST*>(AddExp.get());
        s += add_exp_ast->GenKoopa(ctx);

        return s;
    } 

    auto rel_exp_ast = dynamic_cast<RelationalExpressionAST*>(RelExp.get());
    auto add_exp_ast = dynamic_cast<AddExpressionAST*>(AddExp.get());

    s += rel_exp_ast->GenKoopa(ctx);

    left_result = ctx.result;

    s += add_exp_ast->GenKoopa(ctx);

    right_result = ctx.result;

    std::string index = ctx.newTemp();

    if (op == "<") {
        s += "  " + index + " = lt " + left_result + ", " + right_result + "\n";
    } else if (op == ">") {
        s += "  " + index + " = gt " + left_result + ", " + right_result + "\n";
    } else if (op == "<=") {
        s += "  " + index + " = le " + left_result + ", " + right_result + "\n";
    } else if (op == ">=") {
        s += "  " + index + " = ge " + left_result + ", " + right_result + "\n";
    }

    ctx.result = index;
    return s;

}

std::string EqualityExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;

    if (op == "\0") {
        auto rel_exp_ast = dynamic_cast<RelationalExpressionAST*>(RelExp.get());
        s += rel_exp_ast->GenKoopa(ctx);

        return s;
    }

    auto eq_exp_ast = dynamic_cast<EqualityExpressionAST*>(EqExp.get());
    auto rel_exp_ast = dynamic_cast<RelationalExpressionAST*>(RelExp.get());

    s += eq_exp_ast->GenKoopa(ctx);
    left_result = ctx.result;

    s += rel_exp_ast->GenKoopa(ctx);
    right_result = ctx.result;

    std::string index = ctx.newTemp();

    if (op == "==") {
        s += "  " + index + " = eq " + left_result + ", " + right_result + "\n";
    } else if (op == "!=") {
        s += "  " + index + " = ne " + left_result + ", " + right_result + "\n";
    }

    ctx.result = index;
    return s;
}

std::string LogicalAndExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;

    if (op == "\0") {
        auto eq_exp_ast = dynamic_cast<EqualityExpressionAST*>(EqExp.get());
        s += eq_exp_ast->GenKoopa(ctx);

        return s;
    }

    auto and_exp_ast = dynamic_cast<LogicalAndExpressionAST*>(AndExp.get());
    auto eq_exp_ast = dynamic_cast<EqualityExpressionAST*>(EqExp.get());

    s += and_exp_ast->GenKoopa(ctx);
    left_result = ctx.result;

    s += eq_exp_ast->GenKoopa(ctx);
    right_result = ctx.result;

    std::string index = ctx.newTemp();
    std::string index_1 = ctx.newTemp();
    std::string index_2 = ctx.newTemp();

    s += "  " + index + " = ne " + left_result + ", 0\n"; // 将左操作数转换为布尔值
    s += "  " + index_1 + " = ne " + right_result + ", 0\n"; // 将右操作数转换为布尔值

    s += "  " + index_2 + " = and " + index + ", " + index_1 + "\n";

    ctx.result = index_2;

    return s;
}

std::string LogicalOrExpressionAST::GenKoopa(KoopaContext& ctx) const {
    std::string s;
    if (op == "\0") {
        auto and_exp_ast = dynamic_cast<LogicalAndExpressionAST*>(AndExp.get());
        s += and_exp_ast->GenKoopa(ctx);
        return s;
    }

    auto or_exp_ast = dynamic_cast<LogicalOrExpressionAST*>(OrExp.get());
    auto and_exp_ast = dynamic_cast<LogicalAndExpressionAST*>(AndExp.get());

    s += or_exp_ast->GenKoopa(ctx);
    left_result = ctx.result;

    s += and_exp_ast->GenKoopa(ctx);
    right_result = ctx.result;

    std::string index = ctx.newTemp();

    s += "  " + index + " = or " + left_result + ", " + right_result + "\n";

    std::string index_1 = ctx.newTemp();
    s += "  " + index_1 + " = ne " + index + ", 0\n"; // 将结果转换为布尔值
    ctx.result = index_1;

    return s;
}



