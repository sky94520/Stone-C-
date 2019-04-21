#ifndef __Stone_ClosureParser_H__
#define __Stone_ClosureParser_H__

#include "FuncParser.h"

NS_STONE_BEGIN

class ClosureParser : public FuncParser
{
public:
	ClosureParser();
private:
	bool init();
};
NS_STONE_END
#endif // !__Stone_ClosureParser_H__
