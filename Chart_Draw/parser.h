#pragma once
#include <vector>
#include "polynome.h"
#include "lexer.h"

using namespace std;

class Parser
{
	private:
	TOKEN* m_tokens;  // Указатель на начало массива лексем
	size_t m_total;   // Сколько всего лексем
	
	public:
	Parser(const vector<TOKEN>& tokens);
	~Parser();
	Polynome parse();   // Основная работа тут
	const TOKEN* tokens() const { return m_tokens; }  // Доступ к лексемам для правил
	size_t size() const { return m_total; }           // Сколько всего лексем
	size_t count;                                     // Количество уже обработанных лексем
};

// То, что распознало одно правило
class Node
{
	protected:
	virtual void create_cast() = 0;
	public:
	virtual ~Node() { }
};

// Конкретный результат применения правила
template <typename T>
class DataNode : public Node
{
	private:
	T m_data;
	
	public:
	DataNode(const T data) { m_data = data; }
	T data() { return m_data; }
	
	protected:
	void create_cast() { }
};

// Базовый класс для всех синтаксических правил
class SyntaxRule
{
	public:
	Node * apply(Parser& parser);
	virtual ~SyntaxRule() { }
	
	protected:
	virtual Node * applySpecific(Parser& parser) = 0;
};

// Терминал: синтаксическое правило, которому соответствует одна лексема
class Terminal : public SyntaxRule
{
	private: 
	TOKEN_TYPE m_token_type; // Тип лексемы
	
	protected:
	Node * applySpecific(Parser& parser) override;
	virtual Node * process(const TOKEN* token) { return new DataNode<TOKEN>(*token); }
	
	public:
	Terminal(TOKEN_TYPE type) { m_token_type = type; }
};

// Альтернатива: синтаксическое правило, которому соответствует одно из нескольких правил
class Alternative : public SyntaxRule
{
	protected:
	Node * applySpecific(Parser& parser) override;
	public:
	vector<SyntaxRule*> rules;  // Варианты в альтернативе
	~Alternative();
};

// Последовательность: синтаксическое правило, которому соответствует последовательность из вложенных правил
class Sequence : public SyntaxRule
{
	protected:
	vector<SyntaxRule*> rules;  // Элементы последовательности
	Node * applySpecific(Parser& parser) override;
	virtual Node * collect(Node** items) = 0; // Преобразование списка распознанных узлов в один узел
	public:
	~Sequence();	
};

// Правило, которому соответствует пустота
class Empty : public Sequence
{
    virtual Node * collect(Node**) override { return new DataNode<void*>(nullptr); }
};

// ===============================================================================================================

// Целое число
class IntegerRule : public Terminal
{
	public:
	IntegerRule();
	Node* process(const TOKEN* token) override;
};

class DoubleRule : public Sequence // Вещественное число
{
	public:
	DoubleRule();
	Node * collect(Node** items) override;
};

class NumberXRule : public Sequence // 5x или 5.56x
{
	public:
	NumberXRule();
	Node * collect(Node** items) override;
};

class PowerRule : public Sequence // Показатель степени - целое число со знаком
{
	public:
	PowerRule();
	Node * collect(Node** items) override;
};

class XNRule : public Sequence // x^5 или x^-2
{
	public:
	XNRule();
	Node * collect(Node** items) override;	
};

class NumberXNRule : public Sequence // 5x^3 или 5.56x^-3
{
	public:
	NumberXNRule();
	Node * collect(Node** items) override;
};

class MemberRule : public Alternative // Любые одночлены
{
	public:
	MemberRule();
	Node * applySpecific(Parser& parser) override;
};
