#pragma once
#include <memory>
#include <string>
#include <iostream>

// 所有 AST 的 基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        
        virtual std::string GenKoopa() const {
            return "";
        }
};


// CompUnit 是 BaseAST 
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
    std::unique_ptr<BaseAST> func_def;

    void Dump() const override {
        std::cout << "CompUnitAST { ";
        func_def->Dump();
        std::cout << " }" ;
    }

    std::string GenKoopa() const override;
};


// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> func_type;
     std::string ident;
     std::unique_ptr<BaseAST> block;

     void Dump() const override {
        std::cout << "FuncDefAST { ";
        func_type->Dump();
        std::cout << ", " << ident << ", ";
        block->Dump();
        std::cout << " }" ;
     }

     std::string GenKoopa() const override;

};

// FuncType 也是 BaseAST
class FuncTypeAST : public BaseAST {
    public:
     std::string type;

     void Dump() const override {
        std::cout << "FuncTypeAST { " << type << " }" ;
     }

     std::string GenKoopa() const override;

};


// Block 也是 BaseAST
class BlockAST : public BaseAST {
    public:
     std::unique_ptr<BaseAST> stmt;

     void Dump() const override {
        std::cout << "BlockAST { ";
        stmt->Dump();
        std::cout << " }" ;
     }

     std::string GenKoopa() const override;
};


// Stmt 也是 BaseAST
class StmtAST : public BaseAST {
    public:
     int number;

     void Dump() const override {
        std::cout << "StmtAST { return " << number << "; }" ;
     }

     std::string GenKoopa() const override;
};


