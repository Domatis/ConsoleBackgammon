#include "GameplayManager.h"
#include <stdlib.h> 
#include <ctime>
#include <cstdlib>
#include "FileManager.h"


void GameplayManager::StartGame()
{

	CreateSlots();

	//Oyun ba��nda kay�tl� oynamak m� yoksa yeni oyuna ba�lamak m� istedi�i kullan�c�ya sorulacak verilen cevaba g�re i�lemler yap�lacak.

	char arr[2];

	cout << "Eger yeni oyuna baslamak istiyorsaniz Entera basip devam edin, Kayitli oyundan devam etmek icin S yazip devam edin" << endl;

	cin.get(arr, 2);
	cin.clear();
	cin.ignore(256, '\n');
	arr[0] = toupper(arr[0]);

	if (arr[0] == 'S')
	{
		//TODO burada extra kontrol yap�lmal� e�er kay�tl� oyun dosyas� yok ise yeni oyuna ge�i� yap�lmal�.

		bool isfileOpened = false;

		//Kayitli oyun i�in ayarlar yap�lacak.
		

		fmanager.LoadTheGame(this,isfileOpened);

		if (isfileOpened)
		{
			savedgame = true;
		}

		else       //Yeni oyun i�in ayarlar yap�lacak.
		{	
			cout << "Kayitli oyun dosyasi bulunamadi,devam etmek icin entera basiniz" << endl;
			cin.get();
			cin.clear();
			cin.ignore(256, '\n');

			savedgame = false;
			UpdateSlotsForNewGame();
			fmanager.StartSettings();
		}
		

	}

	else
	{
		//Yeni oyun i�in ayarlar yap�lacak.
		savedgame = false;
		UpdateSlotsForNewGame();
		fmanager.StartSettings();
	}


	system("CLS");
	
	//Buras� oyun ba�lang�c�nda oyun tahtas�n�n d�zenlenmesi.
	fmanager.WriteTableDat(this);
	
	GameLogic();

}

void GameplayManager::GameLogic()
{
	gameOn = true;

	//Bunu ba�latmadan �nce ilk oyun mu saved oyun mu kontrol et sonra ba�lat.
	if(!savedgame)	PlayTheFirstRound();
	

	while (gameOn)
	{

		roundCount++;
		//Oyun d�ng�s� burada olacak.
		system("CLS");
		//Oyun kazan�lmas�n�n kontrol�.
		if (collectedX == 15)
		{
			cout << "Kazanan X Oyuncusu, Oynadiginiz icin tesekkurler" << endl;
			gameOn = false;
			continue;
		}

		if (collectedY == 15)
		{
			cout << "Kazanan Y Oyuncusu, Oynadiginiz icin tesekkurler" << endl;
			gameOn = false;
			continue;
		}

		//Her el ba��nda zar att�r oyunun mevcut durumu, zar bilgisi ve oyunun kimde oldu�u bilgisini ver.
		RollTheDice();
	
		//Oyuncudan hamle i�in bilgi al�nmas�.
		TakePlayerInputs();		

		//Kay�t i�lemleri
		fmanager.WriteTableDat(this);
		fmanager.WritePlayLog(this);

		if (currentRound == "X") currentRound = "Y";
		else currentRound = "X";

		//oyundan ��kma se�ene�i
		char arr[2];
		cout << "Bir sonraki tura gecmek icin entera, Oyundan cikmak icin Y tusuna basip devam ediniz" << endl;
		cin.get(arr,2);
		arr[0] = toupper(arr[0]);
		if (arr[0] == 'Y')
		{
			//Oyunu kaydetme i�lemleri.
			fmanager.WriteGameInfos(this);
			fmanager.WriteSlotInfos(this);
			gameOn = false;
		}

		cin.clear();
		cin.ignore(256, '\n');

	}
}

void GameplayManager::CreateSlots()
{
	for (int i = 0; i < 12; i++)
	{
		//Her harf i�in alt ve �st olmak �zere 2 tane olu�tur ve listeye ekle.
		Slot slot1(letters[i] + "1", 0, slottype::neutral,1);
		Slot slot2(letters[i] + "3", 0, slottype::neutral,3);

		slots.push_back(slot1);
		slots.push_back(slot2);
	}

	// 24 tane normal slotlar i�in olu�turduktan sonra 4 tane de orta slot i�in.
	Slot slot1("E2", 0, slottype::neutral,2);
	Slot slot2("F2", 0, slottype::neutral,2);
	Slot slot3("G2", 0, slottype::neutral,2);
	Slot slot4("H2", 0, slottype::neutral,2);

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


	UpdateTheSlotInfo("L1", 2, slottype::Y);
	UpdateTheSlotInfo("L3", 2, slottype::X);

	UpdateTheSlotInfo("H2", 0, slottype::Y);
}

void GameplayManager::WriteRoundInfo()
{
	cout << roundCount << ".Tur" << endl;
	cout << "----------------------------------------------" << endl << endl;
	cout << "Sira " << currentRound << " oyuncusunda" << endl;
	cout << currentRound << " oyuncusu icin atilan zarlar :" << endl;
	cout << "Zar 1 = " << dice1 << endl;
	cout << "Zar 2 = " << dice2 << endl << endl;
	cout << "----------------------------------------------" << endl;
}

void GameplayManager::TakePlayerInputs()
{
	bool syntaxCorrect = false;		
	bool moveCorrect = false;		
	bool dicevalCorrect = false;
	char diceinputs[4][5];			//  Max 2 ve 4 hamle olabilir ona g�re olu�turulmu� dizi.
	int dicevalues[4];
	int movementNum = 0;

	//Zarlar�n e�it ve 4 hamle oynanabilece�i durum.
	if (dice1 == dice2)
	{
		movementNum = 4;
		for (int i = 0; i < 4; i++)
			dicevalues[i] = dice1;
	}
	else 
	{
		movementNum = 2;
		dicevalues[0] = dice1;
		dicevalues[1] = dice2;
		dicevalues[2] = -1;
		dicevalues[3] = -1;
	}

	for (int i = 0; i < movementNum; i++)
	{
		WriteRoundInfo();
		DrawtheCurrentTable();

		//Konsola inputlar�n giri�i ile bilgi verilecek.
		cout << "--X oyuncusunun taslari saat yonunde, Y oyuncusunun taslari saat yonunun tersi yonunde hareket edebilir, hamlelerinizi buna gore giriniz--" << endl;
		cout << "--Hamleler 'A1 5','B3 4' ve kirik taslar icin 'E2 5', 'H2 3' seklinde olabilir--" << endl;
		//TODO Mevcut olan oyundan ��kmak i�in ayr� bir se�enek olacak.

		//Her hamle ba��nda e�er oyuncunun oynayacak hamlesi yok ise tur bitirilecek(�rnek broken durumda ve oynanacak yeri yok veya ta�lar� toplarken oynanacak yeri yok).
		bool abletoplay = IsPlayerAbleToPlay(dicevalues);
		if (!abletoplay)
		{
			
			cout << "Oynanacak hamle kalmadi bir sonraki tura gecmek icin entera basin" << endl;
			cin.ignore();
			cin.get();
			break;
		}

		//Hamle i�in girilen inputlar�n ilk olarak syntax kontrol� olacak.
		
		while (!syntaxCorrect || !moveCorrect || !dicevalCorrect)
		{
			//Zar i�in syntax control d�ng�s�.
			cout << currentRound << " Oyuncusunun oynanabilecek zar degerleri = ";

			for (int k = 0; k < 4; k++)
			{
				if (dicevalues[k] != -1)
				{
					cout << dicevalues[k];
					if (k != movementNum-1) cout << "-";
				}
			}

			cout << endl;

			
			cin.get(diceinputs[i], 5);
			cin.clear();
			cin.ignore(256,'\n');
			char upperVal = toupper(diceinputs[i][0]);
			diceinputs[i][0] = upperVal;
			syntaxCorrect = IsInputSyntaxCorrect(diceinputs[i]);
			if (!syntaxCorrect)
			{
				cout << "Girilen hamlenin yazimi yanlis lutfen tekrar giriniz, hamleler 'A1 5','B3 4' ve benzeri sekilde olmalidir" << endl;
				continue;
			}			

			//Hamle kontrol�.

			string errorMessage = "";
			int selecteddiceval = CharToInt(diceinputs[i][3]);

			//Se�ilen zar de�eri , at�lan zarlardan biriyle e�le�iyor mu onun kontrol�.

			for (int k = 0; k < 4; k++) 
			{
				if (dicevalues[k] == -1) continue;

				if (dicevalues[k] == selecteddiceval)
				{
					dicevalCorrect = true;
					break;
				}
			}

			if (!dicevalCorrect)
			{
				cout << "Gecersiz zar degeri girdiniz, lutfen tekrar giriniz" << endl;
				continue;
			}

			//Movement kontrol�.
			moveCorrect = IsMovementCorrect(diceinputs[i], &errorMessage,selecteddiceval);
			if (!moveCorrect)
			{
				cout << errorMessage << endl;
				continue;
			}
				
			//E�er hamlelerde sorun yok ise �al��acak k�s�m.

			// Verilen zar de�eri i�in hamle yap�lm��sa o zar de�erini zar de�erleri dizisinden ��kar�lmas�.
			for (int k = 0; k < 4; k++)
			{
				if (selecteddiceval == dicevalues[k])
				{
					dicevalues[k] = -1;
					break;
				}
			}

			//Hem syntax hem movement do�ru ise hamlenin yap�lmas� ve slot de�erlerinin g�ncellenmesi.
			//Hamleyi yapan fonksiyon.		
			Slot* source;
			Slot* target;
			bool clockwise = true;

			string slotname1 = "";

			slotname1 += diceinputs[i][0];
			slotname1 += diceinputs[i][1];

			source = GetSlot(slotname1);
			if (currentRound == "X") clockwise = true;
			else clockwise = false;
			target = CalculateNextSlot(diceinputs[i], selecteddiceval, clockwise);

			MakeMovement(source, target);

			//Broken slotlar�n� brokensayilarla g�ncelle.
			GetSlot("E2")->count = brokenXnum;
			GetSlot("H2")->count = brokenYnum;

			
		}


		syntaxCorrect = false;
		moveCorrect = false;
		dicevalCorrect = false;
	}
	

}

bool GameplayManager::IsMovementCorrect(const char* arr, string* returnmessage,int diceval)
{

	string slotname = "";
	slotname += arr[0];
	slotname += arr[1];

	Slot* currentslot = GetSlot(slotname);
	Slot* nextSlot;

	if (currentRound == "X")
	{

		//E�er broken x durumu var ise, kaynak slot E2 olmal�d�r, de�il ise error mesaj� ile d�nd�r�l�r.
		if (brokenXstate && currentslot->name != "E2")
		{
			*returnmessage = "Ilk olarak kirilmis durumda tasinizi oynamak zorundasiniz";
			return false;
		}

		if (!brokenXstate && currentslot->name == "E2")
		{
			*returnmessage = "Kirilmis durumda tasinizi bulunmamaktadir,gecersiz hamle";
			return false;
		}

		//�lk kontrol hamle yap�lmas� istenen yerde oyuncuya ait ta� var m� yok mu kontrol edilmesi, yok ise false d�nd�r�lmesi ve hata mesaj�n�n bu duruma g�re g�ncellenmesi.
		if (currentslot->stype != slottype::X)
		{
			*returnmessage = "Oynanmasini istediginiz yerde tasiniz bulunmamaktadir";
			return false;
		}

		nextSlot = CalculateNextSlot(arr, diceval, true);	//Se�ilen slot ve oynanan zar i�in bir sonraki slotun bulunmas� ve ard�ndan kontrol edilmesi.

		//E�er oyun tahtas�n�n d���na ��k�yorsa.
		if (nextSlot == nullptr)
		{
			*returnmessage = "Oynamak istediginiz hamle gecersizdir";
			return false;
		}

		//Se�ilen yerde di�er oyuncunun 1 den fazla ta�� var ise de oynanamaz.

		if (nextSlot->stype == slottype::Y && nextSlot->count > 1)
		{
			*returnmessage = "Tasinizin gidecegi yerde 1'den fazla rakip tas bulunmaktadir,hamle gecersizdir.";
			return false;
		}

		//Yukar�da ki durumlar yok ise hamle ge�erli.
		return true;
	}



	// Y oyuncusu i�in ge�erli olan durum.
	else
	{
		//E�er broken x durumu var ise, kaynak slot E2 olmal�d�r, de�il ise error mesaj� ile d�nd�r�l�r.
		if (brokenYstate && currentslot->name != "H2")
		{
			*returnmessage = "�lk olarak kirilmis durumda tasinizi oynamak zorundasiniz";
			return false;
		}

		if (!brokenYstate && currentslot->name == "H2")
		{
			*returnmessage = "Kirilmis durumda tasinizi bulunmamaktadir,gecersiz hamle";
			return false;
		}

		//�lk kontrol hamle yap�lmas� istenen yerde oyuncuya ait ta� var m� yok mu kontrol edilmesi, yok ise false d�nd�r�lmesi ve hata mesaj�n�n bu duruma g�re g�ncellenmesi.
		if (currentslot->stype != slottype::Y)
		{
			*returnmessage = "Oynanmasini istediginiz yerde tasiniz bulunmamaktadir";
			return false;
		}

		nextSlot = CalculateNextSlot(arr, diceval, false);	//Se�ilen slot ve oynanan zar i�in bir sonraki slotun bulunmas� ve ard�ndan kontrol edilmesi.

		//E�er oyun tahtas�n�n d���na ��k�yorsa.
		if (nextSlot == nullptr)
		{
			*returnmessage = "Oynamak istediginiz hamle gecersizdir";
			return false;
		}

		//Se�ilen yerde di�er oyuncunun 1 den fazla ta�� var ise de oynanamaz.

		if (nextSlot->stype == slottype::X && nextSlot->count > 1)
		{
			*returnmessage = "Tasinizin gidecegi yerde 1'den fazla rakip tas bulunmaktadir,hamle gecersizdir.";
			return false;
		}

		//Yukar�da ki durumlar yok ise hamle ge�erli.
		return true;
	}


	//Di�er bir kontrol de ta��n oynanmak istendi�i yer olacak.2 duruma g�re uygun olmayacak, ilk durum ayn� yerde di�er oyuncunun 2 ve daha fazla ta�� olmas�.
	//2.durum ise oyun tahtas�n�n s�n�rlar�nda bir yere oynanmak istemesi olacak.
	//ilk olarak hamle yap�lacak oyuncunun hamle yerine g�re kontroller 2 farkl� �ekilde olacak.



	*returnmessage = "";

	return true;
}

bool GameplayManager::IsInputSyntaxCorrect(const char  *arr)
{
	//Girilen inputun A1,B3 tarz�nda olup olmad���n�n kontrol edilip de�er d�nd�r�lmesi.
	bool state = false;


	//Zar de�eri i�in sadece 1 ile 6 d���nda m� de�il mi kontrol� yap�l�yor.
	int dicenum = CharToInt(arr[3]);

	if (dicenum < 1 || dicenum > 6)
	{
		state = false;
		return state;
	}

	//Broken zarlar�n oynanmas� i�in slotlar.
	if (arr[1] == 2 && (arr[0] != 'E' && arr[0] != 'H'))
	{
		state = false;
		return state;
	}

	//�lk �nce 2.de�er 1 veya 3 de�ilse return false d�nd�r�lecek.
	if (arr[1] != '1' && arr[1] != '3' && arr[1] != '2')
	{
		state = false;
		return state;
	}

	//Char harfin stringe d�nmesi.
	string chartoStr(1, arr[0]);

	for (int i = 0; i < 12; i++)
	{
		if (chartoStr == letters[i])
		{
			state = true;
			return state;
		}
	}

	return state;
}

void GameplayManager::UpdateTheSlotInfo(string nm,int cnt,slottype t)
{
	//Var olan listeden name ile uygun olan� bulup verilen bilgilerle g�ncellenmesi.
	for (int i = 0; i < slots.size(); i++)
	{
		//Arad���m�z slotu bulduk.
		if (slots.at(i).name == nm)
		{
			slots.at(i).UpdateSlot(cnt, t);
		}
	}
}

string GameplayManager::GetSlotValueForBoard(string slotname)
{
	string result = "-1";

	for (int i = 0; i < slots.size(); i++)
	{
		//Arad���m�z slotu bulduk.
		if (slots.at(i).name == slotname)
		{
			if (slots.at(i).count <= 0)		//Bulunan slotta herhangi bir ta� yok.
			{
				return result;
			}

			else //bulunan slotta ta� var t�r� belirlenip uygun de�er d�nd�r�lecek.
			{
				if (slots.at(i).stype == slottype::X && (slots.at(i).layer == 1 || slots.at(i).layer == 3))		//X ta�� var.
				{
					result = to_string(slots.at(i).count) + "X ";
					return result;
				}

				else if (slots.at(i).stype == slottype::Y && (slots.at(i).layer == 1 || slots.at(i).layer == 3) )	//Y ta�� var.
				{
					result = to_string(slots.at(i).count) + "Y ";
					return result;
				}


				else if (slots.at(i).stype == slottype::neutral || slots.at(i).layer == 2)	//N�tr ta�.
				{
					result = " " + to_string(slots.at(i).count) + " ";
					return result;
				}
			}
		}
	}



	return result;

}

Slot* GameplayManager::GetSlot(string slotname)
{


	for (int i = 0; i < slots.size(); i++)
	{
		if (slots.at(i).name == slotname)
		{
			return &slots.at(i);
		}
	}


	return nullptr;

}

Slot* GameplayManager::CalculateNextSlot(const char* arr,int diceval,bool clockwise)
{
	//Broken durumu var ise hedef slot se�imi farkl� yap�lacak ve d�nd�r�lecek.

	if (currentRound == "X" && brokenXstate)
	{
		int letterindex = 12 - diceval;
		string slotname = FindLetterByIndex(letterindex);
		slotname += "3";

		return GetSlot(slotname);
	}

	if (currentRound == "Y" && brokenYstate)
	{
		int letterindex = 12 - diceval;
		string slotname = FindLetterByIndex(letterindex);
		slotname += "1";

		return GetSlot(slotname);
	}

	if (currentRound == "X" && collectXstate)
	{
		string currentslotname = "";
		currentslotname += arr[0];
		int currentindex = FindIndexByLetter(currentslotname);
		int diceindex = 12 - diceval;
		currentslotname += arr[1];
		

		Slot* currentSlot = GetSlot(currentslotname);

		//E�er collect durumunda isek next olarak yine se�ti�imiz slotu g�nderiyoruz.
		if (currentSlot->stype == slottype::X && currentSlot ->count >=1)
		{
			if (currentindex == diceindex)
			{
				return currentSlot;
			}

			//Olmamas� durumunda bir kontrol daha yap�yoruz.
			else if (diceindex < currentindex)
			{
				bool leftslotsempty = true;

				for (int k = currentindex-1; k > 5; k--)
				{
					string slotn = FindLetterByIndex(k);
					slotn += "1";

					Slot* s = GetSlot(slotn);
					if (s->stype == slottype::X && s->count >= 1)
					{
						leftslotsempty = false;
						break;
					}
				}

				if (leftslotsempty)
				{
					return currentSlot;
				}
			}
		}
	}

	if (currentRound == "Y" && collectYstate)
	{
		string currentslotname = "";
		currentslotname += arr[0];
		int currentindex = FindIndexByLetter(currentslotname);
		int diceindex = 12 - diceval;
		currentslotname += arr[1];


		Slot* currentSlot = GetSlot(currentslotname);

		//E�er collect durumunda isek next olarak yine se�ti�imiz slotu g�nderiyoruz.
		if (currentSlot->stype == slottype::Y && currentSlot->count >= 1)
		{
			if (currentindex == diceindex)
			{
				return currentSlot;
			}

			//Olmamas� durumunda bir kontrol daha yap�yoruz.
			else if (diceindex < currentindex)
			{
				bool leftslotsempty = true;

				for (int k = currentindex - 1; k > 5; k--)
				{
					string slotn = FindLetterByIndex(k);
					slotn += "3";

					Slot* s = GetSlot(slotn);
					if (s->stype == slottype::Y && s->count >= 1)
					{
						leftslotsempty = false;
						break;
					}
				}

				if (leftslotsempty)
				{
					return currentSlot;
				}
			}
		}
	}


	//Saat y�n�ndeki hamleye g�re slot bulunacak.
	if (clockwise)
	{
		if (arr[1] == '1')	//1.katmanda ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex += diceval;
			//Eri�ilebilir slot.
			if (letterindex <= 11)
			{
				string slotname = FindLetterByIndex(letterindex);
				slotname += "1";

				return GetSlot(slotname);
			}

			else return nullptr;
		}

		else // 3.katman ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex -= diceval;
			if (letterindex < 0)		//De�er negatif ise �st katmana ge�mi� demektir ona g�re d�zenliyoruz.
			{
				letterindex *= -1;	//Pozitif hale getirdik.
				letterindex -= 1;
				
				string slotname = FindLetterByIndex(letterindex);
				slotname += "1";

				return GetSlot(slotname);
			}

			else //Hala pozitif durumda ise
			{
				string slotname = FindLetterByIndex(letterindex);
				slotname += "3";

				return GetSlot(slotname);
			}

		}
	}

	//Saat y�n�n�n tersi y�n�nde slot bulunacak.
	else
	{
		if (arr[1] == '1')	//1.katmanda ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex -= diceval;
			if (letterindex < 0)		//De�er negatif ise �st katmana ge�mi� demektir ona g�re d�zenliyoruz.
			{
				letterindex *= -1;	//Pozitif hale getirdik.
				letterindex -= 1;

				string slotname = FindLetterByIndex(letterindex);
				slotname += "3";

				return GetSlot(slotname);
			}

			else //Hala pozitif durumda ise
			{
				string slotname = FindLetterByIndex(letterindex);
				slotname += "1";

				return GetSlot(slotname);
			}
		}

		else // 3.katman ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex += diceval;
			//Eri�ilebilir slot.
			if (letterindex <= 11)
			{
				string slotname = FindLetterByIndex(letterindex);
				slotname += "3";

				return GetSlot(slotname);
			}

			else return nullptr;
		}

	}


	return nullptr;
}

void GameplayManager::MakeMovement(Slot* source, Slot* target)
{
	if (currentRound == "X")
	{

		//�lk olarak kaynak �zerine i�lemler yap.

		source->count--;
		if (source->name == "E2")	//Broken yerden ta� eksiliyorsa kontrol�.
		{
			brokenXnum--;
			if (source->count <= 0)
			{
				brokenXstate = false;
			}
		}

		//Normal slot ise.
		else
		{
			if (source->count <= 0)
			{
				source->stype = slottype::neutral;
			}
		}


		//�kinci olarak kaynak �zerinde yap�lan hamleler.

		if (source == target)	//Sadece ta� toplama durumunda ger�ekle�iyor.
		{
			collectedX++;
		}

		else if (target->stype == slottype::neutral && target->count <= 0)
		{
			target->stype = slottype::X;
			target->count = 1;
		}

		//Target y ise x'e d�nd�r�yoruz ve y'nin 1 ta��n� k�rm�� oluyoruz.
		else if (target->stype == slottype::Y)
		{
			target->stype = slottype::X;
			target->count = 1;
			brokenYnum++;
			brokenYstate = true;
		}

		else//Target de sadece x ise.
		{
			target->count++;
		}


		//En sonda da hamleleri yap�p slotlar� g�ncelledikten sonra,Oyuncu collectable duruma d��m��m� kontrol� yap�lacak.
		UpdateCollectableState();

	}

	if (currentRound == "Y")
	{
		//�lk olarak kaynak �zerine i�lemler yap.

		source->count--;
		if (source->name == "H2")	//Broken yerden ta� eksiliyorsa kontrol�.
		{
			brokenYnum--;
			if (source->count <= 0)
			{
				brokenYstate = false;
			}
		}

		//Normal slot ise.
		else
		{
			if (source->count <= 0)
			{
				source->stype = slottype::neutral;
			}
		}


		//�kinci olarak kaynak �zerinde yap�lan hamleler.

		if (source == target)	//Sadece ta� toplama durumunda ger�ekle�iyor.
		{
			collectedY++;
		}

		else if (target->stype == slottype::neutral && target->count <= 0)
		{
			target->stype = slottype::Y;
			target->count = 1;
		}

		//Target x ise y'e d�nd�r�yoruz ve y'nin 1 ta��n� k�rm�� oluyoruz.
		else if (target->stype == slottype::X)
		{
			target->stype = slottype::Y;
			target->count = 1;
			brokenXnum++;
			brokenXstate = true;
		}

		else//Target de sadece x ise.
		{
			target->count++;
		}

		//En sonda da hamleleri yap�p slotlar� g�ncelledikten sonra,Oyuncu collectable duruma d��m��m� kontrol� yap�lacak.
		UpdateCollectableState();
	}



}

int GameplayManager::FindIndexByLetter(string s)
{

	for (int i = 0; i < 12; i++)
	{
		if (letters[i] == s)
		{
			return i;
		}
	}

	return 0;
}

string GameplayManager::FindLetterByIndex(int index)
{
	string result = "-1";

	for (int i = 0; i < 12; i++)
	{
		if (i == index)
		{
			result = letters[i];
		}
	}



	return result;
}

void GameplayManager::PlayTheFirstRound()
{
	//E�er ilk oyun ise, oyuna kimin ba�lamas�n�n belirlenmesi i�in zarlar�n at�ld��� round olacak.

	//Bilgi verilecek, x ve y oyuncusu i�in zar at�lmas� istenecek.

	bool equal = true;
	int diceX = 0;
	int diceY = 0;

	while (equal)
	{
		srand((unsigned)time(0));
		 diceX = (rand() % 6) + 1;
		 diceY = (rand() % 6) + 1;

		 if (diceX == diceY) equal = true;
		 else equal = false;
	}

	//Log dosyas�nda yazd�rmak i�in.
	dice1 = diceX;
	dice2 = diceY;
	fmanager.WritePlayLog(this);

	cout << "Oyuna Kimin Baslayacaginin Belirlenmesi icin Tekli Zarlarin Atilmasi" << endl << endl;
	cout << "X oyuncusu icin atilan zar degeri = " << diceX << endl;
	cout << "Y oyuncusu icin atilan zar degeri = " << diceY << endl << endl;

	if (diceX > diceY)
	{
		cout << "-----Oyuna X oyuncusu baslayacak-----" << endl << endl;
		currentRound = "X";
	}	
	else
	{
		cout << "-----Oyuna Y oyuncusu baslayacak-----" << endl << endl;
		currentRound = "Y";
	}

	int x = 0;

	

	cout << "Devam icin etmek entera basiniz" << endl;
	cin.get();
}

void GameplayManager::DrawtheCurrentTable()
{

	
	cout << "Mevcut Oyun Tahtasi" << endl;
	cout << "----------------------------------------------" << endl;
	//slotlar�n bulundu�u listeden tek tek slotlar� al�p orada ki bilgilere g�re, tavla tahtas� �izilecek.
	//i sat�r say�s�
	for (int i = 0; i < 13; i++)
	{
		if (i == 0)		//ilk sat�r�n  yaz�lmas�
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

		if (i == 1 || i == 5 ||i == 8 ||i == 12)
		{
			for (int j = 0; j < 50; j++)
			{
				cout << "-";
			}
		} 

		if (i == 2 || i == 4 || i == 9 || i == 11)
		{
			for (int j = 0; j < 14; j++)
			{
				if (j == 6) cout << "|";
				else cout << "|   ";
			}
		}

		if (i == 6)
		{
			for (int j = 0; j < 12; j++)
			{
				switch (j) {
				case 4:
					cout << "|BX ";
					break;
				case 5:
					cout << "|D1 ";
					break;
				case 6:
					cout << "||D2 ";
					break;
				case 7:
					cout << "|BY ";
					break;
				case 11:
					cout << "|   |" << "--------> 2. katman(Kirilan taslar ve zar bilgilerinin gosterilmesi)";
					break;
				default:
					cout << "|   ";
					break;
				}
					
			}
		}

		if (i == 7)
		{
			for (int j = 0; j < 12; j++)
			{
				string info = GetSlotValueForBoard(letters[j] + "2");

				switch (j) {
				case 4:
					if (info == "-1") cout << "| 0 ";
					else cout <<"|" << info;		
					break;
				case 5:
					if (info == "-1") cout << "| 0 ";
					else cout << "|" << info;
					break;
				case 6:
					if (info == "-1") cout << "|| 0 ";
					else cout << "||" << info;
					break;
				case 7:
					if (info == "-1") cout << "| 0 ";
					else cout << "|" << info;
					break;
				case 11:
					cout << "|   |";
					break;
				default:
					cout << "|   ";
					break;
				}

			}
		}

		if (i == 3 || i == 10)
		{
			//1.katman m� 3. katmanm� onun belirlenmesi.
			string layer = "";
			if (i == 3) layer = "1";
			else layer = "3";


			//Ta�lar�n bulundu�u slotlar bilgi al�narak yaz�lacak.
			for (int j = 0; j < 12; j++)
			{

				if (j == 6)
				{
					

					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot bo�
					{
						cout << "||   ";
					}

					else cout << "||" << info;
				}

				else if (j == 11)
				{
					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot bo�
					{
						cout << "|   |" << "-------> " << layer << ". katman(Ornek = " << "A" << layer << ",B" << layer << ")";
					}

					else cout << "|" << info << "|" << "-------> " << layer << ". katman(Ornek = " << "A" << layer << ",B" << layer << ")";
					
				}

				else
				{
					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot bo�
					{
						cout << "|   ";
					}

					else cout << "|" << info;
				}
			}
		}

		cout << endl;
	}

}

void GameplayManager::UpdateCollectableState()
{
	if (currentRound == "X")
	{
		//Broken durum var ise ba�tan collect durumu olmayacak.
		if (brokenXstate)
		{
			collectXstate = false;
			return;
		}

		//Di�er durumda X'in kendi slotlar� d���nda ta�lar� var m� yok mu kontrol edilecek.

		bool outsiderock = false;

		//3.katman i�in
		for (int i = 0; i < 12; i++)
		{
			string slotname = letters[i];
			slotname += "3";

			Slot* currentslot = GetSlot(slotname);

			if (currentslot->stype == slottype::X && currentslot->count >= 1)
			{
				outsiderock = true;
				break;
			}
		}

		//1.katman i�in

		for (int i = 0; i < 6; i++)
		{
			string slotname = letters[i];
			slotname += "1";

			Slot* currentslot = GetSlot(slotname);

			if (currentslot->stype == slottype::X && currentslot->count >= 1)
			{
				outsiderock = true;
				break;
			}
		}


		if (outsiderock)
			collectXstate = false;
		else collectXstate = true;


	}

	if (currentRound == "Y")
	{
		//Broken durum var ise ba�tan collect durumu olmayacak.
		if (brokenYstate)
		{
			collectYstate = false;
			return;
		}

		//Di�er durumda X'in kendi slotlar� d���nda ta�lar� var m� yok mu kontrol edilecek.

		bool outsiderock = false;

		//1.katman i�in
		for (int i = 0; i < 12; i++)
		{
			string slotname = letters[i];
			slotname += "1";

			Slot* currentslot = GetSlot(slotname);

			if (currentslot->stype == slottype::Y && currentslot->count >= 1)
			{
				outsiderock = true;
				break;
			}
		}

		//3.katman i�in

		for (int i = 0; i < 6; i++)
		{
			string slotname = letters[i];
			slotname += "3";

			Slot* currentslot = GetSlot(slotname);

			if (currentslot->stype == slottype::Y && currentslot->count >= 1)
			{
				outsiderock = true;
				break;
			}
		}


		if (outsiderock)
			collectYstate = false;
		else collectYstate = true;

	}
}

void GameplayManager::RollTheDice()
{
	srand((unsigned)time(0));
	dice1 = (rand()%6) +1;
	dice2 = (rand()%6) + 1;
	//Zarlar� att�ktan sonra zar slotlar�n�n g�ncellenmesi.
	UpdateTheSlotInfo("F2", dice1, slottype::neutral);
	UpdateTheSlotInfo("G2", dice2, slottype::neutral);
}

int GameplayManager::CharToInt(char a)
{
	int x = a;
	x -= 48;

	return x;
}

bool GameplayManager::IsPlayerAbleToPlay(int *dicevalues)
{
	//�lk olarak broken durumda is oyuncu ona g�re kontrol yap�lacak.

	//X oyuncusu i�in broken durumda ise oynayacak yeri var m� yok mu kontrol edilecek. 
	if (currentRound == "X" && brokenXstate)
	{
		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int letterindex = 12 - dicevalues[i];
			string slotname = FindLetterByIndex(letterindex);
			slotname += "3";
			
			//Oynanacak uygun yer var m� kontrol ediliyor.
			if (GetSlot(slotname)->count <= 1)
			{
				slotable = true;
				break;
			}
		}

		if (!slotable) return false;
	}

	if (currentRound == "Y" && brokenYstate)
	{
		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int letterindex = 12 - dicevalues[i];
			string slotname = FindLetterByIndex(letterindex);
			slotname += "1";

			if (GetSlot(slotname)->count <= 1)
			{
				slotable = true;
				break;
			}
		}

		if (!slotable) return false;
	}

	//Daha sonra ta� toplama durumunda ve toplayacak ta�� yoksa kontrol yap�lacak ve ona g�re durum d�nd�r�lecek.

	if (currentRound == "X" && collectXstate)
	{

		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int diceval = dicevalues[-1];

			for (int j = 6; j < 12; j++)
			{
				string slotname = FindLetterByIndex(j);
				slotname += "1";

				Slot* currentslot = GetSlot(slotname);

				int slotcount = currentslot->count;

				if (slotcount >= 1 && currentslot ->stype == slottype::X)
				{
					int dicetoindex = 12 - diceval;
					if (dicetoindex == j)
					{
						slotable = true;
						break;
					}

					//Normal hamle kontrol� i�in.
					else 
					{
						char slotc[3];

						strcpy_s(slotc, slotname.c_str());

						Slot* targetSlot = CalculateNextSlot(slotc, diceval, true);

						if (targetSlot != nullptr)
						{
							if (targetSlot->stype == slottype::X)
							{
								slotable = true;
								break;
							}

							if (targetSlot->stype == slottype::Y && targetSlot->count <= 1)
							{
								slotable = true;
								break;
							}
						}

						//Target null durumunda bir kontrol daha yap�lmal� sol taraf�n bo� olup olmamas� ile ilgili.
						else
						{
							if (diceval < j)
							{
								bool leftslotsempty = true;

								for (int k = j - 1; k > 5; k--)
								{
									string currentslotname = "";
									currentslotname += FindLetterByIndex(k);
									currentslotname += "1";

									Slot* currentslot = GetSlot(currentslotname);
									if (currentslot->stype == slottype::X && currentslot->count >= 1)
										leftslotsempty = false;
								}
								if (leftslotsempty)
								{
									slotable = true;
									break;
								}

							}


						}

					}

				}

			}


		}

		if (!slotable) return false;
	}

	
	if (currentRound == "Y" && collectYstate)
	{

		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int diceval = dicevalues[-1];

			for (int j = 6; j < 12; j++)
			{
				string slotname = FindLetterByIndex(j);
				slotname += "3";

				Slot* currentslot = GetSlot(slotname);

				int slotcount = currentslot->count;

				if (slotcount >= 1 && currentslot->stype == slottype::Y)
				{
					int dicetoindex = 12 - diceval;
					if (dicetoindex == j)
					{
						slotable = true;
						break;
					}

					//Normal hamle kontrol� i�in.
					else
					{
						char slotc[3];

						strcpy_s(slotc, slotname.c_str());

						Slot* targetSlot = CalculateNextSlot(slotc, diceval, false);

						if (targetSlot != nullptr)
						{
							if (targetSlot->stype == slottype::Y)
							{
								slotable = true;
								break;
							}

							if (targetSlot->stype == slottype::X && targetSlot->count <= 1)
							{
								slotable = true;
								break;
							}
						}

						//Target null durumunda bir kontrol daha yap�lmal� sol taraf�n bo� olup olmamas� ile ilgili.
						else
						{
							if (diceval < j)
							{
								bool leftslotsempty = true;

								for (int k = j - 1; k > 5; k--)
								{
									string currentslotname = "";
									currentslotname += FindLetterByIndex(k);
									currentslotname += "3";

									Slot* currentslot = GetSlot(currentslotname);
									if (currentslot->stype == slottype::Y && currentslot->count >= 1)
										leftslotsempty = false;
								}
								if (leftslotsempty)
								{
									slotable = true;
									break;
								}

							}


						}

					}

				}

			}


		}

		if (!slotable) return false;
	}


	//Normal oyun gidi�at�nda yap�lan kontrol.
	if (currentRound == "X" && !brokenXstate && !collectXstate)
	{
		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int diceval = dicevalues[i];

			for (int k = 0; k < 12; k++)
			{
				//1.katman kontrol.
				string slotname = letters[k];
				slotname += "1";
				
				//�lk olarak var olan slotu al�yoruz.
				Slot* currentslot = GetSlot(slotname);
				//Slotta x i�in ta� var m� kontrol ediyoruz.
				if (currentslot->stype == slottype::X && currentslot->count > 0)
				{
					//E�er ta� var ise, kontrol etti�imiz zar i�in oynanabilir olup olmad��na bak�yoruz.

					char slotc[3];

					strcpy_s(slotc, slotname.c_str());

					Slot* targetSlot = CalculateNextSlot(slotc, diceval, true);

					if (targetSlot != nullptr)
					{
						if (targetSlot->stype == slottype::X || targetSlot->stype == slottype::neutral)
						{
							slotable = true;
							break;
						}

						if (targetSlot->stype == slottype::Y && targetSlot->count <= 1)
						{
							slotable = true;
							break;
						}
						
					}
				}

				//2.katman kontrol.

				string slotname2 = letters[k];
				slotname2 += "3";

				//�lk olarak var olan slotu al�yoruz.
				Slot* currentslot2 = GetSlot(slotname2);
				//Slotta x i�in ta� var m� kontrol ediyoruz.
				if (currentslot2->stype == slottype::X && currentslot2->count > 0)
				{
					//E�er ta� var ise, kontrol etti�imiz zar i�in oynanabilir olup olmad��na bak�yoruz.

					char slotc[3];

					strcpy_s(slotc, slotname2.c_str());

					Slot* targetSlot = CalculateNextSlot(slotc, diceval, true);

					if (targetSlot != nullptr)
					{
						if (targetSlot->stype == slottype::X || targetSlot->stype == slottype::neutral)
						{
							slotable = true;
							break;
						}

						if (targetSlot->stype == slottype::Y && targetSlot->count <= 1)
						{
							slotable = true;
							break;
						}
					}
				}
			}
		}

		if (!slotable) return false;
	}


	if (currentRound == "Y" && !brokenXstate && !collectXstate)
	{
		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int diceval = dicevalues[i];

			for (int k = 0; k < 12; k++)
			{
				//1.katman kontrol.
				string slotname = letters[k];
				slotname += "1";

				//�lk olarak var olan slotu al�yoruz.
				Slot* currentslot = GetSlot(slotname);
				//Slotta Y i�in ta� var m� kontrol ediyoruz.
				if (currentslot->stype == slottype::Y && currentslot->count > 0)
				{
					//E�er ta� var ise, kontrol etti�imiz zar i�in oynanabilir olup olmad��na bak�yoruz.

					char slotc[3];

					strcpy_s(slotc, slotname.c_str());

					Slot* targetSlot = CalculateNextSlot(slotc, diceval, false);

					if (targetSlot != nullptr)
					{
						if (targetSlot->stype == slottype::Y ||targetSlot ->stype == slottype::neutral)
						{
							slotable = true;
							break;
						}

						if (targetSlot->stype == slottype::X && targetSlot->count <= 1)
						{
							slotable = true;
							break;
						}
					}
				}

				//2.katman kontrol.

				string slotname2 = letters[k];
				slotname2 += "3";

				//�lk olarak var olan slotu al�yoruz.
				Slot* currentslot2 = GetSlot(slotname2);
				//Slotta x i�in ta� var m� kontrol ediyoruz.
				if (currentslot2->stype == slottype::Y && currentslot2->count > 0)
				{
					//E�er ta� var ise, kontrol etti�imiz zar i�in oynanabilir olup olmad��na bak�yoruz.

					char slotc[3];

					strcpy_s(slotc, slotname2.c_str());

					Slot* targetSlot = CalculateNextSlot(slotc, diceval, false);

					if (targetSlot != nullptr)
					{
						if (targetSlot->stype == slottype::Y || targetSlot->stype == slottype::neutral)
						{
							slotable = true;
							break;
						}

						if (targetSlot->stype == slottype::X && targetSlot->count <= 1)
						{
							slotable = true;
							break;
						}
					}
				}
			}
		}

		if (!slotable) return false;
	}

	return true;
}


