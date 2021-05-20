module;

#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>

export module Translate;

export class VM {
	std::array<std::string(VM::*)(), 22> operations = {
		&VM::halt,&VM::set,&VM::push,&VM::pop,&VM::eq,
		&VM::gt,&VM::jmp,&VM::jt,&VM::jf,&VM::add,
		&VM::mult,&VM::mod,&VM::and_,&VM::or_,&VM::not_,
		&VM::rmem,&VM::wmem,&VM::call,&VM::ret,&VM::out,
		&VM::in,&VM::noop
	};
	std::vector<uint16_t> memory;
	uint16_t pc;
	
public:
	void load() {
		pc = 0;
		std::ifstream is;
		is.open("../challenge.bin", std::ios::binary);
		is.seekg(0, std::ios::end);
		size_t filesize = is.tellg();
		is.seekg(0, std::ios::beg);
		memory.resize(filesize);
		is.read((char*)memory.data(), filesize);
	}

	void run() {
		std::vector<std::function<std::string(void)>> boundOperations;
		for (auto op : operations) {
			boundOperations.push_back(std::bind(op, this));
		}
		while (pc < memory.size()) {
			uint16_t opcode = memory[pc];
			std::string line;
			uint16_t oldPc = pc;
			if (opcode < boundOperations.size())
				line = boundOperations[opcode]();
			else {
				pc++;
				line = "FILLER: " + std::to_string(opcode);
			}
			std::cout << oldPc << " " << line << std::endl;
		}
	}

private:
	std::string value(uint16_t x) {
		if (x >= 32776)
			throw std::out_of_range("received invalid value: " + x);
		return x <= 32767 ? std::to_string(x) : toRegisterIndex(x);
	}

	std::string toRegisterIndex(uint16_t x) {
		if (x < 32768 || x > 32775)
			return "cannot convert into register index: " + std::to_string(x);
		return "r" + std::to_string(x - 32768);
	}

	std::string halt() {
		pc += 1;
		return "halt";
	}

	std::string set() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "set " + a + " " + b;
	}

	std::string push() {
		std::string a = value(memory[pc + 1]);
		pc += 2;
		return "push " + a;
	}

	std::string pop() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		pc += 2;
		return "pop " + a;
	}

	std::string eq() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "eq " + a + " " + b + " " + c;
	}

	std::string gt() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "gt " + a + " " + b + " " + c;
	}

	std::string jmp() {
		std::string a = value(memory[pc + 1]);
		pc += 2;
		return "jmp " + a;
	}

	std::string jt() {
		std::string a = value(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "jt " + a + " " + b;
	}

	std::string jf() {
		std::string a = value(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "jf " + a + " " + b;
	}

	std::string add() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "add " + a + " " + b + " " + c;
	}

	std::string mult() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "mul " + a + " " + b + " " + c;
	}

	std::string mod() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "mod " + a + " " + b + " " + c;
	}

	std::string and_() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "and " + a + " " + b + " " + c;
	}

	std::string or_() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		std::string c = value(memory[pc + 3]);
		pc += 4;
		return "or " + a + " " + b + " " + c;
	}

	std::string not_() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "not " + a + " " + b;
	}

	std::string rmem() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "rmem " + a + " " + b;
	}

	std::string wmem() {
		std::string a = value(memory[pc + 1]);
		std::string b = value(memory[pc + 2]);
		pc += 3;
		return "wmem " + a + " " + b;
	}

	std::string call() {
		std::string a = value(memory[pc + 1]);
		pc += 2;
		return "call " + a;
	}

	std::string ret() {
		pc += 1;
		return "ret";
	}

	std::string out() {
		std::string a = value(memory[pc + 1]);
		pc += 2;
		return "out " + a;
	}

	std::string in() {
		std::string a = toRegisterIndex(memory[pc + 1]);
		pc += 2;
		return "in " + a;
	}

	std::string noop() {
		pc++;
		return "noop";
	}
};