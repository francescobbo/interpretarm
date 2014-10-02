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
	if (currentInstruction >> 28 == 0xF) {
		// Unconditional instructions A3-6
		return nullptr;
	}

	switch ((instruction >> 25) & 7) {
		case 0:
			if ((I_4 && !I_7) || !I_4) {
				if ((I_OPCODE & 0xC) == 8 && !I_S) {
					// Miscellaneous A3-4
				} else {
					// Data Processing Immediate/Register shift
				}
			} else {
				// Multiplies A3-3, Extra load/store A3-5
			}
		case 1:
			if ((I_OPCODE & 0xD) == 9 && !I_S) {
				// Move immediate to status register
			} else if ((I_OPCODE & 0xD) == 8 && !I_S) {
				// Undefined instruction
			} else {
				// Data processing immediate
				switch (I_OPCODE) {
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
						break;
				}
			}
		case 2:
			// Load/store immediate offset
		case 3:
			if (!I_4) {
				// Load/store register offset
			} else {
				if ((currentInstruction & 0x07F000F0) == 0x07F000F0) {
					// Undefined
				} else {
					// Media instructions, A3-2
				}
			}
		case 4:
			// Load/store multiple
		case 5:
			// Branch and branch with link
		case 6:
			// Coprocessor load/store and double register transfers
		case 7:
			if (currentInstruction & 0x01000000) {
				// SWI
			} else {
				if (!I_4) {
					// Coprocessor data processing
				} else {
					// Coprocessor register transfers
				}
			}
	}

	return nullptr;
}

bool Cpu::conditionCheck() {
	uint32_t condition = currentInstruction >> 28;

	switch (condition) {
		case 0:
			return F_Z;
		case 1:
			return !F_Z;
		case 2:
			return F_C;
		case 3:
			return !F_C;
		case 4:
			return F_N;
		case 5:
			return !F_N;
		case 6:
			return F_V;
		case 7:
			return !F_V;
		case 8:
			return F_C && !F_Z;
		case 9:
			return !F_C || F_Z;
		case 10:
			return (F_N && F_V) || (!F_N && !F_V);
		case 11:
			return (F_N && !F_V) || (!F_N && F_V);
		case 12:
			return !F_Z && ((F_N && F_V) || (!F_N && !F_V));
		case 13:
			return F_Z || (F_N && !F_V) || (!F_N && F_V);
		case 14:
			return true;
		case 15:
			return false;
	}

	return false;
}
