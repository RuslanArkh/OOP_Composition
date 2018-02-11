#include "year_month.h"
#include "date_and_time.h"

#include <stdexcept>
#include <cassert>

YearMonth::YearMonth(const DateTime & _DT) 
	: m_year(_DT.Year()), m_month(_DT.Month())
{}

YearMonth::YearMonth(const int _y, const int _m)
{
	if (_y < 1 || _m < 1 || _m > 12)
		throw std::logic_error("Error: invalid year or month.");
	m_year = _y;
	m_month = _m;
}

void YearMonth::newYearMonth(int _y, int _m)
{
	if (_y < 1 || _m < 1 || _m > 12)
		throw std::logic_error("Error: invalid year or month.");
	m_year = _y;
	m_month = _m;
}

bool YearMonth::operator < (const YearMonth & _ym) const
{
	if (m_year < _ym.m_year)
		return true;
	else if (m_year == _ym.m_year)
		return m_month < _ym.m_month;
	return false;
}

bool YearMonth::operator > (const YearMonth & _ym) const
{
	return _ym < *this;
}

bool YearMonth::operator <= (const YearMonth & _ym) const
{
	return (*this < _ym) || (*this == _ym);
}

bool YearMonth::operator >= (const YearMonth & _ym) const
{
	return (_ym < *this) || (*this == _ym);
}

bool YearMonth::operator == (const YearMonth & _ym) const
{
	return !(_ym < *this) && !(*this < _ym);
}

bool YearMonth::operator != (const YearMonth & _ym) const
{
	return (_ym < *this) || (*this < _ym);
}

YearMonth YearMonth::PlusOneMonth() const
{
	if (m_month == 12)
		return YearMonth(m_year + 1, 1);
	else
		return YearMonth(m_year, m_month + 1);
}

int YearMonth::GetMonthDifference(const YearMonth & _ym1, const YearMonth & _ym2)
{
	return (_ym1.getYear() - _ym2.getYear()) * 12 + (_ym1.getMonth() - _ym2.getMonth());
}