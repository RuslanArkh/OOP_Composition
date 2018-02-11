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

	//	������ ����� ������� ������� � �������
	int m_numOfBiggestPurchase;

public:

	//	����������� �� ���������, ���������� ����������� ����
	PayCheck();
	//	������������� �����������, ������������ ���� ��� ������ ��� � �����
	PayCheck(const DateTime & _pDT);
	//	������������� �����������, ������������ ��� ����� � ������� ������� ������� ����� initializer_list
	PayCheck(const DateTime & _pDT, std::initializer_list< PurchaseItem > _l);
	//	������������� ����������� � ����������� ����� � ��������� ������� ������� �������
	PayCheck(std::initializer_list< PurchaseItem > _l);

	//	����� ������������ ���� � ����� ����
	const DateTime & getCheckDateTime() const;

	//	����� �������� ���������� �������
	int getPurchaseItemCount() const;

	//	����� ���������� ������� ������
	const std::vector< PurchaseItem > * getHistoryVector() const;

	//	�����, ������������ ����� ������� �� ��������� �������
	const PurchaseItem * getPurchaseItem(int _index) const;

	//	����� �������� ����� �����, ����������� �� �������
	float getTotalSum() const;

	int getId() const;

	//	������� ���������� ������ �������
	void addItem(const PurchaseItem _pItem);

	//	������� ���������� ������ �� �������� ���������� �����
	const PurchaseItem & getTheBiggestPurchase() const;

	//	������� ���������� ��������� �� ����� ������� �� �������� ������
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