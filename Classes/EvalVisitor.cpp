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
	//����ñ���Ϊ���������������Ҫ�ͷ�
	if (_allocated)
	{
		delete result;
	}
}

void EvalVisitor::visit(ASTree* t, Environment* env)
{
	throw StoneException("cannot eval: " + t->toString(), t);
}

void EvalVisitor::visit(ASTList* t, Environment* env)
{
	throw StoneException("cannot eval: " + t->toString(), t);
}

void EvalVisitor::visit(ASTLeaf* t, Environment* env)
{
	throw StoneException("cannot eval: " + t->toString(), t);
}

void EvalVisitor::visit(NumberLiteral* t, Environment* env)
{
	//TODO:��ʱֻ֧������
	this->setResult(t->getToken()->asInt());
}

void EvalVisitor::visit(StringLiteral* t, Environment* env)
{
	this->setResult(t->getToken()->asString());
}

void EvalVisitor::visit(Name* t, Environment* env)
{
	//��ȡ���������ƺ��ڻ����е�ֵ
	std::string name = t->getName();
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
	//���������
	t->getOperand()->accept(this, env);

	//TODO:ֻ�����Ͳ���ʹ�ø���
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
	//��ȡ������
	std::string op = t->getOperator();
	//��ֵ���
	if (op == "=")
	{
		this->computeAssign(t, env);
	}
	else
	{
		this->computeOp(t, op, env);
	}
}

void EvalVisitor::visit(BlockStmnt* t, Environment* env)
{
	//�����ڵ㣬������accept
	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = *it;
		child->accept(this, env);
	}
}

void EvalVisitor::visit(IfStmnt* t, Environment* env)
{
	//��ȡ��ǰ���������ĸ���
	unsigned int size = t->getIfNumber();

	//����if {elif}
	for (unsigned int i = 0; i < size; i++)
	{
		//�ж�������� ������ֵΪtrue,��ִ�и����飬���˳�
		t->getCondition(i)->accept(this, env);
		if (this->result->asBool())
		{
			t->getThenBlock(i)->accept(this, env);
			return ;
		}
	}
	//���ϲ���������������ִ��else��
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
		//�����ж�
		t->getCondition()->accept(this, env);
		//�������������˳�
		if (!this->result->asBool())
		{
			break;
		}
		//ִ�����
		t->getBody()->accept(this, env);
		//�ݴ淵��ֵ
		value = *this->result;

	} while (true);
	//�������һ������ֵ
	this->setResult(value);
}

void EvalVisitor::visit(PrimaryExpr* t, Environment* env)
{
	//Name {Arguments}, ��ִ�������� fib(2)(3)��
	this->evalSubExpr(t, env, 0);
}

void EvalVisitor::visit(Postfix* t, Environment* env)
{
}

void EvalVisitor::visit(Arguments* t, Environment* env)
{
	Function* function = this->result->asFunction();
	//������ͬ����������ʧ��
	if (t->getSize() != function->getParamSize())
		throw StoneException("bad number of arguments", t);

	//����һ���µ����Զ��ͷŵĻ���
	Environment* newEnv = function->makeEnv();
	//��������Ͷ�Ӧ��ֵ
	for (int i = 0; i < t->getNumChildren(); i++)
	{
		auto args = t->getChild(i);
		//�ȼ��㲢��ӱ�����������
		args->accept(this, env);
		newEnv->putNew(function->getParamName(i), *this->result);
	}
	//ִ�к�����
	function->execute(this, newEnv);
}

void EvalVisitor::visit(DefStmnt* t, Environment* env)
{
	//ֱ���ڱ����������Function����
	ParameterList* params = t->getParameters();
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
	//����ֵ
	this->setResult(closure);
}

void EvalVisitor::visit(ArrayLiteral* t, Environment* env)
{
	std::vector<Value> list;

	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = (*it);
		//��ȡֵ
		child->accept(this, env);
		list.push_back(*this->result);
	}
	//�����ɵ�������ӵ�result��
	this->setResult(list);
}

void EvalVisitor::visit(ArrayRef* t, Environment* env)
{
	//��ʱ��������
	std::vector<Value> list = this->result->asValueVector();
	//��ȡ����
	t->getIndex()->accept(this, env);
	Value index = *this->result;

	//Ŀǰ��������֧������
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
void EvalVisitor::computeOp(BinaryExpr* t, const std::string& op, Environment* env)
{
	//������ֵ
	t->getLeft()->accept(this, env);
	Value left = *this->result;
	//������ֵ
	t->getRight()->accept(this, env);
	Value right = *this->result;

	Value value;
	//TODO:Ŀǰ��֧������
	if (left.getType() == Value::Type::INTEGER && right.getType() == Value::Type::INTEGER)
	{
		value = this->computeNumber(t, left.asInt(), op, right.asInt());
	}
	//ת��Ϊ�ַ���
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

	//����ֵ
	this->setResult(value);
}

void EvalVisitor::computeAssign(BinaryExpr* t, Environment* env)
{
	//������ֵ���ݴ�ֵ
	t->getRight()->accept(this, env);
	Value right = *this->result;

	//��ֵ������Name,�����޸ĵ���ֵ
	Name* left = nullptr;
	if (t->getLeft()->getTreeID() == Name::TREE_ID)
		left = static_cast<Name*>(t->getLeft());
	bool ret = false;
	//�����ж��Ƿ���PrimaryExpr
	if (left == nullptr)
	{
		PrimaryExpr* primary = static_cast<PrimaryExpr*>(t->getLeft());

		if (primary->getNumChildren() > 1)
		{
			//����ó�����
			ArrayRef* ref = static_cast<ArrayRef*>(primary->getChild(primary->getNumChildren() - 1));
			ref->getIndex()->accept(this, env);

			if (this->result->getType() == Value::Type::INTEGER)
			{
				auto index = this->result->asInt();
				//��ȡ����
				this->evalSubExpr(primary, env, 1);
				std::vector<Value>& list = this->result->asValueVector();
				list[index] = right;
				ret = true;
				//����ֵ
				this->setResult(right);
			}
		}
	}//��ӵ�������
	else
	{
		env->put(left->getName(), right);
		ret = true;
	}
	if (!ret)
		throw StoneException("bad assignment", t);
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
	//�������� foo(2)(3) ���δ������ҵ���
	if (t->getNumChildren() - nest > 1)
	{
		this->evalSubExpr(t, env, nest + 1);
		//��ȡʵ��
		auto postfix = t->getChild(t->getNumChildren() - nest - 1);
		//����Arguments,�����ú��� ����ĺ����Ѿ���this->result֮��
		postfix->accept(this, env);
	}
	else
	{
		//�������ֶ�Ӧ�ĺ�����������this->result֮��
		auto name = t->getChild(0);
		name->accept(this, env);
	}
}
NS_STONE_END
