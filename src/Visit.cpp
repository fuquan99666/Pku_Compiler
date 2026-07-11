#include "Visit.h"
#include <cassert>
#include <iostream>

void GenRISCVVisitor::GenRISCV_0(std::string s, std::string &output) {

  koopa_program_t program;
  koopa_error_code_t error = koopa_parse_from_string(s.c_str(), &program);

  assert(error == KOOPA_EC_SUCCESS);

  // create a raw program builder
  koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();

  // build the raw program
  koopa_raw_program_t raw = koopa_build_raw_program(builder, program);

  // release the program 
  koopa_delete_program(program);

  // 注意：本地环境没有 koopa.h ， 最好还是前往github下载一下
  // 既去掉了一堆报错信息， 也有IDE的代码提示，方便编程

  std::string riscv_as = "  .text\n";
  riscv_as += "  .globl main\n";


  // process the raw program ... 

  for (size_t i = 0; i < raw.funcs.len; i++) {

    assert(raw.funcs.kind == KOOPA_RSIK_FUNCTION);

    // get the i-th function 
    koopa_raw_function_t func = (koopa_raw_function_t) raw.funcs.buffer[i];

    // std::string func_name = func->name;
    riscv_as += "main:\n";

    // process the function ...
    for (size_t j = 0; j < func->bbs.len; j++) {
      assert(func->bbs.kind == KOOPA_RSIK_BASIC_BLOCK);
      koopa_raw_basic_block_t bb = (koopa_raw_basic_block_t) func->bbs.buffer[j];

      // process the basic block ...
      for (size_t k = 0; k < bb->insts.len; k++) {
        assert(bb->insts.kind == KOOPA_RSIK_VALUE);
        koopa_raw_value_t val = (koopa_raw_value_t) bb->insts.buffer[k];

        // process the value ...
        // 在Lv1 中，这里应该就是一个 return 66
        assert(val->kind.tag == KOOPA_RVT_RETURN);

        koopa_raw_value_t ret_value = val->kind.data.ret.value;
        assert(ret_value->kind.tag == KOOPA_RVT_INTEGER);

        int32_t int_val = ret_value->kind.data.integer.value;

        // assert(int_val == 0);

        riscv_as += "  li a0, " + std::to_string(int_val) + "\n";
        riscv_as += "  ret\n";
      }
    }
  }

  output = riscv_as;

  // release the raw program builder 
  koopa_delete_raw_program_builder(builder);

}


std::string GenRISCVVisitor::GenRISCV(std::string s) {
    koopa_program_t program;
    koopa_error_code_t error = koopa_parse_from_string(s.c_str(), &program);

    assert(error == KOOPA_EC_SUCCESS);    

    // create a raw program builder
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();

    // build the raw program
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);

    // release the program 
    koopa_delete_program(program);

    Visit(raw);

    koopa_delete_raw_program_builder(builder);
    return output;
}


// program is the raw program in memory format 
void GenRISCVVisitor::Visit(const koopa_raw_program_t &program) {
    // 执行一些其他的必要操作
    // ...
    output = "  .text\n";
    output += "  .globl main\n";

    
    Visit(program.values);


    output += "main:\n";
    Visit(program.funcs);
}

// slice is a raw slice that can store any kind of items as array ?
void GenRISCVVisitor::Visit(const koopa_raw_slice_t &slice) {
    for (size_t i = 0; i < slice.len; ++i) {
        auto ptr = slice.buffer[i];
        switch (slice.kind) {
            case KOOPA_RSIK_FUNCTION:
                Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
                break;
            case KOOPA_RSIK_BASIC_BLOCK:
                Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
                break;
            case KOOPA_RSIK_VALUE:
                Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
                break;
            default:
                assert(false); // 暂时不会遇到
        }
    }
}

void GenRISCVVisitor::Visit(const koopa_raw_function_t &func) {
    // do something else 

    Visit(func->bbs);
}

void GenRISCVVisitor::Visit(const koopa_raw_basic_block_t &bb) {
    // do something else 

    Visit(bb->insts);
}

void GenRISCVVisitor::Visit(const koopa_raw_value_t &value) {

    const auto &kind = value->kind;
    switch (kind.tag) {
        case KOOPA_RVT_RETURN: 
            // kind.data.ret is koopa_raw_return_t
            Visit(kind.data.ret);
            break;
        case KOOPA_RVT_INTEGER:
            // kind.data.integer is koopa_raw_integer_t
            Visit(kind.data.integer);
            break;
        case KOOPA_RVT_BINARY:
            // kind.data.binary is koopa_raw_binary_t
            Visit(kind.data.binary);
            break;
        default:
            // 现阶段，只处理return 0此类指令，所以只处理KOOPA_RVT_RETURN和KOOPA_RVT_INTEGER
            assert(false);
    }
}

void GenRISCVVisitor::Visit(const koopa_raw_integer_t &int_val) {
    output += std::to_string(int_val.value);
}

void GenRISCVVisitor::Visit(const koopa_raw_return_t &ret) {

    // 处理返回值
    if (temp_counter == 0) {
        // 没有临时寄存器，直接加载立即数
        output += "  li a0, ";
        Visit(ret.value);
        output += "\n"; 

    } else {
        // 有临时寄存器，从最后一个移到 a0
        output += "  mv a0, " + getLastTemp() + "\n";
    }
    output += "  ret\n";
    resetTemp();  // 重置计数器
}

void GenRISCVVisitor::Visit(const koopa_raw_binary_t &binary) {

    koopa_raw_binary_op_t op = binary.op;
    koopa_raw_value_t lhs = binary.lhs;
    koopa_raw_value_t rhs = binary.rhs;

    // 先处理左右操作数
    std::string lhs_reg, rhs_reg;

    // now use temp_map to track each inst's temporary register
    if (lhs->kind.tag == KOOPA_RVT_INTEGER) {
        if (lhs->kind.data.integer.value != 0) {
            lhs_reg = allocTemp();
            output += "  li " + lhs_reg + ", ";
            Visit(lhs->kind.data.integer);
            output += "\n";
        } else {
            lhs_reg = "x0"; // zero register
        }
    } else if (lhs->kind.tag == KOOPA_RVT_BINARY) {
        if (temp_map.find(&lhs->kind.data.binary) != temp_map.end()) {
            lhs_reg = "t" + std::to_string(temp_map[&lhs->kind.data.binary]);
        } else {
            assert(false); // should not happen
        }
    } else {
        // 现阶段只处理整数和二元操作数
        assert(false);
    }

    if (rhs->kind.tag == KOOPA_RVT_INTEGER) {
        if (rhs->kind.data.integer.value != 0) {
            rhs_reg = allocTemp();
            output += "  li " + rhs_reg + ", ";
            Visit(rhs->kind.data.integer);
            output += "\n";
        } else {
            rhs_reg = "x0"; // zero register
        }
    } else if (rhs->kind.tag == KOOPA_RVT_BINARY) {
        if (temp_map.find(&rhs->kind.data.binary) != temp_map.end()) {
            rhs_reg = "t" + std::to_string(temp_map[&rhs->kind.data.binary]);
        } else {
            assert(false); // should not happen
        }
    } else {
        // 现阶段只处理整数和二元操作数
        assert(false);
    }

    std::string result_reg;
    if (temp_counter == 0) {
        result_reg = allocTemp(); // 分配一个新的临时寄存器
    } else {
        result_reg = getLastTemp(); // 使用当前最后一个临时寄存器
    }
    
    switch (op) {
        case KOOPA_RBO_ADD:
            output += "  add " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register for this binary instruction
            break;
            
        case KOOPA_RBO_SUB:

            output += "  sub " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register for this binary instruction
            break;
            
        case KOOPA_RBO_MUL:
            output += "  mul " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register for this binary instruction
            break;
            
        case KOOPA_RBO_DIV:
            output += "  div " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register for this binary instruction
            break;
        case KOOPA_RBO_MOD:
            output += "  rem " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register
            break;
            
        case KOOPA_RBO_EQ: {
            // 比较是否相等
            output += "  xor " + result_reg + ", " + lhs_reg + ", " + rhs_reg + "\n";
            output += "  seqz " + result_reg + ", " + result_reg + "\n";
            temp_map[&binary] = temp_counter - 1; // track the temporary register for this binary instruction
            break;
        }
            
        default:
            assert(false);
    }
    
}
