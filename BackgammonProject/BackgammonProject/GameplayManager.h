#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Slot.h"
#include "FileManager.h"

using namespace std;

class GameplayManager
{
public :
	void StartGame();
	void GameLogic();
	void CreateSlots();
	void UpdateSlotsForNewGame();
	void WriteRoundInfo();
	bool IsMovementCorrect(const char*arr,string *returnmessage,int diceval);
	bool IsInputSyntaxCorrect(const char* arr);
	void TakePlayerInputs();
	void UpdateTheSlotInfo(string name, int count, slottype t);
	string GetSlotValueForBoard(string slotname);
	Slot* GetSlot(string slotname);
	Slot* CalculateNextSlot(const char* arr,int val,bool clockwise);
	void MakeMovement(Slot* source, Slot* target);
	int FindIndexByLetter(string s);
	string FindLetterByIndex(int index);
	void PlayTheFirstRound();
	void DrawtheCurrentTable();
	void UpdateCollectableState();
	void RollTheDice();
	int CharToInt(char a);
	bool IsPlayerAbleToPlay(int *dicevalues);
	// We need reference to filemanager for reading and writing info.
	FileManager fmanager;
	string letters[12] = { "A","B","C","D","E","F","G","H","I","J","K","L" };
	vector <Slot> slots;
	int roundCount = 0;
	int dice1, dice2 = 0;
	int brokenXnum, brokenYnum = 0;
	int collectedX, collectedY = 0;
	bool brokenXstate, brokenYstate = false;
	bool collectXstate, collectYstate = false;
	bool gameOn = true;
	bool savedgame = false;
	string currentRound = "X";
	//Slot listesi olacak.
	//Oyun tahtasýný çizen bir fonksiyon olacak.
};

