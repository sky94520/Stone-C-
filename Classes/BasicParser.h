#ifndef __Stone_BasicParser_H__
#define __Stone_BasicParser_H__

#include <string>
#include <unordered_set>

#include "StoneMarcos.h"
NS_STONE_BEGIN

class ASTree;
class Parser;
class Lexer;
class Operators;

class BasicParser
{
public:
	BasicParser();
	virtual ~BasicParser();

	ASTree* parse(Lexer* lexer);
private:
	bool init();
protected:
	//²Ù×÷·û
	Operators* _operators;
	std::unordered_set<std::string>* _reserved;

	Parser* _primary;
	Parser* _factor;
	Parser* _expr;
	Parser* _block;
	Parser* _simple;
	Parser* _statement;
	Parser* _program;
};
NS_STONE_END
#endif