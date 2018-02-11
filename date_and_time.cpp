#include "date_and_time.h"

#include <ctime>
#include <cstdio>
#include <cassert>
#include <stdexcept>

DateTime::DateTime()
{
	// Выясняем текущее системное время
	time_t currentTime = time(nullptr);

	// Используем функции стандартной библиотеки для декодирования компонентов времени к удобному виду
	tm* currentTM = gmtime(&currentTime);

	m_Year = currentTM->tm_year + 1900;
	m_Month = currentTM->tm_mon + 1;
	m_Day = currentTM->tm_mday;
	m_Hours = currentTM->tm_hour + 3;
	if (m_Hours > 23)
		m_Hours %= 23;
	m_Minutes = currentTM->tm_min;


	if (!IsValid())
		throw std::logic_error("System error: date and time is not valid due to the system error.");
}

DateTime::DateTime(int _year, int _month, int _day, int _hours, int _minutes)
	: m_Year(_year)
	, m_Month(_month)
	, m_Day(_day)
	, m_Hours(_hours)
	, m_Minutes(_minutes)
{}

DateTime::DateTime(const char * _yyyyMMDD_HHMM, char _sep1, char _sep2, char _sep3)
{
	// Пытаемся распознать строковое представление даты по шаблону.
	// Функция sscanf возвращает число успешно обнаруженных частей шаблона.
	char sep1, sep1_1, sep2, sep3;
	int nMatched = sscanf(_yyyyMMDD_HHMM, "%d%c%d%c%d%c%d%c%d", 
						&m_Year, &sep1, &m_Month, &sep1_1, &m_Day, &sep2, &m_Hours, &sep3, &m_Minutes);

	// Формат должен быть корректным - 3 целых поля + 2 корректных разделителя
	if (nMatched != 9 || sep1 != _sep1 || sep1_1 != _sep1 || sep2 != _sep2 || sep3 != _sep3)
		throw std::logic_error("Error: date format is incorrect!");

	// Проверяем инвариант
	if (!IsValid())
		throw std::logic_error("Error: date is not valid!");
}

DateTime * DateTime::MakeDT(int _year, int _month, int _day, int _hours, int _minutes)
{
	if (DateTime::IsValid(_year, _month, _day, _hours, _minutes))
		return new DateTime(_year, _month, _day, _hours, _minutes);
	else
	{
		assert(!"Error: invalid date and time!");
		return nullptr;
	}
}

//***********************************************************************************


bool DateTime::IsValid() const
{
	return DateTime::IsValid(m_Year, m_Month, m_Day, m_Hours, m_Minutes);
}

bool DateTime::IsValid(int _year, int _month, int _day, int _hours, int _minutes)
{
	// Год не должен быть равен нулю
	if (_year == 0)
		return false;

	// Месяц должен находиться в интервале от января до декабря
	if (_month < 1 || _month > 12)
		return false;

	// Дни обязаны быть положительными
	if (_day < 1)
		return false;

	// Часы должны находиться в интервале [0:23]
	if (_hours < 0 || _hours > 23)
		return false;

	// Минуты должны находиться в интервале [0:59]
	if (_minutes < 0 || _minutes > 59)
		return false;

	// В феврале високосного года 29 дней
	else if (_month == 2 && IsLeapYear(_year))
		return _day <= 29;

	else
	{
		// В каждой ячейке таблицы храним максимальное количество дней в месяце.
		static const int s_daysInMonth[] = {
			31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
		};

		// Проверяем количество дней в остальных случаях по таблице.
		// Используем выражение _month - 1 в качестве индекса месяца в таблице
		return _day <= s_daysInMonth[_month - 1];
	}
}

bool DateTime::IsLeapYear() const
{
	return DateTime::IsLeapYear(m_Year);
}

bool DateTime::IsLeapYear(int _year)
{
	if (_year % 4 != 0)
		return false;

	else if (_year % 100 == 0)
		return (_year % 400 == 0);

	return true;
}


//**************************************************************************************

YearMonth DateTime::YM_MinusMonths(const int _mM) const
{
	if (_mM < 0 || _mM > m_Year * m_Month)
		throw std::range_error("Error: out of range (in MinusMonth()).");
	else if (_mM == 0)
		return YearMonth(m_Year, m_Month);

	int mM = (12 - (_mM % 12) + m_Month) % 12;
	mM += (mM == 0 ? 12 : 0);
		//mM = 12;
	int yY = m_Year - (_mM / 12);
	yY -= ((_mM > m_Month && (_mM % 12) < 12) ? 1 : 0);

	return YearMonth(yY, mM);
}

DateTime DateTime::DT_MinusMonths(const int _mM) const
{
	YearMonth ym{ YM_MinusMonths(_mM) };
	int Day = m_Day;
	// В каждой ячейке таблицы храним максимальное количество дней в месяце.
	static const int s_daysInMonth[] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	if (Day > s_daysInMonth[m_Month - 1])
		Day = s_daysInMonth[m_Month - 1];
	if (ym.getMonth() == 2)
	{
		if (IsLeapYear(ym.getYear()))
			Day = 29;
		else
			Day = 28;
	}
	return DateTime(ym.getYear(), ym.getMonth(), Day, m_Hours, m_Minutes);
}

//**************************************************************************************

//	OPERATORS

bool DateTime::operator < (const DateTime & d) const
{

	if (m_Year < d.Year())
		return true;
	else if (m_Year == d.Year())
	{
		if (m_Month < d.Month())
			return true;
		else if (m_Month == d.Month())
		{
			if (m_Day < d.Day())
				return true;
			else if (m_Day == d.Day())
			{
				if (m_Hours < d.Hours())
					return true;
				else if (m_Hours == d.Hours())
					return m_Minutes < d.Minutes();
			}
		}
	}
	return false;
}

bool DateTime::operator > (const DateTime & d) const
{
	return d < *this;
}

bool DateTime::operator <= (const DateTime & d) const
{
	return (*this < d) || (*this == d);
}

bool DateTime::operator >= (const DateTime & d) const
{
	return (d < *this) || (*this == d);
}

bool DateTime::operator == (const DateTime & d) const
{
	return !(d < *this) && !(*this < d);
}

bool DateTime::operator != (const DateTime & d) const
{
	return (d < *this) || (*this < d);
}

//	String representation

const char * DateTime::ToString() const
{
	if (!m_bufferUpToDate)
	{
		
		if (m_Minutes < 10)
		{
			sprintf(m_tempBuf, "%d/%d/%d %d:0%d", m_Year, m_Month, m_Day, m_Hours, m_Minutes);
		}
		else
		{
			sprintf(m_tempBuf, "%d/%d/%d %d:%d", m_Year, m_Month, m_Day, m_Hours, m_Minutes);
		}
		
		m_bufferUpToDate = true;
	}
	return m_tempBuf;
}