#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "scanner.h"
#include "notc.h"
#include <ctype.h>

std::unordered_map<std::string, TokenType> Scanner::keywords {
    { "and",    AND },
    { "class",  CLASS },
    { "else",   ELSE },
    { "false",  FALSE },
    { "for",    FOR },
    { "fun",    FUN },
    { "if",     IF },
    { "nil",    NIL },
    { "or",     OR },
    { "print",  PRINT },
    { "return", RETURN },
    { "super",  SUPER },
    { "this",   THIS },
    { "true",   TRUE },
    { "var",    VAR },
    { "while",  WHILE },
};

char Scanner::advance() {
	return this->m_source[current++];	
}

void Scanner::scan_token() {
	char c = advance();

	switch(c) {
		case '(': add_token(TokenType::LEFT_PAREN); break;
		case ')': add_token(TokenType::RIGHT_PAREN); break;
		case '{': add_token(TokenType::LEFT_BRACE); break;
		case '}': add_token(TokenType::RIGHT_BRACE); break;
		case ',': add_token(TokenType::COMMA); break;
		case '.': add_token(TokenType::DOT); break;
		case '-': add_token(TokenType::MINUS); break;
		case '+': add_token(TokenType::PLUS); break;
		case ';': add_token(TokenType::SEMICOLON); break;
		case '*': add_token(TokenType::STAR); break;
		case '/': 
				  if (match('/')) {
					  while (!is_end() && peek() != '\n') advance();
					
				  } else if (match('*')) {	
					  comment_block();
				  } else {
					  add_token(TokenType::SLASH);
				  }
				  break;
		case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
		case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
		case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
		case ' ':
		case '\r':
		case '\t':
				  break;

		case '\n':
				  line++;
				  break;
		case '"': string(); break;

		default: 
				if (isdigit(c)) {
					number();	
				} else if (isalpha(c) || c == '_') {
					identifier();	
				} else {
					Notc::message(line, "Unexpected character: '" + m_source.substr(start, current - start) + '\'');
				}
	};
}
void Scanner::string() { 
	
	int first_quotes_line = line;
	while (peek() != '"' && !is_end()) {
		if (peek() == '\n') ++line;	
		advance();
	}

	if (is_end()) Notc::message(first_quotes_line, "Matching \" not found");
	
	std::string *value = new std::string { m_source.substr(start + 1, current - start - 1) };

	// the closing "
	advance();
	add_token(TokenType::STRING, (void*) value->c_str());
}

void Scanner::number() {
	while (isdigit(peek())) advance();	

	if (peek() == '.' && isdigit(peek_next())) advance();
	
	while (isdigit(peek())) advance();	
	
	double *value = new double { std::stof(m_source.substr(start, current - start)) };

	add_token(NUMBER, (void*) value);
}

std::vector<Token> Scanner::produce_tokens() {

	while (!is_end()) {
		start = current;
		scan_token();
	}
	
	tokens.push_back(Token(TokenType::SEOF, "", NULL, line));
	
	return tokens;
}

void Scanner::add_token(TokenType type, void *literal) {
	std::string lexeme = m_source.substr(start, current - start);
	tokens.push_back(Token(type, lexeme, literal, line));
}

void Scanner::identifier() {
	while (is_alpha_numeric(peek())) advance();	
	std::string id = m_source.substr(start, current - start);	
	std::unordered_map<std::string, TokenType>::const_iterator keyword = keywords.find(id);	
	if (keyword != keywords.end()) add_token(keyword->second);
	else add_token(IDENTIFIER);
}

bool Scanner::is_alpha_numeric(char c) {
	return isdigit(c) || isalpha(c) || c == '_'; 
}

char Scanner::peek() {
	if (is_end()) return '\0';
	return m_source[current];
}

char Scanner::peek_next() {
	size_t next = current + 1;
	if (next >= m_source.length()) return '\0';
	return m_source[next];
}

bool Scanner::match(char expected) {
	return !is_end() && m_source[current] == expected;
}

bool Scanner::is_end() {
	return current >= m_source.length();
}


void Scanner::comment_block() {

	while (!(peek() == '*' && peek_next() == '/')) {
		if (peek() == '\n') ++line;
		advance();
	}

	current += 2;
}
