#pragma once

#include "pipeline.h"
#include "memory.h"

#define PC    regs[15]

class Cpu {
public:
	Cpu(MemoryInterface &memory);
	
	void step();

	executor decode(uint64_t instruction) const;

private:
	void dummyA();
	void dummyB();
	void dummyC();

	uint32_t regs[16];

	Pipeline pipeline;
	MemoryInterface &memory;
	uint64_t currentInstruction;
};
