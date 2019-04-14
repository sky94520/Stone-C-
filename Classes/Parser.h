#ifndef __Stone_Parser_H__
#define __Stone_Parser_H__

#include "StoneMarcos.h"
NS_STONE_BEGIN

class ASTree;
class Lexer;
class FuncParser;
class ClosureParser;

class Parser
{
public:
	Parser();
	~Parser();
	ASTree* parse();
	void setLexer(Lexer* lexer);
private:
	ClosureParser* _parser;
};

NS_STONE_END
#endif