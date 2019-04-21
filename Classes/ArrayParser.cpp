#include "ArrayParser.h"
#include "Parser.h"
#include "ArrayLiteral.h"
#include "ArrayRef.h"

NS_STONE_BEGIN

ArrayParser::ArrayParser()
	:_elements(nullptr)
{
	//elements: expr {"," expr}
	_elements = Parser::rule(ArrayLiteral::TREE_ID)
		->ast(_expr)->repeat(Parser::rule()->sep(1, ",")->ast(_expr));
	_elements->retain();

	this->init();
}

ArrayParser::~ArrayParser()
{
	_elements->release();
	//???
	_expr->release();
}

bool ArrayParser::init()
{
	//Ìí¼Ó±£Áô×Ö
	_reserved->insert("]");

	//primary: ("[" [elements] "]" | "(" expr ")" | NUMBER | IDENTIFIER | STRING) {postfix}
	_primary->insertChoice(Parser::rule()->sep(1, "[")->maybe(_elements)->sep(1, "]"));

	//"(" [args] ")" | "[" expr "]"
	_postfix->insertChoice(Parser::rule(ArrayRef::TREE_ID)->sep(1, "[")->ast(_expr)->sep(1, "]"));
	return true;
}
NS_STONE_END