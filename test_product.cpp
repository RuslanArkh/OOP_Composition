#include "product.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

int main()
{
	Product * pProduct1;
	Product * pProduct2;
	try
	{
		pProduct1 = new Product("Ice Cream", 5.11f, Product::Category::FOOD);
		std::cout << "Price: " << pProduct1->getPrice() << "\n";
		std::cout << "Category: " << pProduct1->categ << "\n";
		std::cout << "Title: " << pProduct1->getTitle() << "\n";
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		pProduct2 = new Product("Shampoo", 10.4f, (Product::Category)2);
		std::cout << "Price: " << pProduct2->getPrice() << "\n";
		std::cout << "Category: " << pProduct2->categ << "\n";
		std::cout << "Title: " << pProduct2->getTitle() << "\n";
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	float a = 2.11f;
	float b = 3.11f;
	std::cout << "\n" << round(a * b * 100) / 100.0 << "\n";
	//std::cout << "Price: " << pProduct1->getPrice() << "\tTitle: " << pProduct1->getTitle() << "\n";

}