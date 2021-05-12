#pragma once
#include <vector>
#include <string>
#include "Slot.h"
using namespace std;

class GameplayManager
{
public :
	void StartGame();
	void GameLogic();
	void CreateSlots();
	void UpdateSlotsForNewGame();
	void UpdateSlotsForSavedGame();
	void UpdateTheSlotInfo(string name, int count, slottype t);
	void PlayTheFirstRound();
	void DrawtheCurrentTable();
	// We need reference to filemanager for reading and writing info.
	string letters[12] = { "A","B","C","D","E","F","G","H","I","J","K","L" };
	vector <Slot> slots;
	int roundCount = 0;
	int dice1, dice2 = 0;
	string currentRound = "X";
	//Slot listesi olacak.
	//Oyun tahtasýný çizen bir fonksiyon olacak.
};

