#include "GameplayManager.h"

#include <iostream>

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

	cout << "Oyuna Kimin Baslayacaginin Belirlenmesi icin Tekli Zarlarýn Atilmasi" << endl << endl;
	cout << "X oyuncusu icin atilan zar degeri =   " << endl << endl;
	cout << "Y oyuncusu icin atilan zar degeri =   " << endl << endl;
	


}


