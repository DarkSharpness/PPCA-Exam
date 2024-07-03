#include <cassert>
#include <iostream>
#include <set>
using ull = unsigned long long;

enum InstructionType {
	Nop,
	Li,// 加载立即数，将某个寄存器设置为某个值
	Load,
	Store,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	And,
	Or,
	Xor,
	Not,
	ShiftL,
	ShiftR,
	// 为了简化问题，branch指令表示的*不是*偏移量，而是跳转的目标地址
	// 详见代码中的实现
	BranchEq,
	BranchNe,
	BranchLt,
	BranchLe,
	BranchGt,
	BranchGe,
	EnterNewEnclave,
	ExitEnclave,
	DoSubSystem,
	Print,
};

struct Instruction {
	InstructionType type : 8;
	unsigned int rd : 5, rs1 : 5, rs2 : 5;
	unsigned int imm : 9;// immediate value only used in branch and li
};

class VM {
	constexpr static int MEM_SIZE = 1 << 12;

	constexpr static int ENCLAVE_COUNT = 64;
	constexpr static int ENCLAVE_SIZE = 8;// to simplify, every enclave only have one dword
	constexpr static int ENCLAVE_START = MEM_SIZE - ENCLAVE_COUNT * ENCLAVE_SIZE;
	enum Privilege {
		MACHINE = 3,
		SUPERVISOR = 1,
		USER = 0
	};

	ull regs[32] = {};
	unsigned char mem[MEM_SIZE] = {};// 4KB
	int currentEnclave = -1;

	unsigned int enclave_keys[ENCLAVE_COUNT] = {};
	bool encrpyted[ENCLAVE_COUNT] = {};
	int enclave_cnt = 0;

	unsigned long long flag = 0;
	unsigned int seed = 0;

public:
	explicit VM(std::istream &in, unsigned int seed, unsigned long long flag) : seed(seed), flag(flag) {
		load_instructions(in);
	}

private:
	unsigned long long &get_enclave_mem(int id) {
		assert(id >= 0 && id < ENCLAVE_COUNT);
		return *reinterpret_cast<unsigned long long *>(mem + ENCLAVE_START + id * ENCLAVE_SIZE);
	}
	Instruction &get_instruction(int pc) {
		return *reinterpret_cast<Instruction *>(mem + pc);
	}
	unsigned int rand() {
		// 1739 may have some special property
		auto ret = seed;
		seed = (unsigned long long) (seed) * (seed + 1739ull) % 998244353;
		return ret;
	}
	void do_load(Instruction ins) {
		auto addr = regs[ins.rs1];
		if (addr < 0 || addr > MEM_SIZE)
			throw std::runtime_error("load out of memory");
		auto val = *reinterpret_cast<unsigned long long *>(mem + addr);
		// if accessing current enclave memory, decrypt the data
		if (addr >= ENCLAVE_START && (addr - ENCLAVE_START) / ENCLAVE_SIZE == currentEnclave && encrpyted[currentEnclave])
			val = decrypt(val, currentEnclave);
		regs[ins.rd] = val;
	}
	void do_store(Instruction ins) {
		auto addr = regs[ins.rs1];
		if (addr < 0 || addr > MEM_SIZE)
			throw std::runtime_error("store out of memory");
		// if accessing current enclave memory, encrypt the data
		if (addr >= ENCLAVE_START && currentEnclave == (addr - ENCLAVE_START) / ENCLAVE_SIZE) {
			*reinterpret_cast<unsigned long long *>(mem + addr) = encrypt(regs[ins.rs2], currentEnclave);
			encrpyted[currentEnclave] = true;
			return;
		}
		*reinterpret_cast<unsigned long long *>(mem + addr) = regs[ins.rs2];
		if (addr > ENCLAVE_START)
			encrpyted[(addr - ENCLAVE_START) / ENCLAVE_SIZE] = false;
	}
	int do_enter_enclave(Instruction ins) {
		int enclave_id = enclave_cnt++;
		enclave_cnt %= ENCLAVE_COUNT;
		currentEnclave = enclave_id;
		enclave_keys[enclave_id] = rand();
		encrpyted[enclave_id] = false;
		get_enclave_mem(enclave_id) = 0;// init the enclave memory
		return enclave_id;
	}
	void do_exit_enclave(Instruction ins) {
		currentEnclave = -1;
	}
	void do_do_subsystem(Instruction ins) {
		int id = do_enter_enclave({});
		auto &enclave_mem = get_enclave_mem(id);
		// to simulate the sub system do some work and saved some data in memory
		enclave_mem = encrypt(flag, id);
		encrpyted[id] = true;
		do_exit_enclave({});
	}
	unsigned long long encrypt(unsigned long long val, int enclave_id) {
		return val ^ enclave_keys[enclave_id];
	}
	unsigned long long decrypt(unsigned long long val, int enclave_id) {
		return val ^ enclave_keys[enclave_id];
	}
	void load_instructions(std::istream &in) {
		int n = 0;
		in >> n;
		if (n > 500)
			throw std::runtime_error("too many instructions");
		for (int i = 0; i < n; ++i) {
			std::string op;
			unsigned int rs1 = 0, rs2 = 0, rd = 0, imm = 0;
			in >> op;
			std::cout << "deal with op:" << op << std::endl;
			if (op == "li") {// load immediate
				in >> rd >> imm;
				get_instruction(i * 4) = {Li, rd, 0, 0, imm};
			}
			else if (op == "ld") {// load double
				in >> rd >> rs1;
				get_instruction(i * 4) = {Load, rd, rs1, 0, 0};
			}
			else if (op == "sd") {// store double
				in >> rs1 >> rs2;
				get_instruction(i * 4) = {Store, 0, rs1, rs2, 0};
			}
			else if (op == "add") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Add, rd, rs1, rs2, 0};
			}
			else if (op == "sub") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Sub, rd, rs1, rs2, 0};
			}
			else if (op == "mul") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Mul, rd, rs1, rs2, 0};
			}
			else if (op == "div") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Div, rd, rs1, rs2, 0};
			}
			else if (op == "mod") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Mod, rd, rs1, rs2, 0};
			}
			else if (op == "and") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {And, rd, rs1, rs2, 0};
			}
			else if (op == "or") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Or, rd, rs1, rs2, 0};
			}
			else if (op == "xor") {
				in >> rd >> rs1 >> rs2;
				get_instruction(i * 4) = {Xor, rd, rs1, rs2, 0};
			}
			else if (op == "not") {
				in >> rd >> rs1;
				get_instruction(i * 4) = {Not, rd, rs1, 0, 0};
			}
			else if (op == "shiftl") {
				in >> rd >> rs1 >> imm;
				get_instruction(i * 4) = {ShiftL, rd, rs1, 0, imm};
			}
			else if (op == "shiftr") {
				in >> rd >> rs1 >> imm;
				get_instruction(i * 4) = {ShiftR, rd, rs1, 0, imm};
			}
			else if (op == "beq") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchEq, 0, rs1, rs2, imm};
			}
			else if (op == "bne") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchNe, 0, rs1, rs2, imm};
			}
			else if (op == "blt") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchLt, 0, rs1, rs2, imm};
			}
			else if (op == "ble") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchLe, 0, rs1, rs2, imm};
			}
			else if (op == "bgt") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchGt, 0, rs1, rs2, imm};
			}
			else if (op == "bge") {
				in >> rs1 >> rs2 >> imm;
				get_instruction(i * 4) = {BranchGe, 0, rs1, rs2, imm};
			}
			else if (op == "enter") {
				get_instruction(i * 4) = {EnterNewEnclave, 0, 0, 0, 0};
			}
			else if (op == "exit") {
				get_instruction(i * 4) = {ExitEnclave, 0, 0, 0, 0};
			}
			else if (op == "do") {
				get_instruction(i * 4) = {DoSubSystem, 0, 0, 0, 0};
			}
			else if (op == "print") {
				in >> rs1;
				get_instruction(i * 4) = {Print, 0, rs1, 0, 0};
			}
			else {
				std::cerr << in.peek() << std::endl;
				throw std::runtime_error("unknown instruction:" + op);
			}
		}
	}


public:
	unsigned long long run() {
		unsigned long long return_val = 0;
		int pc = 0;
		int total_instruction = 0;
		while (total_instruction < 1000000) {
			Instruction cur_instruction = get_instruction(pc);
			int new_pc = (pc + 4) % MEM_SIZE;
			switch (cur_instruction.type) {
				case Nop:
					throw std::runtime_error("nop");
				case Li:
					regs[cur_instruction.rd] = cur_instruction.imm;
					break;
				case Load:
					do_load(cur_instruction);
					break;
				case Store:
					do_store(cur_instruction);
					break;
				case Add:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] + regs[cur_instruction.rs2];
					break;
				case Sub:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] - regs[cur_instruction.rs2];
					break;
				case Mul:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] * regs[cur_instruction.rs2];
					break;
				case Div:
					if (regs[cur_instruction.rs2] == 0)
						throw std::runtime_error("divided by zero");
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] / regs[cur_instruction.rs2];
					break;
				case Mod:
					if (regs[cur_instruction.rs2] == 0)
						throw std::runtime_error("moded by zero");
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] % regs[cur_instruction.rs2];
					break;
				case And:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] & regs[cur_instruction.rs2];
					break;
				case Or:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] | regs[cur_instruction.rs2];
					break;
				case Xor:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] ^ regs[cur_instruction.rs2];
					break;
				case Not:
					regs[cur_instruction.rd] = ~regs[cur_instruction.rs1];
					break;
				case ShiftL:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] << cur_instruction.imm;
					break;
				case ShiftR:
					regs[cur_instruction.rd] = regs[cur_instruction.rs1] >> cur_instruction.imm;
					break;
				case BranchEq:
					if (regs[cur_instruction.rs1] == regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case BranchNe:
					if (regs[cur_instruction.rs1] != regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case BranchLt:
					if (regs[cur_instruction.rs1] < regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case BranchLe:
					if (regs[cur_instruction.rs1] <= regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case BranchGt:
					if (regs[cur_instruction.rs1] > regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case BranchGe:
					if (regs[cur_instruction.rs1] >= regs[cur_instruction.rs2])
						new_pc = cur_instruction.imm << 2;
					break;
				case EnterNewEnclave:
					do_enter_enclave(cur_instruction);
					break;
				case ExitEnclave:
					do_exit_enclave(cur_instruction);
					break;
				case DoSubSystem:
					do_do_subsystem(cur_instruction);
					break;
				case Print:
					return regs[cur_instruction.rs1];
				default:
					throw std::runtime_error("unknown instruction");
			}
			new_pc %= MEM_SIZE;
			pc = new_pc;
			++total_instruction;
			regs[0] = 0;// keep x0 as zero
			std::cout << "done one cycle" << std::endl;
		}
		throw std::runtime_error("too many cycles");
	}
};