#ifndef _DATE_TIME_H_
#define _DATE_TIME_H_

#include "year_month.h"

class DateTime
{

private:

	int m_Year, m_Month, m_Day, m_Hours, m_Minutes;

	//	For factory method
	static bool IsLeapYear(int _year);
	static bool IsValid(int _year, int _month, int _day, int _hours, int _minutes);

	mutable char m_tempBuf[17];
	mutable bool m_bufferUpToDate = false;

	//	Constructors
	DateTime(int _year, int _month, int _day, int _hours, int _minutes);
	
	bool IsValid() const;

public:
	
	DateTime();
	DateTime(const char * _yyyyMMDD_HHMM, char _sep1, char _sep2, char _sep3);

	//DateTime(const DateTime &) = delete;

	bool IsLeapYear() const;

	int Year() const;
	int Month() const;
	int Day() const;
	int Hours() const;
	int Minutes() const;

	//	ÎÁÚßÂËÅÍÈÅ ÂÍÓÒĞÈËÀÑÑÎÂÎÃÎ ÎÏÅĞÀÒÎĞÀ ÑĞÀÂÍÅÍÈß ÍÀ ĞÀÂÅÍÑÒÂÎ ÄÀÒÛ-ÂĞÅÌÅÍÈ
	bool operator == (const DateTime & _dt) const;
	bool operator != (const DateTime & _dt) const;
	bool operator < (const DateTime & _dt) const;
	bool operator <= (const DateTime & _dt) const;
	bool operator > (const DateTime & _dt) const;
	bool operator >= (const DateTime & _dt) const;

	//	Ìåòîä ïîëó÷åíèÿ ñòğîêîâîãî ïğåäñòàâëåíèÿ
	const char * ToString() const;

	// Ñòàòè÷åñêèé ìåòîä-ôàáğèêà
	static DateTime * MakeDT(int _year, int _month, int _day, int _hours, int _minutes);

	YearMonth YM_MinusMonths(const int _mM) const;
	
	DateTime DT_MinusMonths(const int _mM) const;
};

inline int DateTime::Year() const
{
	return m_Year;
}

inline int DateTime::Month() const
{
	return m_Month;
}

inline int DateTime::Day() const
{
	return m_Day;
}

inline int DateTime::Hours() const
{
	return m_Hours;
}

inline int DateTime::Minutes() const
{
	return m_Minutes;
}

#endif