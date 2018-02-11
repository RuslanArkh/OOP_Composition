#include "purchase_item.h"
#include "date_and_time.h"

#include <stdexcept>

PurchaseItem::PurchaseItem()
	: m_Amount(0)
	, m_Cost(0)
	, CostDefined(false)
	, m_Purchase(Product())
{}

PurchaseItem::PurchaseItem(Product _purchase)
	: m_Purchase(_purchase)
	, m_Amount(1)
	, CostDefined(false)
{}

PurchaseItem::PurchaseItem(Product _purchase, int _amount)
	: m_Purchase(_purchase)
	, m_Amount(_amount)
	, CostDefined(false)
{}

void PurchaseItem::Increase(int _n)
{
	if (_n < 1)
		throw std::logic_error("Error: increasement is less than 1.");
	m_Amount += _n;
	CostDefined = false;
}

PurchaseItem PurchaseItem::operator ++ ()
{
	++m_Amount;
	CostDefined = false;

	return *this;
}

PurchaseItem PurchaseItem::operator ++ (int)
{
	PurchaseItem copy{ *this };

	++m_Amount;
	CostDefined = false;

	return copy;
}

PurchaseItem PurchaseItem::operator -- ()
{
	if (m_Amount > 1)
		--m_Amount;
	CostDefined = false;
	return *this;
}

PurchaseItem PurchaseItem::operator -- (int)
{
	PurchaseItem copy{ *this };

	if (m_Amount > 1)
		--m_Amount;
	CostDefined = false;

	return copy;
}

float PurchaseItem::Cost() const
{
	SetCost();
	return m_Cost;
}

void PurchaseItem::SetAmount(const int _amount)
{
	if (_amount < 1)
		throw std::logic_error("Error in SetAmount(): _amount < 1.");
	m_Amount = _amount;
}

void PurchaseItem::SetCost() const
{
	if (!CostDefined)
	{
		m_Cost = Amount() * m_Purchase.getPrice();
		CostDefined = true;
	}
}

PurchaseItem PurchaseItem::DeclareAmount(const int _amount) const
{
	if (_amount < 1)
		throw std::logic_error("Error: less than 1.");
	return PurchaseItem(m_Purchase, _amount);
}