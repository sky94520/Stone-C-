#include "BasicParser.h"
#include "Parser.h"
#include "PrimaryExpr.h"
#include "NumberLiteral.h"
#include "Name.h"
#include "StringLiteral.h"
#include "NegativeExpr.h"
#include "BinaryExpr.h"
#include "Token.h"
#include "BlockStmnt.h"
#include "IfStmnt.h"
#include "WhileStmnt.h"
#include "NullStmnt.h"

NS_STONE_BEGIN
BasicParser::BasicParser()
	:_operators(nullptr)
	,_reserved(nullptr)
	,_primary(nullptr)
	,_factor(nullptr)
	,_expr(nullptr)
	,_statement(nullptr)
	,_block(nullptr)
	,_simple(nullptr)
	,_program(nullptr)
{
	_operators = new Operators();
	_reserved = new std::unordered_set<std::string>();

	Parser* expr0 = Parser::rule();
	//primary: "(" expr ")" | NUMBER | IDENTIFIER | STRING
	_primary = Parser::rule(PrimaryExpr::TREE_ID)->orTree(4, 
		Parser::rule()->sep(1, "(")->ast(expr0)->sep(1, ")"),
		Parser::rule()->number(NumberLiteral::TREE_ID),
		Parser::rule()->identifier(Name::TREE_ID, _reserved),
		Parser::rule()->string(StringLiteral::TREE_ID));

	//factor: "-" primary | primary
	_factor = Parser::rule()->orTree(2,
		Parser::rule(NegativeExpr::TREE_ID)->sep(1, "-")->ast(_primary),
		_primary);

	//expr: factor {OP factor}
	_expr = expr0->expression(BinaryExpr::TREE_ID, _factor, _operators);

	Parser* statement0 = Parser::rule();
	//block: "{" [statement] { (";" | EOL) [statement] } "}"
	_block = Parser::rule(BlockStmnt::TREE_ID)
		->sep(1, "{")->option(statement0)
		->repeat(Parser::rule()->sep(2, ";", Token::TOKEN_EOL)->option(statement0))
		->sep(1, "}");

	//simple: expr
	_simple = Parser::rule(PrimaryExpr::TREE_ID)->ast(_expr);

	/* statement: "if" expr block ["else" block]
				  | "while" expr block
				  | simple
	*/
	_statement = statement0->orTree(3,
		Parser::rule(IfStmnt::TREE_ID)->sep(1, "if")->ast(_expr)->ast(_block)->option(Parser::rule()->sep(1, "else")->ast(_block)),
		Parser::rule(WhileStmnt::TREE_ID)->sep(1, "while")->ast(_expr)->ast(_block),
		_simple
	);

	//program: [statement] (";" | EOL)
	_program = Parser::rule()->orTree(2, _statement, Parser::rule(NullStmnt::TREE_ID))->sep(2, ";", Token::TOKEN_EOL);
	_program->retain();

	this->init();
}

BasicParser::~BasicParser()
{
	_program->release();
	_statement->release();
	_simple->release();
	_block->release();
	_expr->release();
	_factor->release();
	_primary->release();
	//TODO:暂时会发生循环引用
	_statement->release();

	_operators->release();
	delete _reserved;
}

ASTree* BasicParser::parse(Lexer* lexer)
{
	return _program->parse(lexer);
}

bool BasicParser::init()
{
	//添加保留字符
	_reserved->insert(";");
	_reserved->insert("}");
	_reserved->insert(Token::TOKEN_EOL);

	_operators->add("=", 1, Operators::RIGHT);

	_operators->add("==", 2, Operators::LEFT);
	_operators->add(">", 2, Operators::LEFT);
	_operators->add("<", 2, Operators::LEFT);
	
	_operators->add("+", 3, Operators::LEFT);
	_operators->add("-", 3, Operators::LEFT);
	_operators->add("*", 4, Operators::LEFT);
	_operators->add("/", 4, Operators::LEFT);
	_operators->add("%", 4, Operators::LEFT);

	return true;
}

NS_STONE_END