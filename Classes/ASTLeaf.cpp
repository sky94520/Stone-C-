#include "ASTLeaf.h"
#include "Token.h"
NS_STONE_BEGIN

std::vector<ASTree*> ASTLeaf::empty = std::vector<ASTree*>();

ASTLeaf::ASTLeaf(Token* token) 
	:_token(token)
{
}

ASTLeaf::~ASTLeaf()
{
	delete _token;
}

Token* ASTLeaf::getToken() const
{
	return _token;
}

ASTree* ASTLeaf::getChild(unsigned int i) const
{
	return nullptr;
}

int ASTLeaf::getNumChildren() const
{
	return 0;
}

std::string ASTLeaf::getLocation() const
{
	return "at line " + _token->asString();
}

std::vector<ASTree*>::iterator ASTLeaf::begin()
{
	return ASTLeaf::empty.begin();
}

std::vector<ASTree*>::iterator ASTLeaf::end()
{
	return ASTLeaf::empty.end();
}

std::string ASTLeaf::toString() const
{
	return _token->asString();
}
NS_STONE_END