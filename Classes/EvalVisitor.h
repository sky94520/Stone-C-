#ifndef __Stone_EvalVisitor_H__
#define __Stone_EvalVisitor_H__

#include <string>

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

class PrimaryExpr;
class Postfix;
class Arguments;
class ParameterList;
class DefStmnt;

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

	//函数相关
	virtual void visit(PrimaryExpr* t, Environment* env);
	virtual void visit(Postfix* t, Environment* env);
	virtual void visit(Arguments* t, Environment* env);
	virtual void visit(ParameterList* t, Environment* env, unsigned index);
	virtual void visit(DefStmnt* t, Environment* env);
private:
	//BinaryExpr
	Value computeOp(ASTree* t, const Value& left, const std::string& op, const Value& right);
	//TODO: 暂时为整数
	int computeNumber(ASTree* t, int left, const std::string& op, int right);

	//PrimaryExpr
	void evalSubExpr(PrimaryExpr* t, Environment* env, int nest);
};
NS_STONE_END
#endif // ! __Stone_EvalVisitor_H__
