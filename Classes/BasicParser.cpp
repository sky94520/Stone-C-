#include "BasicParser.h"
#include "Token.h"
#include "Lexer.h"
#include "ASTLeaf.h"
#include "BinaryExpr.h"
#include "NumberLiteral.h"
#include "Name.h"
#include "StringLiteral.h"
#include "NegativeExpr.h"
#include "ParseException.h"
#include "BlockStmnt.h"
#include "IfStmnt.h"
#include "WhileStmnt.h"

NS_STONE_BEGIN
//------------------------------------Operators-------------------------------------
bool Operators::LEFT = true;
bool Operators::RIGHT = false;

Operators::Operators()
{
}

Operators::~Operators()
{
	auto it = _mapping.begin();
	//ɾ��
	while (it != _mapping.end())
	{
		auto precedence = it->second;
		delete precedence;

		it = _mapping.erase(it);
	}
}

void Operators::add(const std::string& name, int prec, bool leftAssoc)
{
	auto precedence = new Precedence(prec, leftAssoc);
	_mapping.insert(std::make_pair(name, precedence))	;
}

Precedence* Operators::get(const std::string& name)
{
	auto it = _mapping.find(name);
	if (it == _mapping.end())
		return nullptr;

	return it->second;
}
//------------------------------------BasicParser-----------------------------------
BasicParser::BasicParser()
	:_lexer(nullptr)
{
}

BasicParser::~BasicParser()
{
}

void BasicParser::addReservedWord(const std::string& reserved)
{
	_reserved.insert(reserved);
}

void BasicParser::addOperator(const std::string& name, int prec, bool leftAssoc)
{
	_operators.add(name, prec, leftAssoc);
}

void BasicParser::setLexer(Lexer* lexer)
{
	_lexer = lexer;
}

ASTree* BasicParser::program()
{
	//program: [statement] (";" | EOL)
	ASTree* n = nullptr;
	//����statement
	if (!isToken(";") && !isToken(Token::TOKEN_EOL))
		n = this->statement();

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

	if (ret)
	{
	}
	//û�зֺŻ��߻��У������
	else
		throw ParseException("} expected.", _lexer->peek(0));
	return n;
}

ASTree* BasicParser::statement()
{
	/*
		statement: "if" expr block ["else" block]
				 | "while" expr block
				 | simple
	*/
	if (isToken("if"))
	{
		token("if");
		IfStmnt* ifStmnt = nullptr;

		ASTree* expression = this->expression();
		ASTree* body = this->block();
		ASTree* thenBlock = nullptr;
		std::vector<ASTree*> list;

		list.push_back(expression);
		list.push_back(body);
		//����else���
		if (isToken("else"))
		{
			token("else");
			thenBlock = this->block();
			list.push_back(thenBlock);
		}
		ifStmnt = new IfStmnt(list);
		return ifStmnt;
	}
	//ѭ�����
	else if (isToken("while"))
	{
		token("while");
		ASTree* expression = this->expression();
		ASTree* body = this->block();
		std::vector<ASTree*> list;
		list.push_back(expression);
		list.push_back(body);

		WhileStmnt* whileStmnt = new WhileStmnt(list);
		return whileStmnt;
	}
	else
		return this->simple();
}

ASTree* BasicParser::simple()
{
	return this->expression();
}

ASTree* BasicParser::block()
{
	//block "{" [statement] {(";" | EOL) [statement]} "}"
	//�������"{"��ͷ
	if (this->isToken("{"))
	{
		this->token("{");
		//����ȥ������
		if (this->isToken(Token::TOKEN_EOL))
			this->token(Token::TOKEN_EOL);
		std::vector<ASTree*> list;

		//[statement]
		if (!isToken(";") && !isToken(Token::TOKEN_EOL))
			list.push_back(this->statement());

		//{(";" | EOL) [statement] } "}"
		while (this->isToken(";") || this->isToken(Token::TOKEN_EOL))
		{
			bool ret = false;
			if (this->isToken(";"))
			{
				this->token(";");
				ret = true;
			}
			if (this->isToken(Token::TOKEN_EOL))
			{
				this->token(Token::TOKEN_EOL);
				ret = true;
			}
			//ȱ�ٷֺŻ��߻���
			if (!ret)
				throw ParseException("EOL or ; expected.", _lexer->peek(0));
			//ֱ��Ϊ "}"
			if (this->isToken("}"))
			{
				this->token("}");
				break;
			}
			//[statement]
			if (!this->isToken(";") && !this->isToken(Token::TOKEN_EOL))
				list.push_back(this->statement());
		}
		return new BlockStmnt(list);
	}
	else
		throw ParseException("{ expected.", _lexer->peek(0));
}

ASTree* BasicParser::expression()
{
	//expr: factor {OP factor}
	ASTree* right = factor();
	Precedence* next;

	//��ȡ
	while ((next = nextOperator()) != nullptr)
		right = doShift(right, next->value);

	return right;
}

ASTree* BasicParser::factor()
{
	//factor: "-" primary | primary
	ASTree* primary = nullptr;

	if (isToken("-"))
	{
		token("-");

		primary = this->primary();
		//���NegativeExpr
		primary = new NegativeExpr(primary);
	}
	else
		primary = this->primary();

	return primary;
}

ASTree* BasicParser::primary()
{
	//primary: "(" expression ")" | NUMBER | IDENTIFIER | STRING
	//"(" expression ")"
	if (isToken("("))
	{
		//�ͷ��ڴ�
		token("(");
		ASTree* e = expression();
		token(")");

		return e;
	}
	else
	{
		Token* token = _lexer->read();
		auto type = token->getType();
		
		if (type == Token::Type::Number)
		{
			NumberLiteral* n = new NumberLiteral(token);
			return n;
		}
		//��ʶ��
		if (type == Token::Type::Identifier)
		{
			//TODO:����
			if (_reserved.find(token->asString()) != _reserved.end())
				return nullptr;
			Name* n = new Name(token);
			return n;
		}
		//�ַ���
		else if (type == Token::Type::String)
		{
			StringLiteral* n = new StringLiteral(token);
			return n;
		}
		else
			throw ParseException(token);
	}
	return nullptr;
}

ASTree* BasicParser::doShift(ASTree* left, int prec)
{
	//��ȡ������
	ASTLeaf* op = new ASTLeaf(_lexer->read());
	//factor: NUMBER | "(" expression ")"
	ASTree* right = factor();
	Precedence* next;

	/*
	�����Ҳ������Ľ���Ժ����������������ȼ�
	����true,���ʾ�Ҳ���һ�����ʽ
	*/
	while ((next = nextOperator()) != nullptr && rightIsExpr(prec, next))
		right = doShift(right, next->value);

	return new BinaryExpr(left, op, right);
}

Precedence* BasicParser::nextOperator()
{
	Token* token = _lexer->peek(0);
	//Ϊ��ʶ��,���ȡ���ƶ�Ӧ������Ȩ
	if (token->getType() == Token::Type::Identifier)
		return _operators.get(token->asString());
	return nullptr;
}
bool BasicParser::rightIsExpr(int prec, Precedence* nextPrec)
{
	if (nextPrec->leftAssoc)
		return prec < nextPrec->value;
	else
		return prec <= nextPrec->value;
}

void BasicParser::token(const std::string& name)
{
	Token* t = _lexer->read();
	//�Ǳ�ʶ����������ͬ
	if (t->getType() == Token::Type::Identifier
		&& t->asString() == name)
	{
		delete t;
	}
	else
		throw ParseException(t);
}

bool BasicParser::isToken(const std::string& name)
{
	Token* t = _lexer->peek(0);

	return t->getType() == Token::Type::Identifier && t->asString() == name;
}

NS_STONE_END