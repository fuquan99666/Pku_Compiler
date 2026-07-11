#pragma once
#include <memory>
#include <string>
#include <iostream>


// ========== 前向声明 ==========
class BaseAST;
class UnaryExpressionAST;  
class PrimaryExpAST;
class StmtAST;
class BlockAST;
class FuncDefAST;
class FuncTypeAST;
class CompUnitAST;
class AddExpressionAST;
class MulExpressionAST;


struct KoopaContext {
    int temp_counter = 0;
    std::string result;  // 当前表达式的计算结果
    
    // 获取最后一个临时变量（或直接结果）
    std::string getLastValue() const {
        if (temp_counter == 0) {
            return result;  // 可能是立即数（如 "42"）或 label（如 "%1"）
        }
        return "%" + std::to_string(temp_counter - 1);
    }
    
    // 创建新的临时变量
    std::string newTemp() {
        return "%" + std::to_string(temp_counter++);
    }
    
    // 获取指定索引的临时变量
    std::string getTemp(int index) const {
        return "%" + std::to_string(index);
    }
    
    // 重置上下文（用于新表达式）
    void reset() {
        temp_counter = 0;
        result.clear();
    }
};

// 所有 AST 的 基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        
        virtual std::string GenKoopa(KoopaContext& ctx) const = 0;
};


// CompUnit 是 BaseAST 
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
    std::unique_ptr<BaseAST> func_def;

    void Dump() const override;

    std::string GenKoopa(KoopaContext& ctx) const override;
};


// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> func_type;
     std::string ident;
     std::unique_ptr<BaseAST> block;

     void Dump() const override;

     std::string GenKoopa(KoopaContext& ctx) const override;

};

// FuncType 也是 BaseAST
class FuncTypeAST : public BaseAST {
    public:
     std::string type;

     void Dump() const override;

     std::string GenKoopa(KoopaContext& ctx) const override;

};


// Block 也是 BaseAST
class BlockAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> stmt;

     void Dump() const override;

     std::string GenKoopa(KoopaContext& ctx) const override;
};


// Stmt 也是 BaseAST
class StmtAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> expression;

     void Dump() const override;

     std::string GenKoopa(KoopaContext& ctx) const override;
};

class PrimaryExpAST : public BaseAST {
    public:
        bool is_number = false;
        int number;
        std::unique_ptr<BaseAST> expression;

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};

class UnaryExpressionAST : public BaseAST {
    public:
        char op = '\0'; // over
        std::unique_ptr<BaseAST> primary_exp;
        std::unique_ptr<BaseAST> expression;  // actually is a UnaryExpressionAST

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};


class AddExpressionAST: public BaseAST {
    public:
        char op = '\0'; // '+' or '-' , if not set, only one child 
        std::unique_ptr<BaseAST> AddExpression;
        std::unique_ptr<BaseAST> MulExpression;

        mutable std::string left_result;
        mutable std::string right_result;

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};

class MulExpressionAST: public BaseAST {
    public:
        char op = '\0'; // '*' or '/' or '%', if not set, only one Unary child 
        std::unique_ptr<BaseAST> UnaryExpression;
        std::unique_ptr<BaseAST> MulExpression;

        mutable std::string left_result;
        mutable std::string right_result;

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};


class RelationalExpressionAST: public BaseAST {
    public:
        std::string op = "\0"; // '<', '>', '<=', '>='
        std::unique_ptr<BaseAST> RelExp;
        std::unique_ptr<BaseAST> AddExp;

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};

class EqualityExpressionAST: public BaseAST {
    public:
        std::string op = "\0"; // '==', '!='
        std::unique_ptr<BaseAST> EqExp;
        std::unique_ptr<BaseAST> RelExp;

        void Dump() const override;

        std::string GenKoopa(KoopaContext& ctx) const override;
};

class LogicalAndExpressionAST: public BaseAST {
    public:
        std::string op = "\0"; // '&&'

        std::unique_ptr<BaseAST> AndExp;
        std::unique_ptr<BaseAST> EqExp;

        void Dump() const override;
        std::string GenKoopa(KoopaContext& ctx) const override;
};

class LogicalOrExpressionAST: public BaseAST {
    public:
        std::string op = "\0"; // '||'

        std::unique_ptr<BaseAST> OrExp;
        std::unique_ptr<BaseAST> AndExp;

        void Dump() const override;
        std::string GenKoopa(KoopaContext& ctx) const override;
};

