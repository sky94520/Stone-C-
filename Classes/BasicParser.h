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

//����������ȼ��ͽ����
class Precedence {
public:
	Precedence(int v, bool a) {
		this->value = v;
		this->leftAssoc = a;
	}
public:
	int value;
	//�Ƿ�������
	bool leftAssoc;
};

//�����
class Operators
{
public:
	static bool LEFT;
	static bool RIGHT;
public:
	Operators();
	~Operators();
	//��Ӳ����� ���ȼ��ͽ����
	void add(const std::string& name, int prec, bool leftAssoc);
	//�������ƻ�ȡ��Ӧ�����ȼ�
	Precedence* get(const std::string& name);
private:
	std::unordered_map<std::string, Precedence*> _mapping;
};

class BasicParser
{
public:
	BasicParser();
	~BasicParser();
	//��ӱ�����
	void addReservedWord(const std::string& reserved);
	//��Ӳ�����
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
	//�ж��Ҳ���һ�����ʽ
	bool rightIsExpr(int prec, Precedence* nextPrec);

	//���������ͬ���򷵻ظõ��ʣ����򷵻�nullptr
	Token* token(const std::string& name);
	bool isToken(const std::string& name);
private:
	//������
	Operators _operators;
	//�����ַ�
	std::unordered_set<std::string> _reserved;
	Lexer* _lexer;
};
NS_STONE_END
#endif
