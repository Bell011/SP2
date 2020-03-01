#include "Currency.h"
 //int Currency::money = 500;
Currency::Currency(int m)
{
	money = m;
}

Currency::~Currency()
{
}

int Currency::getMoney()
{
	return money;
}

void Currency::setMoney(int m)
{
	money = m;
}
