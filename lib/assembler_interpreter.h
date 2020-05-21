#ifndef ASSEMBLER_INTERPRETER_H_
#define ASSEMBLER_INTERPRETER_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>


class Interpreter {
private:
	enum InstructionType {
		MOV,
		INC,
		DEC,
		ADD,
		SUB,
		MUL,
		DIV,
		LABEL,
		JMP,
		CMP,
		JNE,
		JE,
		JGE,
		JG,
		JLE,
		JL,
		CALL,
		RET,
		MSG,
		END,
		COMMENT,
		INSTR_MAX
	};

	struct instruction {
		InstructionType type;
		std::vector<std::string> operands;
	};

	/* script program state */
	std::vector<std::string> m_program;
	std::unordered_map<std::string, int> m_labels;
	int m_program_size;

	std::unordered_map<std::string, int> m_regs;
	std::vector<int> m_stack;
	std::string m_out;

	instruction m_current_instr;
	int m_pc;

	/* helpers regex */
	std::regex m_regex_register;
	std::regex m_regex_const;
	std::regex m_regex_label;

	/* fetch instruction from the pc program */
	void fetch();
	void fetch_mov(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_inc(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_dec(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_add(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_sub(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_mul(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_div(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_label(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jmp(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_cmp(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jne(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_je(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jge(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jg(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jle(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_jl(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_call(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_ret(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_msg(std::string &instr, size_t size);
	void fetch_end(std::vector<std::string> &parsed_instr, size_t size);
	void fetch_comment(std::vector<std::string> &parsed_instr, size_t size);

	/* execute current instruction */
	void execute();
	void execute_mov();
	void execute_inc();
	void execute_dec();
	void execute_add();
	void execute_sub();
	void execute_mul();
	void execute_div();
	void execute_label();
	void execute_jmp();
	void execute_cmp();
	void execute_jne();
	void execute_je();
	void execute_jge();
	void execute_jg();
	void execute_jle();
	void execute_jl();
	void execute_call();
	void execute_ret();
	void execute_msg();
	void execute_end();
	void execute_comment();
public:
	Interpreter (const std::string & program);
	void run();
	std::string getMessage() const;
};

#endif /* ASSEMBLER_INTERPRETER_H_ */
