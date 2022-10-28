#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

enum TokenType {
	LEFT_PAREN,
	RIGHT_PAREN,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMA,
	DOT,
	MINUS,
	PLUS,
	SEMICOLON,
	SLASH,
	STAR,
	BANG,
	BANG_EQUAL,
	EQUAL,
	EQUAL_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL,
	IDENTIFIER,
	STRING,
	NUMBER,
	AND,
	CLASS,
	ELSE,
	FALSE,
	FUN,
	FOR,
	IF,
	NIL,
	OR,
	PRINT,
	RETURN,
	SUPER,
	THIS,
	TRUE,
	VAR,
	WHILE,
	SEOF
};

struct Token {
	TokenType type;
	std::string lexeme;
	void *literal;
	std::string str;
	int line;

	Token(TokenType type, const std::string& lexeme, void *literal, int line)
						: type(type), lexeme(lexeme), literal(literal), line(line) {
	}
};

class Scanner {
	private:
		size_t start, current, line;
		bool is_end();
		char advance();
		void scan_token();
		void add_token(TokenType type, void *literal = nullptr);
		bool match(char expected);
		void string();
		void number();
		char peek();
		char peek_next();
		bool is_alpha_numeric(char c);
		void identifier();
		void comment_block();

	public:
		static std::unordered_map<std::string, TokenType> keywords;

		std::string m_source;
		std::vector<Token> tokens;

		Scanner(const std::string& source): m_source(source), start(0), current(0), line(1) {}

		std::vector<Token> produce_tokens();
};

#endif // _SCANNER_H_
