module;

#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>

export module VM;

export class VM {
	std::array<void(VM::*)(), 22> operations = { 
		&VM::halt,& VM::set,& VM::push,& VM::pop,& VM::eq,
		& VM::gt,& VM::jmp,& VM::jt,& VM::jf,& VM::add,
		& VM::mult,& VM::mod,& VM::and_,& VM::or_,& VM::not_,
		& VM::rmem,& VM::wmem,& VM::call,& VM::ret,& VM::out,
		& VM::in,& VM::noop
	};
	std::array<uint16_t, 32768> memory;
	std::array<uint16_t, 8> registers;
	std::vector<uint16_t> stack;
	uint16_t pc;
	bool done;
	std::string input =		// input has been logged, the following string is a savepoint
#pragma region savepoint
"\nrorrim esu\
\nrorrim ekat\
\ntluav\
\ntsae\
\nhtron\
\nhtron\
\ntsew\
\ntsae\
\ntsae\
\nhtuos\
\ntsew\
\nhtron\
\ntsae\
\ntsae\
\nhtron\
\nbro ekat\
\nhtron\
\nhtron\
\ntsew\
\nlanruoj ekat\
\ntsae\
\nhtron\
\nhtron\
\nhtron\
\nhtron\
\nhtron\
\nhtron\
\nhtron\
\nretropelet esu\
\nkoob egnarts ekat\
\ndrac ssenisub ekat\
\nretropelet esu\
\nretropelet ekat\
\nhtron\
\nnioc dedorroc esu\
\nnioc evacnoc esu\
\nnioc ynihs esu\
\nnioc der esu\
\nnioc eulb esu\
\ntsew\
\npu\
\nnioc dedorroc ekat\
\nnwod\
\nnioc evacnoc ekat\
\ntsae\
\ntsae\
\nnwod\
\nnioc ynihs ekat\
\npu\
\nnioc eulb ekat\
\ntsew\
\nhtron\
\nnioc der ekat\
\nhtron\
\ntsew\
\ntsew\
\ntsew\
\ntsew\
\neunitnoc\
\nssenkrad\
\nreddal\
\nvni\
\nnretnal esu\
\nnac esu\
\nvni\
\ntsew\
\nnac ekat\
\nhtron\
\nhtuos\
\ntsew\
\nreddal\
\negassap\
\ntsew\
\ntsew\
\nnretnal ytpme ekat\
\ntsae\
\nnwod\
\neunitnoc\
\negdirb\
\nhtron\
\nhtron\
\nyawrood";
#pragma endregion
	bool atHeadQuarters = false;	// flag is needed during playback of input to collect items at headquarters
	std::string allInputs;		// used for input logging in order to create savepoints

public:
	void load() {
		memory.fill(0);
		registers.fill(0);
		stack.clear();
		pc = 0;
		done = false;
		std::ifstream is;
		is.open("../challenge.bin", std::ios::binary);
		is.seekg(0, std::ios::end);
		size_t filesize = is.tellg();
		is.seekg(0, std::ios::beg);
		is.read((char*)memory.data(), filesize);
	}

	void run() {
		std::vector<std::function<void(void)>> boundOperations;
		for (auto op : operations) {
			boundOperations.push_back(std::bind(op, this));
		}
		while (!done) {
			if (pc == 5489) {
				pc += 2;			// skip ackermann function at 6027
				registers[0] = 6;	// overwrite return value of ackermann function at 6027
			}
			uint16_t opcode = memory[pc];
			boundOperations[opcode]();
			if (pc == 5451) {
				if (!atHeadQuarters) 
					atHeadQuarters = true;
				else 
					registers[7] = 25734;	// overwriting eight register with result from ackermann: r7 has been chosen such that ackermann(4, 1) == 6
			}
		}
		reverse(allInputs);
		std::cout << allInputs << std::endl;	// used to create savepoints
	}

private:
	uint16_t value(uint16_t x) {
		if (x >= 32776)
			throw std::out_of_range("received invalid value: " + x);
		return x <= 32767 ? x : registers[toRegisterIndex(x)];
	}

	uint16_t toRegisterIndex(uint16_t x) {
		if (x < 32768 || x > 32775)
			throw std::out_of_range("cannot convert into register index: " + x);
		//if (x - 32768 == 7 && input == "") {	// figuring out what happens after register 8 is being accessed
		//	std::cout << "eight register is being accessed (set breakpoint...)" << std::endl;
		//}
		return x - 32768;
	}

	void halt() {
		done = true;
	}

	void set() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		registers[a] = b;
		pc += 3;
	}

	void push() {
		uint16_t a = value(memory[pc + 1]);
		stack.push_back(a);
		pc += 2;
	}

	void pop() {
		if (stack.size() == 0) {
			throw  std::logic_error("trying to pop from empty stack!");
		}
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		registers[a] = stack.back();
		stack.pop_back();
		pc += 2;
	}

	void eq() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = b == c ? 1 : 0;
		pc += 4;
	}

	void gt() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = b > c ? 1 : 0;
		pc += 4;
	}

	void jmp() {
		uint16_t a = value(memory[pc + 1]);
		pc = a;
	}

	void jt() {
		uint16_t a = value(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		pc = a != 0 ? b : pc + 3;
	}

	void jf() {
		uint16_t a = value(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		pc = a == 0 ? b : pc + 3;
	}

	void add() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = (b + c) % 32768;
		pc += 4;
	}

	void mult() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = (b * c) % 32768;
		pc += 4;
	}

	void mod() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = b % c;
		pc += 4;
	}

	void and_() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = b & c;
		pc += 4;
	}

	void or_() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		uint16_t c = value(memory[pc + 3]);
		registers[a] = b | c;
		pc += 4;
	}

	void not_() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		registers[a] = 0b111111111111111 ^ b;
		pc += 3;
	}

	void rmem() {
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		registers[a] = memory[b];
		pc += 3;
	}

	void wmem() {
		uint16_t a = value(memory[pc + 1]);
		uint16_t b = value(memory[pc + 2]);
		memory[a] = b;
		pc += 3;
	}

	void call() {
		uint16_t a = value(memory[pc + 1]);
		stack.push_back(pc + 2);
		pc = a;
	}

	void ret() {
		if (stack.size() == 0) {
			done = true;
		}
		uint16_t a = stack.back();
		stack.pop_back();
		pc = a;
	}

	void out() {
		uint16_t a = value(memory[pc + 1]);
		std::cout << (char) a;
		pc += 2;
	}

	void reverse(std::string& word) {
		for (int i = 0; i < word.size() / 2; i++) {
			char tmp = word[i];
			word[i] = word[word.size() - 1 - i];
			word[word.size() - 1 - i] = tmp;
		}
	}

	void in() {
		if (input == "") {
			std::getline(std::cin, input);
			if (input == "quit") {				// custom command
				done = true;
				return;
			}
			input.push_back('\n');
			allInputs.append(input);			// input logging for savepoints
			reverse(input);
		}
		uint16_t a = toRegisterIndex(memory[pc + 1]);
		uint16_t b = input.back();
		input.pop_back();
		registers[a] = b;
		pc += 2;
	}

	void noop() {
		pc++;
	}
};