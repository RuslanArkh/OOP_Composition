#include "product.h"

#include <stdexcept>

//	Инициализация отображения Категория->Строка
const std::map<Product::Category, const char *> Product::_cMap = {
	std::pair<Product::Category, const char *>(Product::Category::FOOD, "Food")
	, std::pair<Product::Category, const char *>(Product::Category::HOUSEHOLD_CHEMICALS, "Household chemicals")
	, std::pair<Product::Category, const char *>(Product::Category::HYGIENE, "Hygiene")
	, std::pair<Product::Category, const char *>(Product::Category::ENTERTAINMENT, "Entertainment")
	, std::pair<Product::Category, const char *>(Product::Category::OTHERS, "Others") 
};


Product::Product()
	: m_title("")
	, m_price(0.0f)
{
}

Product::Product(std::string _title, float _price, Product::Category _categ)
	: m_title(_title)
	, m_price(round(_price * 100) / 100.0)
	, categ(_categ)
{
	if (m_price < 0)
		throw std::logic_error("Error: price is less than zero.");
	
}

bool Product::operator == (const Product _prod) const
{
	return m_title == _prod.m_title &&
		m_price == _prod.m_price &&
		categ == _prod.categ;
}