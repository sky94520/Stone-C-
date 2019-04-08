#ifndef __Token_H__
#define __Token_H__

#include <string>

/*
	���ʣ��б�ʶ�������ֺ��ַ���
*/
class Token
{
public:
	enum class Type
	{
		None,
		Identifier,//��ʶ��
		Number,	   //����
		String,	   //�ַ���
	};
public:
	Token(int line);
	~Token();
	//��ȡ�к�
	int getLineNumber() const { return _line; }
	//��ȡ���ʵ�����
	Type getType() const { return _type; }
	//��ȡ�ı�
	virtual std::string getText() const { return ""; }
	//��ȡ����
	virtual int getNumber() const { return 0; }
protected:
	int _line;
	Type _type;
public:
	//end of file
	static Token* const TOKEN_EOF;
	//end of line
	static const std::string TOKEN_EOL;
};

/*
	NumToken ����
*/
class NumToken: public Token
{
public:
	NumToken(int line, int value);
	virtual std::string getText() const;
	virtual int getNumber() const;
private:
	int _value;
};

/*
	��ʶ��
*/
class IdToken : public Token
{
public:
	IdToken(int line, const std::string& id);
	virtual std::string getText() const;
private:
	std::string _text;
};

/*
	�ַ���StrToken
*/
class StrToken : public Token
{
public:
	StrToken(int line, const std::string& str);
	virtual std::string getText() const;
private:
	std::string _literal;
};
#endif
