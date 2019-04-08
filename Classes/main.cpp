#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <sstream>

#include "Lexer.h"
#include "Token.h"

using namespace std;


std::unique_ptr<char> getUniqueDataFromFile(const std::string& filename);

int main() {
	auto uniquePtr = std::move(getUniqueDataFromFile("1.txt"));
	Lexer* lexer = new Lexer(uniquePtr.get());
	uniquePtr.reset();

	Token* token = lexer->read();
	while (token != Token::TOKEN_EOF) {

		cout << token->getLineNumber();
		auto type = token->getType();
		if (type == Token::Type::Identifier)
			cout << " 标识符";
		else if (type == Token::Type::Number)
			cout << " 数字";
		else if (type == Token::Type::String)
			cout << " 字符串";
		cout << token->getText() << endl;

		delete token;
		token = lexer->read();
	}

	delete lexer;
	return 0;
}

std::unique_ptr<char> getUniqueDataFromFile(const std::string& filename) {
	std::unique_ptr<char> points;
	std::ifstream in;
	in.open(filename);

	//打开文件失败
	if (!in.is_open()) {
		return points;
	}
	//定位到文件的末尾
	in.seekg(0, std::ios::end);
	//获取总长度
	int size = (int)in.tellg();

	char* buffer = new char[size + 1];
	memset(buffer, '\0', size + 1);
	
	//读取
	in.seekg(0, std::ios::beg);
	in.read(buffer, size);
	//关闭文件
	in.close();

	points = std::unique_ptr<char>(buffer);
	return points;
}
