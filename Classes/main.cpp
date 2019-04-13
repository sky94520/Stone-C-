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
#include "NestedEnv.h"
#include "EvalVisitor.h"

using namespace std;
USING_NS_STONE;

std::unique_ptr<char> getUniqueDataFromFile(const std::string& filename);
void outputLexer(Lexer* lexer);

int main() {
	auto uniquePtr = std::move(getUniqueDataFromFile("1.txt"));
	if (uniquePtr == nullptr)
	{
		cout << "文件打开失败" << endl;
		return 1;
	}
	Lexer* lexer = new Lexer(uniquePtr.get());
	uniquePtr.reset();

	Parser* parser = new Parser();
	parser->setLexer(lexer);
	//创建环境
	NestedEnv* env = new NestedEnv();
	//创建解析器
	EvalVisitor* visitor = new EvalVisitor();

	try
	{
		//词法分析
		while (lexer->peek(0) != Token::TOKEN_EOF)
		{
		//语法分析
			auto t = parser->parse();

			if (t != nullptr) 
			{
				//计算
				t->accept(visitor, env);
				cout << t->toString() << "=>" << visitor->result.asString() << endl;
				delete t;
			}
		}
	}
	catch (ParseException& e)
	{
		cout << e.what() << endl;
	}
	catch (StoneException& e)
	{
		cout << e.what() << endl;
	}

	delete visitor;
	delete env;
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
			cout << " 标识符";
		else if (type == Token::Type::Number)
			cout << " 数字";
		else if (type == Token::Type::String)
			cout << " 字符串";
		cout << token->asString() << endl;

		delete token;
		token = lexer->read();
	}
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
