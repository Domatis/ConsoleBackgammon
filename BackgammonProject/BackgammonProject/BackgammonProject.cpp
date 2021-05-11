// BackgammonProject.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//
#include "Slot.h"
#include "GameplayManager.h"
#include <iostream>
#include <string>

#include <ctime>
#include <cstdlib>
using namespace std;

int main()
{
	GameplayManager gmanager;

	//gmanager.StartGame();

	srand((unsigned)time(0));
	int x = rand() % 7;

	cout << x << endl;

	srand((unsigned)time(0));
	x = rand() % 7;

	cout << x << endl;
	
	srand((unsigned)time(0));
	x = rand() % 7;

	cout << x << endl;

	srand((unsigned)time(0));
	x = rand() % 7;

	cout << x << endl;

	
	

    
}


