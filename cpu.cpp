#include "cpu.h"

#include <iostream>
using namespace std;

Cpu::Cpu(MemoryInterface &memory) : pipeline(*this), memory(memory) {
}

void Cpu::step() {
	for (int i = 0; i < 3; i++) {
		bool validRead;
		uint64_t instruction = memory.readW(PC, validRead);

		pipeline.step(instruction, validRead);
		if (pipeline.ready())
			break;
		PC += 4;
	}

	bool valid;
	executor exec = pipeline.next(valid, currentInstruction);

	if (valid && exec)
		(*this.*exec)();
	else
		;// Fetch abort
}

executor Cpu::decode(uint64_t instruction) const {
	return nullptr;
}
