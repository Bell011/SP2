#pragma once

class Currency {
private:
	int money;
public:
	Currency(int m = 500);
	~Currency();
	 void setMoney(int m);
	 int getMoney();
};