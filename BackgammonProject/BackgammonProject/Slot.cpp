#include "Slot.h"



Slot::Slot(string n, int c, slottype t)
{
	Slot ::name = n;
	Slot::count = c;
	Slot::stype = t;
}

void Slot::UpdateSlot(int c, slottype t)
{
	Slot ::count = c;
	Slot::stype = t;
}






