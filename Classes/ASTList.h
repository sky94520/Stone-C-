#ifndef __Stone_ASTList_H__
#define __Stone_ASTList_H__

#include <vector>
#include <string>
#include <sstream>

#include "ASTree.h"

NS_STONE_BEGIN

class ASTree;

class ASTList : public ASTree 
{
public:
	ASTList();
	ASTList(const std::vector<ASTree*>& list);
	virtual ~ASTList();
public:
	virtual ASTree* getChild(unsigned int i) const;
	virtual int getNumChildren() const;
	virtual std::string getLocation() const;

	virtual std::vector<ASTree*>::iterator begin();
	virtual std::vector<ASTree*>::iterator end();
	virtual std::string toString() const;
protected:
	std::vector<ASTree*> _children;
};

NS_STONE_END
#endif