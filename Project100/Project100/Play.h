#pragma once
#include "Mecro.h"
#include "MapDraw.h"
#include "Player.h"

enum BUTTON
{
	BUTTON_LEFT = 'a',
	BUTTON_RIGHT = 'd',
	BUTTON_UP = 'w',
	BUTTON_DOWN = 's',
	BUTTON_OPTION = 'p',
	BUTTON_UNDO = 'n',
	BUTTON_ENTER = 13,
	BUTTON_ESC = 27
};

class Play
{
private:
	MapDraw m_MapDrawManager;
	Player* P1;
	Player* P2;
	int m_iWidth;
	int m_iHeight;
	int m_iTurnNum;
	int m_iTextStartYPos;
	int m_iSelectUndoCount;
	bool m_bIsPlaying;
	string m_sCursorShapeP1;
	string m_sCursorShapeP2;
	string m_sCurCursor;
	int m_iCursorX;
	int m_iCursorY;
	ofstream m_save;
	ifstream m_load;
public:
	Play();
	~Play();
	void PlayerSetting();
	void Reset();
	void PlayWin(Player* P);
	void Playing();
	void PressButton();
	void DrawPastMap();
	bool IsRockPos(); 
	void Init();
	void SwapTurn();
	void DrawIntro();
	void DrawOption();
	void DrawPlayMap();
	void OptionSetting();
	void SetResolution();
	void SetCursor();
	void SetStone();
	void SetUndoCount();
	void LimitSetOption();
	void RecordRock();
	void Replay();
};

