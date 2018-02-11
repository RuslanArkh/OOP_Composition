#include "pay_check.h"
#include "product.h"
#include "year_month.h"

#include <algorithm>

int PayCheck::ms_ID;

PayCheck::PayCheck()
	: _pDT(DateTime())	//	<- �������������� ����������� ����� (�� ���������)
	, m_numOfBiggestPurchase(0)
{
	m_id = ++PayCheck::ms_ID;
}

PayCheck::PayCheck(const DateTime & _pDT)
	: _pDT(_pDT)
	, m_numOfBiggestPurchase(0)
{
	m_id = ++PayCheck::ms_ID;
}

PayCheck::PayCheck(const DateTime & _pDT, std::initializer_list< PurchaseItem > _l)
	: _pDT(_pDT)
	, m_numOfBiggestPurchase(0)
{
	m_id = ++PayCheck::ms_ID;
	for (PurchaseItem x : _l)
	{
		m_pItemList.push_back(x);

		//	���� ���������� ������� �� ���������� ������ ������� ������, 
		//	��� � �������� ������ � ���������� ����������� �������,
		//	�� ������ ���������� ������ ������� ������� �������
		//	����������: �� ������ �������� ������� ������� �� ����������, 
		//	��� ��� ������������ �� ���� ���������� ������ � ���� �� ������ �������
		if (x.Amount() > m_pItemList.at(m_numOfBiggestPurchase).Amount())
			m_numOfBiggestPurchase = m_pItemList.size() - 1;
	}
}

PayCheck::PayCheck(std::initializer_list< PurchaseItem > _l)
	: _pDT(DateTime())
	, m_numOfBiggestPurchase(-1)
{
	m_id = ++PayCheck::ms_ID;
	for (PurchaseItem x : _l)
		m_pItemList.push_back(x);
}

float PayCheck::getTotalSum() const
{
	float sum = 0.0f;
	std::for_each(
		m_pItemList.begin(), 
		m_pItemList.end(),
		[&sum]
		(const PurchaseItem x)
		{
			sum += x.Cost();
		}
	);

	//	return with precision
	return round(sum * 100) / 100.0;
}

void PayCheck::addItem(const PurchaseItem _pItem)
{
	//	���� � ���� ��� ���� ������ �����, �� ����������� ��� ����������
	for (int i = 0; i < m_pItemList.size(); ++i)
		if (m_pItemList.at(i).getProduct() == _pItem.getProduct())
		{
			m_pItemList.at(i).Increase(_pItem.Amount());

			if (m_pItemList.at(i).Amount() > m_pItemList.at(m_numOfBiggestPurchase).Amount())
				m_numOfBiggestPurchase = i;
			return;
		}

	//	���� ������ ����� �� ������, ��������� ��� � �����
	m_pItemList.push_back(_pItem);
	//	���������, ���������� �� � ����� ������ ���������� ������
	if (m_pItemList.at(m_pItemList.size() - 1).Amount() > m_pItemList.at(m_numOfBiggestPurchase).Amount())
		m_numOfBiggestPurchase = m_pItemList.size() - 1;
}

PurchaseItem PayCheck::getItemByProductTitle(const std::string _title) const
{
	for (int i = 0; i < m_pItemList.size(); ++i)
	{
		if (m_pItemList.at(i).getProduct().getTitle() == _title)
			return m_pItemList.at(i);
	}
	return PurchaseItem{};
}