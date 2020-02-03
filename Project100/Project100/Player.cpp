#include "Player.h"

Player::Player()
{
}

Player::Player(int x, int y)
{
	m_iRockNum = 0;
	m_iUndoNum = 5;
	m_iWinCheckCount = 1;
	rock = new Rock[(x*y) / 2];
}

void Player::InputRock(int x, int y)
{
	MapDrawManager.DrawPoint(m_sRockshape, x, y);
	rock[m_iRockNum].m_ix = x;
	rock[m_iRockNum].m_iy = y;
	m_iRockNum++;
}

void Player::DrawRock(int x, int y)
{
	MapDrawManager.DrawPoint(m_sRockshape, x, y);
}

bool Player::SeekRock(int x, int y)
{
	for (int i = 0; i < m_iRockNum; i++)
	{
		if (rock[i].m_ix == x &&
			rock[i].m_iy == y)
			return true;
	}
	return false;
}

void Player::DrawAllRocks()
{
	for (int i = 0; i < m_iRockNum; i++)
	{
		MapDrawManager.DrawPoint(m_sRockshape, rock[i].m_ix, rock[i].m_iy);
	}
}

void Player::SeekLastRock(int* x, int* y)
{
	if (m_iRockNum > 0)
	{
		*x = rock[m_iRockNum - 1].m_ix;
		*y = rock[m_iRockNum - 1].m_iy;
		MapDrawManager.gotoxy(*x, *y);
	}
}

bool Player::UndoTurn()
{
	if (m_iRockNum > 0 && m_iUndoNum > 0)
	{
		--m_iRockNum;
		rock[m_iRockNum].m_ix = NULL;
		rock[m_iRockNum].m_iy = NULL;
		--m_iUndoNum;
		return true;
	}
	return false;
}

bool Player::PlayerWin()
{
	if (WinCheck(rock[m_iRockNum - 1].m_ix, rock[m_iRockNum - 1].m_iy))
		return true;
	return false;
}

bool Player::WinCheck(int x, int y)	
{
	for (int i = 1; i < 5; i++)		
	{
		if (SeekRock(x + i, y))
			m_iWinCheckCount++;
		else if (SeekRock(x - i, y))		//i
			m_iWinCheckCount++;
		else
		{
			m_iWinCheckCount = 1;
			break;
		}
		if (m_iWinCheckCount == 5)
			return true;
	}
	for (int i = 1; i < 5; i++)
	{
		if (SeekRock(x, y + i))
			m_iWinCheckCount++;
		else if (SeekRock(x, y - i))
			m_iWinCheckCount++;
		else
		{
			m_iWinCheckCount = 1;
			break;
		}
		if (m_iWinCheckCount == 5)
			return true;
	}
	for (int i = 1; i < 5; i++)
	{
		if (SeekRock(x + i, y + i))
			m_iWinCheckCount++;
		else if (SeekRock(x - i, y - i))
			m_iWinCheckCount++;
		else
		{
			m_iWinCheckCount = 1;
			break;
		}
		if (m_iWinCheckCount == 5)
			return true;
	}
	for (int i = 1; i < 5; i++)
	{
		if (SeekRock(x - i, y + i))
			m_iWinCheckCount++;
		else if (SeekRock(x + i, y - i))
			m_iWinCheckCount++;
		else
		{
			m_iWinCheckCount = 1;
			break;
		}
		if (m_iWinCheckCount == 5)
			return true;
	}
	return false;
}

void Player::ResetRock(int x, int y)
{
	if (rock != NULL)
	{
		delete[] rock;
		rock = new Rock[x*y];
	}
	m_iRockNum = 0;
}

void Player::GetRocks(int num, int* x, int* y)
{
	if (rock[num].m_ix != NULL)
	{
		*x = rock[num].m_ix;
		*y = rock[num].m_iy;
	}
}

Player::~Player()
{
	delete[] rock;
}
