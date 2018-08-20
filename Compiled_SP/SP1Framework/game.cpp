// This is the main file for the game logic and function
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//--------------------------------------------Global vars---------------------------------------------------------

double  g_dElapsedTime;
double  g_dDeltaTime;

double g_dEnemyTime1;
double g_dEnemyTime2;
double g_dEnemyTime3;
double g_dSlipTime;

bool g_abKeyPressed[K_COUNT];
bool	doorClosed;
bool	g_Door1_Close;

vector<string> map;

vector<string> loading;

ROBOTS BOT[10];

// Game specific variables here
SGameChar   g_sChar , g_sBlock, g_sTrigPlates, g_sDoor, g_sJar , g_sDoor1, g_slever1;

EGAMESTATES g_eGameState = S_SPLASHSCREEN;

int lifepoint = 3;

double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
// Console object
Console g_Console(160, 40, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------

//Gary
void init( void )
{
	// character position
	g_sChar.m_cLocation.X = 5;
	g_sChar.m_cLocation.Y = 16;
	g_sChar.m_bActive = true;

    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

	g_dEnemyTime1 = 0.0;
	g_dEnemyTime2 = 0.0;
	g_dEnemyTime3 = 0.0;

	g_dSlipTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

	//loads map
	loadMap();
	loadTextScreen();

	//bool door,
	doorClosed = true;

	//character & objects
	g_sTrigPlates.m_bActive = true;
	g_sBlock.m_bActive = true;
	g_sChar.m_bActive = true;
	g_sDoor.m_bActive = true;
	g_sJar.m_bActive = true;

	//location of character & objects *MAY CHANGE*
	g_sJar.m_cLocation.X = 7;
	g_sJar.m_cLocation.Y = 8;
	g_sDoor.m_cLocation.X = 9;
	g_sDoor.m_cLocation.Y = 12;
	g_sTrigPlates.m_cLocation.X = 4;
	g_sTrigPlates.m_cLocation.Y = 24;
	g_sBlock.m_cLocation.X = 4;
	g_sBlock.m_cLocation.Y = 25;

	//For Doors
	g_sDoor1.m_cLocation.X = 9;
	g_sDoor1.m_cLocation.Y = 3;
	g_sDoor1.m_bActive = true;

	//Bool Function
	g_Door1_Close = true;
	
	//For Levers
	g_slever1.m_cLocation.X = 5;
	g_slever1.m_cLocation.Y = 2;
	g_slever1.m_bActive = true;


	//BOT POS

	BOT[0].xpos = 3;
	BOT[0].ypos = 7;
	BOT[0].step = 0;
	BOT[0].dir = 1;

	BOT[1].xpos = 16;
	BOT[1].ypos = 9;
	BOT[1].step = 0;
	BOT[1].dir = -1;

	BOT[2].xpos = 30;
	BOT[2].ypos = 27;
	BOT[2].step = 0;
	BOT[2].dir = 1;

	BOT[3].xpos = 69;
	BOT[3].ypos = 27;
	BOT[3].step = 0;
	BOT[3].dir = -1;

	BOT[4].xpos = 84;
	BOT[4].ypos = 30;
	BOT[4].step = 0;
	BOT[4].dir = -1;

	BOT[5].xpos = 3;
	BOT[5].ypos = 11;
	BOT[5].step = 0;
	BOT[5].dir = 1;

	//----------------------------------------------------------------

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}
//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
	g_abKeyPressed[K_UP] = isKeyPressed(0x57);
	g_abKeyPressed[K_DOWN] = isKeyPressed(0x53);
	g_abKeyPressed[K_LEFT] = isKeyPressed(0x41);
	g_abKeyPressed[K_RIGHT] = isKeyPressed(0x44);
	g_abKeyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
	g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------

void update(double dt) // every delta time run this 
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;

        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
    }

    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
	//----for different instances of speed----
	g_dEnemyTime1 += g_dDeltaTime; 
	g_dEnemyTime2 += g_dDeltaTime;
	g_dEnemyTime3 += g_dDeltaTime; 

	g_dSlipTime += g_dDeltaTime;
	//----------------------------------------

    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	
	moveAIS();
	//CP
	blocksMovement();	// movement of the blocks
	panelTrigger();		// panel switch
	Levers();			// Lever for Door

	//PY
	doorCollision();	// door collision
	jarRand();			// jar that gives buff/debuff

	 // sound can be played here too.
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character

	if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
	{
		while (true)
		{
			if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == (char)8)	//check if character next pos not wall & not door then move
			{
				g_sChar.m_cLocation.Y--;
			}
			else break;
		}

		if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != (char)219)	//check if character next pos not wall & not door then move
		{
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
	}
    
	if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
		while (true)
		{
			if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == (char)8)	//check if character next pos not wall & not door then move
			{
				g_sChar.m_cLocation.X--;
			}
			else break;
		}

		if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X -1] != (char)219) 
		{
			//Beep(1440, 30);
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }
    
	if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
		while (true)
		{
			if (map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == (char)8)	//check if character next pos not wall & not door then move
			{
			    g_sChar.m_cLocation.Y++;
			}
			else break;
		}
		if (map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != (char)219)
		{
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }
   
	if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
		while (true)
		{
			if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == (char)8)	//check if character next pos not wall & not door then move
			{
				g_sChar.m_cLocation.X++;
			}
			else break;
		}
		if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] != (char)219) 
		{
			//Beep(1440, 30);
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
		}
    }

    if (g_abKeyPressed[K_SPACE])
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
    }

    if (bSomethingHappened) //after something happens
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}

//----------------------------------------------------------------------------------------------------------------

void Levers()
{
	if (g_sChar.m_cLocation.X == g_slever1.m_cLocation.X && g_sChar.m_cLocation.Y == g_slever1.m_cLocation.Y)
	{
		Beep(1440, 30);
		g_Door1_Close = false;
	}
}

void panelTrigger() {
	//check if object interact with trigger panel
	if (g_sBlock.m_cLocation.Y == g_sTrigPlates.m_cLocation.Y && g_sBlock.m_cLocation.X == g_sTrigPlates.m_cLocation.X
		|| g_sChar.m_cLocation.Y == g_sTrigPlates.m_cLocation.Y && g_sChar.m_cLocation.X == g_sTrigPlates.m_cLocation.X)
	{
		doorClosed = false;
	}
	else
	{
		doorClosed = true;
	}
}

void jarRand() {
	//g_sJar
	//switch(rand() % 6)
	//	case 1:

	//	case 2:

	//	case 3:

	//	case 4:

	//	case 5:
	//	
}

void doorCollision()
{
	if (doorClosed == true)
	{
		if (g_sChar.m_cLocation.Y == g_sDoor.m_cLocation.Y && g_sChar.m_cLocation.X == g_sDoor.m_cLocation.X)
		{
			if (g_abKeyPressed[K_UP])
			{
				if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != (char)178)
				{
					g_sChar.m_cLocation.Y++;
				}
			}
			else if (g_abKeyPressed[K_LEFT])
			{
				if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] != (char)178)
				{
					g_sChar.m_cLocation.X++;
				}

			}
			else if (g_abKeyPressed[K_DOWN])
			{
				if (map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != (char)178)
				{
					g_sChar.m_cLocation.Y--;
				}
			}
			else if (g_abKeyPressed[K_RIGHT])
			{
				if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] != (char)178)
				{
					g_sChar.m_cLocation.X--;
				}
			}
		}
	}

	if (g_Door1_Close == true)
	{
		if (g_sChar.m_cLocation.Y == g_sDoor1.m_cLocation.Y && g_sChar.m_cLocation.X == g_sDoor1.m_cLocation.X)
		{
			if (g_abKeyPressed[K_UP])
			{
				if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != (char)68)
				{
					g_sChar.m_cLocation.Y++;
				}
			}
			else if (g_abKeyPressed[K_LEFT])
			{
				if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] != (char)68)
				{
					g_sChar.m_cLocation.X++;
				}

			}
			else if (g_abKeyPressed[K_DOWN])
			{
				if (map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != (char)68)
				{
					g_sChar.m_cLocation.Y--;
				}
			}
			else if (g_abKeyPressed[K_RIGHT])
			{
				if (map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] != (char)68)
				{
					g_sChar.m_cLocation.X--;
				}
			}
		}
	}
}

void blocksMovement() {
	//detect if player collide with block
	if (g_sBlock.m_cLocation.Y == g_sChar.m_cLocation.Y && g_sBlock.m_cLocation.X == g_sChar.m_cLocation.X)
	{
		//check if which movement key is pressed then check if block is touching wall. if no, player push block. if yes, player cannot push
		if (g_abKeyPressed[K_UP])
		{
			if (map[g_sBlock.m_cLocation.Y - 1][g_sBlock.m_cLocation.X] != (char)219)
			{
				g_sBlock.m_cLocation.Y--;
			}
			else
			{
				g_sChar.m_cLocation.Y++;
			}

		}
		else if (g_abKeyPressed[K_LEFT])
		{
			if (map[g_sBlock.m_cLocation.Y][g_sBlock.m_cLocation.X - 1] != (char)219)
			{
				g_sBlock.m_cLocation.X--;
			}
			else
			{
				g_sChar.m_cLocation.X++;
			}
		}
		else if (g_abKeyPressed[K_DOWN])
		{
			if (map[g_sBlock.m_cLocation.Y + 1][g_sBlock.m_cLocation.X] != (char)219)
			{
				g_sBlock.m_cLocation.Y++;
			}
			else
			{
				g_sChar.m_cLocation.Y--;
			}
		}
		else if (g_abKeyPressed[K_RIGHT])
		{
			if (map[g_sBlock.m_cLocation.Y][g_sBlock.m_cLocation.X + 1] != (char)219)
			{
				g_sBlock.m_cLocation.X++;
			}
			else
			{
				g_sChar.m_cLocation.X--;
			}
		}
	}
}

//Gary
void moveAIS() // movement of AI
{
	//---------------------- if location of player is BOTposition -------------------------------
	//-------------------------------moving horizontally-----------------------------------------
	COORD targetDest1 = { (BOT[0].xpos + (BOT[0].dir * 1)) , (BOT[0].ypos) }; 
	COORD targetDest6 = { (BOT[5].xpos + (BOT[5].dir * 1)) , (BOT[5].ypos) };
	COORD targetDest2 = { (BOT[1].xpos + (BOT[1].dir * 1)) , (BOT[1].ypos) }; 
	COORD targetDest3 = { (BOT[2].xpos + (BOT[2].dir * 1)) , (BOT[2].ypos) }; 
	COORD targetDest4 = { (BOT[3].xpos + (BOT[3].dir * 1)) , (BOT[3].ypos) };
	//
	//-------------------------------moving up and down------------------------------------------ 
	COORD targetDest5 = { (BOT[4].xpos) , (BOT[4].ypos + (BOT[4].dir * 1)) };
	//
	//-------------------------------AI movement begin-------------------------------------------

	//-------------------------------Bot 1 & 2 movement------------------------------------------

	if (g_dEnemyTime1 >= 0.03)  // how fast the enemy moves
	{
		if (g_sChar.m_cLocation.X == targetDest1.X && g_sChar.m_cLocation.Y == targetDest1.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT[0].xpos += BOT[0].dir * 1;
			BOT[0].step++;
		}

		if (g_sChar.m_cLocation.X == targetDest6.X && g_sChar.m_cLocation.Y == targetDest6.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT[5].xpos += BOT[5].dir * 1;
			BOT[5].step++;
		}


		if (g_sChar.m_cLocation.X == targetDest2.X && g_sChar.m_cLocation.Y  == targetDest2.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT[1].xpos += BOT[1].dir * 1;
			g_dEnemyTime1 = 0.0;
			BOT[1].step++;
		}

		if (BOT[0].step >= 13)
		{
			BOT[0].dir *= -1;        
			BOT[0].step = 0;         
		}

		if (BOT[1].step >= 13)
		{
			BOT[1].dir *= -1;        
			BOT[1].step = 0;         
		}

		if (BOT[5].step >= 13)
		{
			BOT[5].dir *= -1;
			BOT[5].step = 0;
		}
	}
	//----------------------------------------Bot 3 & 4 movemement--------------------------------------------------
	if (g_dEnemyTime2 >= 0.04)  // how fast the enemy moves
	{
		if (g_sChar.m_cLocation.X == targetDest3.X && g_sChar.m_cLocation.Y  == targetDest3.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT[2].xpos += BOT[2].dir * 1;
			BOT[2].step++;
		}

		if (g_sChar.m_cLocation.X == targetDest4.X && g_sChar.m_cLocation.Y == targetDest4.Y)
		{
			g_sChar.m_cLocation.X = 30; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 15;
			lifepoint -= 1;
		}
		else
		{
			BOT[3].xpos += BOT[3].dir * 1;
			g_dEnemyTime2 = 0.0;
			BOT[3].step++;
		}

		if (BOT[2].step >= 39)
		{
			BOT[2].dir *= -1;
			BOT[2].step = 0;
		}

		if (BOT[3].step >= 39)
		{
			BOT[3].dir *= -1;
			BOT[3].step = 0;
		}
	}
	//----------------------------------------Bot 5 movement [vertical] ---------------------------------------------------
	if (g_dEnemyTime3 >= 0.05)
	{
		if (g_sChar.m_cLocation.X == targetDest5.X && g_sChar.m_cLocation.Y == targetDest5.Y)
		{
			g_sChar.m_cLocation.X = 84; //setting player spawnpoint set to 1
			g_sChar.m_cLocation.Y = 18;
			lifepoint -= 1;
		}
		else
		{
			BOT[4].ypos += BOT[4].dir * 1;
			g_dEnemyTime3 = 0.0;
			BOT[4].step++;
		}

		if (BOT[4].step >= 11)
		{
			BOT[4].dir *= -1;
			BOT[4].step = 0;
		}
	}
}

void processUserInput()
{
	// quits the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE])
	{
		g_bQuitGame = true;
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00);
}

void loadTextScreen()
{
	//pushing text file into vector
	string line;
	ifstream myfile("loading.txt");

	//storing text text file into vector string
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			loading.push_back(line);
		}
		myfile.close();
	}
}

//Gary
void renderSplashScreen()  // renders the splash screen
{
	COORD c;
	COORD d;

    int pos = 0;
	int xpos = 0;
	for (int i = 0; i < loading.size(); i++) //keep printing each line as long as vector != 0
	{
		c.X = xpos;
		c.Y = pos + 1;
		g_Console.writeToBuffer(c, loading[i], 12);
		pos++;
	}


	d.X = 65;
	d.Y = 20;

	g_Console.writeToBuffer(d, "Game starting in 3 seconds ..." , 12);


}

//Gary
void renderAIS() //All the AIS
{
	COORD a;
	COORD b;
	COORD c;
	COORD d;
	COORD e;
	COORD f;

	a.X = BOT[0].xpos;
	a.Y = BOT[0].ypos;

	g_Console.writeToBuffer(a , 197 , 12);
	 
	b.X = BOT[1].xpos;
	b.Y = BOT[1].ypos;

	g_Console.writeToBuffer(b , 197 , 12);

	c.X = BOT[2].xpos;
	c.Y = BOT[2].ypos;

	g_Console.writeToBuffer(c, 197, 12);

	d.X = BOT[3].xpos;
	d.Y = BOT[3].ypos;

	g_Console.writeToBuffer(d, 197, 12);

	e.X = BOT[4].xpos;
	e.Y = BOT[4].ypos;

	g_Console.writeToBuffer(e, 197, 12);

	f.X = BOT[5].xpos;
	f.Y = BOT[5].ypos;

	g_Console.writeToBuffer(f, 197, 12);
}

//Gary
void renderGame()
{
    renderMap();        // gary
	renderAIS(); //gary
	renderlives(); //gary
	renderLever();//cp
	renderDoor();	//cp & py renders the door into the buffer
	renderCharacter();  // gary
}
//Gary
void loadMap() // level 1
{
	//pushing text file into vector
	string line;
	ifstream myfile("map.txt");

	//storing text text file into vector string
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == '#')
				{
					line[i] = 219;
				}
				if (line[i] == 's')
				{
					line[i] = 8;
				}
			}
			map.push_back(line);
		}
		myfile.close();
	}
}

//Gary
void renderMap()
{
	COORD d;
	COORD e;
	COORD f;
	COORD g;
	COORD h;
	COORD i;

	/*
	if ((map[j])[i] == ((char)219)) // if character is 219
	{
	g_Console.writeToBuffer({ (SHORT)i, (SHORT)j }, 'X', 12); //print out
	}
	*/

	// render 
	for (int j = 0; j < map.size(); j++) //y axis
	{
		for (int i = 0; i < map[j].size(); i++)  //x axis
		{

			if ((map[j])[i] == ((char)219)) // if character is 219
			{
				g_Console.writeToBuffer({(SHORT) i, (SHORT)j }, 219 , 14);  //print out walls
			}
			
			else if ((map[j])[i] == ((char)8)) // if character is 219
			{
				g_Console.writeToBuffer({ (SHORT)i, (SHORT)j }, 8, 3);         
			}
			
			else if ((map[j])[i] == ((char)48)) // if character is 219
			{
				g_Console.writeToBuffer({ (SHORT)i, (SHORT)j }, 48, 6);
			}

			else 
			{                                                                 
				g_Console.writeToBuffer({ (SHORT)i, (SHORT)j }, (map[j][i]), 10); //printing anything else that hasnt been mentioned as the following
			}
			/*
			if ((map[j])[i] == ((char)219)) // if character is 219
			{                           coordinate to print out , character to print , color
				g_Console.writeToBuffer({ (SHORT)i, (SHORT)j }, 'X', 12); //print out 
			}
			*/
		}
	}

	//legend monster

	d.X = 120;
	d.Y = 22;

	g_Console.writeToBuffer(d, 197 , 12);
	d.X += 1;
	g_Console.writeToBuffer(d, " = MONSTER", 12);

	//legend door

	e.X = 120;
	e.Y = 24;

	g_Console.writeToBuffer(e, 178, 5);
	e.X += 1;
	g_Console.writeToBuffer(e, " = Door", 3);

	//legend switch

	f.X = 120;
	f.Y = 26;

	g_Console.writeToBuffer(f, 33, 2);
	f.X += 1;

	g_Console.writeToBuffer(f, " = switch", 2);

	//legend movable block

	g.X = 120;
	g.Y = 28;

	g_Console.writeToBuffer(g, 254 , 8);
	g.X += 1;

	g_Console.writeToBuffer(g, " = moving block", 8);

	h.X = 120;
	h.Y = 29;

	i.X = 120;
	i.Y = 30;

	g_Console.writeToBuffer(h, g_sChar.m_cLocation.X, 6); // x cord of player
	g_Console.writeToBuffer(i, g_sChar.m_cLocation.Y, 6); // y cord of player
}

void renderDoor() {

	COORD c;

	//draw door
	WORD doorColor = 3;
	c.X = g_sDoor.m_cLocation.X;
	c.Y = g_sDoor.m_cLocation.Y + 1;
	if (doorClosed == true)
	{
		g_Console.writeToBuffer(c, (char)178, doorColor);
	}
	else
	{
		g_Console.writeToBuffer(c, (char)32);
	}

	//Door for Switches
	c.X = g_sDoor1.m_cLocation.X;
	c.Y = g_sDoor1.m_cLocation.Y + 1;
	if (g_Door1_Close == true)
	{
		g_Console.writeToBuffer(c, (char)68, doorColor);
	}
	else
	{
		g_Console.writeToBuffer(c, (char)32);
	}
}

void renderLever()
{
	WORD LeverColor = 0x0F;
	COORD a;

	a.X = g_slever1.m_cLocation.X;
	a.Y = g_slever1.m_cLocation.Y + 1;

	g_Console.writeToBuffer(a, (char)79, LeverColor);
}

//Gary
void renderCharacter() //printing character every frame
{
	COORD c;

	//Draw pressure Plates
	WORD platesColor = 0x0B;
	c.X = g_sTrigPlates.m_cLocation.X;
	c.Y = g_sTrigPlates.m_cLocation.Y + 1;
	g_Console.writeToBuffer(c, (char)202, platesColor);

	//Draw pressure Jar
	WORD jarColor = 0x0B;
	c.X = g_sJar.m_cLocation.X;
	c.Y = g_sJar.m_cLocation.Y + 1;
	g_Console.writeToBuffer(c, (char)232, jarColor);

	//Draw moveable blocks
	WORD blockColor = 0x08;
	c.X = g_sBlock.m_cLocation.X;
	c.Y = g_sBlock.m_cLocation.Y + 1;
	g_Console.writeToBuffer(c, (char)254, blockColor);

    // Draw the location of the character

	c.X = g_sChar.m_cLocation.X;
	c.Y = g_sChar.m_cLocation.Y ;
	
    g_Console.writeToBuffer(c, 12, 15);
}

//Gary ---------------------------------Life Points---------------------------------
void renderlives()
{
	COORD c;
	COORD end;
	end.X = 55;
	end.Y = 15;

	c.X = 2;
	c.Y = 32;
	for (int i = 0; i < lifepoint; i++)
	{
		g_Console.writeToBuffer(c, 3 , 13);
		c.X++;
	}
	
	if (lifepoint == 0) //if life points are 0
	{
		g_Console.clearBuffer();
		g_Console.writeToBuffer(end, "Game Over !!! Nice Try . Hit ESC to exit. ", 006);
	}
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

