#include <stdexcept>
#include <sstream>
#include <cmath>
#include "polynome.h"

using namespace std;

MEMBER& Polynome::operator[](size_t index)
{
	if (index >= m_members.size())
		throw out_of_range("Index");  // Выкидываем ошибку при неправильном индексе
	
	return m_members[index];
}

string Polynome::to_string()
{
	ostringstream ret;
	bool first = true;
	
	for (size_t i = 0; i < size(); i++)
	{
		if (m_members[i].multiplier == 0)
			continue;
		
		if (m_members[i].multiplier > 0 && !first) // Между членами полинома ставим + или -, а в начале только -
			ret << "+";
		
		if (m_members[i].multiplier != 1 && m_members[i].multiplier != -1)
			ret << m_members[i].multiplier;
		else if (m_members[i].multiplier == -1)
			ret << "-";
		
		if (m_members[i].power != 0)
			ret << "x";
		else if (m_members[i].multiplier == 1 || m_members[i].multiplier == -1)
			ret << "1";
		
		if (m_members[i].power != 1 && m_members[i].power != 0)
			ret << "^" << m_members[i].power;
		
		first = false;
	}
	
	return ret.str();
}

double Polynome::operator()(double x) // Интерпретатор
{
	double y = 0;
	
	for (size_t i = 0; i < size(); i++)
	{
		if (m_members[i].multiplier == 0)
			continue;
		
		double mul = 1;
		
		for (int j = 0; j < abs(m_members[i].power); j++)
			mul *= x;
		
		if (m_members[i].power < 0)
			mul = 1 / mul;
		
		y += mul * m_members[i].multiplier;
	}
	
	return y;
}