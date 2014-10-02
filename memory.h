#pragma once

class MemoryInterface {
public:
	virtual uint32_t readW(uint32_t addr, bool &valid) = 0;
	virtual void writeW(uint32_t addr, uint32_t value, bool &valid) = 0;	
};
