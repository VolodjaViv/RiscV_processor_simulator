//
// Created by Vahmurka on 31.05.2020.
// Вариант 4, jalr
//

#ifndef RISCV_SIM_TESTD_H
#define RISCV_SIM_TESTD_H
#include "Decoder.h"
#include "RegisterFile.h"
#include "Executor.h"
#include <iostream>

class TestD{
public:
    static void Test()
    {
        if (TestDecoder()) std::cout << "True";
        else std::cout << "False";

        if(TestEx()) std::cout << "True";
        else std ::cout <<"False";
    }

private:
    static bool TestDecoder()
    {
        Decoder dec;
        Word jalr = 0b00000000001000000001100111100111;
        const InstructionPtr &instruction = dec.Decode(jalr);
        return instruction->_imm.value() == 2 && instruction->_dst.has_value();
    }

    static bool TestEx()
    {
        RegisterFile rf(std::array<Word, 32>());
        Decoder dec;
        Executor ex;
        Word Jalr = 0b00000000001000000001100111100111;
        const InstructionPtr &instruction = dec.Decode(Jalr);
        rf.Read(const_cast<InstructionPtr &>(instruction));
        ex.Execute(const_cast<InstructionPtr &>(instruction), 0x200);
        return instruction->_data == 516;
    }
};