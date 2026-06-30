#include <memory>
#include <string>

// 所有 AST 的 基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;
};


// CompUnit 是 BaseAST 
class CompUnit : public BaseAST {
    public:
    // 用智能指针管理对象
    std::unique_ptr<BaseAST> func_def;
};


// FuncDef 也是 BaseAST
class FuncDef : public BaseAST {
    public:
     std::unique_ptr<BaseAST> func_type;
     std::string ident;
     std::unique_ptr<BaseAST> block;
};

// FuncType 也是 BaseAST
class FuncType : public BaseAST {
    public:
     std::string type;
};


// Block 也是 BaseAST
class Block : public BaseAST {
    public:
     std::unique_ptr<BaseAST> stmt;
};


// Stmt 也是 BaseAST
class Stmt : public BaseAST {
    public:
     int number;
};


