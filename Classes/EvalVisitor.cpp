#include "EvalVisitor.h"
#include "Token.h"
#include "Environment.h"
#include "StoneException.h"
#include "ASTree.h"
#include "ASTList.h"
#include "ASTLeaf.h"
#include "NumberLiteral.h"
#include "StringLiteral.h"
#include "Name.h"
#include "NegativeExpr.h"
#include "BinaryExpr.h"
#include "BlockStmnt.h"
#include "IfStmnt.h"
#include "WhileStmnt.h"
#include "PrimaryExpr.h"
#include "Postfix.h"
#include "Arguments.h"
#include "ParameterList.h"
#include "DefStmnt.h"
#include "Function.h"
#include "ClosureStmnt.h"
#include "STAutoreleasePool.h"
#include "STObject.h"
#include "ScriptFunction.h"
#include "ArrayLiteral.h"
#include "ArrayRef.h"

NS_STONE_BEGIN
EvalVisitor::EvalVisitor()
	:result(nullptr)
	,_allocated(false)
{
}

EvalVisitor::~EvalVisitor()
{
	if (_allocated)
	{
		delete result;
	}
}

void EvalVisitor::visit(ASTree* t, Environment* env)
{
}

void EvalVisitor::visit(ASTList* t, Environment* env)
{
}

void EvalVisitor::visit(ASTLeaf* t, Environment* env)
{
}

void EvalVisitor::visit(NumberLiteral* t, Environment* env)
{
	//暂时只支持整型
	this->setResult(t->getToken()->asInt());
}

void EvalVisitor::visit(StringLiteral* t, Environment* env)
{
	this->setResult(t->getToken()->asString());
}

void EvalVisitor::visit(Name* t, Environment* env)
{
	//获取变量的名称
	auto name = t->getName();
	//获取变量对应的值
	Value* value = env->get(name);
	if (value == nullptr)
	{
		throw StoneException("undefined name: " + name, t);
	}
	else
	{
		this->setResult(value);
	}
}

void EvalVisitor::visit(NegativeExpr* t, Environment* env)
{
	//计算操作数
	t->getOperand()->accept(this, env);
	//TODO:只有整型才能使用负号
	if (this->result->getType() == Value::Type::INTEGER)
	{
		this->setResult(-this->result->asInt());
	}
	else
	{
		throw StoneException("bad type for -", t);
	}
}

void EvalVisitor::visit(BinaryExpr* t, Environment* env)
{
	//获取操作符
	std::string op = t->getOperator();
	//赋值语句
	if (op == "=")
	{
		//计算右值
		t->getRight()->accept(this, env);
		//暂存值
		Value right = *this->result;
		//左值必须是Name,即可修改的左值
		Name* left = dynamic_cast<Name*>(t->getLeft());
		bool ret = false;
		//尝试判断是否是PrimaryExpr
		if (left == nullptr)
		{
			PrimaryExpr* primary = static_cast<PrimaryExpr*>(t->getLeft());

			if (primary->getNumChildren() > 1)
			{
				//计算得出索引
				ArrayRef* ref = static_cast<ArrayRef*>(primary->getChild(primary->getNumChildren() - 1));
				ref->getIndex()->accept(this, env);

				if (this->result->getType() == Value::Type::INTEGER)
				{
					auto index = this->result->asInt();
					//获取数组
					this->evalSubExpr(primary, env, 1);
					std::vector<Value>& list = this->result->asValueVector();
					list[index] = right;
					ret = true;
					//保存值
					this->setResult(right);
				}
			}
		}
		//添加到环境中
		else
		{
			env->put(left->getName(), right);
			ret = true;
		}
		if (!ret)
			throw StoneException("bad assignment", t);
	}
	else
	{
		//计算左值
		t->getLeft()->accept(this, env);
		Value left = *this->result;
		//计算右值
		t->getRight()->accept(this, env);
		Value right = *this->result;

		//计算值
		Value value = this->computeOp(t, left, op, right);
		//保存值
		this->setResult(value);
	}
}

void EvalVisitor::visit(BlockStmnt* t, Environment* env)
{
	//遍历节点，并调用accept
	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = *it;
		child->accept(this, env);
	}
}

void EvalVisitor::visit(IfStmnt* t, Environment* env)
{
	//获取当前的条件语句的个数
	unsigned int size = t->getIfNumber();

	//遍历if {elseif}
	for (unsigned int i = 0; i < size; i++)
	{
		//判断条件语句
		t->getCondition(i)->accept(this, env);
		//判断返回值为true,则执行该语句块，并退出
		if (this->result->asBool())
		{
			t->getThenBlock(i)->accept(this, env);
			return ;
		}
	}
	//以上不满足条件，则尝试执行else块
	if (t->getElseBlock() != nullptr)
	{
		t->getElseBlock()->accept(this, env);
	}
}

void EvalVisitor::visit(WhileStmnt* t, Environment* env)
{
	Value value;
	do 
	{
		//条件判断
		t->getCondition()->accept(this, env);
		//不满足条件则退出
		if (!this->result->asBool())
			break;
		//执行语句
		t->getBody()->accept(this, env);
		//暂存返回值
		value = this->result;

	} while (1);
	this->setResult(value);
}

void EvalVisitor::visit(PrimaryExpr* t, Environment* env)
{
	//Name {Arguments}, 可执行类似于 fib(2)(3)等
	this->evalSubExpr(t, env, 0);
}

void EvalVisitor::visit(Postfix* t, Environment* env)
{
}

void EvalVisitor::visit(Arguments* t, Environment* env)
{
	Function* function = this->result->asFunction();
	//个数不同，函数调用失败
	if (t->getSize() != function->getParamSize())
		throw StoneException("bad number of arguments", t);

	//创建一个新的环境
	Environment* newEnv = function->makeEnv();
	//放入参数和对应的值
	for (int i = 0; i < t->getNumChildren(); i++)
	{
		auto args = t->getChild(i);
		//先计算
		args->accept(this, env);
		//添加变量到环境中
		newEnv->putNew(function->getParamName(i), *this->result);
	}
	//执行函数体
	function->execute(this, newEnv);
	//释放环境
	newEnv->release();
}

void EvalVisitor::visit(DefStmnt* t, Environment* env)
{
	//直接在本环境下添加Function对象
	Function* function = new ScriptFunction(t->getParameters(), t->getBody(), env);
	Value value = Value(function);

	env->putNew(t->getName(), value);
	this->setResult(t->getName());

	function->release();
}

void EvalVisitor::visit(ClosureStmnt* t, Environment* env)
{
	Function* closure = new ScriptFunction(t->getParameters(), t->getBody(), env);
	closure->autorelease();
	//保存值
	this->setResult(closure);
}

void EvalVisitor::visit(ArrayLiteral* t, Environment* env)
{
	std::vector<Value> list;

	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = (*it);
		//获取值
		child->accept(this, env);
		list.push_back(*this->result);
	}
	//把生成的数组添加到result中
	//this->result = list;
	this->setResult(list);
}

void EvalVisitor::visit(ArrayRef* t, Environment* env)
{
	//暂时保存数组
	std::vector<Value> list = this->result->asValueVector();
	//获取索引
	t->getIndex()->accept(this, env);
	Value index = *this->result;

	//目前索引仅仅支持整型
	if (index.getType() == Value::Type::INTEGER)
	{
		this->setResult(list.at(index.asInt()));
	}
	else
	{
		throw StoneException("bad array access", t);
	}
}

void EvalVisitor::setResult(int value)
{
	Value v = Value(value);
	this->setResult(v);
}

void EvalVisitor::setResult(const std::string& value)
{
	Value v = Value(value);
	this->setResult(v);
}

void EvalVisitor::setResult(const std::vector<Value>& value)
{
	Value v = Value(value);
	this->setResult(v);
}

void EvalVisitor::setResult(Function* value)
{
	Value v = Value(value);
	this->setResult(v);
}

void EvalVisitor::setResult(const Value& value)
{
	if (!_allocated)
	{
		result = new Value(value);
		_allocated = true;
	}
	else
	{
		*result = value;
	}
}

void EvalVisitor::setResult(Value* value)
{
	if (_allocated)
	{
		delete result;
		result = nullptr;
		_allocated = false;
	}
	result = value;
}
//---------------------------------BinaryExpr---------------------------
Value EvalVisitor::computeOp(ASTree* t, const Value& left, const std::string& op, const Value& right)
{
	Value value;
	//TODO:目前仅支持整型
	if (left.getType() == Value::Type::INTEGER && right.getType() == Value::Type::INTEGER)
	{
		value = this->computeNumber(t, left.asInt(), op, right.asInt());
	}
	//转换为字符串
	else if ("+" == op)
	{
		value = Value(left.asString() + right.asString());
	}
	else if ("==" == op)
	{
		value = Value(left == right ? true: false);
	}
	else
	{
		throw StoneException("bad type", t);
	}

	return value;
}

int EvalVisitor::computeNumber(ASTree* t, int left, const std::string& op, int right)
{
	if ("+" == op)
		return left + right;
	else if ("-" == op)
		return left - right;
	else if ("*" == op)
		return left * right;
	else if ("/" == op)
		return left / right;
	else if ("%" == op)
		return left % right;
	else if ("==" == op)
		return left == right;
	else if (">" == op)
		return left > right;
	else if ("<" == op)
		return left < right;
	else
		throw StoneException("bad operator", t);
}

//---------------------------PrimaryExpr---------------------
void EvalVisitor::evalSubExpr(PrimaryExpr* t, Environment* env, int nest)
{
	//对于形如 foo(2)(3) 依次从左往右调用
	if (t->getNumChildren() - nest > 1)
	{
		this->evalSubExpr(t, env, nest + 1);
		//获取实参
		auto postfix = t->getChild(t->getNumChildren() - nest - 1);
		//调用Arguments,即调用函数 这里的函数已经在this->result之中
		postfix->accept(this, env);
	}
	else
	{
		//返回名字对应的函数，并放入this->result之中
		auto name = t->getChild(0);
		name->accept(this, env);
	}
}
NS_STONE_END
