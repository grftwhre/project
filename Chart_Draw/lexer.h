#pragma once
#include <vector>

using namespace std;

// Какие бывают лексемы
typedef enum
{
	Error,
	Digits,
	Dot,
	X,
	Power,
	Sign,
	Space
} TOKEN_TYPE;

// Лексема
typedef struct
{
	TOKEN_TYPE type;
	const char * text;
	size_t length;
} TOKEN;

// Правило для поиска лексемы
typedef struct
{
	TOKEN_TYPE type;
	size_t (*function)(const char *);
} LEX_RULE;

// Функция, разбивающая строку на лексемы
vector<TOKEN> lex(const char * str);