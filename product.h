#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <string>
#include <map>

class PurchaseHistory;

class Product
{
	friend PurchaseHistory;

public: 

	//	��������� ������
	enum Category{ FOOD, HOUSEHOLD_CHEMICALS, HYGIENE, ENTERTAINMENT, OTHERS }; 

	static std::string GetCategoryString(const Category _cat);
	static const std::map<Category, const char *> * getMap();

private:

	static const std::map<Category, const char *> _cMap;

	//	�������� ������
	std::string m_title;

	//	���� ������
	float m_price;

	Category categ;

public:
	
	explicit Product();

	//	�����������
	Product(std::string _title, float _price, Product::Category _categ);

	//	������ ������� � ���������� ����������
	const std::string getTitle() const;
	const float getPrice() const;
	
	const Product::Category getCategory() const;

	bool operator == (const Product _prod) const;

};

inline const std::string Product::getTitle() const
{
	return m_title;
}

inline const float Product::getPrice() const
{
	return m_price;
}


inline const Product::Category Product::getCategory() const
{
	return categ;
}

inline std::string Product::GetCategoryString(const Category _cat)
{
	return _cMap.find(_cat)->second;
}

inline const std::map<Product::Category, const char *> * Product::getMap()
{
	return &_cMap;
}

#endif