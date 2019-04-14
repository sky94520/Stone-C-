#ifndef __Stone_Arguments_H__
#define __Stone_Arguments_H__

#include <vector>

#include "Postfix.h"

NS_STONE_BEGIN

class Visitor;
class Environment;

class Arguments : public Postfix
{
public:
	Arguments();
	Arguments(const std::vector<ASTree*>& list);
	virtual ~Arguments();

	unsigned getSize() const;
public:
	virtual void accept(Visitor* v, Environment* env);
};
NS_STONE_END
#endif