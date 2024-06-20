#pragma once
#include<iostream>
#include <cstdlib> 
class Menu
{
private:
	static Menu* instance;
	Menu(std::string sursa);
	~Menu();
	static std::string sursa;
public:
	static Menu& createMenu(std::string sursa);
	static void deleteMenu();

	static  std::string getSursa() { return sursa; }
};

