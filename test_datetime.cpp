#include "date_and_time.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include <ctime>
#include <cstdio>

int main()
{
	DateTime * pDT1;
	try
	{
		pDT1 = new DateTime;
	}
	catch (const std::exception & _e)
	{
		std::cout << _e.what() << std::endl;
	}
	const char * str1 = pDT1->ToString();
	std::cout << str1 << "\n";

	DateTime * pDT2;
	try
	{
		pDT2 = new DateTime("2016/02/29 22:50", '/', ' ', ':');
		
	}
	catch (const std::exception & _e2)
	{
		std::cout << _e2.what() << std::endl;
	}
	const char * str2 = pDT2->ToString();
	std::cout << str2 << "\n";

	DateTime * pDT3 = DateTime::MakeDT(2015, 6, 15, 11, 00);
	const char * str3 = pDT3->ToString();
	std::cout << str3 << "\n";

	DateTime * pDT4 = new DateTime("2016/02/29 22:50", '/', ' ', ':');

	assert(*pDT3 < *pDT1);
	assert(*pDT4 > *pDT1);
	assert(*pDT4 == *pDT2);
	assert(*pDT4 <= *pDT2);
	assert(*pDT4 >= *pDT2);
	assert(*pDT1 != *pDT2);

	delete pDT4;
	delete pDT3;
	delete pDT2;
	delete pDT1;
}