#ifndef __Stone_Parser_H__
#define __Stone_Parser_H__

#include <vector>
#include <string>
#include <typeinfo>
#include <cstdarg>
#include <unordered_set>
#include <unordered_map>

#include "STObject.h"

NS_STONE_BEGIN
class ASTree;
class Lexer;
class Token;

//----------------------------------Precedence--------------------------
//����������ȼ��ͽ����
class Precedence {
public:
	Precedence(int v, bool a) {
		this->value = v;
		this->leftAssoc = a;
	}
public:
	int value;
	//�Ƿ�������
	bool leftAssoc;
};
//----------------------------------Operators--------------------------
//�����
class Operators: public Object
{
public:
	static bool LEFT;
	static bool RIGHT;
public:
	Operators();
	~Operators();
	//��Ӳ����� ���ȼ��ͽ����
	void add(const std::string& name, int prec, bool leftAssoc);
	//�������ƻ�ȡ��Ӧ�����ȼ�
	Precedence* get(const std::string& name);
private:
	std::unordered_map<std::string, Precedence*> _mapping;
};

class Parser: public Object
{
protected:
	//----------------------------------Element--------------------------
	class Element
	{
	public:
		virtual ~Element() {}
	public:
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result) = 0;
		virtual bool match(Lexer* lexer) const = 0;
	};
	//----------------------------------Tree--------------------------
	class Tree : public Element
	{
	public:
		Tree(Parser* parser);
		virtual ~Tree();
	public:
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;
	protected:
		Parser* _parser;
	};
	//----------------------------------OrTree--------------------------
	class OrTree : public Element
	{
	public:
		OrTree(Parser* parsers[], unsigned n);
		OrTree(Parser* p1, Parser* p2);
		virtual ~OrTree();
		//ǰ���룿
		void insert(Parser* parser);
	public:
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;
	protected:
		Parser* choose(Lexer* lexer) const;
	protected:
		std::vector<Parser*> _parsers;
	};
	//----------------------------------Repeat--------------------------
	class Repeat : public Element
	{
	public:
		Repeat(Parser* parser, bool once);
		virtual ~Repeat();
	public:
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;
	protected:
		Parser* _parser;
		bool _onlyOnce;
	};
	//----------------------------------AToken--------------------------
	class AToken : public Element
	{
	public:
		AToken(const std::string& factoryName);
		virtual ~AToken();
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;
	protected:
		virtual bool test(Token* token) const = 0;
	protected:
		std::string _factoryName;
	};
	//----------------------------------IdToken--------------------------
	class IdToken : public AToken
	{
	public:
		IdToken(const std::string& factoryName, const std::unordered_set<std::string>* set);
	protected:
		virtual bool test(Token* token) const;
	private:
		const std::unordered_set<std::string>* _reserved;
	};
	//----------------------------------NumToken--------------------------
	class NumToken : public AToken
	{
	public:
		NumToken(const std::string& factoryName);
	protected:
		virtual bool test(Token* token) const;
	};
	//----------------------------------StrToken--------------------------
	class StrToken : public AToken
	{
	public:
		StrToken(const std::string& factoryName);
	protected:
		virtual bool test(Token* token) const;
	};
	//----------------------------------Leaf--------------------------
	class Leaf : public Element
	{
	public:
		Leaf(char* tokens[], unsigned n);
		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;

		virtual void find(std::vector<ASTree*>& result, Token* token);
	protected:
		std::vector<std::string> _tokens;
	};
	//----------------------------------Skip--------------------------
	class Skip : public Leaf
	{
	public:
		Skip(char* tokens[], unsigned n);
		virtual void find(std::vector<ASTree*>& result, Token* token);
	};
	//----------------------------------Expr--------------------------
	class Expr : public Element
	{
	public:
		Expr(const std::string& factoryName, Parser* exp, Operators* map);
		virtual ~Expr();

		virtual void parse(Lexer* lexer, std::vector<ASTree*>& result);
		virtual bool match(Lexer* lexer) const;
	protected:
		ASTree* doShift(Lexer* lexer, ASTree* left, int prec);
		Precedence* nextOperator(Lexer* lexer);
		//�ж��Ҳ���һ�����ʽ
		static bool rightIsExpr(int prec, Precedence* nextPrec);
	private:
		std::string _factoryName;
		Parser* _factor;
		Operators* _operators;
	};
//----------------------------------Parser--------------------------
public:
	Parser();
	Parser(const std::string& factoryName);
	Parser(const Parser* parser);
	virtual ~Parser();

	//ִ���﷨����
	ASTree* parse(Lexer* lexer);

	//����﷨����
	void reset();
	void reset(const std::string& factoryName);

	//����Parser����
	static Parser* rule();
	static Parser* rule(const std::string& factoryName);

	//����ս��ַ� (����)
	Parser* number();
	Parser* number(const std::string& factoryName);

	//����ս�� ����ʶ����
	Parser* identifier(const std::unordered_set<std::string>* reserved);
	Parser* identifier(const std::string& factoryName, const std::unordered_set<std::string>* reserved);

	//����ս�����ַ�����
	Parser* string();
	Parser* string(const std::string& factoryName);

	//����ս��������
	Parser* token(unsigned n, ...);

	//���δ�����ڳ����﷨�����ս������patƥ��ı�ʶ����
	Parser* sep(unsigned n, ...);

	//��ӷ��ս��p
	Parser* ast(Parser* p);

	//������ɸ���or��ϵ���ӵķ��ս��
	Parser* orTree(unsigned n, ...);

	//��ӿ�ʡ�Եķ��ս�������ʡ�ԣ�����Ϊһ�Ž��и��ڵ�ĳ����﷨������
	Parser* maybe(Parser* p);

	//��ӷ��ս�Ŀ�ʡ�Եķ��ս��p
	Parser* option(Parser* p);

	//��������ظ�0�εķ��ս��p
	Parser* repeat(Parser* p);

	//���˫Ŀ����� subexpʱ���ӣ�operatorsʱ�������
	Parser* expression(Parser* subexp, Operators* operators);
	Parser* expression(const std::string& factoryName, Parser* subexp, Operators* operators);

	//λ�﷨������ʼ����or����µķ�֧ѡ��
	Parser* insertChoice(Parser* p);
protected:
	bool match(Lexer* lexer) const;
protected:
	std::vector<Element*> _elements;
	std::string _factoryName;
};
NS_STONE_END
#endif