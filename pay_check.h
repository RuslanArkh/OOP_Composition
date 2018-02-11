#ifndef _PAY_CHECK_H_
#define _PAY_CHECK_H_

#include "date_and_time.h"
#include "purchase_item.h"
#include "year_month.h"

#include <vector>
#include <initializer_list>

class Product;

class PayCheck 
{
	const DateTime & _pDT;
	std::vector< PurchaseItem > m_pItemList;

	static int ms_ID;
	int m_id;

	//	Индекс самой большой покупки в векторе
	int m_numOfBiggestPurchase;

public:

	//	Конструктор по умолчанию, использует сегодняшнюю дату
	PayCheck();
	//	Перегруженный конструктор, генерирующий пока что пустой чек с датой
	PayCheck(const DateTime & _pDT);
	//	Перегруженный конструктор, генерирующий чек сразу с набором пунктов покупок через initializer_list
	PayCheck(const DateTime & _pDT, std::initializer_list< PurchaseItem > _l);
	//	Перегруженный конструктор с сегодняшней датой и начальным набором пунктов покупки
	PayCheck(std::initializer_list< PurchaseItem > _l);

	//	Метод возвращающий дату и время чека
	const DateTime & getCheckDateTime() const;

	//	Метод подсчета количества покупок
	int getPurchaseItemCount() const;

	//	Метод возвращает рабочий вектор
	const std::vector< PurchaseItem > * getHistoryVector() const;

	//	Метод, возвращающий пункт покупки по заданному индексу
	const PurchaseItem * getPurchaseItem(int _index) const;

	//	Метод рассчета общей суммы, потраченной на покупку
	float getTotalSum() const;

	int getId() const;

	//	Функция добавления пункта покупки
	void addItem(const PurchaseItem _pItem);

	//	Функция возвращает ссылку на наиболее покупаемый пункт
	const PurchaseItem & getTheBiggestPurchase() const;

	//	Функция возвращает указатель на пункт покупки по названию товара
	PurchaseItem getItemByProductTitle(const std::string _title) const;
};

inline const DateTime & PayCheck::getCheckDateTime() const
{
	return _pDT;
}

inline int PayCheck::getPurchaseItemCount() const
{
	return m_pItemList.size();
}

inline const PurchaseItem * PayCheck::getPurchaseItem(int _index) const
{
	return &(m_pItemList.at(_index));
}

inline int PayCheck::getId() const
{
	return m_id;
}

inline const PurchaseItem & PayCheck::getTheBiggestPurchase() const
{
	return m_pItemList.at(m_numOfBiggestPurchase);
}

inline const std::vector< PurchaseItem > * PayCheck::getHistoryVector() const
{
	return &m_pItemList;
}

#endif