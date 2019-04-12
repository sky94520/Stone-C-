#ifndef __Stone_ASTLeaf_H__
#define __Stone_ASTLeaf_H__

#include <vector>

#include "ASTree.h"

NS_STONE_BEGIN
class Token;
class Visitor;
class Environment;

class ASTLeaf : public ASTree {
public:
	ASTLeaf(Token* token);
	virtual ~ASTLeaf();
	//ªÒ»°token
	Token* getToken() const;

	virtual void accept(Visitor* v, Environment* env);
	virtual ASTree* getChild(unsigned int i) const;
	virtual int getNumChildren() const;
	virtual std::string getLocation() const;

	virtual std::vector<ASTree*>::iterator begin();
	virtual std::vector<ASTree*>::iterator end();
	virtual std::string toString() const;
private:
	static std::vector<ASTree*> empty;
private:
	Token* _token;
};

NS_STONE_END
#endif