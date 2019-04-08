#include "Lexer.h"
#include "Token.h"

NS_STONE_BEGIN

std::string Lexer::REGEX_POT = "\\s*((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")|[A-Z_a-z][A-Z_a-z0-9]*|==|!=|<=|>=|[\\{\\}<>=;+\\-\\*/%()])?";


Lexer::Lexer(const char* buffer)
	:_reader(buffer)
	,_lineNo(1)
	,_hasMore(true)
{
	_pattern = std::regex(REGEX_POT);
}

Lexer::~Lexer()
{
}

Token* Lexer::read() {
	//��ȡ����
	if (fillQueue(0)) {
		auto token = _queue.front();
		_queue.erase(_queue.begin());
		return token;
	}
	else {
		return Token::TOKEN_EOF;
	}
}

Token* Lexer::peek(int i) {
	if (fillQueue(i))
		return _queue.at(i);
	else
		return Token::TOKEN_EOF;
}

void Lexer::readLine() {
	char line[MAX_LENGTH];
	//��ȡһ�гɹ�
	if (_reader.getline(line, MAX_LENGTH)) {
		std::cmatch match;

		while (strlen(line) > 0) {
			std::regex_search(line, match, _pattern);
			//TODO:ƥ��ʧ��
			if (match[0] == "") {
				std::cout << "bad token at line " << _lineNo << std::endl;
				return;
			}
			else {
				//��ӵ���
				this->addToken(_lineNo, match);
				//�������½���
				strcpy(line, match.suffix().str().c_str());
			}
		}
		//��ӻ��з�
		_queue.push_back(new IdToken(_lineNo++, Token::TOKEN_EOL));
	}
	else
		_hasMore = false;
}

void Lexer::addToken(int lineNo, std::cmatch& matcher) {
	std::string m = matcher[1];

	if (!m.empty()) {
		//ƥ��Ĳ���ע��
		if (matcher[2] != m) {
			Token* token = nullptr;
			//����
			if (matcher[3] == m)
				token = new NumToken(lineNo, std::stoi(m));
			//�ַ���
			else if (matcher[4] == m)
				token = new StrToken(lineNo, this->toStringLiteral(m));
			//��ʶ��
			else
				token = new IdToken(lineNo, m);

			//add
			_queue.push_back(token);
		}//end if
	}//end if
}

std::string Lexer::toStringLiteral(const std::string& s) {
	int len = s.length();
	std::stringstream buffer;
	int index = 1;

	//ȥ�����ߵ�˫����
	while (index < len - 1) {
		char c = s.at(index);

		if (c == '\\' && index + 1 < len) {
			int c2 = s.at(index + 1);

			if (c2 == '"' || c2 == '\\')
				c = s.at(++index);
			else if (c2 == 'n') {
				++index;
				c = '\n';
			}
		}
		buffer << c;
		index++;
	}

	return buffer.str();
}

bool Lexer::fillQueue(unsigned int i) {
	//��ȡ
	while (i >= _queue.size()) {
		if (_hasMore)
			this->readLine();
		else
			return false;
	}
	return true;
}
NS_STONE_END
