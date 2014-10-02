#pragma once

#include "pipeline.h"
#include "memory.h"

#define CHECK_CONDITION if (!conditionCheck()) return false

#define Nidx	31
#define Zidx	30
#define Cidx	29
#define Vidx	28
#define F_N	(cpsr & (1 << Nidx))
#define F_Z	(cpsr & (1 << Zidx))
#define F_C	(cpsr & (1 << Cidx))
#define F_V	(cpsr & (1 << Vidx))

#define I_4	(currentInstruction & (1 << 4))
#define I_7	(currentInstruction & (1 << 7))
#define I_S	(currentInstruction & (1 << 20))
#define I_OPCODE	((currentInstruction >> 21) & 0xF)

#define PC	regs[15]

class Cpu {
public:
	Cpu(MemoryInterface &memory);
	
	void step();

	executor decode(uint64_t instruction) const;

private:
	bool conditionCheck();

	uint32_t regs[16];
	uint32_t cpsr;

	Pipeline pipeline;
	MemoryInterface &memory;
	uint64_t currentInstruction;
};
