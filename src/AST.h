#pragma once
#include <memory>
#include <string>
#include <iostream>


// ========== 前向声明 ==========
class ExpressionAST;  
class PrimaryExpAST;
class StmtAST;
class BlockAST;
class FuncDefAST;
class FuncTypeAST;
class CompUnitAST;

// 所有 AST 的 基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        
        virtual std::string GenKoopa() const = 0;
};


// CompUnit 是 BaseAST 
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
    std::unique_ptr<BaseAST> func_def;

    void Dump() const override;

    std::string GenKoopa() const override;
};


// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> func_type;
     std::string ident;
     std::unique_ptr<BaseAST> block;

     void Dump() const override;

     std::string GenKoopa() const override;

};

// FuncType 也是 BaseAST
class FuncTypeAST : public BaseAST {
    public:
     std::string type;

     void Dump() const override;

     std::string GenKoopa() const override;

};


// Block 也是 BaseAST
class BlockAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> stmt;

     void Dump() const override;

     std::string GenKoopa() const override;
};


// Stmt 也是 BaseAST
class StmtAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> expression;

     void Dump() const override;

     std::string GenKoopa() const override;
};

class PrimaryExpAST : public BaseAST {
    public:
        bool is_number = false;
        int number;
        std::unique_ptr<BaseAST> expression;

        mutable std::string koopa_result;
        mutable int temp_counter = 0;

        void Dump() const override;

        std::string GenKoopa() const override;
};

class ExpressionAST : public BaseAST {
    public:
        char op = '\0'; // over
        std::unique_ptr<BaseAST> primary_exp;
        std::unique_ptr<BaseAST> expression;

        mutable std::string koopa_result;
        mutable int temp_counter = 0;

        void Dump() const override;

        std::string GenKoopa() const override;
};




