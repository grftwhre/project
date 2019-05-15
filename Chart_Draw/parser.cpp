#include "parser.h"
#include <cstring>
#include <cstdio>

// PARSER ====================================================================
Parser::Parser(const vector<TOKEN>& tokens)
{
	m_tokens = new TOKEN[tokens.size()];
	count = 0;	
	m_total = 0;
	
	// Выкидываем пробелы
	for (auto it : tokens)
	{
		if (it.type != Space)
			m_tokens[m_total++] = it;
	}
}

Parser::~Parser()
{
	delete [] m_tokens;
}

Polynome Parser::parse()
{
	Polynome ret;
	
	MemberRule rule;
	Terminal sign(Sign);
	
	DataNode<TOKEN> * signn = dynamic_cast<DataNode<TOKEN>*>(sign.apply(*this));
	bool minus = false;
	
	if (signn != nullptr && signn->data().text[0] == '-')
		minus = true;
	
	delete signn;
	
	while (count < m_total)
	{
		DataNode<MEMBER> *mbrn = dynamic_cast<DataNode<MEMBER>*>(rule.apply(*this));
		
		if (mbrn == nullptr)
			break;
		
		MEMBER mbr = mbrn->data();
		
		if (minus)
			mbr.multiplier = -mbr.multiplier;
		
		ret.add(mbr);
		
		delete mbrn;
		
		if (count < m_total)
		{
			signn = dynamic_cast<DataNode<TOKEN>*>(sign.apply(*this));
			
			if (signn == nullptr)
				break;
			
			minus = signn->data().text[0] == '-';
			
			delete signn;
		}
	};
	
	
	return ret;
}

// COMMON RULES =================================================================

Node * SyntaxRule::apply(Parser& parser)
{
	size_t old_count = parser.count;
	Node * ret = applySpecific(parser);
	
	if (ret == nullptr)
		parser.count = old_count;
	
	return ret;
}

Node * Terminal::applySpecific(Parser& parser)
{
	if (parser.tokens()[parser.count].type == m_token_type)
		return this->process(parser.tokens() + (parser.count++));
	else
		return nullptr;
}

Node * Alternative::applySpecific(Parser& parser)
{
	for (SyntaxRule* rule : rules)
	{
		Node * res = rule->apply(parser);
		
		if (res != nullptr)
			return res;
	}
	
	return nullptr;
}

Alternative::~Alternative()
{
	for (SyntaxRule* rule : rules)
	{
		delete rule;
	}
}

Node * Sequence::applySpecific(Parser& parser)
{
	Node ** ret = new Node* [rules.size()];
	
	for (size_t i = 0; i < rules.size(); i++)
	{
		ret[i] = rules[i]->apply(parser);
		
		if (ret[i] == nullptr)
			return nullptr;
	}

	Node * res = collect(ret);
	
	for (size_t i = 0; i < rules.size(); i++)
		delete ret[i];
	delete[] ret;
	
	return res;
}

Sequence::~Sequence()
{
	for (SyntaxRule* rule : rules)
	{
		delete rule;
	}
}

// NUMBER RULES =================================================================

IntegerRule::IntegerRule() : Terminal(Digits) { }

Node * IntegerRule::process(const TOKEN* token)
{
	char * str = new char[token->length + 1];
	strncpy(str, token->text, token->length);
	str[token->length] = '\0';
	int ret;
	sscanf(str, "%d", &ret);
	delete [] str;
	return new DataNode<int>(ret);
}

DoubleRule::DoubleRule()
{
	rules.push_back(new Terminal(Digits));
	rules.push_back(new Terminal(Dot));
	rules.push_back(new Terminal(Digits));
}

Node * DoubleRule::collect(Node** items)
{
	char * str = new char[dynamic_cast<DataNode<TOKEN>*>(items[0])->data().length 
		+ dynamic_cast<DataNode<TOKEN>*>(items[2])->data().length + 2]; // Один на точку, другой на \0
		
	strncpy(str, dynamic_cast<DataNode<TOKEN>*>(items[0])->data().text, 
		dynamic_cast<DataNode<TOKEN>*>(items[0])->data().length);
	
	str[dynamic_cast<DataNode<TOKEN>*>(items[0])->data().length] = '.';
	
	strncpy(str + dynamic_cast<DataNode<TOKEN>*>(items[0])->data().length + 1,  
		dynamic_cast<DataNode<TOKEN>*>(items[2])->data().text, 
		dynamic_cast<DataNode<TOKEN>*>(items[2])->data().length);
		
	str[dynamic_cast<DataNode<TOKEN>*>(items[0])->data().length 
		+ dynamic_cast<DataNode<TOKEN>*>(items[2])->data().length + 1] = '\0';
	
	double ret;
	
	sscanf(str, "%lf", &ret);
	delete [] str;
	return new DataNode<double>(ret);
}

// MEMBER RULES ==================================================================

NumberXRule::NumberXRule()
{
	Alternative *alt = new Alternative();
	alt->rules.push_back(new DoubleRule());
	alt->rules.push_back(new IntegerRule());
	
	rules.push_back(alt);
	rules.push_back(new Terminal(X));
}

Node * NumberXRule::collect(Node** items)
{
	DataNode<int> *int_value = dynamic_cast<DataNode<int>*>(items[0]);
	DataNode<double> *double_value = dynamic_cast<DataNode<double>*>(items[0]);
	
	MEMBER mbr;
	
	if (int_value != nullptr)
		mbr.multiplier = int_value->data();
	else
		mbr.multiplier = double_value->data();
	
	mbr.power = 1;
	
	return new DataNode<MEMBER>(mbr);
}

PowerRule::PowerRule()
{
	Alternative *alt = new Alternative();
	alt->rules.push_back(new Terminal(Sign));
	alt->rules.push_back(new Empty());

	rules.push_back(alt);
	rules.push_back(new IntegerRule());
}

Node * PowerRule::collect(Node** items)
{
	DataNode<TOKEN>* sign = dynamic_cast<DataNode<TOKEN>*>(items[0]);
	
	int value = dynamic_cast<DataNode<int>*>(items[1])->data();
	
	if (sign != nullptr && sign->data().text[0] == '-')
		value = -value;
	
	return new DataNode<int>(value);
}

XNRule::XNRule()
{
	rules.push_back(new Terminal(X));
	rules.push_back(new Terminal(Power));
	rules.push_back(new PowerRule());
}

Node * XNRule::collect(Node** items)
{
	MEMBER mbr;
	mbr.multiplier = 1;
	mbr.power = dynamic_cast<DataNode<int>*>(items[2])->data();
	
	return new DataNode<MEMBER>(mbr);
}

NumberXNRule::NumberXNRule()
{
	Alternative *alt = new Alternative();
	alt->rules.push_back(new DoubleRule());
	alt->rules.push_back(new IntegerRule());
	
	rules.push_back(alt);
	rules.push_back(new Terminal(X));
	rules.push_back(new Terminal(Power));
	rules.push_back(new PowerRule());

}

Node * NumberXNRule::collect(Node** items)
{
	DataNode<int> *int_value = dynamic_cast<DataNode<int>*>(items[0]);
	DataNode<double> *double_value = dynamic_cast<DataNode<double>*>(items[0]);
	
	MEMBER mbr;
	
	if (int_value != nullptr)
		mbr.multiplier = int_value->data();
	else
		mbr.multiplier = double_value->data();
	
	mbr.power = dynamic_cast<DataNode<int>*>(items[3])->data();;
	
	return new DataNode<MEMBER>(mbr);
}

MemberRule::MemberRule()
{
	rules.push_back(new NumberXNRule());
	rules.push_back(new NumberXRule());
	rules.push_back(new XNRule());
	rules.push_back(new Terminal(X));
	rules.push_back(new DoubleRule());
	rules.push_back(new IntegerRule());
}

Node * MemberRule::applySpecific(Parser& parser)
{
	Node * n = Alternative::applySpecific(parser);
	
	if (n == nullptr)
		return nullptr;
	
	DataNode<MEMBER> *mbrn = dynamic_cast<DataNode<MEMBER>*>(n);
	
	if (mbrn != nullptr)
		return mbrn;
	
	MEMBER mbr;
	mbr.power = 0;
	
	if (dynamic_cast<DataNode<TOKEN>*>(n) != nullptr)
	{
		mbr.multiplier = 1;
		mbr.power = 1;
	}
	
	if (dynamic_cast<DataNode<int>*>(n) != nullptr)
		mbr.multiplier = dynamic_cast<DataNode<int>*>(n)->data();
	
	if (dynamic_cast<DataNode<double>*>(n) != nullptr)
		mbr.multiplier = dynamic_cast<DataNode<double>*>(n)->data();
	
	delete n;
	
	return new DataNode<MEMBER>(mbr);
}
