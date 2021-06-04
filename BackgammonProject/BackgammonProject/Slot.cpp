#include "Slot.h"



Slot::Slot(string n, int c, slottype t,int lyr)
{
	Slot ::name = n;	
	Slot::count = c;
	Slot::stype = t;
	Slot::layer = lyr;
}

void Slot::UpdateSlot(int c, slottype t)
{
	Slot ::count = c;
	Slot::stype = t;
}






