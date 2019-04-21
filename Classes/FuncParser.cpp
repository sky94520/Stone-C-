#include "FuncParser.h"
#include "Parser.h"
#include "ParameterList.h"
#include "DefStmnt.h"
#include "Arguments.h"

NS_STONE_BEGIN

FuncParser::FuncParser()
	:BasicParser()
	,_param(nullptr)
	,_params(nullptr)
	,_paramList(nullptr)
	,_def(nullptr)
	,_args(nullptr)
	,_postfix(nullptr)
{
	//param: IDENTIFIER
	_param = Parser::rule()->identifier(_reserved);
	//params: param {"," param}
	_params = Parser::rule(ParameterList::TREE_ID)
		->ast(_param)->repeat(Parser::rule()->sep(1, ",")->ast(_param));
	//param_list: "(" [params ] ")"
	_paramList = Parser::rule()->sep(1, "(")->maybe(_params)->sep(1, ")");

	//def: "def" IDENTIFIER param_list block
	_def = Parser::rule(DefStmnt::TREE_ID)
		->sep(1, "def")->identifier(_reserved)->ast(_paramList)->ast(_block);

	//----------------------函数调用----------------
	//args: expr {"," expr}
	_args = Parser::rule(Arguments::TREE_ID)
		->ast(_expr)->repeat(Parser::rule()->sep(1, ",")->ast(_expr));
	//postfix: "(" [args] ")"
	_postfix = Parser::rule()->sep(1, "(")->maybe(_args)->sep(1, ")");
	_postfix->retain();

	this->init();
}

FuncParser::~FuncParser()
{
	_postfix->release();
}

bool FuncParser::init()
{
	//添加保留字符
	_reserved->insert(")");

	_primary->repeat(_postfix);
	_simple->option(_args);

	//program: [ def | statement] (";" | EOL)
	_program->insertChoice(_def);

	return true;
}

NS_STONE_END