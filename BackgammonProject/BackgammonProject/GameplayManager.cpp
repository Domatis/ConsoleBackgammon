#include "GameplayManager.h"
#include <stdlib.h> 
#include <ctime>
#include <cstdlib>
#include "FileManager.h"


void GameplayManager::StartGame()
{

	CreateSlots();

	//Oyun baþýnda kayýtlý oynamak mý yoksa yeni oyuna baþlamak mý istediði kullanýcýya sorulacak verilen cevaba göre iþlemler yapýlacak.

	char arr[2];

	cout << "Eger yeni oyuna baslamak istiyorsaniz Entera basip devam edin, Kayitli oyundan devam etmek icin S yazip devam edin" << endl;

	cin.get(arr, 2);
	cin.clear();
	cin.ignore(256, '\n');
	arr[0] = toupper(arr[0]);

	if (arr[0] == 'S')
	{
		//TODO burada extra kontrol yapýlmalý eðer kayýtlý oyun dosyasý yok ise yeni oyuna geçiþ yapýlmalý.

		bool isfileOpened = false;

		//Kayitli oyun için ayarlar yapýlacak.
		

		fmanager.LoadTheGame(this,isfileOpened);

		if (isfileOpened)
		{
			savedgame = true;
		}

		else       //Yeni oyun için ayarlar yapýlacak.
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
		//Yeni oyun için ayarlar yapýlacak.
		savedgame = false;
		UpdateSlotsForNewGame();
		fmanager.StartSettings();
	}


	system("CLS");
	
	//Burasý oyun baþlangýcýnda oyun tahtasýnýn düzenlenmesi.
	fmanager.WriteTableDat(this);
	
	GameLogic();

}

void GameplayManager::GameLogic()
{
	gameOn = true;

	//Bunu baþlatmadan önce ilk oyun mu saved oyun mu kontrol et sonra baþlat.
	if(!savedgame)	PlayTheFirstRound();
	

	while (gameOn)
	{

		roundCount++;
		//Oyun döngüsü burada olacak.
		system("CLS");
		//Oyun kazanýlmasýnýn kontrolü.
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

		//Her el baþýnda zar attýr oyunun mevcut durumu, zar bilgisi ve oyunun kimde olduðu bilgisini ver.
		RollTheDice();
	
		//Oyuncudan hamle için bilgi alýnmasý.
		TakePlayerInputs();		

		//Kayýt iþlemleri
		fmanager.WriteTableDat(this);
		fmanager.WritePlayLog(this);

		if (currentRound == "X") currentRound = "Y";
		else currentRound = "X";

		//oyundan çýkma seçeneði
		char arr[2];
		cout << "Bir sonraki tura gecmek icin entera, Oyundan cikmak icin Y tusuna basip devam ediniz" << endl;
		cin.get(arr,2);
		arr[0] = toupper(arr[0]);
		if (arr[0] == 'Y')
		{
			//Oyunu kaydetme iþlemleri.
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
		//Her harf için alt ve üst olmak üzere 2 tane oluþtur ve listeye ekle.
		Slot slot1(letters[i] + "1", 0, slottype::neutral,1);
		Slot slot2(letters[i] + "3", 0, slottype::neutral,3);

		slots.push_back(slot1);
		slots.push_back(slot2);
	}

	// 24 tane normal slotlar için oluþturduktan sonra 4 tane de orta slot için.
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
	char diceinputs[4][5];			//  Max 2 ve 4 hamle olabilir ona göre oluþturulmuþ dizi.
	int dicevalues[4];
	int movementNum = 0;

	//Zarlarýn eþit ve 4 hamle oynanabileceði durum.
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

		//Konsola inputlarýn giriþi ile bilgi verilecek.
		cout << "--X oyuncusunun taslari saat yonunde, Y oyuncusunun taslari saat yonunun tersi yonunde hareket edebilir, hamlelerinizi buna gore giriniz--" << endl;
		cout << "--Hamleler 'A1 5','B3 4' ve kirik taslar icin 'E2 5', 'H2 3' seklinde olabilir--" << endl;
		//TODO Mevcut olan oyundan çýkmak için ayrý bir seçenek olacak.

		//Her hamle baþýnda eðer oyuncunun oynayacak hamlesi yok ise tur bitirilecek(Örnek broken durumda ve oynanacak yeri yok veya taþlarý toplarken oynanacak yeri yok).
		bool abletoplay = IsPlayerAbleToPlay(dicevalues);
		if (!abletoplay)
		{
			
			cout << "Oynanacak hamle kalmadi bir sonraki tura gecmek icin entera basin" << endl;
			cin.ignore();
			cin.get();
			break;
		}

		//Hamle için girilen inputlarýn ilk olarak syntax kontrolü olacak.
		
		while (!syntaxCorrect || !moveCorrect || !dicevalCorrect)
		{
			//Zar için syntax control döngüsü.
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

			//Hamle kontrolü.

			string errorMessage = "";
			int selecteddiceval = CharToInt(diceinputs[i][3]);

			//Seçilen zar deðeri , atýlan zarlardan biriyle eþleþiyor mu onun kontrolü.

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

			//Movement kontrolü.
			moveCorrect = IsMovementCorrect(diceinputs[i], &errorMessage,selecteddiceval);
			if (!moveCorrect)
			{
				cout << errorMessage << endl;
				continue;
			}
				
			//Eðer hamlelerde sorun yok ise çalýþacak kýsým.

			// Verilen zar deðeri için hamle yapýlmýþsa o zar deðerini zar deðerleri dizisinden çýkarýlmasý.
			for (int k = 0; k < 4; k++)
			{
				if (selecteddiceval == dicevalues[k])
				{
					dicevalues[k] = -1;
					break;
				}
			}

			//Hem syntax hem movement doðru ise hamlenin yapýlmasý ve slot deðerlerinin güncellenmesi.
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

			//Broken slotlarýný brokensayilarla güncelle.
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

		//Eðer broken x durumu var ise, kaynak slot E2 olmalýdýr, deðil ise error mesajý ile döndürülür.
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

		//Ýlk kontrol hamle yapýlmasý istenen yerde oyuncuya ait taþ var mý yok mu kontrol edilmesi, yok ise false döndürülmesi ve hata mesajýnýn bu duruma göre güncellenmesi.
		if (currentslot->stype != slottype::X)
		{
			*returnmessage = "Oynanmasini istediginiz yerde tasiniz bulunmamaktadir";
			return false;
		}

		nextSlot = CalculateNextSlot(arr, diceval, true);	//Seçilen slot ve oynanan zar için bir sonraki slotun bulunmasý ve ardýndan kontrol edilmesi.

		//Eðer oyun tahtasýnýn dýþýna çýkýyorsa.
		if (nextSlot == nullptr)
		{
			*returnmessage = "Oynamak istediginiz hamle gecersizdir";
			return false;
		}

		//Seçilen yerde diðer oyuncunun 1 den fazla taþý var ise de oynanamaz.

		if (nextSlot->stype == slottype::Y && nextSlot->count > 1)
		{
			*returnmessage = "Tasinizin gidecegi yerde 1'den fazla rakip tas bulunmaktadir,hamle gecersizdir.";
			return false;
		}

		//Yukarýda ki durumlar yok ise hamle geçerli.
		return true;
	}



	// Y oyuncusu için geçerli olan durum.
	else
	{
		//Eðer broken x durumu var ise, kaynak slot E2 olmalýdýr, deðil ise error mesajý ile döndürülür.
		if (brokenYstate && currentslot->name != "H2")
		{
			*returnmessage = "Ýlk olarak kirilmis durumda tasinizi oynamak zorundasiniz";
			return false;
		}

		if (!brokenYstate && currentslot->name == "H2")
		{
			*returnmessage = "Kirilmis durumda tasinizi bulunmamaktadir,gecersiz hamle";
			return false;
		}

		//Ýlk kontrol hamle yapýlmasý istenen yerde oyuncuya ait taþ var mý yok mu kontrol edilmesi, yok ise false döndürülmesi ve hata mesajýnýn bu duruma göre güncellenmesi.
		if (currentslot->stype != slottype::Y)
		{
			*returnmessage = "Oynanmasini istediginiz yerde tasiniz bulunmamaktadir";
			return false;
		}

		nextSlot = CalculateNextSlot(arr, diceval, false);	//Seçilen slot ve oynanan zar için bir sonraki slotun bulunmasý ve ardýndan kontrol edilmesi.

		//Eðer oyun tahtasýnýn dýþýna çýkýyorsa.
		if (nextSlot == nullptr)
		{
			*returnmessage = "Oynamak istediginiz hamle gecersizdir";
			return false;
		}

		//Seçilen yerde diðer oyuncunun 1 den fazla taþý var ise de oynanamaz.

		if (nextSlot->stype == slottype::X && nextSlot->count > 1)
		{
			*returnmessage = "Tasinizin gidecegi yerde 1'den fazla rakip tas bulunmaktadir,hamle gecersizdir.";
			return false;
		}

		//Yukarýda ki durumlar yok ise hamle geçerli.
		return true;
	}


	//Diðer bir kontrol de taþýn oynanmak istendiði yer olacak.2 duruma göre uygun olmayacak, ilk durum ayný yerde diðer oyuncunun 2 ve daha fazla taþý olmasý.
	//2.durum ise oyun tahtasýnýn sýnýrlarýnda bir yere oynanmak istemesi olacak.
	//ilk olarak hamle yapýlacak oyuncunun hamle yerine göre kontroller 2 farklý þekilde olacak.



	*returnmessage = "";

	return true;
}

bool GameplayManager::IsInputSyntaxCorrect(const char  *arr)
{
	//Girilen inputun A1,B3 tarzýnda olup olmadýðýnýn kontrol edilip deðer döndürülmesi.
	bool state = false;


	//Zar deðeri için sadece 1 ile 6 dýþýnda mý deðil mi kontrolü yapýlýyor.
	int dicenum = CharToInt(arr[3]);

	if (dicenum < 1 || dicenum > 6)
	{
		state = false;
		return state;
	}

	//Broken zarlarýn oynanmasý için slotlar.
	if (arr[1] == 2 && (arr[0] != 'E' && arr[0] != 'H'))
	{
		state = false;
		return state;
	}

	//Ýlk önce 2.deðer 1 veya 3 deðilse return false döndürülecek.
	if (arr[1] != '1' && arr[1] != '3' && arr[1] != '2')
	{
		state = false;
		return state;
	}

	//Char harfin stringe dönmesi.
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
	//Var olan listeden name ile uygun olaný bulup verilen bilgilerle güncellenmesi.
	for (int i = 0; i < slots.size(); i++)
	{
		//Aradýðýmýz slotu bulduk.
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
		//Aradýðýmýz slotu bulduk.
		if (slots.at(i).name == slotname)
		{
			if (slots.at(i).count <= 0)		//Bulunan slotta herhangi bir taþ yok.
			{
				return result;
			}

			else //bulunan slotta taþ var türü belirlenip uygun deðer döndürülecek.
			{
				if (slots.at(i).stype == slottype::X && (slots.at(i).layer == 1 || slots.at(i).layer == 3))		//X taþý var.
				{
					result = to_string(slots.at(i).count) + "X ";
					return result;
				}

				else if (slots.at(i).stype == slottype::Y && (slots.at(i).layer == 1 || slots.at(i).layer == 3) )	//Y taþý var.
				{
					result = to_string(slots.at(i).count) + "Y ";
					return result;
				}


				else if (slots.at(i).stype == slottype::neutral || slots.at(i).layer == 2)	//Nötr taþ.
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
	//Broken durumu var ise hedef slot seçimi farklý yapýlacak ve döndürülecek.

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

		//Eðer collect durumunda isek next olarak yine seçtiðimiz slotu gönderiyoruz.
		if (currentSlot->stype == slottype::X && currentSlot ->count >=1)
		{
			if (currentindex == diceindex)
			{
				return currentSlot;
			}

			//Olmamasý durumunda bir kontrol daha yapýyoruz.
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

		//Eðer collect durumunda isek next olarak yine seçtiðimiz slotu gönderiyoruz.
		if (currentSlot->stype == slottype::Y && currentSlot->count >= 1)
		{
			if (currentindex == diceindex)
			{
				return currentSlot;
			}

			//Olmamasý durumunda bir kontrol daha yapýyoruz.
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


	//Saat yönündeki hamleye göre slot bulunacak.
	if (clockwise)
	{
		if (arr[1] == '1')	//1.katmanda ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex += diceval;
			//Eriþilebilir slot.
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
			if (letterindex < 0)		//Deðer negatif ise üst katmana geçmiþ demektir ona göre düzenliyoruz.
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

	//Saat yönünün tersi yönünde slot bulunacak.
	else
	{
		if (arr[1] == '1')	//1.katmanda ise.
		{
			string currentLetter(1, arr[0]);
			int letterindex = FindIndexByLetter(currentLetter);
			letterindex -= diceval;
			if (letterindex < 0)		//Deðer negatif ise üst katmana geçmiþ demektir ona göre düzenliyoruz.
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
			//Eriþilebilir slot.
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

		//Ýlk olarak kaynak üzerine iþlemler yap.

		source->count--;
		if (source->name == "E2")	//Broken yerden taþ eksiliyorsa kontrolü.
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


		//Ýkinci olarak kaynak üzerinde yapýlan hamleler.

		if (source == target)	//Sadece taþ toplama durumunda gerçekleþiyor.
		{
			collectedX++;
		}

		else if (target->stype == slottype::neutral && target->count <= 0)
		{
			target->stype = slottype::X;
			target->count = 1;
		}

		//Target y ise x'e döndürüyoruz ve y'nin 1 taþýný kýrmýþ oluyoruz.
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


		//En sonda da hamleleri yapýp slotlarý güncelledikten sonra,Oyuncu collectable duruma düþmüþmü kontrolü yapýlacak.
		UpdateCollectableState();

	}

	if (currentRound == "Y")
	{
		//Ýlk olarak kaynak üzerine iþlemler yap.

		source->count--;
		if (source->name == "H2")	//Broken yerden taþ eksiliyorsa kontrolü.
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


		//Ýkinci olarak kaynak üzerinde yapýlan hamleler.

		if (source == target)	//Sadece taþ toplama durumunda gerçekleþiyor.
		{
			collectedY++;
		}

		else if (target->stype == slottype::neutral && target->count <= 0)
		{
			target->stype = slottype::Y;
			target->count = 1;
		}

		//Target x ise y'e döndürüyoruz ve y'nin 1 taþýný kýrmýþ oluyoruz.
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

		//En sonda da hamleleri yapýp slotlarý güncelledikten sonra,Oyuncu collectable duruma düþmüþmü kontrolü yapýlacak.
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
	//Eðer ilk oyun ise, oyuna kimin baþlamasýnýn belirlenmesi için zarlarýn atýldýðý round olacak.

	//Bilgi verilecek, x ve y oyuncusu için zar atýlmasý istenecek.

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

	//Log dosyasýnda yazdýrmak için.
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
			//1.katman mý 3. katmanmý onun belirlenmesi.
			string layer = "";
			if (i == 3) layer = "1";
			else layer = "3";


			//Taþlarýn bulunduðu slotlar bilgi alýnarak yazýlacak.
			for (int j = 0; j < 12; j++)
			{

				if (j == 6)
				{
					

					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot boþ
					{
						cout << "||   ";
					}

					else cout << "||" << info;
				}

				else if (j == 11)
				{
					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot boþ
					{
						cout << "|   |" << "-------> " << layer << ". katman(Ornek = " << "A" << layer << ",B" << layer << ")";
					}

					else cout << "|" << info << "|" << "-------> " << layer << ". katman(Ornek = " << "A" << layer << ",B" << layer << ")";
					
				}

				else
				{
					string info = GetSlotValueForBoard(letters[j] + layer);
					if (info == "-1") //Slot boþ
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
		//Broken durum var ise baþtan collect durumu olmayacak.
		if (brokenXstate)
		{
			collectXstate = false;
			return;
		}

		//Diðer durumda X'in kendi slotlarý dýþýnda taþlarý var mý yok mu kontrol edilecek.

		bool outsiderock = false;

		//3.katman için
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

		//1.katman için

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
		//Broken durum var ise baþtan collect durumu olmayacak.
		if (brokenYstate)
		{
			collectYstate = false;
			return;
		}

		//Diðer durumda X'in kendi slotlarý dýþýnda taþlarý var mý yok mu kontrol edilecek.

		bool outsiderock = false;

		//1.katman için
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

		//3.katman için

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
	//Zarlarý attýktan sonra zar slotlarýnýn güncellenmesi.
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
	//Ýlk olarak broken durumda is oyuncu ona göre kontrol yapýlacak.

	//X oyuncusu için broken durumda ise oynayacak yeri var mý yok mu kontrol edilecek. 
	if (currentRound == "X" && brokenXstate)
	{
		bool slotable = false;

		for (int i = 0; i < 4; i++)
		{
			if (dicevalues[i] == -1) continue;

			int letterindex = 12 - dicevalues[i];
			string slotname = FindLetterByIndex(letterindex);
			slotname += "3";
			
			//Oynanacak uygun yer var mý kontrol ediliyor.
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

	//Daha sonra taþ toplama durumunda ve toplayacak taþý yoksa kontrol yapýlacak ve ona göre durum döndürülecek.

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

					//Normal hamle kontrolü için.
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

						//Target null durumunda bir kontrol daha yapýlmalý sol tarafýn boþ olup olmamasý ile ilgili.
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

					//Normal hamle kontrolü için.
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

						//Target null durumunda bir kontrol daha yapýlmalý sol tarafýn boþ olup olmamasý ile ilgili.
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


	//Normal oyun gidiþatýnda yapýlan kontrol.
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
				
				//Ýlk olarak var olan slotu alýyoruz.
				Slot* currentslot = GetSlot(slotname);
				//Slotta x için taþ var mý kontrol ediyoruz.
				if (currentslot->stype == slottype::X && currentslot->count > 0)
				{
					//Eðer taþ var ise, kontrol ettiðimiz zar için oynanabilir olup olmadýðna bakýyoruz.

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

				//Ýlk olarak var olan slotu alýyoruz.
				Slot* currentslot2 = GetSlot(slotname2);
				//Slotta x için taþ var mý kontrol ediyoruz.
				if (currentslot2->stype == slottype::X && currentslot2->count > 0)
				{
					//Eðer taþ var ise, kontrol ettiðimiz zar için oynanabilir olup olmadýðna bakýyoruz.

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

				//Ýlk olarak var olan slotu alýyoruz.
				Slot* currentslot = GetSlot(slotname);
				//Slotta Y için taþ var mý kontrol ediyoruz.
				if (currentslot->stype == slottype::Y && currentslot->count > 0)
				{
					//Eðer taþ var ise, kontrol ettiðimiz zar için oynanabilir olup olmadýðna bakýyoruz.

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

				//Ýlk olarak var olan slotu alýyoruz.
				Slot* currentslot2 = GetSlot(slotname2);
				//Slotta x için taþ var mý kontrol ediyoruz.
				if (currentslot2->stype == slottype::Y && currentslot2->count > 0)
				{
					//Eðer taþ var ise, kontrol ettiðimiz zar için oynanabilir olup olmadýðna bakýyoruz.

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


