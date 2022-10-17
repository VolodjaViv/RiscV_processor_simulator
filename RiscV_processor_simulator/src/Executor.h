
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        switch(instr->_type)
        {
            case IType::Ld:     
            { 
                instr->_addr = alu_calculating(instr); 
                instr->_nextIp = ip + 4; 
                break; 
            }
            case IType::St:     
            { 
                instr->_addr = alu_calculating(instr); 
                instr->_data = instr->_src2Val; 
                instr->_nextIp = ip + 4; 
                break;
            }
            case IType::Csrw:   
            { 
                instr->_data = instr->_src1Val; 
                instr->_nextIp = ip + 4;
                break; 
            }
            case IType::Csrr:   
            { 
                instr->_data = instr->_csrVal; 
                instr->_nextIp = ip + 4;
                break; 
            }
            case IType::J:      
            { 
                instr->_data = ip + 4; 
            }
            case IType::Br:     
            { 
                if (logic_treating(instr)) instr->_nextIp = ip + *instr->_imm; 
                else instr->_nextIp = ip + 4;
                break;
            }
            case IType::Jr:     
            { 
                instr->_data = ip + 4; 
                if(logic_treating(instr)) instr->_nextIp = *instr->_imm + instr->_src1Val;
                else instr->_nextIp = ip + 4;
                break;
            }
            case IType::Auipc:  
            { 
                instr->_data = ip + *instr->_imm; 
                instr->_nextIp = ip + 4; 
                break; 
            }
            case IType::Alu:    
            { 
                instr->_data = alu_calculating(instr); 
                instr->_nextIp = ip + 4; 
                break; 
            }
        }
    }

private:
    bool logic_treating(InstructionPtr& instr)
    {
        Word operand_one, operand_two;
        if (instr->_src1) operand_one = instr->_src1Val;
        if (instr->_src2) operand_two = instr->_src2Val;
        switch (instr->_brFunc) 
        {
            case BrFunc::Eq :   
                return operand_one == operand_two;
            case BrFunc::Neq :  
                return operand_one != operand_two;
            case BrFunc::Lt :   
            { 
                int first = operand_one, second = operand_two;
                return first < second; 
            }
            case BrFunc::Ltu :  
            { 
                return operand_one < operand_two; 
            }
            case BrFunc::Ge :   
            { 
                int first = operand_one, second = operand_two;
                return first >= second; 
            }
            case BrFunc::Geu :  
                return operand_one >= operand_two;
            case BrFunc::AT :   
                return true;
            case BrFunc::NT :   
                return false;
        }
    }
    Word alu_calculating (InstructionPtr& instr)
    {
        Word  word;
        bool test_value = true;
        if (!instr->_src1) test_value = false;
        if (instr->_imm) word = *instr->_imm;
        else if (instr->_src2) word = instr->_src2Val;
        else test_value = false;

        if (test_value)
        {
            switch (instr->_aluFunc)
            {
                case AluFunc::Add:      
                    return instr->_src1Val + word;
                case AluFunc::Sub:      
                    return instr->_src1Val - word;
                case AluFunc::And:      
                    return instr->_src1Val & word;
                case AluFunc::Or:       
                    return instr->_src1Val | word;
                case AluFunc::Xor:      
                    return instr->_src1Val ^ word;
                case AluFunc::Slt:      
                    return instr->_src1Val < word;
                case AluFunc::Sltu:     
                    return instr->_src1Val < word;
                case AluFunc::Sll:      
                    return instr->_src1Val << (word % 32);
                case AluFunc::Srl:      
                    return instr->_src1Val >> (word % 32);
                case AluFunc::Sra:      
                { 
                    int temp = instr->_src1Val; 
                    temp = temp >> (word % 32); 
                    return Word(temp); 
                }
            }
        }
        return Word();
    }
};

#endif // RISCV_SIM_EXECUTOR_H
