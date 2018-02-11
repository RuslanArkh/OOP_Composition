#include "purchase_history.h"
#include "purchase_item.h"
#include "pay_check.h"
#include "year_month.h"
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <ctime>

int main()
{
	clock_t clock1, clock2;
	//	Creating products
	std::cout << "\t1) Creating products, purchase items, checks and initializing history\n\n";

	//	Category: HYGIENE
	Product *pShampoo = new Product("Shampoo", 60.4f, Product::Category::HYGIENE);			//	1
	Product *pSoap = new Product("Soap", 14.54f, Product::Category::HYGIENE);				//	2

	//	Category: FOOD
	Product *pBread = new Product("Bread", 8.45f, Product::Category::FOOD);					//	1 - Ought to be continuous purchase
	Product *pMilk = new Product("Milk", 9.85f, Product::Category::FOOD);					//	2

	//	Category: ENTERTAINMENT
	Product *pAkaiMPK = new Product("Akai MPK49 Pro", 25.0f, Product::Category::ENTERTAINMENT);					//	1

	//	Category: HOUSEHOLD_CHEMICALS
	Product *pBleach = new Product("Bleach", 47.89f, Product::Category::HOUSEHOLD_CHEMICALS);					//	1
	Product *pWashingPowder = new Product("Washing powder", 70.5f, Product::Category::HOUSEHOLD_CHEMICALS);		//	2

	//	Category: OTHERS
	Product *pGun = new Product("Gun", 100.0f, Product::Category::OTHERS);					//	1
	Product *pFlower = new Product("Flower", 15.0f, Product::Category::OTHERS);				//	2

	//	Check if category is returned good
	assert(pGun->getCategory() == Product::Category::OTHERS);

	//	Declaring all possible purchases
	PurchaseItem * pIHyg1 = new PurchaseItem(*pShampoo);
	PurchaseItem * pIHyg2 = new PurchaseItem(*pSoap);

	PurchaseItem * pIFood1 = new PurchaseItem(*pBread);
	PurchaseItem * pIFood2 = new PurchaseItem(*pMilk);
	
	PurchaseItem * pIEnt1 = new PurchaseItem(*pAkaiMPK);

	PurchaseItem * pIHouse1 = new PurchaseItem(*pBleach);
	PurchaseItem * pIHouse2 = new PurchaseItem(*pWashingPowder);

	PurchaseItem * pIOth1 = new PurchaseItem(*pGun);
	PurchaseItem * pIOth2 = new PurchaseItem(*pFlower);

	//	Testing PurchaseItem
	//	Check increase function
	assert(pIOth1->DeclareAmount(4).Amount() == 4);
	//	Also has increment and decrement operators
	++*pIHyg1;
	assert(pIHyg1->Amount() == 2);
	//	Check cost function
	assert(pIHyg1->Cost() == (float)(60.4 * 2));
	--*pIHyg1;
	assert(pIHyg1->Amount() == 1);
	//	Check function returning Product
	assert(pIHyg1->getProduct().getTitle() == "Shampoo" && 
		pIHyg1->getProduct().getPrice() == 60.4f);

	//	Проверка работы класса DateTime представлена в файле "test_datetime.cpp"

	PayCheck * pPC1 = new PayCheck(DateTime("2015/7/20 16:34", '/', ' ', ':'), 
	{ pIHyg2->DeclareAmount(2), pIFood1->DeclareAmount(3), pIFood2->DeclareAmount(2), *pIHouse2 });
	//	14.54 * 2 + 8.45 * 3 + 9.85 * 2 + 70.5 = 144.63

	PayCheck * pPC2 = new PayCheck(DateTime("2015/8/25 11:15", '/', ' ', ':'), 
	{ pIFood1->DeclareAmount(2), *pIFood2, *pIHouse1});
	//	Total cost: 8.45 * 2 + 9.85 + 47.89 = 74.64

	PayCheck * pPC3 = new PayCheck(DateTime("2015/9/14 20:54", '/', ' ', ':'), 
	{ *pIFood1, pIOth2->DeclareAmount(11), *pIOth1 });
	//	Total cost: 8.45 + 15 * 11 + 100 = 273.45

	PayCheck * pPC4 = new PayCheck(DateTime("2015/10/16 13:47", '/', ' ', ':'), 
	{ *pIFood1, pIHyg1->DeclareAmount(4), *pIEnt1, *pIFood2 });
	//	Можно также добавлять пункт покупки в чек и через функцию
	pPC4->addItem(*pIHouse2);
	//	Total cost = 8.45 + 60.4 * 4 + 25 + 9.85 + 70.5 = 355.4

	//	Average cost = (74.64 + 273.45 + 355.4) / 3 = 234.49

	clock1 = clock();

	//	Инициализация с помощью initializer_list
	PurchaseHistory * pHistory = new PurchaseHistory({ pPC1, pPC2, pPC3 });
	//	Можно также добавлять чек с помощью фукнции InsertCheck()
	pHistory->InsertCheck(*pPC4);
	clock2 = clock();


	//	
	double timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for history initialization: " << timeInSec << "\n";

	clock1 = clock();

	////////////////////////////////////////////////////////////////////////////////////////
	//		Задания начиная с второго пункта
	////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "\n\n\tTesting\n\n";
	std::cout << "!================================================!\n\n";

	//	Средний месячный расход за последние 3 месяца
	clock1 = clock();
	try
	{
		std::cout << "\t2) Average month expense: " << pHistory->AverageExpense(3) << "\n\n";
	}
	catch(const std::exception & _e)
	{
		std::cout << _e.what() << "\n\n";
	}
	clock2 = clock();
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "Time of getting average month expense: " << timeInSec << "\n\n";


	//	5 наиболее часто покупаемых товаров по убыванию количества покупок (наименования)
	std::cout << "\t3) 5 most purchaseable items: \n\n";
	clock1 = clock();
	std::vector < const PurchaseItem * > most;
	try
	{
		most = pHistory->GetMostPurchaseable();
		for (int i = 0; i < 5; ++i)
		{
			std::cout << "\t" << i + 1 << ". ";
			std::cout << most.at(i)->getProduct().getTitle() << "\n";
		}
	}
	catch (const std::exception & _e)
	{
		std::cout << "\n" << _e.what() << "\n";
	}
	clock2 = clock();
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	
	
	std::cout << "\nTime for getting most purchaseable items: " << timeInSec << "\n";

	//	Расходы последнего месяца по категориям товаров
	std::cout << "\n\t4) Cost by category for last month.\n\n";
	//	Берем последний чек *pPC4, считаем
	//	Cost(food) = (*pIFood1 + pIFood2) = *pBread + pMilk = 8.45 + 9.85 = 18.3
	//	Cost(chemicals) = *pIHouse  = *pWashingPowder  = 70.5
	//	Cost(chemicals) = *pIHyg1 * 4 = *pShampoo * 4 = 60.4 * 4 = 241.6
	std::map < Product::Category, float > * costByCat;
	try
	{
		clock1 = clock();
		costByCat = pHistory->CostByCategoryByLastNMonth(1);
		clock2 = clock();
		for (int i = 0; i < 3; ++i)
			std::cout << "\t" << i + 1 << ". Spent " << costByCat->find((Product::Category)i)->second << " UAH on " << Product::GetCategoryString((Product::Category)i) << ".\n";
	}	
	catch (const std::exception & _e)
	{
		std::cout << "\n" << _e.what() << "\n";
	}
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for getting costs by category: " << timeInSec << "\n";

	//	3 самые дорогие затраты за всю историю покупок
	std::cout << "\n\t5) 3 Biggest Purchases\n\n";
	try
	{
		clock1 = clock();
		std::vector<PurchaseItem> items = pHistory->GetTheBiggestPurchases();
		clock2 = clock();
		for (int i = 0; i < 3; ++i)
		{
			std::cout << "\t#" << i + 1 <<" the biggest purchase: " << items.at(i).getProduct().getTitle();
			std::cout << "\n\tPrice: " << items.at(i).getProduct().getPrice() << "\n";
			std::cout << "\tAmount: " << items.at(i).Amount() << "\n";
			std::cout << "\tCost: " << items.at(i).Cost() << "\n\n";
		}
	}
	catch (const std::exception & _e)
	{
		std::cout << "\n" << _e.what() << "\n";
	}
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for getting 3 biggest purchases: " << timeInSec << "\n";

	//	Классифицировать все затраты на разовые, периодические и постоянные

	std::cout << "\n\t6) Classificationg all purchases\n";
	clock1 = clock();
	for (int i = 0; i < 3; ++i)
	{
		try
		{
			clock1 = clock();
			std::vector < const PurchaseItem * > items = pHistory->GetByClassification((PurchaseHistory::Classification)i);
			clock2 = clock();
			float sum{ 0.0f };

			for (unsigned int j = 0; j < items.size(); ++j)
				sum += items.at(j)->Cost();

			sum = (float)(round(sum * 100) / 100.0);
			std::cout << "\tSum of costs of '" << pHistory->GetClassificationString((PurchaseHistory::Classification)i) << "' classification: " << sum << "\n";
		}
		catch (const std::exception & _e)
		{
			std::cout << pHistory->GetClassificationString((PurchaseHistory::Classification)i) << ": ";
			std::cout << _e.what() << "\n";
		}
	}
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for classificating all purchases: " << timeInSec << "\n\n";
	
	//	Прогноз затрат на следующий месяц
	//	Forecast = (Средняя месячная сумма периодических затрат за 3 месяца) + (сумма постоянных затра за последний месяц) =
	//	= average(pIHouse(Washing powder) + pIFood2(Milk)) + sum(pIFood1(Bread)) = 
	//	= average(70.5 + 9.85 * 2) + 8.45 = 30.06 + 8.45 = 38.51
	std::cout << "\t7) Forecast for next month: ";
	clock1 = clock();
	std::cout << pHistory->ForecastForNextMonth() << "\n";
	clock2 = clock();
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for forecast: " << timeInSec << "\n\n";
	

	//	Удаление всех объектов из динамической памяти
	std::cout << "\t8) Deleting all objects\n";
	clock1 = clock();
	delete costByCat;
	delete pHistory;

	delete pPC1;
	delete pPC2;
	delete pPC3;
	delete pPC4;

	delete pIHyg1;
	delete pIHyg2;
	delete pIFood1;
	delete pIFood2;
	delete pIEnt1;
	delete pIHouse1;
	delete pIHouse2;
	delete pIOth1;
	delete pIOth2;

	delete pShampoo;
	delete pSoap;
	delete pBread;
	delete pMilk;
	delete pAkaiMPK;
	delete pBleach;
	delete pWashingPowder;
	delete pGun;
	delete pFlower;
	clock2 = clock();
	timeInSec = double(clock2 - clock1) / (double)CLOCKS_PER_SEC;
	std::cout << "\nTime for deleting objects: " << timeInSec << "\n\n";
}

/*
std::vector<PayCheck *> * vec;
try
{
if (vec = pHistory->GetChecksByYearMonth(&YearMonth(2015, 9), 15))
{
std::cout << vec->at(0)->getPurchaseItem(0)->getProduct().getTitle() << "\n";
std::cout << vec->at(0)->getCheckDateTime().ToString() << "\n";
}
else
std::cout << "No check in this year/month\n";
}
catch (const std::exception & _e)
{
std::cout << _e.what() << "\n";
}
*/