#include "FileManager.h"
#include "GameplayManager.h"

void FileManager::WriteTableDat(GameplayManager* manager)
{
	//Her seferinde overwrite olarak yazýlacak.

	ofstream tablefile;
	tablefile.open("Table.dat", ios::trunc);


	tablefile << manager->roundCount << ".Tur" << endl;
	tablefile << "Mevcut Oyun Tahtasi" << endl;
	tablefile << "----------------------------------------------" << endl;
	//slotlarýn bulunduðu listeden tek tek slotlarý alýp orada ki bilgilere göre, tavla tahtasý çizilecek.
	//i satýr sayýsý
	for (int i = 0; i < 13; i++)
	{
		if (i == 0)		//ilk satýrýn  yazýlmasý
		{
			for (int j = 0; j < 12; j++)
			{
				if (j % 2 == 0)
				{
					if (j == 6) tablefile << "   " << manager->letters[j] << "  ";
					else
						tablefile << "  " << manager->letters[j] << "  ";
				}
				else tablefile << " " << manager->letters[j] << " ";
			}
		}

		if (i == 1 || i == 5 || i == 8 || i == 12)
		{
			for (int j = 0; j < 50; j++)
			{
				tablefile << "-";
			}
		}

		if (i == 2 || i == 4 || i == 9 || i == 11)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 6) tablefile << "|";
				else tablefile << "|   ";
			}
		}

		if (i == 6)
		{
			for (int j = 0; j < 12; j++)
			{
				switch (j) {
				case 4:
					tablefile << "|BX ";
					break;
				case 5:
					tablefile << "|D1 ";
					break;
				case 6:
					tablefile << "||D2 ";
					break;
				case 7:
					tablefile << "|BY ";
					break;
				case 11:
					tablefile << "|   |" << "--------> 2. katman";
					break;
				default:
					tablefile << "|   ";
					break;
				}

			}
		}

		if (i == 7)
		{
			for (int j = 0; j < 12; j++)
			{
				string info = manager->GetSlotValueForBoard(manager->letters[j] + "2");

				switch (j) {
				case 4:
					if (info == "-1") tablefile << "| 0 ";
					else tablefile << "|" << info;
					break;
				case 5:
					if (info == "-1") tablefile << "| 0 ";
					else tablefile << "|" << info;
					break;
				case 6:
					if (info == "-1") tablefile << "|| 0 ";
					else tablefile << "||" << info;
					break;
				case 7:
					if (info == "-1") tablefile << "| 0 ";
					else tablefile << "|" << info;
					break;
				case 11:
					tablefile << "|   |";
					break;
				default:
					tablefile  << "|   ";
					break;
				}

			}
		}

		if (i == 3 || i == 10)
		{
			//1.katman mý 3. katmanmý onun belirlenmesi.
			string layer = "";
			if (i == 3) layer = "1";
			else layer = "3";


			//Taþlarýn bulunduðu slotlar bilgi alýnarak yazýlacak.
			for (int j = 0; j < 12; j++)
			{

				if (j == 6)
				{


					string info = manager->GetSlotValueForBoard(manager->letters[j] + layer);
					if (info == "-1") //Slot boþ
					{
						tablefile << "||   ";
					}

					else tablefile << "||" << info;
				}

				else if (j == 11)
				{
					string info = manager->GetSlotValueForBoard(manager->letters[j] + layer);
					if (info == "-1") //Slot boþ
					{
						tablefile << "|   |" << "-------> " << layer << ". katman";
					}

					else tablefile << "|" << info << "|" << "-------> " << layer << ". katman";

				}

				else
				{
					string info = manager->GetSlotValueForBoard(manager->letters[j] + layer);
					if (info == "-1") //Slot boþ
					{
						tablefile << "|   ";
					}

					else tablefile << "|" << info;
				}
			}
		}
		tablefile << endl;
	}

	tablefile.close();
}

void FileManager::WritePlayLog(GameplayManager* manager)
{
	

	ofstream playlog;
	playlog.open("DiceRollLog.dat", ios::app);

	//Daha oyun  tekli zarlarýn yazýlmasý için.
	if (manager->roundCount == 0)
	{
		playlog << "-------------------------------" << endl;
		playlog << manager->roundCount <<".Tur" << endl;
		playlog << "-------------------------------" << endl;
		playlog << "X = " << manager->dice1 << endl;
		playlog << "Y = " << manager->dice2 << endl;
		playlog << "-------------------------------" << endl;

	}

	else
	{

		playlog << "-------------------------------" << endl;
		playlog << manager->roundCount << ".Tur" << endl;
		playlog << "-------------------------------" << endl;
		playlog << manager->currentRound << " = " << manager->dice1 << ", " << manager->dice2 << endl;
		playlog << "-------------------------------" << endl;

	}

	playlog.close();




	
}

void FileManager::WriteGameInfos(GameplayManager* manager)
{
	ofstream gameinfos;
	gameinfos.open("GameInfos.dat", ios::trunc);

	gameinfos << "RoundCount = " << manager->roundCount << endl;
	gameinfos << "Dice 1 = " << manager->dice1 << endl;
	gameinfos << "Dice 2 = " << manager->dice2 << endl;
	gameinfos << "BrokenXNum = " << manager->brokenXnum << endl;
	gameinfos << "BrokenYNum = " << manager->brokenYnum << endl;
	gameinfos << "CollectedX = " << manager->collectedX << endl;
	gameinfos << "CollectedY = " << manager->collectedY << endl;
	gameinfos << "BrokenXstate = " << manager->brokenXstate << endl;
	gameinfos << "BrokenYstate = " << manager->brokenYstate << endl;
	gameinfos << "CollectXstate = " << manager->collectedX << endl;
	gameinfos << "CollectYstate = " << manager->collectedY << endl;
	gameinfos << "CurrentRound = " << manager->currentRound << endl;

	gameinfos.close();



}

void FileManager::WriteSlotInfos(GameplayManager* manager)
{
	ofstream slotinfos;
	slotinfos.open("SlotInfos.dat", ios::trunc);

	for (int i = 0; i < 28; i++)
	{
		slotinfos << manager->slots.at(i).name << " - " << manager ->slots.at(i).count << " - " << manager->slots.at(i).stype<< endl;
	}

	slotinfos.close();


}

void FileManager::LoadTheGame(GameplayManager* manager, bool& state)
{
	//Diðer fonksiyonlarý çalýþtýrmadan save dosyalarýndan biri kontrol edilmeli.

	ifstream slotinfo;

	slotinfo.open("SlotInfos.dat", ios::in);
	//Dosya açýlamýyor.
	if (!slotinfo.is_open())
	{
		state = false;
		slotinfo.close();
		return;
	}

	else
	{
		state = true;
		slotinfo.close();
		ReadSlotInfos(manager);
		ReadGameInfos(manager);
	}

}

void FileManager::ReadSlotInfos(GameplayManager* manager)
{

	//tek tek satýrlarý al , yazým standartý belli olduðundan index sayilari
	//slot name = 0-1	
	//slot count = 5
	//slot type = 9 olacak þekilde. Ýsimden slotu alýp count ve tip deðerini bilgilere göre güncellenecek.

	ifstream slotinfo;

	slotinfo.open("SlotInfos.dat", ios::in);

	if (!slotinfo.is_open()) cout << "dosya acik degil";


	char line[30];
	while (!slotinfo.eof())
	{
		slotinfo.getline(line, 30);

		string slotname = "";
		slotname += line[0];
		slotname += line[1];

		Slot* currentslot = manager->GetSlot(slotname);

		if (currentslot == nullptr) continue;

		string scount = "";
		scount += line[5];
		int count = stoi(scount);

		slottype cslottype;

		if (line[9] == '0') cslottype = slottype::X;
		else if (line[9] == '1') cslottype = slottype::Y;
		else cslottype = slottype::neutral;
		currentslot->UpdateSlot(count, cslottype);
	}

}

void FileManager::ReadGameInfos(GameplayManager* manager)
{
	ifstream gameinfo;

	gameinfo.open("GameInfos.dat", ios::in);

	char line[30];
	int index = 0;

	while (!gameinfo.eof())
	{
		gameinfo.getline(line,30);
		EvaluateGameInfos(index, line, manager);
		index++;
	}


	gameinfo.close();

}

void FileManager::EvaluateGameInfos(int index, char* arr, GameplayManager* manager)
{

	string s = "";
	int count = 0;

	switch (index)
	{
	case 0:		//roundcount info
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				s += arr[i + 3];
				break;
			}
		}
		count = stoi(s);
		manager->roundCount = count;

	break;
	case 1:		//dice1 info
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				break;
			}
		}
		count = stoi(s);
		manager->dice1= count;
		break;
	case 2:			//dice2 info
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				break;
			}
		}
		count = stoi(s);
		manager->dice2 = count;
		break;
	case 3: //Broken x num
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				break;
			}
		}
		count = stoi(s);
		manager->brokenXnum = count;
		break;
	case 4:		//Broken y num
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				break;
			}
		}
		count = stoi(s);
		manager->brokenYnum = count;
		break;
	case 5: //Collected X
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				s += arr[i + 3];
				break;
			}
		}
		count = stoi(s);
		manager->collectedX = count;
		break;
	case 6:	//collected y
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				s += arr[i + 2];
				s += arr[i + 3];
				break;
			}
		}
		count = stoi(s);
		manager->collectedY = count;
		break;
	case 7:	//Broken x state
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				if (arr[i + 2] == '0')
				{
					manager->brokenXstate = false;
					break;
				}
				else if (arr[i + 2] == '1')
				{
					manager->brokenXstate = true;
					break;
				}
			}
		}
		break;
	case 8: //Broken y state
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				if (arr[i + 2] == '0')
				{
					manager->brokenYstate = false;
					break;
				}
				else if (arr[i + 2] == '1')
				{
					manager->brokenYstate = true;
					break;
				}
			}
		}
		break;
	case 9:		//Collect x state
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				if (arr[i + 2] == '0')
				{
					manager->collectXstate = false;
					break;
				}
				else if (arr[i + 2] == '1')
				{
					manager->collectXstate = true;
					break;
				}
			}
		}
		break;
	case 10:	//collect y state
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				if (arr[i + 2] == '0')
				{
					manager->collectYstate = false;
					break;
				}
				else if (arr[i + 2] == '1')
				{
					manager->collectYstate = true;
					break;
				}
			}
		}
		break;
	case 11:	//current round info
		for (int i = 0; i < 30; i++)
		{
			if (arr[i] == '=')
			{
				manager->currentRound = arr[i + 2];
			}
		}
		break;
	}


}

void FileManager::StartSettings()
{
	ofstream playlog;
	playlog.open("DiceRollLog.dat", ios::trunc);
	playlog.close();
}
