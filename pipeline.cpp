#include "pipeline.h"
#include "cpu.h"

Pipeline::Pipeline(const Cpu &cpu) : cpu(cpu) {
	flush();
}

executor Pipeline::next(bool &valid, uint64_t &currentInstruction) {
	valid = e_valid;
	currentInstruction = this->currentInstruction;
	return execute;
}

bool Pipeline::ready() {
	return fill == 3;	
}

void Pipeline::step(uint64_t next, bool valid) {
	currentInstruction = decode;
	if (d_valid)
		execute = cpu.decode(currentInstruction);
	else
		execute = nullptr;
	decode = fetch;
	fetch = next;

	e_valid = d_valid;
	d_valid = f_valid;
	f_valid = valid;

	fill = fill < 3 ? fill + 1 : fill;
}

void Pipeline::flush() {
	fetch = decode = 0;
	execute = nullptr;
	fill = 0;
	f_valid = d_valid = e_valid = false;
}
