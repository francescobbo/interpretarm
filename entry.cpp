#include "cpu.h"

class Memory : public MemoryInterface {
public:
	uint32_t readW(uint32_t addr, bool &valid) {
		valid = true;
		return 0;
	}

	void writeW(uint32_t addr, uint32_t val, bool &valid) {
		valid = true;
	}
};

int main() {
	Memory mem;
	Cpu cpu(mem);

	cpu.step();
	cpu.step();
	cpu.step();
	cpu.step();
	cpu.step();
	cpu.step();
	cpu.step();

	return 0;
}