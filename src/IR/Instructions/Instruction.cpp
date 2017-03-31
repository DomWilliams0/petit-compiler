#include "Instruction.h"

using namespace IR;

Instruction::Instruction() {

}

std::vector<sh_Register> Instruction::getReadRegisterVector() const {
	return readRegisterVector;
}

std::vector<sh_Register> Instruction::getWroteRegisterVector() const {
	return wroteRegisterVector;
}

std::vector<sh_Data> Instruction::getReadMemoryVector() const {
	return readMemoryVector;
}

std::vector<sh_Data> Instruction::getWroteMemoryVector() const {
	return wroteMemoryVector;
}

std::string Instruction::toAsm(int asmType) const {
	std::string ret;
	switch (asmType) {
	case ASM_TYPE_X86:
		ret = toX86();
		break;
	default:
		break;
	}
	return ret;
}
