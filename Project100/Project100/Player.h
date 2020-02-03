#pragma once
#include "Mecro.h"
#include "MapDraw.h"

class Player
{
private:
	typedef struct {
		int m_ix;
		int m_iy;
	}Rock;
	Rock *rock;
	int m_iWinCheckCount;
	int m_iRockNum;
	string m_sTeamname;
	string m_sRockshape;
	int m_iUndoNum;
	bool m_bMyTurn;
	MapDraw MapDrawManager;
public:
	Player();
	Player(int, int);
	~Player();
	bool PlayerWin();
	void ResetRock(int, int);
	void InputRock(int, int);
	void DrawRock(int, int);
	bool UndoTurn();
	bool SeekRock(int, int);
	void SeekLastRock(int*, int*);
	void DrawAllRocks();
	bool WinCheck(int, int);
	inline string GetRockShape(){ return m_sRockshape; }
	inline void SetRockShape(string Shape){ m_sRockshape = Shape; }
	inline int GetUndoNum(){ return m_iUndoNum; }
	inline void SetUndoNum(int count){ m_iUndoNum = count; }
	inline string GetTeamName(){ return m_sTeamname; }
	inline void SetTeamName(string name){ m_sTeamname = name; }
	inline bool GetMyTurn(){ return m_bMyTurn; }
	inline void SetMyTurn(bool turn){ m_bMyTurn = turn; }
	void GetRocks(int, int*, int*);
};

