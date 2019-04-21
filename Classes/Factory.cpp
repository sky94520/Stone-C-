#include "Factory.h"
#include "Token.h"
#include "Name.h"
#include "NumberLiteral.h"
#include "StringLiteral.h"

#include "ASTList.h"
#include "Postfix.h"
#include "Arguments.h"
#include "ArrayLiteral.h"
#include "ArrayRef.h"
#include "BinaryExpr.h"
#include "BlockStmnt.h"
#include "ClosureStmnt.h"
#include "DefStmnt.h"
#include "IfStmnt.h"
#include "NegativeExpr.h"
#include "ParameterList.h"
#include "PrimaryExpr.h"
#include "WhileStmnt.h"
#include "NullStmnt.h"

NS_STONE_BEGIN
ASTree* Factory::make(const std::string& name, const std::vector<ASTree*>& list)
{
	//�еĽڵ���㺢�ӽ���һ��Ҳ����ʡ�ԣ��򷵻��Ӻ���
	if (name != NegativeExpr::TREE_ID &&
		name != BlockStmnt::TREE_ID &&
		name != Arguments::TREE_ID &&
		name != ParameterList::TREE_ID &&
		list.size() == 1)
	{
		return list.at(0);
	}

	ASTList* t = nullptr;

	if (name == ArrayLiteral::TREE_ID)
		t = new ArrayLiteral(list);
	if (name == Arguments::TREE_ID)
		t = new Arguments(list);
	else if (name == ArrayRef::TREE_ID)
		t = new ArrayRef(list);
	else if (name == BinaryExpr::TREE_ID)
		t = new BinaryExpr(list);
	else if (name == ClosureStmnt::TREE_ID)
		t = new ClosureStmnt(list);
	else if (name == DefStmnt::TREE_ID)
		t = new DefStmnt(list);
	else if (name == IfStmnt::TREE_ID)
		t = new IfStmnt(list);
	else if (name == NegativeExpr::TREE_ID)
		t = new NegativeExpr(list);
	else if (name == ParameterList::TREE_ID)
		t = new ParameterList(list);
	else if (name == PrimaryExpr::TREE_ID)
		t = new PrimaryExpr(list);
	else if (name == WhileStmnt::TREE_ID)
		t = new WhileStmnt(list);
	else if (name == BlockStmnt::TREE_ID)
		t = new BlockStmnt(list);
	else if (name == NullStmnt::TREE_ID)
		t = new NullStmnt();
	else
		t = new ASTList(list);

	//�Զ��ͷ�
	t->autorelease();

	return t;
}

ASTLeaf* Factory::make(const std::string& name, Token* token)
{
	ASTLeaf* t = nullptr;
	
	if (name == Name::TREE_ID)
		t = new Name(token);
	else if (name == NumberLiteral::TREE_ID)
		t = new NumberLiteral(token);
	else if (name == StringLiteral::TREE_ID)
		t = new StringLiteral(token);
	else
		t = new ASTLeaf(token);

	//�Զ��ͷ�
	t->autorelease();

	return t;
}
NS_STONE_END