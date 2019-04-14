#include "NativeFunction.h"
#include "EvalVisitor.h"

NS_STONE_BEGIN

NativeFunction::NativeFunction(const char* params[], int len, const nativeFunc& callback, Environment* env)
	:Function(env)
	,_paramNum(len)
	,_callback(callback)
{
	unsigned i = 0;
	while (i < len)
		_parameters.push_back(params[i++]);
}

NativeFunction::~NativeFunction()
{
}

unsigned int NativeFunction::getParamSize() const
{
	return _paramNum;
}

std::string NativeFunction::getParamName(unsigned index) const
{
	return _parameters[index];
}

void NativeFunction::execute(Visitor* v, Environment* env)
{
	static_cast<EvalVisitor*>(v)->result = _callback(env);
}
NS_STONE_END
