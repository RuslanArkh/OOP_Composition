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

	//	Переменная, определяющее не изменялось ли состояние истории покупок
	bool isNotModified;

	//////////////////////////////////////////////////////////////////////////////////////

	class MostPurchaseable
	{
		friend PurchaseHistory;

		//	Constructor
		MostPurchaseable();

		//	Пункты и количества (частота) покупок
		//	Отображение по пунктам покупок
		std::map<const PurchaseItem *, int> * m_nProducts;

		//	Отображение пунктов по количеству покупок
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
		
		//	Вектор отсортированный по количеству покупок
		std::vector<const PurchaseItem *> m_itemsSortedByFrequency;
	};
	MostPurchaseable * _mostP;

	//	Сортирующий метод
	void MakeSequenceByFrequency();

public:

	////////////////////////////////////////////////////////////////////////////////////////////////

	PurchaseHistory();
	PurchaseHistory(std::initializer_list<PayCheck *> _l);

	~PurchaseHistory();

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	Метод получения чека по ключу-дате
	PayCheck * getCheckByDateTime(const DateTime _DT) const;

	//	Метод добавления нового ключ-значения 
	void InsertCheck(PayCheck & _check);

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	Метод получения набора чеков по году и месяцу
	std::vector<PayCheck *> * GetChecksByYearMonth(const YearMonth & _YM, int _currentDay) const;

	//	Средний расход за _n месяцев
	float AverageExpense(const int _n) const;

	//	Вспомагательная функция к AverageExpense()
	//	Рассчитывается потраченная сумма за предыдущий месяц, 
	//	который передается аргументом в эту функцию.
	//	Важно заметить, что сумма рассчитывается от дня текущего месяца
	//	к тому же самому дню в предыдущем месяце. 
	//	Это позвоялет точно рассчитать средний расход именно от сегодняшнего дня
	float CostByPrevMonth(const YearMonth & _ym, int _currentDay) const;

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	Функция возвращает вектор наиболее часто покупаемых товаров отсортированный в порядке убывания
	std::vector < const PurchaseItem * > GetMostPurchaseable(const int _n);

	//	Такая же функция, только подсчет за все время
	std::vector < const PurchaseItem * > GetMostPurchaseable();

	////////////////////////////////////////////////////////////////////////////////////////////////

	//	Метод возвращает отображение категория->(общие затраты) за _n месяцев до сегодняшнего дня
	std::map<Product::Category, float> * CostByCategoryByLastNMonth(const int _n);
	//	Дополнительный метод, возвращает то же отображение за все время
	std::map<Product::Category, float> * WholeCostByCategory();

private: 

	//	Вспомагательная функция
	float getCostByCategoryAndRange(const Product::Category & _categ, const std::vector < PayCheck * > & _checkVec) const;

public: 

	std::vector<PurchaseItem> GetTheBiggestPurchases() const;

	////////////////////////////////////////////////////////////////////////////////////////////////


	//	Классификация затрат
	enum Classification { SINGLE, PERIODIC, ALWAYS };

	//	Метод возвращает строку классификации
	const std::string GetClassificationString(const Classification _class);

private:
	
	//	Отображение для строкового представления классификатора
	static const std::map<const Classification, const char *> _classificMap;

	//	Отображение классифицированных затрат
	static std::map < Classification, std::vector< const PurchaseItem * > > m_itemsClassified;

	bool isClassificated = false;

	//	Функция инициализирует названия
	void ClassificationInit();

	//	Метод распределяет все покупки по категориям
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