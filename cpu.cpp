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
					if (!I_7) {
						switch ((currentInstruction >> 4) & 7) {
							case 0:
								if (!I_21)
									return &Cpu::opMSR;
								else
									return &Cpu::opMRS;
							case 1:
								if (!I_22)
									return &Cpu::opBX;
								else
									return &Cpu::opCLZ;
							case 2:
								return &Cpu::opBXJ;
							case 3:
								return &Cpu::opBLX;
							// case 4: not used
							case 5:
								switch ((currentInstruction >> 21) & 3) {
									case 0:
										return &Cpu::opQADD;
									case 1:
										return &Cpu::opQSUB;
									case 2:
										return &Cpu::opQDADD;
									case 3:
										return &Cpu::opQDSUB;
								}
							// case 6: not used
							case 7:
								return &Cpu::opBKPT;
						}
					} else {
						// Signed multiplies (type 2)
						switch ((currentInstruction >> 21) & 3) {
							case 0:
								return &Cpu::opSMLA;
							case 1:
								if (!I_5)
									return &Cpu::opSMLAW;
								else
									return &Cpu::opSMULW;
							case 2:
								return &Cpu::opSMLAL;
							case 3:
								return &Cpu::opSMUL;
						}
					}
				} else {
					switch (I_OPCODE) {
						case 0:
							return &Cpu::opAND;
						case 1:
							return &Cpu::opEOR;
						case 2:
							return &Cpu::opSUB;
						case 3:
							return &Cpu::opRSB;
						case 4:
							return &Cpu::opADD;
						case 5:
							return &Cpu::opADC;
						case 6:
							return &Cpu::opSBC;
						case 7:
							return &Cpu::opRSC;
						case 8:
							return &Cpu::opTST;
						case 9:
							return &Cpu::opTEQ;
						case 10:
							return &Cpu::opCMP;
						case 11:
							return &Cpu::opCMN;
						case 12:
							return &Cpu::opORR;
						case 13:
							return &Cpu::opMOV;
						case 14:
							return &Cpu::opBIC;
						case 15:
							return &Cpu::opMVN;
					}
				}
			} else {
				if ((currentInstruction & 0xF0) == 0x90) {
					if (!I_24) {
						// Multiplies
						if ((currentInstruction & 0x00E00000) == 0x200000) {
							return &Cpu::opMLA;
						} else if ((currentInstruction & 0x00E00000) == 0) {
							return &Cpu::opMUL;
						} else if ((currentInstruction & 0x00F00000) == 0x400000) {
							return &Cpu::opUMAAL;
						} else if (currentInstruction & 0x00800000) {
							if (currentInstruction & 0x00400000) {
								if (currentInstruction & 0x00200000) {
									return &Cpu::opUMLAL;
								} else {
									return &Cpu::opUMULL;
								}
							} else {
								if (currentInstruction & 0x00200000) {
									return &Cpu::opSMLAL;	//xy
								} else {
									return &Cpu::opSMULL;
								}
							}
						}
					} else {
						if (!I_23) {
							if (!I_22) {
								return &Cpu::opSWP;
							} else {
								return &Cpu::opSWPB;
							}
						} else {
							if (!I_20) {
								return &Cpu::opSTREX;
							} else {
								return &Cpu::opLDREX;
							}
						}
					}
				} else {
					// Extra load/store A3-5
					if (!I_6) {
						if (!I_20) {
							return &Cpu::opSTRH;
						} else {
							return &Cpu::opSTRH;
						}
					} else {
						if (!I_20) {
							if (!I_5) {
								return &Cpu::opLDRD;
							} else {
								return &Cpu::opSTRD;
							}
						} else {
							if (!I_5) {
								return &Cpu::opLDRSB;
							} else {
								return &Cpu::opLDRSH;
							}
						}
					}
				}
			}
		case 1:
			if ((I_OPCODE & 0xD) == 9 && !I_S) {
				return &Cpu::opMSR;
			} else if ((I_OPCODE & 0xD) == 8 && !I_S) {
				// Undefined instruction
				return nullptr;
			} else {
				// Data processing immediate
				switch (I_OPCODE) {
					case 0:
						return &Cpu::opAND;
					case 1:
						return &Cpu::opEOR;
					case 2:
						return &Cpu::opSUB;
					case 3:
						return &Cpu::opRSB;
					case 4:
						return &Cpu::opADD;
					case 5:
						return &Cpu::opADC;
					case 6:
						return &Cpu::opSBC;
					case 7:
						return &Cpu::opRSC;
					case 8:
						return &Cpu::opTST;
					case 9:
						return &Cpu::opTEQ;
					case 10:
						return &Cpu::opCMP;
					case 11:
						return &Cpu::opCMN;
					case 12:
						return &Cpu::opORR;
					case 13:
						return &Cpu::opMOV;
					case 14:
						return &Cpu::opBIC;
					case 15:
						return &Cpu::opMVN;
				}
			}
		case 2:
		case 3:
			if (!I_25 || (I_25 && !I_4)) {
				// Load/store
				if (!I_20) {
					if (!(I_21 && !I_24)) {
						if (!I_22) {
							return &Cpu::opSTR;
						} else {
							return &Cpu::opSTRB;
						}
					} else {
						if (!I_22) {
							return &Cpu::opSTRT;
						} else {
							return &Cpu::opSTRBT;
						}
					}
				} else {
					if (!(I_21 && !I_24)) {
						if (!I_22) {
							return &Cpu::opLDR;
						} else {
							return &Cpu::opLDRB;
						}
					} else {
						if (!I_22) {
							return &Cpu::opLDRT;
						} else {
							return &Cpu::opLDRBT;
						}
					}
				}
			} else {
				if ((currentInstruction & 0x07F000F0) == 0x07F000F0) {
					// Undefined
					return nullptr;
				} else {
					switch ((currentInstruction >> 23) & 3) {
						case 0:
							// Parallel add subtract
						case 1:
							if (!I_5) {
								if (!I_21) {
									// Halfword pack
								} else {
									// Word saturate
								}
							} else {
								if (!I_6) {
									if (!I_7) {
										if (!I_20) {
											// Parallel halfword saturate
										} else {
											// Byte reverse word
										}
									} else {
										if (!I_20) {
											// Select bytes
										} else {
											if (!I_22) {
												// Byte reverse packed halfword
											} else {
												// Byte reverse signed halfword
											}
										}
									}
								} else {
									// Sign/zero extend (add)
								}
							}
						case 2:
							// Multiplies (type 3)
						case 3:
							if (((currentInstruction >> 12) & 0xF) == 15) {
								// Unsigned sum of absolute differences, acc
							} else {
								// Unsigned sum of absolute differences
							}
					}
				}
			}
		case 4:
			// Load/store multiple
			if (!I_20) {
				return &Cpu::opSTM;
			} else {
				return &Cpu::opLDM;
			}
		case 5:
			if (I_LINK)
				return &Cpu::opBL;
			else
				return &Cpu::opB;
		case 6:
			// Coprocessor load/store and double register transfers
		case 7:
			if (currentInstruction & 0x01000000) {
				return &Cpu::opSWI;
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

void Cpu::opADC() {
	CHECK_CONDITION;
}

void Cpu::opADD() {
	CHECK_CONDITION;
}

void Cpu::opAND() {
	CHECK_CONDITION;
}

void Cpu::opB() {
	CHECK_CONDITION;

	int32_t delta = (I_SIMMED_24 << 8) >> 2;
	PC += delta;
	pipeline.flush();
}

void Cpu::opBIC() {
	CHECK_CONDITION;
}

void Cpu::opBKPT() {
	CHECK_CONDITION;
}

void Cpu::opBL() {
	CHECK_CONDITION;

	LR = PC - 4;

	int32_t delta = (I_SIMMED_24 << 8) >> 2;
	PC += delta;
	pipeline.flush();
}

void Cpu::opBLX() {
	CHECK_CONDITION;
}

void Cpu::opBX() {
	CHECK_CONDITION;
}

void Cpu::opBXJ() {
	CHECK_CONDITION;
}

void Cpu::opCLZ() {
	CHECK_CONDITION;
}

void Cpu::opCMN() {
	CHECK_CONDITION;
}

void Cpu::opCMP() {
	CHECK_CONDITION;
}

void Cpu::opEOR() {
	CHECK_CONDITION;
}

void Cpu::opLDM() {
	CHECK_CONDITION;
}

void Cpu::opLDR() {
	CHECK_CONDITION;
}

void Cpu::opLDRB() {
	CHECK_CONDITION;
}

void Cpu::opLDRBT() {
	CHECK_CONDITION;
}

void Cpu::opLDRD() {
	CHECK_CONDITION;
}

void Cpu::opLDREX() {
	CHECK_CONDITION;
}

void Cpu::opLDRSB() {
	CHECK_CONDITION;
}

void Cpu::opLDRSH() {
	CHECK_CONDITION;
}

void Cpu::opLDRT() {
	CHECK_CONDITION;
}

void Cpu::opMLA() {
	CHECK_CONDITION;
}

void Cpu::opMOV() {
	CHECK_CONDITION;
}

void Cpu::opMRS() {
	CHECK_CONDITION;
}

void Cpu::opMSR() {
	CHECK_CONDITION;
}

void Cpu::opMUL() {
	CHECK_CONDITION;
}

void Cpu::opMVN() {
	CHECK_CONDITION;
}

void Cpu::opORR() {
	CHECK_CONDITION;
}

void Cpu::opQADD() {
	CHECK_CONDITION;
}

void Cpu::opQDADD() {
	CHECK_CONDITION;
}

void Cpu::opQDSUB() {
	CHECK_CONDITION;
}

void Cpu::opQSUB() {
	CHECK_CONDITION;
}

void Cpu::opRSB() {
	CHECK_CONDITION;
}

void Cpu::opRSC() {
	CHECK_CONDITION;
}

void Cpu::opSBC() {
	CHECK_CONDITION;
}

void Cpu::opSMLA() {
	CHECK_CONDITION;
}

void Cpu::opSMLAL() {
	CHECK_CONDITION;
}

void Cpu::opSMLAW() {
	CHECK_CONDITION;
}

void Cpu::opSMUL() {
	CHECK_CONDITION;
}

void Cpu::opSMULL() {
	CHECK_CONDITION;
}

void Cpu::opSMULW() {
	CHECK_CONDITION;
}

void Cpu::opSTM() {
	CHECK_CONDITION;
}

void Cpu::opSTR() {
	CHECK_CONDITION;
}

void Cpu::opSTRB() {
	CHECK_CONDITION;
}

void Cpu::opSTRBT() {
	CHECK_CONDITION;
}

void Cpu::opSTRD() {
	CHECK_CONDITION;
}

void Cpu::opSTREX() {
	CHECK_CONDITION;
}

void Cpu::opSTRH() {
	CHECK_CONDITION;
}

void Cpu::opSTRT() {
	CHECK_CONDITION;
}

void Cpu::opSUB() {
	CHECK_CONDITION;
}

void Cpu::opSWI() {
	CHECK_CONDITION;
}

void Cpu::opSWP() {
	CHECK_CONDITION;
}

void Cpu::opSWPB() {
	CHECK_CONDITION;
}

void Cpu::opTEQ() {
	CHECK_CONDITION;
}

void Cpu::opTST() {
	CHECK_CONDITION;
}

void Cpu::opUMAAL() {
	CHECK_CONDITION;
}

void Cpu::opUMLAL() {
	CHECK_CONDITION;
}

void Cpu::opUMULL() {
	CHECK_CONDITION;
}
