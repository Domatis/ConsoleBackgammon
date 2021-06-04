#pragma once
class GameplayManager;

class FileManager
{
public :
	void WriteTableDat(GameplayManager* manager);
	void WritePlayLog(GameplayManager* manager);
	void WriteGameInfos(GameplayManager* manager);
	void WriteSlotInfos(GameplayManager* manager);

	void LoadTheGame(GameplayManager* manager,bool& state);
	void ReadSlotInfos(GameplayManager* manager);
	void ReadGameInfos(GameplayManager* manager);
	
	void EvaluateGameInfos(int index, char* arr, GameplayManager* manager);

	void StartSettings();
};

