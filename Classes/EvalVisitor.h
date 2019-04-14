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
	//以下三个函数不会调用到
	virtual void visit(ASTree* t, Environment* env);
	virtual void visit(ASTList* t, Environment* env);
	virtual void visit(ASTLeaf* t, Environment* env);

	//获取其内包含的数值，并放入result中
	virtual void visit(NumberLiteral* t, Environment* env);
	//获取其内包含的字符串，并放入result中
	virtual void visit(StringLiteral* t, Environment* env);
	//获取对应名字的Value，并放入result中
	virtual void visit(Name* t, Environment* env);
	//目前仅限于对整型的转换
	virtual void visit(NegativeExpr* t, Environment* env);
	//计算表达式，计算的值存入result中
	virtual void visit(BinaryExpr* t, Environment* env);
	//运行块代码，最后一个语句的值存入result中
	virtual void visit(BlockStmnt* t, Environment* env);
	//运行条件语句块,最后一个语句的值存入result中
	virtual void visit(IfStmnt* t, Environment* env);
	//运行循环语句块，最后一个语句的值存入result中
	virtual void visit(WhileStmnt* t, Environment* env);

	//----------------函数相关-----------------
	//函数执行
	virtual void visit(PrimaryExpr* t, Environment* env);
	//一般不会执行
	virtual void visit(Postfix* t, Environment* env);
	//真正的函数执行
	virtual void visit(Arguments* t, Environment* env);
	//函数定义，会添加函数到环境中，并把函数名放在result中
	virtual void visit(DefStmnt* t, Environment* env);
private:
	//------BinaryExpr----
	Value computeOp(ASTree* t, const Value& left, const std::string& op, const Value& right);
	//TODO: 暂时为整数
	int computeNumber(ASTree* t, int left, const std::string& op, int right);

	//------PrimaryExpr-----
	void evalSubExpr(PrimaryExpr* t, Environment* env, int nest);
};
NS_STONE_END
#endif // ! __Stone_EvalVisitor_H__
