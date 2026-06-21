#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>    
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


using namespace std;

// GAME SETTINGS 
int WIDTH = 60;
int HEIGHT = 20;

// PLAYER VARIABLES 
int playerX = 20;

// BULLET VARIABLES 
int bulletX[10], bulletY[10];
const int MAX_BULLETS = 10;
bool bulletActive[10];

// DRONE VARIABLES 
int droneX[5] = { 0, 0, 0, 0, 0 };
int droneY[5] = { 0, 0, 0, 0, 0 };
bool droneAlive[5] = { false, false, false, false, false };
int droneLifetime[5] = { 0, 0, 0, 0, 0 };

// GAME VARIABLES 
int currentLevel = 1;
int score = 0;
int ammo = 999;
int dronesKilled = 0;
int timeLeft = 0;
int frameCounter = 0;
int droneLifespan = 150;
int maxDrones = 3;
int minDrones = 2;
int killTarget = 15;
bool unlimitedAmmo = true;

// SPECIFIC LEVEL 4
int totalDronesLevel4 = 30;
int dronesSpawnedLevel4 = 0;

// HELPER FUNCTIONS 
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void clearScreen() {
    system("cls");
}

void waitForEnter() {
    cout << "\nPress ENTER to continue...";
    while (_getch() != 13);
}

void playShootSound() {
    PlaySound(TEXT("shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playWinSound() {
    PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_SYNC);
}


void playLoseSound() {
    PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_SYNC);
}

// LEVEL INITIALIZATION 
void initLevel(int level) {
    currentLevel = level;
    dronesKilled = 0;
    for (int i = 0; i < MAX_BULLETS; i++)
        bulletActive[i] = false;
    playerX = WIDTH / 2;
    frameCounter = 0;

    for (int i = 0; i < 5; i++) {
        droneAlive[i] = false;
        droneLifetime[i] = 0;
    }

    if (level == 1) {
        unlimitedAmmo = true;
        ammo = 999;
        killTarget = 15;
        timeLeft = 30 * 30;  // 30 seconds (total time)
        droneLifespan = 150; // 5 seconds (duration for each drone)
        minDrones = 2;
        maxDrones = 3;
    }
    else if (level == 2) {
        unlimitedAmmo = true;
        ammo = 999;
        killTarget = 20;
        timeLeft = 40 * 30;  // 40 seconds
        droneLifespan = 90;  // 3 seconds 
        minDrones = 3;
        maxDrones = 4;
    }
    else if (level == 3) {
        unlimitedAmmo = false;
        ammo = 40;
        killTarget = 25;
        timeLeft = 50 * 30;  // 50 seconds
        droneLifespan = 90;  // 3 seconds
        minDrones = 4;
        maxDrones = 5;
    }
    else if (level == 4) {
        unlimitedAmmo = false;
        ammo = 40;
        killTarget = 30;
        timeLeft = 999999;  // No time limit
        droneLifespan = 150;  // 5 seconds
        minDrones = 2;
        maxDrones = 3;
        totalDronesLevel4 = 30;
        dronesSpawnedLevel4 = 0;
    }
}

// STORY SCREENS
void showIntroStory() {
    clearScreen();
    cout << "\n\n";
    cout << "  =============================================\n";
    cout << "                      STRIKEZONE\n";
    cout << "  =============================================\n\n";
    cout << "  The year is 2157.\n";
    cout << "  Hostile alien drones have entered our orbit.\n";
    cout << "  You are humanity's last line of defense.\n\n";
    cout << "  As the planetary defense pilot, you must\n";
    cout << "  eliminate all threats and save Earth.\n\n";
    cout << "  The fate of billions rests in your hands.\n\n";
    cout << "  =============================================\n";
    waitForEnter();
}

void showLevelStory(int level) {
    clearScreen();
    cout << "\n\n";

    if (level == 1) {
        cout << "  =============================================\n";
        cout << "       MISSION 1: FIRST WAVE\n";
        cout << "  =============================================\n\n";
        cout << "  Hostile drones detected in orbit!\n";
        cout << "  Eliminate 15 targets in 30 seconds.\n\n";
        cout << "  Ammunition: UNLIMITED\n";
        cout << "  Threat Level: LOW\n\n";
    }
    else if (level == 2) {
        cout << "  =============================================\n";
        cout << "       MISSION 2: ORBITAL DISTURBANCE\n";
        cout << "  =============================================\n\n";
        cout << "  More drones incoming! Faster waves detected.\n";
        cout << "  Eliminate 20 targets in 40 seconds.\n\n";
        cout << "  Ammunition: UNLIMITED\n";
        cout << "  Threat Level: MEDIUM\n\n";
    }
    else if (level == 3) {
        cout << "  =============================================\n";
        cout << "       MISSION 3: AMMUNITION CRISIS\n";
        cout << "  =============================================\n\n";
        cout << "  Supply lines disrupted! Limited ammo.\n";
        cout << "  Eliminate 25 targets in 50 seconds.\n\n";
        cout << "  Ammunition: 40 BULLETS\n";
        cout << "  Threat Level: HIGH\n";
        cout << "  Make every shot count!\n\n";
    }
    else if (level == 4) {
        cout << "  =============================================\n";
        cout << "       FINAL MISSION: LAST STAND\n";
        cout << "  =============================================\n\n";
        cout << "  This is it! 30 hostile drones detected.\n";
        cout << "  No time limit. Destroy them all.\n\n";
        cout << "  Ammunition: 40 BULLETS\n";
        cout << "  Threat Level: EXTREME\n";
        cout << "  No reinforcements. Good luck, pilot.\n\n";
    }

    cout << "  =============================================\n";
    waitForEnter();
}

void showVictoryScreen() {
    playWinSound();
    clearScreen();
    cout << "\n\n";
    cout << "  =============================================\n";
    cout << "       MISSION ACCOMPLISHED!\n";
    cout << "  =============================================\n\n";
    cout << "  All hostile drones eliminated!\n";
    cout << "  The planet is safe.\n\n";
    cout << "  Final Score: " << score << "\n\n";
    cout << "  Thank you for your service, pilot.\n";
    cout << "  Humanity is in your debt.\n\n";
    cout << "  =============================================\n";
    waitForEnter();
}

void showLevelComplete(int level) {
    if(level==4) return;
    clearScreen();
    cout << "\n\n";
    cout << "  =============================================\n";
    cout << "       LEVEL " << level << " COMPLETE!\n";
    cout << "  =============================================\n\n";
    cout << "  Drones Eliminated: " << dronesKilled << "\n";
    cout << "  Current Score: " << score << "\n\n";
    cout << "  Excellent work, pilot!\n\n";
    cout << "  =============================================\n";
    cout << "\n  1. Next Level\n";
    cout << "  2. Exit Game\n\n";
    cout << "  Choose: ";
    playWinSound();
    while (true) {
        char choice = _getch();
        if (choice == '1') break;
        else if (choice == '2') exit(0);
    }
}

void showLevelFailed(int level)
{
    clearScreen();
    cout << "\n\n";
    cout << "  =============================================\n";
    cout << "       MISSION FAILED\n";
    cout << "  =============================================\n\n";
    cout << "  Level " << level << " was not completed.\n";
    cout << "  Drones Eliminated: " << dronesKilled << "/" << killTarget << "\n\n";
    cout << "  Don't give up, pilot!\n\n";
    cout << "  =============================================\n";
    cout << "\n  1. Restart Level\n";
    cout << "  2. Exit Game\n\n";
    cout << "  Choose: ";
    playLoseSound();
    while (true) {
        char choice = _getch();
        if (choice == '1') break;
        else if (choice == '2') exit(0);
    }
}


void showPauseMenu(int level) {
    clearScreen();
    cout << "\n\n";
    cout << "  =============================================\n";
    cout << "       GAME PAUSED\n";
    cout << "  =============================================\n\n";
    cout << "  Level " << level << " - Mission Interrupted\n";
    cout << "  Current Score: " << score << "\n";
    cout << "  Drones Eliminated: " << dronesKilled << "/" << killTarget << "\n\n";
    cout << "  =============================================\n";
    cout << "\n  1. Resume Mission\n";
    cout << "  2. Restart Level\n";
    cout << "  3. Exit Game\n\n";
    cout << "  Choose: ";

    while (true) {
        char choice = _getch();
        if (choice == '1') return;
        else if (choice == '2')
        {
            break;
        }
        else if (choice == '3') exit(0);
    }
}
//GAME FUNCTIONS
void shoot()
{
    if (ammo > 0)
        //find first empty slot
    {
        for (int i = 0;i < MAX_BULLETS;i++)
        {
            if (!bulletActive[i])
            {
                bulletX[i] = playerX;
                bulletY[i] = HEIGHT - 2;
                bulletActive[i] = true;
                if (!unlimitedAmmo)
                    ammo--;
                playShootSound();
                break;//create 1 bullet per shot
            }
        }
    }
}

void spawnDrone(int index)
{
    //level 4:check if all drones are spawned
    if (currentLevel == 4)
    {
        if (dronesSpawnedLevel4 >= totalDronesLevel4)
            return;
        dronesSpawnedLevel4++;
    }
    //Random position(not too close to edges or bottom)
    droneX[index] = rand() % (WIDTH - 10) + 5;
    droneY[index] = rand() % (HEIGHT - 5) + 2;
    droneAlive[index] = true;
    droneLifetime[index] = droneLifespan;
}

void manageDroneSpawning()
{
    //count active drones
    int activeCount = 0;
    for (int i = 0;i < 5;i++)
    {
        if (droneAlive[i])
            activeCount++;
    }

    //random target between min and max
    int targetDrones = minDrones + (rand() % (maxDrones - minDrones + 1));

    //spawn if below range
    if (activeCount < targetDrones)
    {
        for (int i = 0;i < 5;i++)
        {
            if (!droneAlive[i])
            {
                spawnDrone(i);
                break;
            }
        }
    }
}
void updateBullets()
{
    for (int i = 0;i < MAX_BULLETS;i++)
    {
        if (bulletActive[i])
        {
            bulletY[i]--;//Move up all the bullets
            if (bulletY[i] < 2) //remove bullet when it reaches the top
                bulletActive[i] = false;
        }
    }
}

void updateDrones()
{
    for (int i = 0;i < 5;i++)
    {
        if (droneAlive[i])
        {
            droneLifetime[i]--;
            // if drone lifespan espires- respawn except at level 4
            if (droneLifetime[i] == 0)
            {
                if (currentLevel == 4)
                    droneAlive[i] = false;
                else
                    spawnDrone(i);
            }
        }
    }
}
void checkCollisions()
{
    //check all bullets against all drones
    for (int i = 0;i < MAX_BULLETS;i++)//loop through bullets
    {
        if (!bulletActive[i]) continue;//skip inactive bullets
        for (int j = 0;j < 5;j++)//loop through drones
        {
            if (droneAlive[j] && bulletX[i] == droneX[j] && bulletY[i] == droneY[j])
            {
                //hit detected
                droneAlive[j] = false;
                bulletActive[i] = false;
                score += 100;
                dronesKilled++;
                //respawn drones except level 4
                if (currentLevel != 4)
                    spawnDrone(j);
                break;// this drone is dead, check next bullet
            }
        }
    }
}

void drawGame()
{
    //draw UI at top
    gotoxy(0, 0);
    cout << "LEVEL: " << currentLevel << " | SCORE: " << score << " | KILLS: " << dronesKilled << "/" << killTarget;
    if (!unlimitedAmmo)
        cout << " | AMMO: " << ammo << "  ";
    if (currentLevel != 4)
        cout << " | TIME: " << (timeLeft / 30) << "  ";

    gotoxy(0, 1);
    for (int i = 0; i < WIDTH; i++)
        cout << "=";

    //draw game area
    for (int y = 0; y < HEIGHT; y++)
    {
        gotoxy(0, y + 2);
        for (int x = 0; x < WIDTH; x++)
        {
            bool drawn = false;

            //draw drones
            for (int i = 0; i < 5; i++)
            {
                if (droneAlive[i] && x == droneX[i] && y == droneY[i])
                {
                    cout << "O";
                    drawn = true;
                    break;
                }
            }

            //draw bullets
            if (!drawn) {
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (bulletActive[i] && x == bulletX[i] && y == bulletY[i])
                    {
                        cout << "*";
                        drawn = true;
                        break;
                    }
                }
            }

            //draw player
            if (!drawn && y == HEIGHT - 1 && x == playerX)
            {
                cout << "A";
                drawn = true;
            }

            //draw aim indicator
            if (!drawn && y == HEIGHT - 2 && x == playerX)
            {
                //check if there's any bullet at this position
                bool bulletHere = false;
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (bulletActive[i] && x == bulletX[i] && y == bulletY[i])
                    {
                        bulletHere = true;
                        break;
                    }
                }
                if (!bulletHere)
                {
                    cout << "^";
                    drawn = true;
                }
            }

            //draw empty space
            if (!drawn)
                cout << " ";

        }
    }

    //draw bottom border
    gotoxy(0, HEIGHT + 2);
    for (int i = 0; i < WIDTH; i++)
        cout << "=";

    gotoxy(0, HEIGHT + 3);
    cout << " Arrow Keys: Move | SPACE: Shoot | ESC: Menu  ";
}
bool checkLevelComplete()
{
    if (currentLevel == 4)
    {
        // Level 4: Check if all drones spawned and killed
        bool allDead = true;
        for (int i = 0;i < 5;i++)
        {
            if (droneAlive[i])
                allDead = false;
        }
        return(dronesKilled == killTarget && allDead && dronesSpawnedLevel4 == totalDronesLevel4);

    }
    return (dronesKilled == killTarget);
}
bool checkLevelFailed()
{
    if (currentLevel != 4 && timeLeft == 0)
        return true;

    // For Level 4: Check if all drones spawned but target not met
    if (currentLevel == 4 && dronesSpawnedLevel4 >= totalDronesLevel4)
    {
        // Check if all current drones are dead
        bool allCurrentDead = true;
        for (int i = 0; i < 5; i++)
        {
            if (droneAlive[i])
            {
                allCurrentDead = false;
                break;
            }
        }

        // If all drones spawned, all current drones dead, and target not met - FAIL
        if (allCurrentDead && dronesKilled < killTarget)
            return true;
    }

    if (!unlimitedAmmo && ammo == 0)
    {
        bool anyBulletActive = false;
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bulletActive[i])
            {
                anyBulletActive = true;
                break;
            }
        }
        // If no bullets active and no ammo left
        if (!anyBulletActive)
        {
            if (dronesKilled < killTarget)
                return true;
        }
    }

    return false;
}
// MAIN GAME LOOP
void playLevel(int level) {
    initLevel(level);
    showLevelStory(level);

    bool levelRunning = true;
    bool restartLevel = false;
    frameCounter = 0;

    while (levelRunning) {
        if (_kbhit()) {
            char key = _getch();

            if (key == -32) {
                key = _getch();
                if (key == 75) {// Left arrow
                    if (playerX > 1)
                        playerX--;
                }
                else if (key == 77) { // Right arrow
                    if (playerX < WIDTH - 2)
                        playerX++;
                }
            }
            else if (key == ' ')
                shoot();
            else if (key == 27) {// ESC - Show pause menu
                showPauseMenu(level);
                char choice = _getch();
                if (choice == '1')// resume, continue loop
                    continue;
                else if (choice == '2') {
                    restartLevel = true;
                    levelRunning = false;
                }
                else if (choice == '3')
                    exit(0);
            }
        }
        //update
        frameCounter++;
        if (currentLevel != 4)
            timeLeft--;

        updateBullets();
        updateDrones();
        checkCollisions();
        manageDroneSpawning();

        //check win or lose
        if (checkLevelComplete()) {
            showLevelComplete(level);
            return;
        }
        if (checkLevelFailed()) {
            showLevelFailed(level);
            playLevel(level);//restart level
            return;
        }
        //redraw game
        drawGame();
        Sleep(33);//30 FPS
    }
    //if restart requested 
    if (restartLevel)
        playLevel(level);
}
//main function

int main() {
    srand(time(0));
    hideCursor();
    showIntroStory();

    playLevel(1);
    playLevel(2);
    playLevel(3);
    playLevel(4);

    showVictoryScreen();
    return 0;
}