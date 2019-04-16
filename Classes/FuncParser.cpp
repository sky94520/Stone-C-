#include "FuncParser.h"
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "Name.h"
#include "ParameterList.h"
#include "DefStmnt.h"
#include "Arguments.h"
#include "PrimaryExpr.h"

NS_STONE_BEGIN

ASTree* FuncParser::program()
{
	//program: [def | statement] (";" | EOL)
	ASTree* n = nullptr;
	//����def ���� statement
	if (!isToken(";") && !isToken(Token::TOKEN_EOL))
	{
		if (isToken("def"))
			n = this->def();
		else
			n = this->statement();
	}

	//�����зֺŻ��߻���
	bool ret = false;

	if (isToken(";"))
	{
		token(";");
		ret = true;
	}
	if (isToken(Token::TOKEN_EOL))
	{
		token(Token::TOKEN_EOL);
		ret = true;
	}

	//û�зֺŻ��߻��У������
	if (!ret)
		throw ParseException("} expected.", _lexer->peek(0));
	return n;
}

ASTree* FuncParser::simple()
{
	//simple: expr [args]
	ASTree* t = BasicParser::expression();

	if (!isToken(Token::TOKEN_EOL) && !isToken(";"))
	{
		this->args();
	}
	return t;
}

ASTree* FuncParser::primary()
{
	//primary: ("(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix}
	ASTree* primary = BasicParser::primary();
	std::vector<ASTree*> list;

	//���� {postfix}
	if (isHasPostfix())
	{
		list.push_back(primary);

		while (isHasPostfix())
		{
			list.push_back(this->postfix());
		}

		return new PrimaryExpr(list);
	}
	return primary;
}

ASTree* FuncParser::postfix()
{
	//postfix: "(" [args] ")"
	ASTree* t = nullptr;
	this->token("(");

	//�������޶�����һ��Arguments
	if (!isToken(")"))
	{
		t = this->args();
	}
	else
		t = new Arguments();

	this->token(")");

	return t;
}

bool FuncParser::isHasPostfix()
{
	return isToken("(");
}

//args: expr {"," expr}
ASTree* FuncParser::args()
{
	std::vector<ASTree*> list;

	ASTree* expr = BasicParser::expression();
	if (expr != nullptr)
		list.push_back(expr);

	while (isToken(","))
	{
		token(",");
		expr = BasicParser::expression();
		if (expr != nullptr)
			list.push_back(expr);
	}

	return new Arguments(list);
}

//def: "def" IDENTIFIER param_list block
ASTree* FuncParser::def()
{
	this->token("def");

	std::vector<ASTree*> list;
	//TODO:�׳��쳣�����ڴ�й©
	Token* token = _lexer->read();
	if (token->getType() == Token::Type::Identifier)
	{
		//TODO:����
		if (_reserved.find(token->asString()) != _reserved.end())
			return nullptr;
		list.push_back(new Name(token));
	}
	else
		throw ParseException(token);

	//��ȡ����
	list.push_back(this->paramList());

	//��������
	list.push_back(this->block());

	return new DefStmnt(list);
}

ASTree* FuncParser::paramList()
{
	ASTree* t = nullptr;
	//param_list: "(" [params] ")"
	this->token("(");

	//��������params������һ��ParameterList
	if (!isToken(")"))
	{
		t = this->params();
	}
	else
		t = new ParameterList();

	this->token(")");

	return t;
}

ASTree* FuncParser::params()
{
	std::vector<ASTree*> list;

	//params: param {"," param}
	list.push_back(this->param());

	while (isToken(","))
	{
		token(",");
		list.push_back(this->param());
	}

	return new ParameterList(list);
}

ASTree* FuncParser::param()
{
	//param: IDENTIFIER
	//TODO:�׳��쳣�����ڴ�й©
	Token* token = _lexer->read();
	if (token->getType() == Token::Type::Identifier)
	{
		//TODO:����
		if (_reserved.find(token->asString()) != _reserved.end())
			return nullptr;
		Name* n = new Name(token);
		return n;
	}
	else
		throw ParseException(token);
}
NS_STONE_END