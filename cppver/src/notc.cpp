#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "notc.h"
#include "scanner.h"

bool Notc::hadError = false;

int Notc::exec(const std::string& source) {

	Scanner scanner(source); 	
	std::vector<Token> tokens = scanner.produce_tokens();
	for (size_t j = 0; j < tokens.size(); ++j) {
		std::cout << "[" << tokens[j].type << "]\t";

		if (tokens[j].type == TokenType::NUMBER) {
			double value = *(static_cast<double*> (tokens[j].literal));
			std::cout << value << std::endl;
		} else if (tokens[j].type == TokenType::STRING) {
			std::string value = static_cast<char*>(tokens[j].literal);
			std::cout << value << std::endl;
		} else {
			std::cout << tokens[j].lexeme << std::endl;
		}
	}

	return 1;
}

int Notc::run_from_file(const std::string& file_name) {
	std::ifstream file;
	file.open(file_name);

	if (!file.is_open()) {
		
		std::cerr << "Couldn't find " << file_name << std::endl;
		exit(-1); 
	}

	std::string source;
	std::stringstream buffer;

	buffer << file.rdbuf();

	file.close();
	source = buffer.str();

	exec(source);
	if (hadError) exit(-1);

	return 0;
}

void Notc::run_prompt() {

	while (true) {
		std::string cmd;
		std::cout << "> ";
		std::getline(std::cin, cmd);
		exec(cmd);
		hadError = false;
	}
}

void Notc::message(int line, const std::string msg) {

	std::cout << "[" << line << "] FAIL: " << msg << std::endl;
	hadError = true;
}
