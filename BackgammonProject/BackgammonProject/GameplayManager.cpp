#include "GameplayManager.h"

#include <iostream>

void GameplayManager::StartGame()
{
	//Yar�m kalm�� bir oyun var m� kontrol et, yar�m kald�ysa o oyunun bilgilerini al�p oyuna �yle devam et de�il ise yeni oyun i�in haz�rl�k yap.

	//Slotlar� olu�tur ve listeye ekle.
	CreateSlots();

	//Slotlar�n ilk durum i�in bilgilerinin verilmesi.
	UpdateSlotsForNewGame();
	//TODO oyun e�er ortada b�rak�lm��sa ilk durum i�in de�ilde bilgileri dosyadan �ekip var olan slot bilgileri ona g�re g�ncellenecek.
	UpdateSlotsForSavedGame();
	

	GameLogic();


	//TODO oyuna ba�lamadan �nce g�ncellenen bilgileri dosyalara aktar.
	//Start the GameLogic
	
}



void GameplayManager::GameLogic()
{
	bool gameOn = true;

	//Bunu ba�latmadan �nce ilk oyun mu saved oyun mu kontrol et sonra ba�lat.
	PlayTheFirstRound();

	while (gameOn)
	{
		//Oyun d�ng�s� burada olacak.
		//ilk oyun i�in zar atma olaylar� burada olmayacak.
	
	}
}

void GameplayManager::CreateSlots()
{
	for (int i = 0; i < 12; i++)
	{
		//Her harf i�in alt ve �st olmak �zere 2 tane olu�tur ve listeye ekle.
		Slot slot1(letters[i] + "1", 0, slottype::neutral);
		Slot slot2(letters[i] + "3", 0, slottype::neutral);

		slots.push_back(slot1);
		slots.push_back(slot2);
	}

	// 24 tane normal slotlar i�in olu�turduktan sonra 4 tane de orta slot i�in.
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
	//Filemanagerdan bilgi alarak slot bilgilerini g�ncelle.
}

void GameplayManager::UpdateTheSlotInfo(string nm,int cnt,slottype t)
{
	//Var olan listeden name ile uygun olan� bulup verilen bilgilerle g�ncellenmesi.

	for (int i = 0; i < slots.size(); i++)
	{
		//Arad���m�z slotu bulduk.
		if (slots.at(i).name == nm)
		{
			slots.at(i).count = cnt;
			slots.at(i).stype = t;
		}


	}
}

void GameplayManager::PlayTheFirstRound()
{
	//E�er ilk oyun ise, oyuna kimin ba�lamas�n�n belirlenmesi i�in zarlar�n at�ld��� round olacak.

	//Bilgi verilecek, x ve y oyuncusu i�in zar at�lmas� istenecek.

	cout << "Oyuna Kimin Baslayacaginin Belirlenmesi icin Tekli Zarlar�n Atilmasi" << endl << endl;
	cout << "X oyuncusu icin atilan zar degeri =   " << endl << endl;
	cout << "Y oyuncusu icin atilan zar degeri =   " << endl << endl;
	


}


