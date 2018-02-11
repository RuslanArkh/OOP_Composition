#ifndef _YEAR_MONTH_H_
#define _YEAR_MONTH_H_

class DateTime;

class YearMonth 
{
	int m_year, m_month;

public:

	YearMonth() : m_year(1), m_month(1) {}
	YearMonth(const DateTime & _DT);
	YearMonth(const int _y, const int _m);

	void newYearMonth(int _y, int _m);

	int getYear() const;
	int getMonth() const;

	bool operator < (const YearMonth & _ym) const;
	bool operator > (const YearMonth & _ym) const;
	bool operator <= (const YearMonth & _ym) const;
	bool operator >= (const YearMonth & _ym) const;

	bool operator == (const YearMonth & _ym) const;
	bool operator != (const YearMonth & _ym) const;

	YearMonth PlusOneMonth() const;

	static int GetMonthDifference(const YearMonth & _ym1, const YearMonth & _ym2);
};

inline int YearMonth::getYear() const
{
	return m_year;
}

inline int YearMonth::getMonth() const
{
	return m_month;
}

#endif