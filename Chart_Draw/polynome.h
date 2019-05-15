#pragma once

#include <vector>
#include <string>

// ОБъектная модель полинома

using namespace std;

typedef struct
{
	double multiplier;  // Линейный множитель перед x
	int power;          // степень x
} MEMBER;               // Один член полинома

class Polynome          // Весь полином
{
	private:
	vector<MEMBER> m_members;
	public:
	size_t size() { return m_members.size(); }
	void add(MEMBER m) { m_members.push_back(m); }
	MEMBER& operator[](size_t index); // Чтобы с полиномом можно было работать так: myPolynome[5].power = 2;
	double  operator()(double x);     // Чтобы с полиномом можно было работать так: y = myPolynome(x);
	string to_string();
};