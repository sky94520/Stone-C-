#include "Arguments.h"
#include "Visitor.h"

NS_STONE_BEGIN
Arguments::Arguments()
{
}

Arguments::Arguments(const std::vector<ASTree*>& list)
	:Postfix(list)
{
}

Arguments::~Arguments()
{
}

unsigned Arguments::getSize() const
{
	return getNumChildren();
}

void Arguments::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END