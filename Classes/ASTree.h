#ifndef __ASTree_H__
#define __ASTree_H__

#include <string>
#include <vector>

#include "StoneMarcos.h"

NS_STONE_BEGIN

class ASTree {
public:
	virtual ~ASTree(){ }
	virtual ASTree* getChild(unsigned int i) const = 0;
	virtual int getNumChildren() const = 0;
	virtual std::string getLocation() const = 0;
	virtual std::vector<ASTree*>::iterator begin() = 0;
	virtual std::vector<ASTree*>::iterator end() = 0;
	virtual std::string toString() const = 0;
};

NS_STONE_END
#endif
