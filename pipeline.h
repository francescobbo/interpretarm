#pragma once

#include <cstdint>

class Cpu;

typedef void (Cpu::*executor)();

class Pipeline {
public:
	Pipeline(const Cpu &cpu);

	bool ready();
	void step(uint64_t next, bool valid);
	executor next(bool &valid, uint64_t &currentInstruction);
	void flush();

private:
	const Cpu &cpu;

	int fill = 0;

	uint64_t fetch;
	uint64_t decode;
	executor execute;
	bool f_valid, d_valid, e_valid;
	uint64_t currentInstruction;
};
