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
			cout << " ��ʶ��";
		else if (type == Token::Type::Number)
			cout << " ����";
		else if (type == Token::Type::String)
			cout << " �ַ���";
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

	//���ļ�ʧ��
	if (!in.is_open()) {
		return points;
	}
	//��λ���ļ���ĩβ
	in.seekg(0, std::ios::end);
	//��ȡ�ܳ���
	int size = (int)in.tellg();

	char* buffer = new char[size + 1];
	memset(buffer, '\0', size + 1);
	
	//��ȡ
	in.seekg(0, std::ios::beg);
	in.read(buffer, size);
	//�ر��ļ�
	in.close();

	points = std::unique_ptr<char>(buffer);
	return points;
}
