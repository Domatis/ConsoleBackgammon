#include "GameplayManager.h"

#include <iostream>

#include <stdlib.h> 
#include <ctime>
#include <cstdlib>


void GameplayManager::StartGame()
{
	//Yarým kalmýþ bir oyun var mý kontrol et, yarým kaldýysa o oyunun bilgilerini alýp oyuna öyle devam et deðil ise yeni oyun için hazýrlýk yap.

	//Slotlarý oluþtur ve listeye ekle.
	CreateSlots();

	//Slotlarýn ilk durum için bilgilerinin verilmesi.
	UpdateSlotsForNewGame();
	//TODO oyun eðer ortada býrakýlmýþsa ilk durum için deðilde bilgileri dosyadan çekip var olan slot bilgileri ona göre güncellenecek.
	UpdateSlotsForSavedGame();
	

	GameLogic();


	//TODO oyuna baþlamadan önce güncellenen bilgileri dosyalara aktar.
	//Start the GameLogic
	
}



void GameplayManager::GameLogic()
{
	bool gameOn = true;

	//Bunu baþlatmadan önce ilk oyun mu saved oyun mu kontrol et sonra baþlat.
	PlayTheFirstRound();

	while (gameOn)
	{
		//Oyun döngüsü burada olacak.
		system("CLS");
		
		DrawtheCurrentTable();

		cin.get();
		

		//Oyunun mevcut durumunun ekrana çizilmesi.


		//ilk oyun için zar atma olaylarý burada olmayacak.
	}
}

void GameplayManager::CreateSlots()
{
	for (int i = 0; i < 12; i++)
	{
		//Her harf için alt ve üst olmak üzere 2 tane oluþtur ve listeye ekle.
		Slot slot1(letters[i] + "1", 0, slottype::neutral);
		Slot slot2(letters[i] + "3", 0, slottype::neutral);

		slots.push_back(slot1);
		slots.push_back(slot2);
	}

	// 24 tane normal slotlar için oluþturduktan sonra 4 tane de orta slot için.
	Slot slot1("E2", 0, slottype::neutral);
	Slot slot2("F2", 0, slottype::neutral);
	Slot slot3("G2", 0, slottype::neutral);
	Slot slot4("H2", 0, slottype::neutral);

	slots.push_back(slot1);
	slots.push_back(slot2);
	slots.push_back(slot3);
	slots.push_back(slot4);

	
}

void GameplayManager::UpdateSlotsForNewGame()
{
	UpdateTheSlotInfo("A1", 5, slottype::Y);
	UpdateTheSlotInfo("A3", 5, slottype::X);

	UpdateTheSlotInfo("E1", 3, slottype::X);
	UpdateTheSlotInfo("E2", 0, slottype::X);
	UpdateTheSlotInfo("E3", 3, slottype::Y);


	UpdateTheSlotInfo("G1", 5, slottype::X);
	UpdateTheSlotInfo("G3", 5, slottype::Y);

	UpdateTheSlotInfo("H2", 0, slottype::Y);

	UpdateTheSlotInfo("L1", 2, slottype::Y);
	UpdateTheSlotInfo("L3", 2, slottype::X);
}

void GameplayManager::UpdateSlotsForSavedGame()
{
	//Filemanagerdan bilgi alarak slot bilgilerini güncelle.
}

void GameplayManager::UpdateTheSlotInfo(string nm,int cnt,slottype t)
{
	//Var olan listeden name ile uygun olaný bulup verilen bilgilerle güncellenmesi.

	for (int i = 0; i < slots.size(); i++)
	{
		//Aradýðýmýz slotu bulduk.
		if (slots.at(i).name == nm)
		{
			slots.at(i).count = cnt;
			slots.at(i).stype = t;
		}
	}
}

void GameplayManager::PlayTheFirstRound()
{
	//Eðer ilk oyun ise, oyuna kimin baþlamasýnýn belirlenmesi için zarlarýn atýldýðý round olacak.

	//Bilgi verilecek, x ve y oyuncusu için zar atýlmasý istenecek.

	bool equal = true;
	int diceX = 0;
	int diceY = 0;

	while (equal)
	{
		srand((unsigned)time(0));
		 diceX = (rand() % 6) + 1;

		srand((unsigned)rand());
		 diceY = (rand() % 6) + 1;

		 if (diceX == diceY) equal = true;
		 else equal = false;
	}

	cout << "Oyuna Kimin Baslayacaginin Belirlenmesi icin Tekli Zarlarin Atilmasi" << endl << endl;
	cout << "X oyuncusu icin atilan zar degeri = " << diceX << endl;
	cout << "Y oyuncusu icin atilan zar degeri = " << diceY << endl << endl;

	if (diceX > diceY) cout << "-----Oyuna X oyuncusu baslayacak-----" << endl << endl;
	else cout << "-----Oyuna Y oyuncusu baslayacak-----" << endl << endl;

	int x = 0;

	cout << "Devam etmek entera basiniz" << endl;
	cin.get();
}

void GameplayManager::DrawtheCurrentTable()
{
	//slotlarýn bulunduðu listeden tek tek slotlarý alýp orada ki bilgilere göre, tavla tahtasý çizilecek.
	//i satýr sayýsý
	for (int i = 0; i < 12; i++)
	{
		if (i == 0)		//ilk satýrýn  yazýlmasý
		{
			for (int j = 0; j < 12; j++)
			{
				if (j % 2 == 0)
				{
					if (j == 6) cout << "   " << letters[j] << "  ";
					else 
					cout << "  " << letters[j] << "  ";
				}
				else cout << " " << letters[j] << " ";
			}
		}

		if (i == 1 || i == 5 ||i == 7 ||i == 11)
		{
			for (int j = 0; j < 50; j++)
			{
				cout << "-";
			}
		} 

		if (i == 2 || i == 4 || i == 8 || i == 10)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 6) cout << "|";
				else cout << "|   ";
			}
		}

		//TODO diðer satýrlarýn yazýlmasýný devam et ve yazýlýrken mevcut slot deðerleri için bilgileri alýp yazýlmasýný saðla.

		cout << endl;
	}



}


