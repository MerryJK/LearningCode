#include "Play.h"

Play::Play()
{
	m_iWidth = 20;
	m_iHeight = 20;
	m_iSelectUndoCount = 5;
	m_bIsPlaying = false;
	P1 = new Player(m_iWidth, m_iHeight);
	P2 = new Player(m_iWidth, m_iHeight);
	P1->SetRockShape("○");
	P2->SetRockShape("●");
	P1->SetMyTurn(true);
	P2->SetMyTurn(false);
	m_sCursorShapeP1 = "○";
	m_sCursorShapeP2 = "●";
}

void Play::Init()
{
	int select;
	while (1) 
	{
		DrawIntro();
		cin >> select;
		switch (select)
		{
		case 1:
			PlayerSetting();
			Playing();
			break;
		case 2:
			m_load.open("savefile.txt");
			if (!m_load)
			{
				system("cls");
				m_MapDrawManager.DrawMidText("기록된 게임이 없습니다.", m_iWidth, m_iHeight * 0.5);
				m_MapDrawManager.gotoxy(0, m_iHeight);
				system("pause");
				break;
			}
			else
			{
				Replay();
			}
			m_load.close();
			break;
		case 3:
			OptionSetting();
			break;
		case 4:
			return;
		}
	}
}

void Play::PlayerSetting()
{
	string name;
	m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("P1 이름", m_iWidth, m_iHeight * 0.3);
	m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iHeight * 0.4);
	m_MapDrawManager.gotoxy(m_iWidth + 4, m_iHeight * 0.4);
	cin >> name;
	P1->SetTeamName(name);
	m_MapDrawManager.DrawMidText("P2 이름", m_iWidth, m_iHeight * 0.6);
	m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iHeight * 0.7);
	m_MapDrawManager.gotoxy(m_iWidth + 4, m_iHeight * 0.7);
	cin >> name;
	P2->SetTeamName(name);
}

void Play::Playing()
{
	m_bIsPlaying = true;
	m_iCursorX = m_iWidth / 2;
	m_iCursorY = m_iHeight / 2;
	m_MapDrawManager.GridDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("==== 조작키 ====", m_iWidth, m_iHeight + 1);
	m_MapDrawManager.DrawMidText("이동 : A,S,W,D 돌놓기 : ENTER", m_iWidth, m_iHeight + 2);
	m_MapDrawManager.DrawMidText("무르기 : N 옵션 : P 종료 : ESC", m_iWidth, m_iHeight + 3);
	while(m_bIsPlaying)
	{
		DrawPlayMap();
		PressButton();
		if (P1->PlayerWin())
		{
			RecordRock();
			PlayWin(P1);
		}
		else if (P2->PlayerWin())
		{
			RecordRock();
			PlayWin(P2);
		}
	}
}

void Play::PlayWin(Player* P)
{
	m_MapDrawManager.DrawMidText(P->GetTeamName() + "팀 승리!", m_iWidth, m_iHeight / 2);
	char ch = _getch();
	Reset();
}

void Play::PressButton()
{
	char ch = _getch();
	switch (ch)
	{
	case BUTTON_LEFT:
		if (m_iCursorX * 2 - 1 > 0)
		{
			DrawPastMap();
			m_iCursorX--;
		}
		break;
	case BUTTON_RIGHT:
		if (m_iCursorX + 1 < m_iWidth)
		{
			DrawPastMap();
			m_iCursorX++;
		}
		break;
	case BUTTON_UP:
		if (m_iCursorY > 0)
		{
			DrawPastMap();
			m_iCursorY--;
		}
		break;
	case BUTTON_DOWN:
		if (m_iCursorY + 1 < m_iHeight)
		{
			DrawPastMap();
			m_iCursorY++;
		}
		break;
	case BUTTON_ENTER:
		if (!(P1->SeekRock(m_iCursorX, m_iCursorY)) && !(P2->SeekRock(m_iCursorX, m_iCursorY)))
		{
			if (P1->GetMyTurn())
			{
				P1->InputRock(m_iCursorX, m_iCursorY);
				SwapTurn();
			}
			else
			{
				P2->InputRock(m_iCursorX, m_iCursorY);
				SwapTurn();
			}
			m_iTurnNum++;
		}
		break;
	case BUTTON_OPTION:
		OptionSetting();
		m_MapDrawManager.GridDraw(m_iWidth, m_iHeight);
		m_MapDrawManager.DrawMidText("==== 조작키 ====", m_iWidth, m_iHeight + 1);
		m_MapDrawManager.DrawMidText("이동 : A,S,W,D 돌놓기 : ENTER", m_iWidth, m_iHeight + 2);
		m_MapDrawManager.DrawMidText("무르기 : N 옵션 : P 종료 : ESC", m_iWidth, m_iHeight + 3);
		P1->DrawAllRocks();
		P2->DrawAllRocks();
		break;
	case BUTTON_UNDO:
		if (P1->GetMyTurn())
		{
			if (!P2->UndoTurn())
				return;
		}
		else
		{
			if (!P1->UndoTurn())
				return;
		}
		if (m_iTurnNum > 0)
		{
			m_iTurnNum--;
		}
		SwapTurn();
		m_MapDrawManager.GridDraw(m_iWidth, m_iHeight);
		P1->DrawAllRocks();
		P2->DrawAllRocks();
		break;
	case BUTTON_ESC:
		Reset();
		return;
		}
}

void Play::DrawPastMap()
{
	if (IsRockPos())
		return;
	if (m_iCursorX == 0 && m_iCursorY == 0)
		m_MapDrawManager.DrawPoint("┌", 0,0);
	if(m_iCursorX>0 && m_iCursorX < m_iWidth && m_iCursorY == 0)
		m_MapDrawManager.DrawPoint("┬", m_iCursorX, m_iCursorY);
	if(m_iCursorX + 1 == m_iWidth && m_iCursorY == 0)
		m_MapDrawManager.DrawPoint("┐", m_iCursorX, m_iCursorY);
	if(m_iCursorX == 0 && m_iCursorY > 0 && m_iCursorY < m_iHeight)
		m_MapDrawManager.DrawPoint("├", m_iCursorX, m_iCursorY);
	if(m_iCursorX == 0 && m_iCursorY +1 == m_iHeight) 
		m_MapDrawManager.DrawPoint("└", m_iCursorX, m_iCursorY);	
	if(m_iCursorX > 0 && m_iCursorX < m_iWidth && m_iCursorY > 0 && m_iCursorY < m_iHeight)
		m_MapDrawManager.DrawPoint("┼", m_iCursorX, m_iCursorY);
	if(m_iCursorX + 1 == m_iWidth && m_iCursorY > 0 && m_iCursorY < m_iHeight)
		m_MapDrawManager.DrawPoint("┤", m_iCursorX, m_iCursorY);
	if(m_iCursorX > 0 && m_iCursorX < m_iWidth && m_iCursorY + 1 == m_iHeight)
		m_MapDrawManager.DrawPoint("┴", m_iCursorX, m_iCursorY);
	if(m_iCursorX + 1 == m_iWidth && m_iCursorY + 1 == m_iHeight)
		m_MapDrawManager.DrawPoint("┘", m_iCursorX, m_iCursorY);
}

void Play::SwapTurn()
{
	if (P1->GetMyTurn())
	{
		P1->SetMyTurn(false);
		P2->SetMyTurn(true);
		P2->SeekLastRock(&m_iCursorX, &m_iCursorY);
	}
	else if (P2->GetMyTurn())
	{
		P1->SetMyTurn(true);
		P2->SetMyTurn(false);
		P1->SeekLastRock(&m_iCursorX, &m_iCursorY);
	}
}

void Play::OptionSetting()
{
	int select;
	while (1)
	{
		DrawOption();
		cin >> select;
		switch (select)
		{
		case 1:
			if (m_bIsPlaying)
			{
				LimitSetOption();
				break;
			}
			SetResolution();
			break;
		case 2:
			SetCursor();
			break;
		case 3:
			SetStone();
			break;
		case 4:
			if (m_bIsPlaying)
			{
				LimitSetOption();
				break;
			}
			SetUndoCount();
			break;
		case 5:
			return;
		}
	}
}

void Play::Reset()
{
	m_bIsPlaying = false;
	m_iTurnNum = 0;
	P1->SetUndoNum(m_iSelectUndoCount);
	P2->SetUndoNum(m_iSelectUndoCount);
	P1->SetMyTurn(true);
	P2->SetMyTurn(false);
	P1->ResetRock(m_iWidth, m_iHeight);
	P2->ResetRock(m_iWidth, m_iHeight);
	system("cls");
}

bool Play::IsRockPos()
{
	if (P1->SeekRock(m_iCursorX, m_iCursorY))
	{
		P1->DrawRock(m_iCursorX, m_iCursorY);
		return true;
	}
	else if (P2->SeekRock(m_iCursorX, m_iCursorY))
	{
		P2->DrawRock(m_iCursorX, m_iCursorY);
		return true;
	}
	else
		return false;
}

void Play::SetResolution()
{
	string strWidth = "Width : ";
	string strHeight = "Height : ";
	int selectWidth;
	int selectHeight;
	char c;
	while (1)
	{
		system("cls");
		m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
		m_iTextStartYPos = m_iHeight * 0.4;
		m_MapDrawManager.DrawMidText(strWidth, m_iWidth, m_iTextStartYPos);
		m_MapDrawManager.gotoxy(m_iWidth + strWidth.size()/2, m_iTextStartYPos);
		cin >> selectWidth;
		m_MapDrawManager.DrawMidText(strHeight, m_iWidth, m_iTextStartYPos *1.5);
		m_MapDrawManager.gotoxy(m_iWidth + strHeight.size()/2, m_iTextStartYPos *1.5);
		cin >> selectHeight;
		if (selectWidth > 90 || selectWidth < 20 || selectHeight > 45 || selectHeight < 20)
		{
			m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
			m_MapDrawManager.DrawMidText("변경 불가능", m_iWidth, m_iTextStartYPos);
			m_MapDrawManager.DrawMidText("(가로 : 20 ~ 90, 세로 : 20 ~ 45)", m_iWidth, m_iTextStartYPos *1.4);
			m_MapDrawManager.gotoxy(0, m_iHeight + 1);
			system("pause");
		}
		else
		{
			m_iWidth = selectWidth;
			m_iHeight = selectHeight;
			P1->ResetRock(m_iWidth, m_iHeight);
			P2->ResetRock(m_iWidth, m_iHeight);
			return;
		}
	}
}

void Play::SetCursor()
{
	int select;
	m_iTextStartYPos = m_iHeight * 0.13;
	m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("= Set Cursor =", m_iWidth, m_iTextStartYPos);
	m_MapDrawManager.DrawMidText("1. ○, ●", m_iWidth, m_iTextStartYPos * 2);
	m_MapDrawManager.DrawMidText("2. ♡, ♥", m_iWidth, m_iTextStartYPos * 3);
	m_MapDrawManager.DrawMidText("3. ☞, ☜", m_iWidth, m_iTextStartYPos * 4);
	m_MapDrawManager.DrawMidText("4. ①, ②", m_iWidth, m_iTextStartYPos * 5);
	m_MapDrawManager.DrawMidText("5. Return", m_iWidth, m_iTextStartYPos * 6);
	m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iTextStartYPos * 7);
	m_MapDrawManager.gotoxy(m_iWidth + 4, m_iTextStartYPos * 7);
	cin >> select;
	switch (select)
	{
	case 1:
		m_sCursorShapeP1 = "○";
		m_sCursorShapeP2 = "●";
		break;
	case 2:
		m_sCursorShapeP1 = "♡";
		m_sCursorShapeP2 = "♥";
		break;
	case 3:
		m_sCursorShapeP1 = "☞";
		m_sCursorShapeP2 = "☜";
		break;
	case 4:
		m_sCursorShapeP1 = "①";
		m_sCursorShapeP2 = "②";
		break;
	case 5:
		return;
	default:
		break;
	}
}

void Play::SetStone()
{
	int select;
	m_iTextStartYPos = m_iHeight * 0.13;
	m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("= Set Cursor =", m_iWidth, m_iTextStartYPos);
	m_MapDrawManager.DrawMidText("1. ○, ●", m_iWidth, m_iTextStartYPos * 2);
	m_MapDrawManager.DrawMidText("2. ♡, ♥", m_iWidth, m_iTextStartYPos * 3);
	m_MapDrawManager.DrawMidText("3. ☏, ☎", m_iWidth, m_iTextStartYPos * 4);
	m_MapDrawManager.DrawMidText("4. ①, ②", m_iWidth, m_iTextStartYPos * 5);
	m_MapDrawManager.DrawMidText("5. Return", m_iWidth, m_iTextStartYPos * 6);
	m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iTextStartYPos * 7);
	m_MapDrawManager.gotoxy(m_iWidth + 4, m_iTextStartYPos * 7);
	cin >> select;
	switch (select)
	{
	case 1:
		P1->SetRockShape("○");
		P2->SetRockShape("●");
		break;
	case 2:
		P1->SetRockShape("♡");
		P2->SetRockShape("♥");
		break;
	case 3:
		P1->SetRockShape("☏");
		P2->SetRockShape("☎");
		break;
	case 4:
		P1->SetRockShape("①");
		P2->SetRockShape("②");
		break;
	case 5:
		return;
	default:
		break;
	}
}

void Play::SetUndoCount()
{
	int select;
	while (1)
	{
		m_iTextStartYPos = m_iHeight * 0.14;
		m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
		m_MapDrawManager.DrawMidText("= Set Undo =", m_iWidth, m_iTextStartYPos);
		m_MapDrawManager.DrawMidText("1. Set Undo Count", m_iWidth, m_iTextStartYPos * 2);
		m_MapDrawManager.DrawMidText("2. Undo Off", m_iWidth, m_iTextStartYPos * 3);
		m_MapDrawManager.DrawMidText("3. Return", m_iWidth, m_iTextStartYPos * 4);
		m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iTextStartYPos * 5);
		m_MapDrawManager.gotoxy(m_iWidth + 4, m_iTextStartYPos * 5);
		cin >> select;
		switch (select)
		{
		case 1:
			while (1)
			{
				system("cls");
				m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
				m_MapDrawManager.DrawMidText("무르기 횟수 입력<최대 10회> : ", m_iWidth, m_iHeight * 0.5);
				m_MapDrawManager.gotoxy(m_iWidth  * 1.8, m_iHeight * 0.5);
				cin >> m_iSelectUndoCount;
				if (m_iSelectUndoCount >= 0 && m_iSelectUndoCount <= 10)
				{
					P1->SetUndoNum(m_iSelectUndoCount);
					P2->SetUndoNum(m_iSelectUndoCount);
					break;
				}
				else
				{
					m_MapDrawManager.DrawMidText("범위가 맞지 않습니다 < 0 ~ 10 >", m_iWidth, m_iHeight * 0.6);
					m_MapDrawManager.gotoxy(0, m_iHeight +2);
					system("pause");
				}
			}
			break;
		case 2:
			system("cls");
			m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
			m_MapDrawManager.DrawMidText("무르기 Off", m_iWidth, m_iHeight * 0.5);
			P1->SetUndoNum(0);
			P2->SetUndoNum(0);
			m_MapDrawManager.gotoxy(0, m_iHeight + 2);
			system("pause");
			break;
		case 3:
			return;
		default:
			break;
		}
	}
}

void Play::LimitSetOption()
{
	system("cls");
	m_iTextStartYPos = m_iHeight * 0.4;
	m_MapDrawManager.DrawMidText("접근 불가능", m_iWidth, m_iTextStartYPos);
	m_MapDrawManager.DrawMidText("<Game Play중>", m_iWidth, m_iTextStartYPos * 1.2);
	m_MapDrawManager.gotoxy(0, m_iHeight + 1);
	system("pause");
}

void Play::DrawIntro()
{
	m_iTextStartYPos = m_iHeight * 0.25;
	m_MapDrawManager.GridDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("★ 오  목 ★", m_iWidth, m_iTextStartYPos);
	m_MapDrawManager.DrawMidText("1. 게임 시작", m_iWidth, m_iTextStartYPos + 2);
	m_MapDrawManager.DrawMidText("2. Replay", m_iWidth, m_iTextStartYPos + 4);
	m_MapDrawManager.DrawMidText("3. 옵션 설정", m_iWidth, m_iTextStartYPos + 6);
	m_MapDrawManager.DrawMidText("4. 종료", m_iWidth, m_iTextStartYPos + 8);
	m_MapDrawManager.BoxDraw(m_iWidth / 2, 3, m_iWidth / 2, m_iTextStartYPos + 10);
	m_MapDrawManager.gotoxy(m_iWidth, m_iTextStartYPos + 11);
}

void Play::DrawOption()
{
	system("cls");
	m_iTextStartYPos = m_iHeight * 0.13;
	m_MapDrawManager.BoxDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("= Option =", m_iWidth, m_iTextStartYPos);
	m_MapDrawManager.DrawMidText("1. Map Size Set", m_iWidth, m_iTextStartYPos * 2);
	m_MapDrawManager.DrawMidText("2. Cursor Custom", m_iWidth, m_iTextStartYPos * 3);
	m_MapDrawManager.DrawMidText("3. Stone Custom", m_iWidth, m_iTextStartYPos * 4);
	m_MapDrawManager.DrawMidText("4. Undo Count Set", m_iWidth, m_iTextStartYPos * 5);
	m_MapDrawManager.DrawMidText("5. Return", m_iWidth, m_iTextStartYPos * 6);
	m_MapDrawManager.DrawMidText("입력 : ", m_iWidth, m_iTextStartYPos * 7);
	m_MapDrawManager.gotoxy(m_iWidth + 4, m_iTextStartYPos * 7);
}

void Play::DrawPlayMap()
{
	string CurPN;
	string CurUD;
	if (P1->GetMyTurn())
	{
		CurPN = P1->GetTeamName();
		CurUD = to_string(P1->GetUndoNum());
		m_sCurCursor = m_sCursorShapeP1;
	}
	else
	{
		CurPN = P2->GetTeamName();
		CurUD = to_string(P2->GetUndoNum());
		m_sCurCursor = m_sCursorShapeP2;
	}
	m_MapDrawManager.DrawMidText("Player Name : " + CurPN + "   무르기 : " + CurUD, m_iWidth, m_iHeight + 4);
	m_MapDrawManager.DrawMidText("Turn : " + to_string(m_iTurnNum), m_iWidth, m_iHeight + 5);
	
	m_MapDrawManager.DrawPoint(m_sCurCursor, m_iCursorX, m_iCursorY);
}

void::Play::RecordRock()
{
	m_save.open("savefile.txt");
	int x, y;
	if (m_save.is_open())
	{
		m_save << m_iTurnNum << endl;
		m_save << P1->GetTeamName() << " " << P2->GetTeamName() << endl;
		m_save << P1->GetRockShape() << " " << P2->GetRockShape();
		for (int i = 0; i < m_iTurnNum / 2; i++)
		{
			P1->GetRocks(i, &x, &y);
			m_save << endl << x << " " << y << endl;
			P2->GetRocks(i, &x, &y);
			m_save << x << " " << y;
		}
		if (m_iTurnNum % 2 == 1)
		{
			P1->GetRocks(m_iTurnNum / 2, &x, &y);
			m_save << endl	<< x << " " << y;
		}
		m_save.close();
	}
}

void Play::Replay()
{
	string teamA, teamB, rockA, rockB, curTeam;
	m_MapDrawManager.GridDraw(m_iWidth, m_iHeight);
	m_MapDrawManager.DrawMidText("==== 조작키 ====", m_iWidth, m_iHeight + 1);
	m_MapDrawManager.DrawMidText("이동 : A,S,W,D 돌놓기 : ENTER", m_iWidth, m_iHeight + 2);
	m_MapDrawManager.DrawMidText("무르기 : N 옵션 : P 종료 : ESC", m_iWidth, m_iHeight + 3);
	m_load >> m_iTurnNum;
	m_load >> teamA >> teamB >> rockA >> rockB;
	P1->SetTeamName(teamA);
	P2->SetTeamName(teamB);
	P1->SetRockShape(rockA);
	P2->SetRockShape(rockB);
	while (!m_load.eof())
	{
		Sleep(1000);
		m_load >> m_iCursorX >> m_iCursorY;
		if (P1->GetMyTurn())
		{
			curTeam = teamA;
			P1->InputRock(m_iCursorX, m_iCursorY);
			SwapTurn();
		}
		else
		{
			curTeam = teamB;
			P2->InputRock(m_iCursorX, m_iCursorY);
			SwapTurn();
		}
		m_MapDrawManager.DrawMidText("Player Name : " +  curTeam, m_iWidth, m_iHeight + 4);
	}
	m_MapDrawManager.gotoxy(0, m_iHeight + 5);
	system("pause");
	system("cls");
	Reset();
}

Play::~Play()
{
	delete P1;
	delete P2;
}