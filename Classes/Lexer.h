#ifndef __Lexer_H__
#define __Lexer_H__
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

//һ�е�����ַ�����
#define MAX_LENGTH 256

class Token;
class Lexer
{
public:
	static std::string REGEX_POT;
public:
	Lexer(const char* buffer);
	~Lexer();
	Token* read();
	Token* peek(int i);

protected:
	//��ȡ��
	void readLine();
	//���token
	void addToken(int lineNo, std::cmatch& matcher);
	//�����ַ�����ȥ��һЩ�ַ�
	std::string toStringLiteral(const std::string& s);
private:
	bool fillQueue(int i);
private:
	//c++������ƥ����
	std::regex _pattern;
	//�����ַ���
	std::stringstream _reader;
	bool _hasMore;
	//��ǰ��
	int _lineNo;
	std::vector<Token*> _queue;
};

#endif
