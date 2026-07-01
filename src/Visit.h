// Some help function to visit Koopa IR in memory format 
// to avoid writing a lot of loop code 
#include "koopa.h"
#include <string>

void GenRISCV(std::string s, std::string &output);

void Visit(const koopa_raw_program_t &program);
void Visit(const koopa_raw_slice_t &slice);
void Visit(const koopa_raw_function_t &func);
void Visit(const koopa_raw_basic_block_t &bb);
void Visit(const koopa_raw_value_t &value);
