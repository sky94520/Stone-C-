#ifndef __Stone_Visitor_H__
#define __Stone_Visitor_H__

#include "StoneMarcos.h"
#include "Value.h"

NS_STONE_BEGIN

class Environment;
class ASTree;
class ASTList;
class ASTLeaf;
class NumberLiteral;
class StringLiteral;
class Name;
class NegativeExpr;
class BinaryExpr;
class BlockStmnt;
class IfStmnt;
class WhileStmnt;

class PrimaryExpr;
class Postfix;
class Arguments;
class ParameterList;
class DefStmnt;

class Visitor
{
public:
	virtual void visit(ASTree* t, Environment* env) = 0;
	virtual void visit(ASTList* t, Environment* env) = 0;
	virtual void visit(ASTLeaf* t, Environment* env) = 0;
	virtual void visit(NumberLiteral* t, Environment* env) = 0;
	virtual void visit(StringLiteral* t, Environment* env) = 0;
	virtual void visit(Name* t, Environment* env) = 0;
	virtual void visit(NegativeExpr* t, Environment* env) = 0;
	virtual void visit(BinaryExpr* t, Environment* env) = 0;
	virtual void visit(BlockStmnt* t, Environment* env) = 0;
	virtual void visit(IfStmnt* t, Environment* env) = 0;
	virtual void visit(WhileStmnt* t, Environment* env) = 0;
	
	//º¯ÊýÏà¹Ø
	virtual void visit(PrimaryExpr* t, Environment* env) = 0;
	virtual void visit(Postfix* t, Environment* env) = 0;
	virtual void visit(Arguments* t, Environment* env) = 0;
	virtual void visit(DefStmnt* t, Environment* env) = 0;
};

NS_STONE_END
#endif