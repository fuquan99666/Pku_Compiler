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
    auto expr_ast = dynamic_cast<AddExpressionAST*>(expression.get());
    
    std::string exp = expr_ast->GenKoopa(ctx);
    s += exp;

    // 使用 KoopaContext 获取最后一个临时变量或直接结果
    s += "  ret " + ctx.getLastValue() + "\n";  

    s += "\n";

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

        auto expr_ast = dynamic_cast<UnaryExpressionAST*>(expression.get());

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