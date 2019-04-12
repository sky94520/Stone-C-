#ifndef __Stone_EvalVisitor_H__
#define __Stone_EvalVisitor_H__

#include "Visitor.h"
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

class EvalVisitor : public Visitor 
{
public:
	Value result;
public:
	virtual void visit(ASTree* t, Environment* env);
	virtual void visit(ASTList* t, Environment* env);
	virtual void visit(ASTLeaf* t, Environment* env);
	virtual void visit(NumberLiteral* t, Environment* env);
	virtual void visit(StringLiteral* t, Environment* env);
	virtual void visit(Name* t, Environment* env);
	virtual void visit(NegativeExpr* t, Environment* env);
	virtual void visit(BinaryExpr* t, Environment* env);
	virtual void visit(BlockStmnt* t, Environment* env);
	virtual void visit(IfStmnt* t, Environment* env);
	virtual void visit(WhileStmnt* t, Environment* env);
private:
};
NS_STONE_END
#endif // ! __Stone_EvalVisitor_H__
