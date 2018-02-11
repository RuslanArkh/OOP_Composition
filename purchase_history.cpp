#include "purchase_history.h"
#include "date_and_time.h"
#include "pay_check.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cassert>

PurchaseHistory::PurchaseHistory()
	: m_pHistory(new std::multimap < YearMonth, PayCheck * >)
	, _mostP(new MostPurchaseable{})
	, isNotModified(false)
{

}

PurchaseHistory::MostPurchaseable::MostPurchaseable()
	: m_nProducts(new std::map < const PurchaseItem *, int >)
{}

PurchaseHistory::PurchaseHistory(std::initializer_list<PayCheck *> _l)
	: isNotModified(false)
{
	m_pHistory = new std::multimap < YearMonth, PayCheck * > ;
	_mostP = new MostPurchaseable;
	_mostP->m_nProducts = new std::map < const PurchaseItem *, int > ;
	//_mostP->m_sortdItems = new std::map < int, 
	for (PayCheck *s : _l)
	{
		m_pHistory->insert(std::make_pair(YearMonth(s->getCheckDateTime().Year(), s->getCheckDateTime().Month()), s));
		for (int i = 0; i < s->getPurchaseItemCount(); ++i)
		{
			bool d = false;
			for (std::map<const PurchaseItem *, int>::iterator it{ _mostP->m_nProducts->begin() };
				it != _mostP->m_nProducts->end();
				++it)
			{
				if (it->first->getProduct().getTitle() == s->getPurchaseItem(i)->getProduct().getTitle())
				{
					++it->second;
					d = true;
				}
			}
			if (!d)
				_mostP->m_nProducts->insert(std::pair<const PurchaseItem *, int>(s->getPurchaseItem(i), 1));
		}
		
	}


}

PurchaseHistory::~PurchaseHistory()
{
	delete m_pHistory;
	delete _mostP->m_nProducts;
	delete _mostP;
}

void PurchaseHistory::InsertCheck(PayCheck & _check)
{
	YearMonth ym(_check.getCheckDateTime().Year(), _check.getCheckDateTime().Month());
	m_pHistory->insert(std::make_pair(ym, &_check));

	for (int i = 0; i < _check.getPurchaseItemCount(); ++i)
	{
		bool d = false;
		for (std::map<const PurchaseItem *, int>::iterator it{ _mostP->m_nProducts->begin() };
			it != _mostP->m_nProducts->end();
			++it)
		{
			if (it->first->getProduct().getTitle() == _check.getPurchaseItem(i)->getProduct().getTitle())
			{
				++it->second;
				d = true;
			}
		}
		if (!d)
			_mostP->m_nProducts->insert(std::make_pair(_check.getPurchaseItem(i), 1));
	}

	//	
	isNotModified = false;
}

PayCheck * PurchaseHistory::getCheckByDateTime(const DateTime _DT) const
{
	return nullptr;
}

std::vector<PayCheck *> * PurchaseHistory::GetChecksByYearMonth(const YearMonth & _YM, int _currentDay) const
{
	assert(_currentDay > 0 && _currentDay <= 31);
	std::vector<PayCheck *> * VectorOfChecks = new std::vector<PayCheck *>;

	for (	std::multimap<YearMonth, PayCheck *>::iterator it{ m_pHistory->begin() }; 
			it != m_pHistory->end(); 
			++it)
		if ((it->first == _YM && it->second->getCheckDateTime().Day() > _currentDay) ||
			(it->first.getMonth() == _YM.PlusOneMonth().getMonth() && it->second->getCheckDateTime().Day() <= _currentDay))
			VectorOfChecks->push_back(it->second);

	//	≈сли √од-мес€ц не найден, то они либо были указаны неверно, 
	//	либо за данный год-мес€ц не было покупок в супермаркете
	//	а значит вернет пустой вектор
	return VectorOfChecks;
}


float PurchaseHistory::AverageExpense(const int _n) const
{
	//	ƒл€ начала нужно вз€ть сегодн€шнюю дату,
	//	дл€ того, чтобы было с чего начинать отсчет назад (на 3 мес€цa)
	DateTime dt{};
	float average = 0;
	int curDay = dt.Day();
	for (int i = _n; i > 0; --i)
	{
		average += CostByPrevMonth(dt.YM_MinusMonths(i), curDay);
	}
	average /= _n;
	return round(average * 100) / 100.0f;
}

float PurchaseHistory::CostByPrevMonth(const YearMonth & _ym, int _currentDay) const
{
	assert(_currentDay > 0 && _currentDay <= 31);
	std::vector<PayCheck *> * tempVec = GetChecksByYearMonth(_ym, _currentDay);
	float sum{ 0.0f };
	std::for_each(
		tempVec->begin(),
		tempVec->end(),
		[&sum]
	(const PayCheck * _pChList)
	{
		
		sum += _pChList->getTotalSum();
	}
	);
	delete tempVec;

	return sum;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PurchaseHistory::MakeSequenceByFrequency()
{

	std::vector< std::pair< const PurchaseItem *, int > > * items =
		new std::vector< std::pair< const PurchaseItem *, int> >(_mostP->m_nProducts->begin(), _mostP->m_nProducts->end());

	std::sort(items->begin(), items->end(), MostPurchaseable::less_second< const PurchaseItem *, int>());

	if (!isNotModified)
		_mostP->m_itemsSortedByFrequency.clear();

	for (unsigned int i = 0; i < items->size(); ++i)
		_mostP->m_itemsSortedByFrequency.push_back(items->at(i).first);

	delete items;
	isNotModified = true;
}

std::vector < const PurchaseItem * > PurchaseHistory::GetMostPurchaseable(const int _n)
{
	if (_n < 1)
		throw std::logic_error("Error: less than one.");
	
	if (!isNotModified)
		MakeSequenceByFrequency();

	if (_n > (int)_mostP->m_nProducts->size())
		throw std::logic_error("Error: out of range.");

	std::vector < const PurchaseItem * > temp;
	for (int i = 0; i < _n; ++i)
		temp.push_back(_mostP->m_itemsSortedByFrequency.at(i));

	return temp;
}

std::vector < const PurchaseItem * > PurchaseHistory::GetMostPurchaseable()
{
	if (!isNotModified)
	{
		MakeSequenceByFrequency();
		isNotModified = true;
	}
	return GetMostPurchaseable(_mostP->m_itemsSortedByFrequency.size());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


float PurchaseHistory::getCostByCategoryAndRange(const Product::Category & _categ, const std::vector < PayCheck * > & _checkVec) const
{
	float cost = 0.0f;
	std::for_each(
		_checkVec.begin(),
		_checkVec.end(),
		[&cost, _categ]
	(const PayCheck * x)
	{
		std::for_each(
			x->getHistoryVector()->begin(),
			x->getHistoryVector()->end(),
			[&cost, _categ]
		(const PurchaseItem pI)
		{
			if (pI.getProduct().getCategory() == _categ)
				cost += pI.Cost();
		}
		);
	}
	);

	return cost;
}

std::map<Product::Category, float> * PurchaseHistory::CostByCategoryByLastNMonth(const int _n)
{
	DateTime dt{};
	int curDay = dt.Day();

	std::vector < PayCheck *> * payC = new std::vector < PayCheck *>;
	//	Ѕерем вектор с сегодн€шней даты до того же дн€ указанного мес€ца
	for (int i = _n; i > 0; --i)
	{
		std::vector < PayCheck * > tempVec = *GetChecksByYearMonth(dt.YM_MinusMonths(i), curDay);
		payC->insert(payC->end(), tempVec.begin(), tempVec.end());
	}

	std::map < Product::Category, float > * CatCostMap = new std::map < Product::Category, float > ;

	for (unsigned int i = 0; i < Product::getMap()->size(); ++i)
		CatCostMap->insert(std::pair<Product::Category, float>((Product::Category)i, getCostByCategoryAndRange((Product::Category)i, *payC)));

	delete payC;
	return CatCostMap;
}

std::map<Product::Category, float> * PurchaseHistory::WholeCostByCategory()
{
	DateTime dt{};
	return CostByCategoryByLastNMonth(YearMonth::GetMonthDifference(YearMonth(dt.Year(), dt.Month()), getLatest()));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::vector<PurchaseItem> PurchaseHistory::GetTheBiggestPurchases() const
{
	int amount = m_pHistory->size();

	std::vector <PurchaseItem> purchases;

	for (std::multimap<YearMonth, PayCheck *>::iterator it{ m_pHistory->begin() };
		it != m_pHistory->end();
		++it)
		purchases.push_back(it->second->getTheBiggestPurchase());
	
	std::sort(purchases.begin(), purchases.end());


	return purchases;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const std::map< const PurchaseHistory::Classification, const char *> PurchaseHistory::_classificMap = {
	std::pair < const PurchaseHistory::Classification, const char * >(PurchaseHistory::Classification::SINGLE, "Single")
	, std::pair < const PurchaseHistory::Classification, const char * >(PurchaseHistory::Classification::PERIODIC, "Periodic")
	, std::pair < const PurchaseHistory::Classification, const char * >(PurchaseHistory::Classification::ALWAYS, "Always")
};


std::map < PurchaseHistory::Classification, std::vector< const PurchaseItem * > > PurchaseHistory::m_itemsClassified = {
	std::pair<Classification, std::vector< const PurchaseItem * >>(PurchaseHistory::Classification::SINGLE, std::vector<const PurchaseItem *>()),
	std::pair<Classification, std::vector< const PurchaseItem * >>(PurchaseHistory::Classification::PERIODIC, std::vector<const PurchaseItem *>()),
	std::pair<Classification, std::vector< const PurchaseItem * >>(PurchaseHistory::Classification::ALWAYS, std::vector<const PurchaseItem *>())
};

const std::string PurchaseHistory::GetClassificationString(const PurchaseHistory::Classification _class)
{
	return PurchaseHistory::_classificMap.find(_class)->second;
}

void PurchaseHistory::Classificate()
{
	if (!isNotModified)
	{
		MakeSequenceByFrequency();
		isNotModified = true;
		isClassificated = false;
	}
	if (!isClassificated)
	{
		unsigned int allProducts{ m_pHistory->size() };
		std::map < Classification, std::vector< const PurchaseItem * > > * pItemsClass = &m_itemsClassified;
		std::map<const PurchaseItem *, int> * pMap = &*_mostP->m_nProducts;
		std::for_each(
			_mostP->m_nProducts->begin(),
			_mostP->m_nProducts->end(),
			[&pItemsClass, allProducts, pMap]
		(std::pair<const PurchaseItem *, int> x)
		{
			if (x.second == 1)
				pItemsClass->find(PurchaseHistory::Classification::SINGLE)->second.push_back(x.first);
			else if (x.second > 1 && x.second <= (int)(allProducts * 90 / 100))
				for (int i = 0; i < pMap->find(x.first)->second; ++i)
					pItemsClass->find(PurchaseHistory::Classification::PERIODIC)->second.push_back(x.first);
			else if (x.second > (int)(allProducts * 90 / 100))
				for (int i = 0; i < pMap->find(x.first)->second; ++i)
					pItemsClass->find(PurchaseHistory::Classification::ALWAYS)->second.push_back(x.first);
		});
		isClassificated = true;
	}
}

std::map < PurchaseHistory::Classification, std::vector< const PurchaseItem * > > PurchaseHistory::GetClassificatedPurchases()
{
	Classificate();
	return m_itemsClassified;
}

std::vector < const PurchaseItem * > PurchaseHistory::GetByClassification(const PurchaseHistory::Classification _class)
{
	Classificate();
	if (m_itemsClassified.find(_class)->second.empty())
		throw std::range_error("current classification is empty.");
	else
		return m_itemsClassified.find(_class)->second;

	
}


YearMonth PurchaseHistory::getLatest()
{
	return m_pHistory->begin()->first;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



float PurchaseHistory::SumByClassification(const Classification & _class)
{
	DateTime dt{};
	std::map < Classification, std::vector< const PurchaseItem * > > * pItemsClass = &m_itemsClassified;

	int minus;
	if (_class == PurchaseHistory::Classification::PERIODIC)
		minus = 3;
	else if (_class == PurchaseHistory::Classification::ALWAYS)
		minus = 1;
	else
		minus = YearMonth::GetMonthDifference(YearMonth{ dt }, getLatest());

	float sum{ 0.0f };

	std::vector < const PurchaseItem * > tempVec{ m_itemsClassified.find(_class)->second };

	for (int i = minus; i > 0; --i)
	{
		bool found = false;
		std::for_each(
			m_pHistory->find(dt.YM_MinusMonths(i)),
			m_pHistory->end(),
			[&sum, tempVec, &pItemsClass, &found]
		(std::pair < YearMonth, PayCheck * > x)
		{
			
			std::for_each(
				x.second->getHistoryVector()->begin(),
				x.second->getHistoryVector()->end(),
				[&sum, tempVec, &found]
			(PurchaseItem _item)
			{
				for (int i = 0; i < tempVec.size(); ++i)
					if (tempVec.at(i)->getProduct() == _item.getProduct())
					{
						sum += _item.Cost();
						break;
					}
			});
			found = true;
		});
		if (found)
			break;
	}

	sum = (float)(round(sum * 100) / 100.0);

	return sum;
}

float PurchaseHistory::ForecastForNextMonth()
{
	float sum = SumByClassification(PurchaseHistory::Classification::ALWAYS);
	float average = SumByClassification(PurchaseHistory::Classification::PERIODIC) / 3.0f;
	return (float)(round((sum + average) * 100) / 100.0);
}