#ifndef __Stone_NestedEnv_H__
#define __Stone_NestedEnv_H__

#include <unordered_map>

#include "Environment.h"

NS_STONE_BEGIN

class NestedEnv : public Environment
{
public:
	NestedEnv();
	NestedEnv(Environment* env);
	virtual ~NestedEnv();

	void setOuter(Environment* env);
	
	//���Ұ����ñ�����������Ӧ�Ļ��������ظû���
	Environment* where(const std::string& name);

	//ֱ���ڱ����������/���±���
	virtual void putNew(const std::string& name, const Value& value);
	virtual void put(const std::string& name, const Value& value);
	virtual const Value* get(const std::string& name) const;
	virtual Value* get(const std::string& name);
private:
	std::unordered_map<std::string, Value> _values;
	Environment* _outer;
};
NS_STONE_END
#endif