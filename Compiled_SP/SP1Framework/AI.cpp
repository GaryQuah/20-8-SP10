#include "game.h"
#include "AI.h"

void moveAIS() // movement of AI
{
	//
	//-------------------------------moving horizontally----------------------- // if location of player is BOTposition
	COORD targetDest1 = { (BOT1.xpos + (BOT1.dir * 1)) , (BOT1.ypos) };
	COORD targetDest6 = { (BOT6.xpos + (BOT6.dir * 1)) , (BOT6.ypos) };
	COORD targetDest2 = { (BOT2.xpos + (BOT2.dir * 1)) , (BOT2.ypos) };
	COORD targetDest3 = { (BOT3.xpos + (BOT3.dir * 1)) , (BOT3.ypos) };
	COORD targetDest4 = { (BOT4.xpos + (BOT4.dir * 1)) , (BOT4.ypos) };
	//
	//-------------------------------moving up and down------------------------ // if location of player is BOTposition
	COORD targetDest5 = { (BOT5.xpos) , (BOT5.ypos + (BOT5.dir * 1)) };
	//
	//-------------------------------AI movement begin-------------------------------------------

	//-------------------------------Bot 1 & 2 movement------------------------------------------

	if (g_dEnemyTime1 >= 0.03)  // how fast the enemy moves
	{
		if (g_sChar.m_cLocation.X == targetDest1.X && g_sChar.m_cLocation.Y + 1 == targetDest1.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT1.xpos += BOT1.dir * 1;
			BOT1.step++;
		}

		if (g_sChar.m_cLocation.X == targetDest6.X && g_sChar.m_cLocation.Y + 1 == targetDest6.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT6.xpos += BOT6.dir * 1;
			BOT6.step++;
		}


		if (g_sChar.m_cLocation.X == targetDest2.X && g_sChar.m_cLocation.Y + 1 == targetDest2.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT2.xpos += BOT2.dir * 1;
			g_dEnemyTime1 = 0.0;
			BOT2.step++;
		}

		if (BOT1.step >= 13)
		{
			BOT1.dir *= -1;        // reverses the direction of the AI [if the position is positive, after multiplying 
								   // it by -1 it becomes negative and moves backwards. if if it is negative , 
								   // multiplying it by another negative makes it positive , making the AI move forward.
			BOT1.step = 0;         // after moving 15 steps , reset the number of steps to 0.
		}

		if (BOT2.step >= 13)
		{
			BOT2.dir *= -1;
			BOT2.step = 0;
		}

		if (BOT6.step >= 13)
		{
			BOT6.dir *= -1;
			BOT6.step = 0;
		}
	}
	//----------------------------------------Bot 3 & 4 movemement--------------------------------------------------
	if (g_dEnemyTime2 >= 0.04)  // how fast the enemy moves
	{
		if (g_sChar.m_cLocation.X == targetDest3.X && g_sChar.m_cLocation.Y + 1 == targetDest3.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT3.xpos += BOT3.dir * 1;
			BOT3.step++;
		}

		if (g_sChar.m_cLocation.X == targetDest4.X && g_sChar.m_cLocation.Y + 1 == targetDest4.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT4.xpos += BOT4.dir * 1;
			g_dEnemyTime2 = 0.0;
			BOT4.step++;
		}

		if (BOT3.step >= 39)
		{
			BOT3.dir *= -1;
			BOT3.step = 0;
		}

		if (BOT4.step >= 39)
		{
			BOT4.dir *= -1;
			BOT4.step = 0;
		}
	}
	//----------------------------------------Bot 5 movement [vertical] ---------------------------------------------------
	if (g_dEnemyTime3 >= 0.05)
	{
		if (g_sChar.m_cLocation.X == targetDest5.X && g_sChar.m_cLocation.Y + 1 == targetDest5.Y)
		{
			g_sChar.m_cLocation.X = 83; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT5.ypos += BOT5.dir * 1;
			g_dEnemyTime3 = 0.0;
			BOT5.step++;
		}

		if (BOT5.step >= 11)
		{
			BOT5.dir *= -1;
			BOT5.step = 0;
		}
	}
}