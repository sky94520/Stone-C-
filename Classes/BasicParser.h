#ifndef __BasicParser_H__
#define __BasicParser_H__

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "StoneMarcos.h"

NS_STONE_BEGIN

class ASTree;
class Lexer;
class Token;

//运算符的优先级和结合性
class Precedence {
public:
	Precedence(int v, bool a) {
		this->value = v;
		this->leftAssoc = a;
	}
public:
	int value;
	//是否是左结合
	bool leftAssoc;
};

//运算符
class Operators
{
public:
	static bool LEFT;
	static bool RIGHT;
public:
	Operators();
	~Operators();
	//添加操作符 优先级和结合性
	void add(const std::string& name, int prec, bool leftAssoc);
	//根据名称获取对应的优先级
	Precedence* get(const std::string& name);
private:
	std::unordered_map<std::string, Precedence*> _mapping;
};

class BasicParser
{
public:
	BasicParser();
	~BasicParser();
	//添加保留字
	void addReservedWord(const std::string& reserved);
	//添加操作符
	void addOperator(const std::string& name, int prec, bool leftAssoc);

	void setLexer(Lexer* lexer);

	//program: [statement] (";" | EOL)
	ASTree* program();

	/*
		statement: "if" expr block ["else" block]
				 | "while" expr block
				 | simple
	*/
	ASTree* statement();

	//simple: expr
	ASTree* simple();

	//block "{" [statement] {(";" | EOL) [statement]} "}"
	ASTree* block();

	//expr: factor {OP factor}
	ASTree* expression();

	//factor: "-" primary | primary
	ASTree* factor();

	//primary: "(" expression ")" | NUMBER | IDENTIFIER | STRING
	ASTree* primary();
protected:
	ASTree* doShift(ASTree* left, int prec);
	Precedence* nextOperator();
	//判断右侧是一个表达式
	bool rightIsExpr(int prec, Precedence* nextPrec);

	//如果名称相同，则返回该单词，否则返回nullptr
	Token* token(const std::string& name);
	bool isToken(const std::string& name);
private:
	//操作符
	Operators _operators;
	//保留字符
	std::unordered_set<std::string> _reserved;
	Lexer* _lexer;
};
NS_STONE_END
#endif
