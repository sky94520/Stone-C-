#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <sstream>

#include "Lexer.h"
#include "Token.h"
#include "Parser.h"
#include "ASTree.h"
#include "ASTLeaf.h"
#include "ParseException.h"
#include "StoneException.h"

using namespace std;
USING_NS_STONE;

std::unique_ptr<char> getUniqueDataFromFile(const std::string& filename);
void outputLexer(Lexer* lexer);

int main() {
	auto uniquePtr = std::move(getUniqueDataFromFile("1.txt"));
	if (uniquePtr == nullptr)
	{
		cout << "�ļ���ʧ��" << endl;
		return 1;
	}
	Lexer* lexer = new Lexer(uniquePtr.get());
	uniquePtr.reset();

	Parser* parser = new Parser();
	parser->setLexer(lexer);

	cout <<"token size is: "<< sizeof(Token) << endl;
	cout <<"ASTLeaf size is: "<< sizeof(ASTLeaf) << endl;

	try
	{
		//�ʷ�����
		while (lexer->peek(0) != Token::TOKEN_EOF)
		{
		//�﷨����
			ASTree* t = parser->parse();

			if (t != nullptr) 
			{
				cout << t->toString() << endl;
				delete t;
			}
		}
	}
	catch (ParseException& e)
	{
		cout << e.what() << endl;
	}

	delete parser;
	delete lexer;
	delete Token::TOKEN_EOF;

	return 0;
}

void outputLexer(Lexer* lexer)
{
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
