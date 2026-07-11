// Some help function to visit Koopa IR in memory format 
// to avoid writing a lot of loop code 
#include "koopa.h"
#include <string>

class GenRISCVVisitor {
    private:
        std::string output;
    public:


        // In RISCV, we can use t0-t6 for temporary registers, and a0-a7 for argument registers
        int temp_counter = 0; // counter for temporary registers

        int left_result = 0; // store the result of left expression
        int mul_left_result = 0; // store the result of left expression in mul expression

        // 分配一个新的临时寄存器
        std::string allocTemp() {
            return "t" + std::to_string(temp_counter++);
        }

        // 获取当前使用的最后一个寄存器
        std::string getLastTemp() {
            if (temp_counter == 0) return "a0";
            return "t" + std::to_string(temp_counter - 1);
        }

        void resetTemp() { temp_counter = 0; }

        void GenRISCV_0(std::string s, std::string &output);
        std::string GenRISCV(std::string s);
        void Visit(const koopa_raw_program_t &program);
        void Visit(const koopa_raw_slice_t &slice);
        void Visit(const koopa_raw_function_t &func);
        void Visit(const koopa_raw_basic_block_t &bb);
        void Visit(const koopa_raw_value_t &value);
        void Visit(const koopa_raw_integer_t &int_val);
        void Visit(const koopa_raw_return_t &ret);
        void Visit(const koopa_raw_binary_t &binary);

};

