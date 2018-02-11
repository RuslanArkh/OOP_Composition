#ifndef _PURCHASE_HISTORY_H_
#define _PURCHASE_HISTORY_H_

//#include "pay_check.h"
#include "purchase_item.h"

#include <map>
#include <initializer_list>
#include <vector>

class PayCheck;
class DateTime;
class YearMonth;

class PurchaseHistory
{

	std::multimap<YearMonth,  PayCheck *> * m_pHistory;

	//	����������, ������������ �� ���������� �� ��������� ������� �������
	bool isNotModified;

	//////////////////////////////////////////////////////////////////////////////////////

	class MostPurchaseable
	{
		friend PurchaseHistory;

		//	Constructor
		MostPurchaseable();

		//	������ � ���������� (�������) �������
		//	����������� �� ������� �������
		std::map<const PurchaseItem *, int> * m_nProducts;

		//	����������� ������� �� ���������� �������
		//std::map< int, const PurchaseItem *> * m_sortdItems;

		template <typename T1, typename T2>
		struct less_second
		{
			typedef std::pair<T1, T2> type;
			bool operator () (type const & a, type const & b) const
			{
				return a.second > b.second;
			}
		};
		
		//	������ ��������������� �� ���������� �������
		std::vector<const PurchaseItem *> m_itemsSortedByFrequency;
	};
	MostPurchaseable * _mostP;

	//	����������� �����
	void MakeSequenceByFrequency();

public:

	////////////////////////////////////////////////////////////////////////////////////////////////

	PurchaseHistory();
	PurchaseHistory(std::initializer_list<PayCheck *> _l);

	~PurchaseHistory();

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	����� ��������� ���� �� �����-����
	PayCheck * getCheckByDateTime(const DateTime _DT) const;

	//	����� ���������� ������ ����-�������� 
	void InsertCheck(PayCheck & _check);

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	����� ��������� ������ ����� �� ���� � ������
	std::vector<PayCheck *> * GetChecksByYearMonth(const YearMonth & _YM, int _currentDay) const;

	//	������� ������ �� _n �������
	float AverageExpense(const int _n) const;

	//	��������������� ������� � AverageExpense()
	//	�������������� ����������� ����� �� ���������� �����, 
	//	������� ���������� ���������� � ��� �������.
	//	����� ��������, ��� ����� �������������� �� ��� �������� ������
	//	� ���� �� ������ ��� � ���������� ������. 
	//	��� ��������� ����� ���������� ������� ������ ������ �� ������������ ���
	float CostByPrevMonth(const YearMonth & _ym, int _currentDay) const;

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	������� ���������� ������ �������� ����� ���������� ������� ��������������� � ������� ��������
	std::vector < const PurchaseItem * > GetMostPurchaseable(const int _n);

	//	����� �� �������, ������ ������� �� ��� �����
	std::vector < const PurchaseItem * > GetMostPurchaseable();

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	����� ���������� ����������� ���������->(����� �������) �� _n ������� �� ������������ ���
	std::map<Product::Category, float> * CostByCategoryByLastNMonth(const int _n);
	//	�������������� �����, ���������� �� �� ����������� �� ��� �����
	std::map<Product::Category, float> * WholeCostByCategory();

private: 

	//	��������������� �������
	float getCostByCategoryAndRange(const Product::Category & _categ, const std::vector < PayCheck * > & _checkVec) const;

public: 

	std::vector<PurchaseItem> GetTheBiggestPurchases() const;

	////////////////////////////////////////////////////////////////////////////////////////////////


	//	������������� ������
	enum Classification { SINGLE, PERIODIC, ALWAYS };

	//	����� ���������� ������ �������������
	const std::string GetClassificationString(const Classification _class);

private:
	
	//	����������� ��� ���������� ������������� ��������������
	static const std::map<const Classification, const char *> _classificMap;

	//	����������� ������������������ ������
	static std::map < Classification, std::vector< const PurchaseItem * > > m_itemsClassified;

	bool isClassificated = false;

	//	������� �������������� ��������
	void ClassificationInit();

	//	����� ������������ ��� ������� �� ����������
	void Classificate();

	

public:

	std::map < PurchaseHistory::Classification, std::vector< const PurchaseItem * > > GetClassificatedPurchases();

	std::vector < const PurchaseItem * > GetByClassification(const Classification _class);

	YearMonth getLatest();

	////////////////////////////////////////////////////////////////////////////////////////////////

	float SumByClassification(const Classification & _class);

	float ForecastForNextMonth();
}; 

#endif