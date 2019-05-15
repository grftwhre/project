#include <string.h>
#include <cctype> // isdigit
#include "lexer.h"

static size_t findDigits(const char * str)
{
	size_t idx = 0;
	
	while (isdigit(str[idx])) // Считаем, сколько в начале строки цифр
		idx++;
	
	return idx;
}

static size_t findSpaces(const char * str)
{
	size_t idx = 0;
	
	while (isspace(str[idx])) // Считаем, сколько в начале строки пробельных символов
		idx++;
	
	return idx;
}

vector<TOKEN> lex(const char * str)
{
	LEX_RULE rules[] = 
	{
		{ X, [](const char * str) -> size_t { return str[0] == 'x' ? 1 : 0; } },
		{ Dot, [](const char * str) -> size_t { return str[0] == '.' ? 1 : 0; } },
		{ Power, [](const char * str) -> size_t { return str[0] == '^' ? 1 : 0; } },
		{ Sign, [](const char * str) -> size_t { return (str[0] == '+' || str[0] == '-') ? 1 : 0; } },
		{ Digits, &findDigits },
		{ Space, &findSpaces },
		{ Error, [](const char * str) -> size_t { return str[0] != '\0' ? 1 : 0; } } // Это правило должно быть в конце
	};
	
	size_t str_len = strlen(str), lexed_count = 0; // Сколько символов уже распознали
	vector<TOKEN> tokens;
	
	while (lexed_count < str_len) // Пока не закончилась строка
	{
		for (LEX_RULE rule : rules)  // Проверяем нераспознанную часть строки всеми правилами
		{
			size_t length = rule.function(str + lexed_count);
			
			if (length) // Как только правило подошло
			{
				TOKEN token;  // Создаём лексему
				token.type = rule.type;
				token.text = str + lexed_count;
				token.length = length;
				
				tokens.push_back(token); // Добавляем её в список распознанных лексем
				
				lexed_count += length;
				break;
			}
		}
	}
	
	return tokens;
}