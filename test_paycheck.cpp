#include "pay_check.h"
#include "product.h"

#include <iostream>
#include <ostream>

int main()
{
	
	PurchaseItem pI1(Product("Shampoo", 20.4, Product::Category::HYGIENE), 2);
	PurchaseItem pI2(Product("Meat", 15.95, Product::Category::FOOD));
	PayCheck * pPC1 = new PayCheck(DateTime("2014/03/19 20:54", '/', ' ', ':'), { pI1, pI2 });

	pPC1->addItem(Product("Coffee", 50.35, Product::Category::FOOD));

	std::cout << pPC1->getCheckDateTime().ToString() << "\n";
	std::cout << pPC1->getPurchaseItem(0).getProduct().getTitle() << "\n";
	std::cout << "Total cost for shampoo: " << pPC1->getPurchaseItem(0).Cost() << "\n";
	std::cout << pPC1->getPurchaseItem(1).getProduct().getTitle() << "\n";
	std::cout << "Last item: " << pPC1->getPurchaseItem(pPC1->getPurchaseItemCount() - 1).getProduct().getTitle() << "\t Price: ";
	std::cout << pPC1->getPurchaseItem(pPC1->getPurchaseItemCount() - 1).getProduct().getPrice() << "\n";

	std::cout << "\nCheck Id: " << PayCheck::getStatID() << "\n";
	std::cout << "Total sum: " << pPC1->getTotalSum() << "\n";

	delete pPC1;
}