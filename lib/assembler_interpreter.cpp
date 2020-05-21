#include "assembler_interpreter.h"

Interpreter::Interpreter(const std::string & program)
{
	if (program.size() == 0)
		std::cout << "There is empty program" << std::endl;

	std::stringstream ss (program);
	std::string item;

	while (getline (ss, item, '\n'))
		m_program.push_back (item);

	m_program_size = m_program.size();
	m_pc = 0;
	m_regex_register = std::regex("^[A-Za-z]+$");
	m_regex_const = std::regex("-?[0-9]+");
	m_regex_label = std::regex("[A-Za-z0-9_]+:");

	for (int line = 0; line < m_program_size; line++)
		if (std::regex_match(m_program[line], m_regex_label)) {
			std::string label = m_program[line];
			label.erase(label.find(':'), label.length());
			m_labels[label] = line;
		}

	m_stack.push_back(0);

}

void Interpreter::fetch_mov(std::vector<std::string> &parsed_instr, size_t size)
{
	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());

	if (parsed_instr.size() != 3)
		throw "There is invalid mov operands num";

	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The first operand of mov must be a register";

	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);
	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);

	if (!is_src_reg && !is_src_constant)
		throw "Second operand of mov instruction is not a digit and not a register";

	bool is_src_reg_exist = !!(m_regs.find(parsed_instr[2].c_str()) != m_regs.end());

	if (is_src_reg && !is_src_reg_exist)
		throw "There is no such register in right operand of mov instruction";

	m_current_instr.type = MOV;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_inc(std::vector<std::string> &parsed_instr, size_t size)
{
	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of inc must be a register";

	if (size != 2)
		throw "There is invalid inc operands num";

	m_current_instr.type = INC;
	m_current_instr.operands.push_back(parsed_instr[1]);
}

void Interpreter::fetch_dec( std::vector<std::string> &parsed_instr, size_t size)
{
	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of dec must be a register";

	if (size != 2)
		throw "There is invalid inc operands num";

	m_current_instr.type = DEC;
	m_current_instr.operands.push_back(parsed_instr[1]);
}

void Interpreter::fetch_add(std::vector<std::string> &parsed_instr, size_t size)
{
	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());

	if (size != 3)
		throw "There is invalid add operands num";

	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of add must be a register";

	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);
	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);

	if (!is_src_constant && !is_src_reg)
		throw "There is invalid add second operand";

	m_current_instr.type = ADD;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_sub(std::vector<std::string> &parsed_instr, size_t size)
{
	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());

	if (size != 3)
		throw "There is invalid sub operands num";

	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of sub must be a register";

	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);
	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);

	if (!is_src_constant && !is_src_reg)
		throw "There is invalid sub second operand";

	m_current_instr.type = SUB;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_mul(std::vector<std::string> &parsed_instr, size_t size)
{
	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());

	if (size != 3)
		throw "There is invalid mul operands num";

	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of mul must be a register";

	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);
	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);

	if (!is_src_constant && !is_src_reg)
		throw "There is invalid mul second operand";

	m_current_instr.type = MUL;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_div(std::vector<std::string> &parsed_instr, size_t size)
{
	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());

	if (size != 3)
		throw "There is invalid div operands num";

	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);

	if (!is_dst_reg)
		throw "The operand of div must be a register";

	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);
	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);

	if (!is_src_constant && !is_src_reg)
		throw "There is invalid div second operand";

	if (is_src_constant && std::atoi(parsed_instr[2].c_str()) == 0) 
		throw "There div by 0 exception";

	m_current_instr.type = DIV;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_label(std::vector<std::string> &parsed_instr, size_t size)
{
	m_current_instr.type = LABEL;
	return;
}

void Interpreter::fetch_jmp(std::vector<std::string> &parsed_instr, size_t size)
{
	if (m_labels.find(parsed_instr[1]) == m_labels.end())
		throw "There is no such label for jmp";

	if (size != 2)
		throw "There is invalid jmp operands num";

	m_current_instr.type = JMP;
	m_current_instr.operands.push_back(parsed_instr[1]);
}

void Interpreter::fetch_cmp(std::vector<std::string> &parsed_instr, size_t size)
{
	if (size != 3)
		throw "There is invalid jmp operands num";

	size_t comma_pos = parsed_instr[1].find(',');
	if (comma_pos != std::string::npos && comma_pos > 0)
		parsed_instr[1].erase(comma_pos, parsed_instr[1].length());


	bool is_dst_reg = std::regex_match(parsed_instr[1], m_regex_register);
	bool is_dst_constant = std::regex_match(parsed_instr[1], m_regex_const);

	if (!is_dst_reg && !is_dst_constant)
		throw "The left operand of cmp must be a register or a constant";

	bool is_src_constant = std::regex_match(parsed_instr[2], m_regex_const);
	bool is_src_reg = std::regex_match(parsed_instr[2], m_regex_register);

	if (!is_src_constant && !is_src_reg)
		throw "There is invalid cmp second operand";

	m_current_instr.type = CMP;
	for (int i = 1; i < size; i++)
		m_current_instr.operands.push_back(parsed_instr[i]);
}

void Interpreter::fetch_jne(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JNE;
}

void Interpreter::fetch_je(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JE;
}

void Interpreter::fetch_jge(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JGE;
}

void Interpreter::fetch_jg(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JG;
}

void Interpreter::fetch_jle(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JLE;
}

void Interpreter::fetch_jl(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = JL;
}

void Interpreter::fetch_call(std::vector<std::string> &parsed_instr, size_t size)
{
	fetch_jmp(parsed_instr, size);
	m_current_instr.type = CALL;
}

void Interpreter::fetch_ret(std::vector<std::string> &parsed_instr, size_t size)
{
	if (size != 1)
		throw "There is invalid ret operands num";

	m_current_instr.type = RET;
}

void Interpreter::fetch_msg(std::string instr, size_t size)
{
	std::string result;

	int is_str = 0; 
	size_t pos = 0;
	size_t len;

	while(instr[0] == ' ') instr.erase(0, 1);

	instr.erase(0, 3);

	while(instr[0] == ' ') instr.erase(0, 1);

	len = instr.length();
	while (pos != len) {
		if (is_str == 0 && instr[pos] == '\'')
			is_str++;
		else if (is_str > 0 && instr[pos] == '\'')
			is_str--;
		else if (is_str)
			result += instr[pos];
		else if (is_str == 0)
			if (std::isalpha(instr[pos])) {
				std::string reg = instr.substr(pos, 1);
				result += std::to_string(m_regs[reg]);
			} else if (instr[pos] == ';')
				break;
		pos++;
	}

	m_current_instr.type = MSG;
	m_current_instr.operands.push_back(result);
}

void Interpreter::fetch_end(std::vector<std::string> &parsed_instr, size_t size)
{
	m_current_instr.type = END;
}

void Interpreter::fetch_comment(std::vector<std::string> &parsed_instr, size_t size)
{
	m_current_instr.type = COMMENT;
}

void Interpreter::fetch()
{
	while (m_program[m_pc].empty()) m_pc++;

	std::string &instr = m_program[m_pc];
	size_t comment_pos = instr.find(';');

	if (comment_pos != std::string::npos && comment_pos > 0)
		instr.erase(comment_pos, instr.length());

	std::istringstream iss(instr);
	std::vector<std::string> parsed_instr((std::istream_iterator<std::string>(iss)),
						std::istream_iterator<std::string>());

	size_t size = parsed_instr.size();

	if (parsed_instr[0] == ";") {
		fetch_comment(parsed_instr, size);
		return;
	}

	if (parsed_instr[0].back () == ':') {
		fetch_label(parsed_instr, size);
		return;
	}


	if (parsed_instr[0] == "mov") 
		fetch_mov(parsed_instr, size);

	else if (parsed_instr[0] == "inc") 
		fetch_inc(parsed_instr, size);

	else if (parsed_instr[0] == "dec") 
		fetch_dec(parsed_instr, size);

	else if (parsed_instr[0] == "add") 
		fetch_add(parsed_instr, size);

	else if (parsed_instr[0] == "sub") 
		fetch_sub(parsed_instr, size);

	else if (parsed_instr[0] == "mul") 
		fetch_mul(parsed_instr, size);

	else if (parsed_instr[0] == "div") 
		fetch_div(parsed_instr, size);

	else if (parsed_instr[0] == "jmp") 
		fetch_jmp(parsed_instr, size);

	else if (parsed_instr[0] == "cmp") 
		fetch_cmp(parsed_instr, size);

	else if (parsed_instr[0] == "jne") 
		fetch_jne(parsed_instr, size);

	else if (parsed_instr[0] == "je") 
		fetch_je(parsed_instr, size);

	else if (parsed_instr[0] == "jge") 
		fetch_jge(parsed_instr, size);

	else if (parsed_instr[0] == "jg") 
		fetch_jg(parsed_instr, size);

	else if (parsed_instr[0] == "jle") 
		fetch_jle(parsed_instr, size);

	else if (parsed_instr[0] == "jl") 
		fetch_jl(parsed_instr, size);

	else if (parsed_instr[0] == "call") 
		fetch_call(parsed_instr, size);

	else if (parsed_instr[0] == "ret") 
		fetch_ret(parsed_instr, size);

	else if (parsed_instr[0] == "msg") 
		fetch_msg(instr, size);

	else if (parsed_instr[0] == "end") 
		fetch_end(parsed_instr, size);
	else	
		throw "Unknown instruction";
}

void Interpreter::execute_mov()
{
	auto &dst_reg = m_regs[m_current_instr.operands[0]];
	bool is_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_constant)
		dst_reg = std::atoi(m_current_instr.operands[1].c_str());
	else
		dst_reg = m_regs[m_current_instr.operands[1]];
	m_pc++;
}

void Interpreter::execute_inc()
{
	m_regs[m_current_instr.operands[0]]++;
	m_pc++;
}

void Interpreter::execute_dec()
{
	m_regs[m_current_instr.operands[0]]--;
	m_pc++;
}

void Interpreter::execute_add()
{
	bool is_src_reg = std::regex_match(m_current_instr.operands[1], m_regex_register);
	bool is_src_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_src_reg)
		m_regs[m_current_instr.operands[0]] += m_regs[m_current_instr.operands[1]];
	else if (is_src_constant)
		m_regs[m_current_instr.operands[0]] += std::atoi(m_current_instr.operands[1].c_str());
	else
		throw "add src not register and not constant";

	m_pc++;
}

void Interpreter::execute_sub()
{
	bool is_src_reg = std::regex_match(m_current_instr.operands[1], m_regex_register);
	bool is_src_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_src_reg)
		m_regs[m_current_instr.operands[0]] -= m_regs[m_current_instr.operands[1]];
	else if (is_src_constant)
		m_regs[m_current_instr.operands[0]] -= std::atoi(m_current_instr.operands[1].c_str());
	else
		throw "sub src not register and not constant";

	m_pc++;
}

void Interpreter::execute_mul()
{
	bool is_src_reg = std::regex_match(m_current_instr.operands[1], m_regex_register);
	bool is_src_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_src_reg)
		m_regs[m_current_instr.operands[0]] *= m_regs[m_current_instr.operands[1]];
	else if (is_src_constant)
		m_regs[m_current_instr.operands[0]] *= std::atoi(m_current_instr.operands[1].c_str());
	else
		throw "mul src not register and not constant";

	m_pc++;
}

void Interpreter::execute_div()
{
	bool is_src_reg = std::regex_match(m_current_instr.operands[1], m_regex_register);
	bool is_src_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_src_reg)
		m_regs[m_current_instr.operands[0]] /= m_regs[m_current_instr.operands[1]];
	else if (is_src_constant)
		m_regs[m_current_instr.operands[0]] /= std::atoi(m_current_instr.operands[1].c_str());
	else
		throw "Div src not register and not constant";

	m_pc++;
}

void Interpreter::execute_label()
{
	m_pc++;
}

void Interpreter::execute_jmp()
{
	m_pc = m_labels[m_current_instr.operands[0]];
}

void Interpreter::execute_cmp()
{
	int left, right;

	bool is_left_reg = std::regex_match(m_current_instr.operands[0], m_regex_register);
	bool is_left_constant = std::regex_match(m_current_instr.operands[0], m_regex_const);

	if (is_left_reg)
		left = m_regs[m_current_instr.operands[0]];
	else if (is_left_constant)
		left = std::atoi(m_current_instr.operands[0].c_str());
	else
		throw "Unknown left operand type of cmp instruction";

	bool is_right_reg = std::regex_match(m_current_instr.operands[1], m_regex_register);
	bool is_right_constant = std::regex_match(m_current_instr.operands[1], m_regex_const);

	if (is_right_reg)
		right = m_regs[m_current_instr.operands[1]];
	else if (is_right_constant)
		right = std::atoi(m_current_instr.operands[1].c_str());
	else
		throw "Unknown right operand type of cmp instruction";

	m_regs["cmp"] = left - right;
	m_pc++;
}


void Interpreter::execute_jne()
{
	if (m_regs["cmp"] != 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_je()
{
	if (m_regs["cmp"] == 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_jge()
{
	if (m_regs["cmp"] >= 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_jg()
{
	if (m_regs["cmp"] > 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_jle()
{
	if (m_regs["cmp"] <= 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_jl()
{
	if (m_regs["cmp"] < 0)
		execute_jmp();
	else
		m_pc++;
}

void Interpreter::execute_call()
{
	m_stack.push_back(m_pc + 1);
	m_pc = m_labels[m_current_instr.operands[0]];
}

void Interpreter::execute_ret()
{
	m_pc = m_stack.back();
	m_stack.pop_back();
}

void Interpreter::execute_msg()
{
	m_out += m_current_instr.operands[0];
	m_pc++;
}

void Interpreter::execute_end()
{
	m_stack.clear();
	m_pc = m_program_size;
}

void Interpreter::execute_comment()
{
	m_pc++;
}

void Interpreter::execute()
{
	switch(m_current_instr.type) {
		case MOV:
			execute_mov();
			break;
		case INC:
			execute_inc();
			break;
		case DEC:
			execute_dec();
			break;
		case ADD:
			execute_add();
			break;
		case SUB:
			execute_sub();
			break;
		case MUL:
			execute_mul();
			break;
		case DIV:
			execute_div();
			break;
		case LABEL:
			execute_label();
			break;
		case JMP:
			execute_jmp();
			break;
		case CMP:
			execute_cmp();
			break;
		case JNE:
			execute_jne();
			break;
		case JE:
			execute_je();
			break;
		case JGE:
			execute_jge();
			break;
		case JG:
			execute_jg();
			break;
		case JLE:
			execute_jle();
			break;
		case JL:
			execute_jl();
			break;
		case CALL:
			execute_call();
			break;
		case RET:
			execute_ret();
			break;
		case MSG:
			execute_msg();
			break;
		case END:
			execute_end();
			break;
		case COMMENT:
			execute_comment();
			break;
		break;
	}
	m_current_instr.operands.clear();
}

void Interpreter::run()
{
	while (m_pc < m_program_size) {
		fetch();
		execute();
	}
}

std::string Interpreter::getMessage() const
{
	if (m_stack.empty())
		return m_out;

	return "-1";
}
