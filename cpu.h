#pragma once

#include "pipeline.h"
#include "memory.h"

#define CHECK_CONDITION if (!conditionCheck()) return

#define Nidx	31
#define Zidx	30
#define Cidx	29
#define Vidx	28
#define F_N	(cpsr & (1 << Nidx))
#define F_Z	(cpsr & (1 << Zidx))
#define F_C	(cpsr & (1 << Cidx))
#define F_V	(cpsr & (1 << Vidx))

#define I_BYTE(x)	(currentInstruction & (1 << (x)))

#define I_0	I_BYTE(0)
#define I_1	I_BYTE(1)
#define I_2	I_BYTE(2)
#define I_3	I_BYTE(3)
#define I_4	I_BYTE(4)
#define I_5	I_BYTE(5)
#define I_6	I_BYTE(6)
#define I_7	I_BYTE(7)
#define I_8	I_BYTE(8)
#define I_9	I_BYTE(9)
#define I_10	I_BYTE(10)
#define I_11	I_BYTE(11)
#define I_12	I_BYTE(12)
#define I_13	I_BYTE(13)
#define I_14	I_BYTE(14)
#define I_15	I_BYTE(15)
#define I_16	I_BYTE(16)
#define I_17	I_BYTE(17)
#define I_18	I_BYTE(18)
#define I_19	I_BYTE(19)
#define I_20	I_BYTE(20)
#define I_21	I_BYTE(21)
#define I_22	I_BYTE(22)
#define I_23	I_BYTE(23)
#define I_24	I_BYTE(24)
#define I_25	I_BYTE(25)
#define I_26	I_BYTE(26)
#define I_27	I_BYTE(27)
#define I_28	I_BYTE(28)
#define I_29	I_BYTE(29)
#define I_30	I_BYTE(30)
#define I_31	I_BYTE(31)

#define I_S					I_20
#define I_LINK			I_24
#define I_SIMMED_24	((int32_t) (currentInstruction & 0xFFFFFF))
#define I_OPCODE		((currentInstruction >> 21) & 0xF)

#define SP	regs[13]
#define LR	regs[14]
#define PC	regs[15]

class Cpu {
public:
	Cpu(MemoryInterface &memory);
	
	void step();

	executor decode(uint64_t instruction) const;

private:
	bool conditionCheck();

	void opADC();
	void opADD();
	void opAND();
	void opB();
	void opBIC();
	void opBKPT();
	void opBL();
	void opBLX();
	void opBX();
	void opBXJ();
	void opCLZ();
	void opCMN();
	void opCMP();
	void opEOR();
	void opLDM();
	void opLDR();
	void opLDRB();
	void opLDRBT();
	void opLDRD();
	void opLDREX();
	void opLDRSB();
	void opLDRSH();
	void opLDRT();
	void opMLA();
	void opMOV();
	void opMRS();
	void opMSR();
	void opMUL();
	void opMVN();
	void opORR();
	void opQADD();
	void opQDADD();
	void opQDSUB();
	void opQSUB();
	void opRSB();
	void opRSC();
	void opSBC();
	void opSMLA();
	void opSMLAL();
	void opSMLAW();
	void opSMUL();
	void opSMULL();
	void opSMULW();
	void opSTM();
	void opSTR();
	void opSTRB();
	void opSTRBT();
	void opSTRD();
	void opSTREX();
	void opSTRH();
	void opSTRT();
	void opSUB();
	void opSWI();
	void opSWP();
	void opSWPB();
	void opTEQ();
	void opTST();
	void opUMAAL();
	void opUMLAL();
	void opUMULL();

	uint32_t regs[16];
	uint32_t cpsr;

	Pipeline pipeline;
	MemoryInterface &memory;
	uint64_t currentInstruction;
};
