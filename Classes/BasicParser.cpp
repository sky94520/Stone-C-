#include "BasicParser.h"
#include "Token.h"
#include "Lexer.h"
#include "ASTLeaf.h"
#include "BinaryExpr.h"
#include "NumberLiteral.h"
#include "Name.h"
#include "StringLiteral.h"
#include "NegativeExpr.h"

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
	//删除
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
	//存在statement
	if (!isToken(";") && isToken(Token::TOKEN_EOL))
		n = this->statement();

	//必须有分号或者换行
	bool ret = false;

	if (isToken(";"))
	{
		delete token(";");
		ret = true;
	}
	if (isToken(Token::TOKEN_EOL))
	{
		delete token(Token::TOKEN_EOL);
		ret = true;
	}

	//TODO:出错
	if (!ret)
	{
	}
	return n;
}

ASTree* BasicParser::statement()
{
	/*
		statement: "if" expr block ["else" block]
				 | "while" expr block
				 | simple
	*/
	return nullptr;
}

ASTree* BasicParser::simple()
{
	return nullptr;
}

ASTree* BasicParser::block()
{
	return nullptr;
}

ASTree* BasicParser::expression()
{
	//expr: factor {OP factor}
	ASTree* right = factor();
	Precedence* next;

	//获取
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
		delete token("-");

		primary = this->primary();
		//添加NegativeExpr
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
		//释放内存
		delete token("(");
		ASTree* e = expression();
		delete token(")");

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
		//标识符
		if (type == Token::Type::Identifier)
		{
			//TODO:出错
			if (_reserved.find(token->getText()) != _reserved.end())
				return nullptr;
			Name* n = new Name(token);
			return n;
		}
		//字符串
		else if (type == Token::Type::String)
		{
			StringLiteral* n = new StringLiteral(token);
			return n;
		}
	}
	return nullptr;
}

ASTree* BasicParser::doShift(ASTree* left, int prec)
{
	//获取操作符
	ASTLeaf* op = new ASTLeaf(_lexer->read());
	//factor: NUMBER | "(" expression ")"
	ASTree* right = factor();
	Precedence* next;

	/*
	考虑右操作符的结合性和两个操作符的优先级
	返回true,则表示右侧是一个表达式
	*/
	while ((next = nextOperator()) != nullptr && rightIsExpr(prec, next))
		right = doShift(right, next->value);

	return new BinaryExpr(left, op, right);
}

Precedence* BasicParser::nextOperator()
{
	Token* token = _lexer->peek(0);
	//为标识符,则获取名称对应的优先权
	if (token->getType() == Token::Type::Identifier)
		return _operators.get(token->getText());
	return nullptr;
}
bool BasicParser::rightIsExpr(int prec, Precedence* nextPrec)
{
	if (nextPrec->leftAssoc)
		return prec < nextPrec->value;
	else
		return prec <= nextPrec->value;
}

Token* BasicParser::token(const std::string& name)
{
	Token* t = _lexer->read();
	//是标识符且名称相同
	if (t->getType() == Token::Type::Identifier
		&& t->getText() == name)
	{
		return t;
	}
	return nullptr;
}

bool BasicParser::isToken(const std::string& name)
{
	Token* t = _lexer->peek(0);

	return t->getType() == Token::Type::Identifier && t->getText() == name;
}

NS_STONE_END