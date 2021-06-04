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
	int layer; //1,2,3
	slottype stype = neutral;

	Slot(string n,int c,slottype t,int layer);
	void UpdateSlot(int c,slottype t);


};

