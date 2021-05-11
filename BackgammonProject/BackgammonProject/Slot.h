#pragma once
#include <iostream>
#include <string>

using namespace std;

enum  slottype { X, Y, neutral }; // 0,1,2

class Slot
{
	
public:
	
	string name;
	int count;
	slottype stype = neutral;

	Slot(string n,int c,slottype t);
	void UpdateSlot(int c,slottype t);


};

