#include "Parser.h"
#include "FuncParser.h"
#include "Lexer.h"
#include "Token.h"
#include "ClosureParser.h"
#include "ArrayParser.h"

NS_STONE_BEGIN

Parser::Parser()
{
	_parser = new ArrayParser();
	//��ӱ����ַ�
	_parser->addReservedWord(";");
	_parser->addReservedWord("}");
	_parser->addReservedWord(Token::TOKEN_EOL);

	//��������
	_parser->addOperator("=", 1, Operators::RIGHT);
	_parser->addOperator("==", 2, Operators::LEFT);
	_parser->addOperator(">", 2, Operators::LEFT);
	_parser->addOperator("<", 2, Operators::LEFT);
	_parser->addOperator("+", 3, Operators::LEFT);
	_parser->addOperator("-", 3, Operators::LEFT);
	_parser->addOperator("*", 4, Operators::LEFT);
	_parser->addOperator("/", 4, Operators::LEFT);
	_parser->addOperator("%", 4, Operators::LEFT);
}

Parser::~Parser()
{
	delete _parser;
}

ASTree* Parser::parse()
{
	return _parser->program();
}
void Parser::setLexer(Lexer* lexer)
{
	_parser->setLexer(lexer);
}
NS_STONE_END