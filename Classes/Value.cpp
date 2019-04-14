#include "Value.h"
#include "Function.h"
#include "StoneException.h"
NS_STONE_BEGIN

const Value Value::Null;

Value::Value()
	:_type(Type::NONE)
{
	memset(&_field, 0, sizeof(_field));
}

Value::Value(unsigned char v)
	: _type(Type::BYTE)
{
	_field.byteVal = v;
}

Value::Value(int v)
	: _type(Type::INTEGER)
{
	_field.intVal = v;
}

Value::Value(float v)
	: _type(Type::FLOAT)
{
	_field.floatVal = v;
}

Value::Value(double v)
	: _type(Type::DOUBLE)
{
	_field.doubleVal = v;
}

Value::Value(bool v)
	: _type(Type::BOOLEAN)
{
	_field.boolVal = v;
}

Value::Value(const char* v)
	: _type(Type::STRING)
{
	_field.stringVal = new std::string();
	if (v != nullptr)
	{
		*_field.stringVal = v;
	}
}

Value::Value(const std::string& v)
	:_type(Type::STRING)
{
	_field.stringVal = new std::string();
	*_field.stringVal = v;
}

Value::Value(Function* function)
	:_type(Type::FUNCTION)
{
	function->retain();
	_field.functionVal = function;
}

Value::Value(const ValueVector& v)
	: _type(Type::VECTOR)
{
	_field.vectorVal = new ValueVector();
	*_field.vectorVal = v;
}

Value::Value(const ValueMap& v)
	: _type(Type::MAP)
{
	_field.mapVal = new ValueMap();
	*_field.mapVal = v;
}

Value::Value(const ValueMapIntKey& v)
	: _type(Type::INT_KEY_MAP)
{
	_field.intKeyMapVal = new ValueMapIntKey();
	*_field.intKeyMapVal = v;
}

Value::Value(const Value& v)
	: _type(Type::NONE)
{
	*this = v;
}

Value::~Value()
{
	clear();
}

//operator
Value& Value::operator=(const Value& v)
{
	//�������ַ��ͬ��ֱ�ӷ���
	if (this == &v)
		return *this;

	this->reset(v._type);

	switch (v._type)
	{
	case Type::BYTE:_field.byteVal = v._field.byteVal; break;
	case Type::INTEGER:_field.intVal = v._field.intVal; break;
	case Type::FLOAT:_field.floatVal = v._field.floatVal; break;
	case Type::DOUBLE:_field.doubleVal = v._field.doubleVal; break;
	case Type::BOOLEAN:_field.boolVal = v._field.boolVal; break;
	case Type::STRING:
	{
		if (_field.stringVal == nullptr)
			_field.stringVal = new std::string();

		*_field.stringVal = v._field.stringVal->c_str();
	}; break;
	case Type::FUNCTION:
	{
		v.asFunction()->retain();
		if (_field.functionVal != nullptr)
			_field.functionVal->release();
		_field.functionVal = v.asFunction();
	}break;
	case Type::VECTOR:
	{
		if (_field.vectorVal == nullptr)
			_field.vectorVal = new ValueVector();
		*_field.vectorVal = *(v._field.vectorVal);
	}; break;
	case Type::MAP:
	{
		if (_field.mapVal == nullptr)
			_field.mapVal = new ValueMap();
		*_field.mapVal = *v._field.mapVal;
	}break;
	case Type::INT_KEY_MAP:
	{
		if (_field.mapVal == nullptr)
			_field.intKeyMapVal = new ValueMapIntKey();
		*_field.intKeyMapVal = *v._field.intKeyMapVal;
	}break;
	default:break;
	}
	return *this;
}

Value& Value::operator=(unsigned char v)
{
	reset(Type::BYTE);
	_field.byteVal = v;
	return *this;
}

Value& Value::operator=(int v)
{
	reset(Type::INTEGER);
	_field.intVal = v;
	return *this;
}

Value& Value::operator=(float v)
{
	reset(Type::FLOAT);
	_field.floatVal = v;
	return *this;
}

Value& Value::operator=(double v)
{
	reset(Type::DOUBLE);
	_field.doubleVal = v;
	return *this;
}

Value& Value::operator=(bool v)
{
	reset(Type::BYTE);
	_field.byteVal = v;
	return *this;
}

Value& Value::operator=(const char* v)
{
	reset(Type::STRING);
	*_field.stringVal = v;
	return *this;
}

Value& Value::operator=(const std::string& v)
{
	reset(Type::STRING);
	*_field.stringVal = v;
	return *this;
}

Value& Value::operator=(Function* func)
{
	//ԭ��Ϊfunction
	if (_type == Type::FUNCTION && _field.functionVal != nullptr)
		_field.functionVal->release();

	reset(Type::FUNCTION);
	func->retain();
	_field.functionVal = func;

	return *this;
}

Value& Value::operator=(const ValueVector& v)
{
	reset(Type::VECTOR);
	*_field.vectorVal = v;
	return *this;
}

Value& Value::operator=(const ValueMap& v)
{
	reset(Type::MAP);
	*_field.mapVal = v;
	return *this;
}

Value& Value::operator=(const ValueMapIntKey& v)
{
	reset(Type::INT_KEY_MAP);
	*_field.intKeyMapVal = v;
	return *this;
}

bool Value::operator==(const Value& v)const
{
	if (this == &v)
		return true;
	if (_type != v._type)
		return false;
	switch (_type)
	{
	case Type::BYTE:return _field.byteVal == v._field.byteVal; break;
	case Type::INTEGER:return _field.intVal == v._field.intVal; break;
	case Type::FLOAT:return _field.floatVal == v._field.floatVal; break;
	case Type::DOUBLE:return _field.doubleVal == v._field.doubleVal; break;
	case Type::BOOLEAN:return _field.boolVal == v._field.boolVal; break;
	case Type::STRING:return *_field.stringVal == *v._field.stringVal; break;
	case Type::FUNCTION: 
		return _field.functionVal == v._field.functionVal; 
		break;
	case Type::VECTOR:return *_field.vectorVal == *v._field.vectorVal; break;
	case Type::MAP:return *_field.mapVal == *v._field.mapVal; break;
	case Type::INT_KEY_MAP:return *_field.intKeyMapVal == *v._field.intKeyMapVal; break;
	default:break;
	}
	return false;
}

bool Value::operator!=(const Value& v)
{
	if (this == &v)
		return false;
	if (_type != v._type)
		return true;
	switch (_type)
	{
	case Type::BYTE:return _field.byteVal != v._field.byteVal; break;
	case Type::INTEGER:return _field.intVal != v._field.intVal; break;
	case Type::FLOAT:return _field.floatVal != v._field.floatVal; break;
	case Type::DOUBLE:return _field.doubleVal != v._field.doubleVal; break;
	case Type::BOOLEAN:return _field.boolVal != v._field.boolVal; break;
	case Type::STRING:return *_field.stringVal != *v._field.stringVal; break;
	case Type::FUNCTION:
		return _field.functionVal != v._field.functionVal;
		break;
	case Type::VECTOR:return *_field.vectorVal != *v._field.vectorVal; break;
	case Type::MAP:return *_field.mapVal != *v._field.mapVal; break;
	case Type::INT_KEY_MAP:return *_field.intKeyMapVal != *v._field.intKeyMapVal; break;

	default:break;
	}
	return true;
}

unsigned char Value::asByte() const
{
	//�޷�ת��
	if (_type == Type::VECTOR)
		return 0;
	//Ŀǰ��˵����ת��
	switch (_type)
	{
	case Type::BYTE:return _field.byteVal; break;
	case Type::INTEGER:return static_cast<unsigned char>(_field.intVal); break;
	case Type::FLOAT:return static_cast<unsigned char>(_field.floatVal); break;
	case Type::DOUBLE:return static_cast<unsigned char>(_field.doubleVal); break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<unsigned char>(std::atoi(_field.stringVal->c_str())); break;
	default:break;
	}
	return 0;
}

int Value::asInt()const
{
	//�޷�ת��
	if (_type == Type::VECTOR)
		return 0;
	switch (_type)
	{
	case Type::BYTE:return _field.byteVal; break;
	case Type::INTEGER:return _field.intVal; break;
	case Type::FLOAT:return static_cast<int>(_field.floatVal); break;
	case Type::DOUBLE:return static_cast<int>(_field.doubleVal); break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<int>(atoi(_field.stringVal->c_str())); break;
	default:break;
	}
	return 0;
}

float Value::asFloat()const
{
	//�޷�ת��
	if (_type == Type::VECTOR)
		return 0.f;
	switch (_type)
	{
	case Type::BYTE:return static_cast<float>(_field.byteVal); break;
	case Type::INTEGER:return static_cast<float>(_field.intVal); break;
	case Type::FLOAT:return _field.floatVal; break;
	case Type::DOUBLE:return static_cast<float>(_field.doubleVal); break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<float>(atof(_field.stringVal->c_str())); break;
	default:break;
	}
	return 0.f;
}

double Value::asDouble()const
{
	//�޷�ת��
	if (_type == Type::VECTOR)
		return 0;
	switch (_type)
	{
	case Type::BYTE:return static_cast<double>(_field.byteVal); break;
	case Type::INTEGER:return static_cast<double>(_field.intVal); break;
	case Type::FLOAT:return static_cast<double>(_field.floatVal); break;
	case Type::DOUBLE:return _field.doubleVal; break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<double>(atof(_field.stringVal->c_str())); break;
	default:break;
	}
	return 0.0;
}

bool Value::asBool() const
{
	bool ret = false;
	//�޷�ת��
	if (_type == Type::VECTOR)
		return ret;

	switch (_type)
	{
	case Type::BYTE:
		ret =  (_field.byteVal == 0 ? false : true);
		break;
	case Type::INTEGER:
		ret = (_field.intVal == 0 ? false : true);
		break;
	case Type::FLOAT:
		ret = (_field.floatVal == 0.f ? false : true);
		break;
	case Type::DOUBLE:
		ret = (_field.doubleVal == 0.0 ? false : true);
		break;
	case Type::BOOLEAN:
		ret = _field.boolVal;
		break;
	case Type::STRING:
		ret = (*_field.stringVal == "0" || *_field.stringVal == "false" ? false : true); 
		break;
	default:break;
	}
	return ret;
}

std::string Value::asString()const
{
	//�޷�ת��
	if (_type == Type::VECTOR)
		return "";
	if (_type == Type::STRING)
		return *_field.stringVal;

	std::stringstream ret;
	switch (_type)
	{
	case Type::BYTE:
		ret << _field.byteVal; 
		break;
	case Type::INTEGER:
		ret << _field.intVal;
		break;
	case Type::FLOAT:
		ret << _field.floatVal;
		break;
	case Type::DOUBLE:
		ret << _field.doubleVal; 
		break;
	case Type::BOOLEAN:
		ret << (_field.boolVal ? "true" : "false"); 
		break;
	}
	return ret.str();
}

Function* Value::asFunction() const
{
	if (_type != Type::FUNCTION)
		throw StoneException("the type is not function");
	return _field.functionVal;
}

ValueVector &Value::asValueVector()const
{
	if (_type != Type::VECTOR)
		throw StoneException("the type is not vector");
	return *_field.vectorVal;
}

ValueMap &Value::asValueMap()const
{
	if (_type != Type::VECTOR)
		throw StoneException("the type is not map");
	return *_field.mapVal;
}

ValueMapIntKey &Value::asValueIntKey()const
{
	//TODO:SDLASSERT(_type == Type::INT_KEY_MAP, "type����INT_KEY_MAP");
	return *_field.intKeyMapVal;
}

void Value::clear()
{
	switch (_type)
	{
	case Type::BYTE:_field.byteVal = 0; break;
	case Type::INTEGER:_field.intVal = 0; break;
	case Type::FLOAT:_field.floatVal = 0.f; break;
	case Type::DOUBLE:_field.doubleVal = 0.0; break;
	case Type::BOOLEAN:_field.boolVal = false; break;
	case Type::STRING:STONE_SAFE_DELETE(_field.stringVal); break;
	case Type::FUNCTION: 
		if (_field.functionVal != nullptr)
			_field.functionVal->release();
		break;
	case Type::VECTOR:STONE_SAFE_DELETE(_field.vectorVal); break;
	case Type::MAP:STONE_SAFE_DELETE(_field.mapVal); break;
	case Type::INT_KEY_MAP:STONE_SAFE_DELETE(_field.intKeyMapVal); break;
	default:break;
	}
	memset(&_field, 0, sizeof(_field));
	_type = Type::NONE;
}

void Value::reset(Type type)
{
	if (_type == type)
		return;
	clear();
	//����ռ��
	switch (type)
	{
	case Type::STRING:
		_field.stringVal = new std::string(); break;
	case Type::VECTOR:
		_field.vectorVal = new ValueVector(); break;
	case Type::MAP:
		_field.mapVal = new ValueMap(); break;
	case Type::INT_KEY_MAP:
		_field.intKeyMapVal = new ValueMapIntKey(); break;
	default:break;
	}
	_type = type;
}
NS_STONE_END