#include "ClosureParser.h"
#include "Parser.h"
#include "ClosureStmnt.h"

NS_STONE_BEGIN

ClosureParser::ClosureParser()
{
	this->init();
}

bool ClosureParser::init()
{
	//primary: "fun" param_list block
	_primary->insertChoice(Parser::rule(ClosureStmnt::TREE_ID)
		->sep(1, "closure")->ast(_paramList)->ast(_block));

	return true;
}
NS_STONE_END