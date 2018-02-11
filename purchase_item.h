#ifndef _PURCHASE_ITEM_H_
#define _PURCHASE_ITEM_H_

#include "product.h"
#include "year_month.h"

class DateTime;

class PurchaseItem
{

	Product m_Purchase;
	int m_Amount;

	mutable float m_Cost;
	mutable bool CostDefined;

public:

	const DateTime & getDT() const;
	
	explicit PurchaseItem();
	PurchaseItem(Product _purchase);
	PurchaseItem(Product _purchase, int _amount);

	Product getProduct() const;

	//	Метод рассчета стоимости
	float Cost() const;

	int Amount() const;

	//	Метод для увеличения количества купленного товара
	void Increase(int _n);
	//	Инкременты количества
	PurchaseItem operator ++ ();
	PurchaseItem operator ++ (int);

	PurchaseItem operator -- ();
	PurchaseItem operator -- (int);

	void SetAmount(const int _amount);
	void SetCost() const;

	bool operator < (const PurchaseItem & _cmpVal) const;
	bool operator < (const PurchaseItem * _cmpVal) const;

	bool operator == (const PurchaseItem & _cmpItem) const;
	bool operator != (const PurchaseItem & _cmpItem) const;

	//	Returns item with declared amount while inserting in check
	//	Default amount is 1
	PurchaseItem DeclareAmount(const int _amount) const;

};

inline Product PurchaseItem::getProduct() const
{
	return m_Purchase;
}

inline int PurchaseItem::Amount() const
{
	return m_Amount;
}

inline bool PurchaseItem::operator < (const PurchaseItem & _cmpVal) const
{
	return m_Amount > _cmpVal.m_Amount;
}

inline bool PurchaseItem::operator == (const PurchaseItem & _cmpItem) const
{
	return m_Purchase == _cmpItem.m_Purchase;
}

inline bool operator == (const PurchaseItem & _i1, const PurchaseItem & _i2)
{
	return _i1.getProduct() == _i2.getProduct();
}

inline bool PurchaseItem::operator < (const PurchaseItem * _cmpVal) const
{
	return m_Amount > _cmpVal->m_Amount;
}

#endif