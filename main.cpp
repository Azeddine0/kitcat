#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

// Color constants
enum ConsoleColor {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5,
    BROWN = 6, LIGHTGRAY = 7, DARKGRAY = 8, LIGHTBLUE = 9, LIGHTGREEN = 10,
    LIGHTCYAN = 11, LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};

// Game constants
const int WIDTH = 80;
const int HEIGHT = 25;
const int GROUND_Y = HEIGHT - 4;
const char EMPTY = ' ';

// Game variables
int catY = GROUND_Y - 5; // Adjusted for cat height
bool isJumping = false;
bool isDucking = false;
int jumpHeight = 0;
int score = 0;
int highScore = 0;
bool gameOver = false;
bool gamePaused = false;
vector<pair<int, int>> obstacles; // x position and type
vector<pair<int, int>> powerups; // x position and type
vector<pair<int, int>> backgrounds; // x position and type
int obstacleTimer = 0;
int powerupTimer = 0;
int gameSpeed = 50;
int animationFrame = 0;
int timeOfDay = 0; // 0-1000 cycle
bool isNight = false;
bool hasDoubleJump = false;
bool canDoubleJump = false;
bool isInvincible = false;
int invincibleTimer = 0;
int lives = 3;
int level = 1;
int levelScore = 0;
int catType = 0; // 0=normal, 1=ninja, 2=space
bool showTutorial = true;
HANDLE hConsole;

// Minimalistic ASCII Art for Cat (standing)
const string catStand[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < ",
    " /   \\ ",
    " \\_^_/ "
};

// Minimalistic ASCII Art for Cat (running frame 1)
const string catRun1[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < ",
    " / ^ \\ ",
    " \\_^_/ "
};

// Minimalistic ASCII Art for Cat (running frame 2)
const string catRun2[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < ",
    " \\ ^ / ",
    " \\_^_/ "
};

// Minimalistic ASCII Art for Cat (jumping)
const string catJump[5] = {
    " /\\_/\\ ",
    "( ^.^ )",
    " > ^ < ",
    "       ",
    " \\_^_/ "
};

// Minimalistic ASCII Art for Cat (ducking)
const string catDuck[3] = {
    "       ",
    " /\\_/\\ ",
    "(_o.o_)"
};

// Minimalistic ASCII Art for Cat (dead)
const string catDead[5] = {
    " /\\_/\\ ",
    "( x.x )",
    " > ^ < ",
    " /   \\ ",
    " \\_^_/ "
};

// Ninja Cat
const string ninjaCatRun1[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " >|^|< ",
    " / ^ \\ ",
    " \\_^_/ "
};

const string ninjaCatRun2[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " >|^|< ",
    " \\ ^ / ",
    " \\_^_/ "
};

// Space Cat
const string spaceCatRun1[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " >[*]< ",
    " / ^ \\ ",
    " \\_^_/ "
};

const string spaceCatRun2[5] = {
    " /\\_/\\ ",
    "( o.o )",
    " >[*]< ",
    " \\ ^ / ",
    " \\_^_/ "
};

// Minimalistic ASCII Art for Small Fence
const string smallFence[3] = {
    "|---|",
    "|   |",
    "|___|"
};

// Minimalistic ASCII Art for Medium Fence
const string mediumFence[3] = {
    "|---|---|",
    "|   |   |",
    "|___|___|"
};

// Minimalistic ASCII Art for Large Fence
const string largeFence[3] = {
    "|---|---|---|",
    "|   |   |   |",
    "|___|___|___|"
};

// Minimalistic ASCII Art for Cactus
const string cactus[4] = {
    " _  ",
    "| | ",
    "| |_",
    "|_| "
};

// Minimalistic ASCII Art for Cloud
const string cloud[2] = {
    " .-. ",
    "(_._)"
};

// Minimalistic ASCII Art for Dog (boss)
const string dog[5] = {
    "  /\\__/\\  ",
    " (  ww  ) ",
    "  \\ -- /  ",
    "  /    \\  ",
    " (_/\\/\\_) "
};

// Minimalistic ASCII Art for Mountain Background
const string mountain[4] = {
    "      /\\      ",
    "     /  \\     ",
    "    /    \\    ",
    "___/      \\___"
};

// Minimalistic ASCII Art for Building Background
const string building[5] = {
    "   _____   ",
    "  |     |  ",
    "  | [] |  ",
    "  |     |  ",
    "__|_____|__"
};

// Minimalistic ASCII Art for Star
const string star = "*";

// Minimalistic ASCII Art for Sun/Moon
const string sun[3] = {
    " \\ | / ",
    "- ( ) -",
    " / | \\ "
};

const string moon[3] = {
    "   _   ",
    " /   \\ ",
    " \\___/ "
};

// Minimalistic ASCII Art for Powerups
const string milkBottle[3] = {
    " ___ ",
    "| M |",
    "|___|"
};

const string fish[3] = {
    "  _  ",
    "><_> ",
    "     "
};

const string catnip[3] = {
    " \\|/ ",
    "--o--",
    " /|\\ "
};

const string yarnBall[3] = {
    " ___ ",
    "/   \\",
    "\\___/"
};

// ASCII Art for Title (updated with your custom art)
const string titleArt[9] = {
    ".--.   .--. .-./`) ,---------.   _______      ____   ,---------.  ",
    "|  | _/  /  \\ .-.')\\          \\ /   __  \\   .'  __ `.\\          \\ ",
    "| (`' ) /   / `-' \\ `--.  ,---'| ,_/  \\__) /   '  \\  \\`--.  ,---' ",
    "|(_ ()_)     `-'`\"`    |   \\ ,-./  )       |___|  /  |   |   \\    ",
    "| (_,_)   __ .---.     :_ _: \\  '_ '`)        _.-`   |   :_ _:    ",
    "|  |\\ \\  |  ||   |     (_I_)  > (_)  )  __ .'   _    |   (_I_)    ",
    "|  | \\ `'   /|   |    (_(=)_)(  .  .-'_/  )|  _( )_  |  (_(=)_)   ",
    "|  |  \\    / |   |     (_I_)  `-'`-'     / \\ (_ o _) /   (_I_)    ",
    "`--'   `'-'  '---'     '---'    `._____.'   '.(_,_).'    '---'    "
};

// Function to set cursor position
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

// Function to hide cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Function to set console size
void setConsoleSize() {
    COORD bufferSize = {WIDTH + 5, HEIGHT + 5};
    SMALL_RECT windowSize = {0, 0, WIDTH, HEIGHT};
    
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

// Function to set text color
void setColor(int textColor, int bgColor = BLACK) {
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Function to play sound
void playSound(const char* soundType) {
    if (strcmp(soundType, "jump") == 0) {
        Beep(500, 100);
    } else if (strcmp(soundType, "powerup") == 0) {
        Beep(700, 100);
    } else if (strcmp(soundType, "hit") == 0) {
        Beep(200, 200);
    } else if (strcmp(soundType, "gameover") == 0) {
        Beep(300, 300);
        Beep(200, 300);
        Beep(100, 500);
    } else if (strcmp(soundType, "levelup") == 0) {
        Beep(500, 100);
        Beep(600, 100);
        Beep(700, 100);
        Beep(800, 200);
    }
}

// Function to draw a string at a specific position
void drawStringAt(const string& str, int x, int y, char buffer[][WIDTH + 1]) {
    for (size_t i = 0; i < str.length(); i++) {
        if (y >= 0 && y < HEIGHT && x + i >= 0 && x + i < WIDTH) {
            buffer[y][x + i] = str[i];
        }
    }
}

// Function to draw the game
void draw() {
    // Create buffer
    static char buffer[HEIGHT][WIDTH + 1];
    
    // Clear buffer
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x] = ' ';
        }
        buffer[y][WIDTH] = '\0';
    }
    
    // Draw day/night cycle
    isNight = (timeOfDay >= 500);
    
    if (isNight) {
        // Draw stars
        for (int i = 0; i < 15; i++) {
            int starX = (animationFrame + i * 7) % WIDTH;
            int starY = (i * 3) % (GROUND_Y - 5);
            if (starX >= 0 && starX < WIDTH && starY >= 0 && starY < HEIGHT) {
                buffer[starY][starX] = '*';
            }
        }
        
        // Draw moon
        for (int i = 0; i < 3; i++) {
            drawStringAt(moon[i], WIDTH - 15, 2 + i, buffer);
        }
    } else {
        // Draw sun
        for (int i = 0; i < 3; i++) {
            drawStringAt(sun[i], WIDTH - 15, 2 + i, buffer);
        }
    }
    
    // Draw background elements
    for (const auto& bg : backgrounds) {
        int x = bg.first;
        int type = bg.second;
        
        if (type == 0) { // Mountain
            for (int i = 0; i < 4; i++) {
                drawStringAt(mountain[i], x, GROUND_Y - 8 + i, buffer);
            }
        } else if (type == 1) { // Building
            for (int i = 0; i < 5; i++) {
                drawStringAt(building[i], x, GROUND_Y - 5 + i, buffer);
            }
        }
    }
    
    // Draw ground
    for (int x = 0; x < WIDTH; x++) {
        buffer[GROUND_Y][x] = '_';
    }
    
    // Draw cat
    const string* catFrame;
    
    if (gameOver) {
        catFrame = catDead;
    } else if (isDucking) {
        // Draw ducking cat (smaller)
        for (int i = 0; i < 3; i++) {
            drawStringAt(catDuck[i], 10, catY + 2 + i, buffer);
        }
        return; // Skip the normal cat drawing
    } else if (isJumping) {
        catFrame = catJump;
    } else {
        // Alternate between running frames based on cat type
        if (catType == 0) { // Normal cat
            catFrame = (animationFrame % 6 < 3) ? catRun1 : catRun2;
        } else if (catType == 1) { // Ninja cat
            catFrame = (animationFrame % 6 < 3) ? ninjaCatRun1 : ninjaCatRun2;
        } else if (catType == 2) { // Space cat
            catFrame = (animationFrame % 6 < 3) ? spaceCatRun1 : spaceCatRun2;
        }
    }
    
    // Apply invincibility effect (flashing)
    if (isInvincible && animationFrame % 4 < 2) {
        setColor(YELLOW);
    } else if (catType == 1) { // Ninja cat
        setColor(DARKGRAY);
    } else if (catType == 2) { // Space cat
        setColor(CYAN);
    } else {
        setColor(LIGHTGRAY);
    }
    
    for (int i = 0; i < 5; i++) {
        drawStringAt(catFrame[i], 10, catY + i, buffer);
    }
    
    setColor(LIGHTGRAY); // Reset color
    
    // Draw obstacles
    for (const auto& obs : obstacles) {
        int x = obs.first;
        int type = obs.second;
        
        if (type == 0) { // Small fence
            setColor(BROWN);
            for (int i = 0; i < 3; i++) {
                drawStringAt(smallFence[i], x, GROUND_Y - 3 + i, buffer);
            }
        } else if (type == 1) { // Medium fence
            setColor(BROWN);
            for (int i = 0; i < 3; i++) {
                drawStringAt(mediumFence[i], x, GROUND_Y - 3 + i, buffer);
            }
        } else if (type == 2) { // Large fence
            setColor(BROWN);
            for (int i = 0; i < 3; i++) {
                drawStringAt(largeFence[i], x, GROUND_Y - 3 + i, buffer);
            }
        } else if (type == 3) { // Cactus
            setColor(GREEN);
            for (int i = 0; i < 4; i++) {
                drawStringAt(cactus[i], x, GROUND_Y - 4 + i, buffer);
            }
        } else if (type == 4) { // Cloud
            setColor(WHITE);
            int cloudY = 2 + (x % 3);
            for (int i = 0; i < 2; i++) {
                drawStringAt(cloud[i], x, cloudY + i, buffer);
            }
        } else if (type == 5) { // Dog (boss)
            setColor(BROWN);
            for (int i = 0; i < 5; i++) {
                drawStringAt(dog[i], x, GROUND_Y - 5 + i, buffer);
            }
        }
    }
    
    setColor(LIGHTGRAY); // Reset color
    
    // Draw powerups
    for (const auto& pu : powerups) {
        int x = pu.first;
        int type = pu.second;
        
        if (type == 0) { // Milk
            setColor(WHITE);
            for (int i = 0; i < 3; i++) {
                drawStringAt(milkBottle[i], x, GROUND_Y - 6 + i, buffer);
            }
        } else if (type == 1) { // Fish
            setColor(LIGHTBLUE);
            for (int i = 0; i < 3; i++) {
                drawStringAt(fish[i], x, GROUND_Y - 6 + i, buffer);
            }
        } else if (type == 2) { // Catnip
            setColor(GREEN);
            for (int i = 0; i < 3; i++) {
                drawStringAt(catnip[i], x, GROUND_Y - 6 + i, buffer);
            }
        } else if (type == 3) { // Yarn
            setColor(RED);
            for (int i = 0; i < 3; i++) {
                drawStringAt(yarnBall[i], x, GROUND_Y - 6 + i, buffer);
            }
        }
    }
    
    setColor(LIGHTGRAY); // Reset color
    
    // Check collisions
    if (!gameOver && !isInvincible) {
        for (const auto& obs : obstacles) {
            int x = obs.first;
            int type = obs.second;
            
            if (x < 20 && x > -5) {
                bool collision = false;
                
                // Collision box for cat
                int catLeft = 11;
                int catRight = 16;
                int catTop = isDucking ? catY + 2 : catY + 1;
                int catBottom = catY + (isDucking ? 4 : 4);
                int catHeight = isDucking ? 2 : 3;
                
                // Collision box for obstacle
                int obsLeft = x;
                int obsWidth = 0;
                int obsTop = 0;
                int obsHeight = 0;
                
                switch (type) {
                    case 0: // Small fence
                        obsWidth = 5;
                        obsTop = GROUND_Y - 3;
                        obsHeight = 3;
                        break;
                    case 1: // Medium fence
                        obsWidth = 9;
                        obsTop = GROUND_Y - 3;
                        obsHeight = 3;
                        break;
                    case 2: // Large fence
                        obsWidth = 13;
                        obsTop = GROUND_Y - 3;
                        obsHeight = 3;
                        break;
                    case 3: // Cactus
                        obsWidth = 4;
                        obsTop = GROUND_Y - 4;
                        obsHeight = 4;
                        break;
                    case 5: // Dog (boss)
                        obsWidth = 10;
                        obsTop = GROUND_Y - 5;
                        obsHeight = 5;
                        break;
                    default: // Cloud or other non-collidable
                        continue;
                }
                
                int obsRight = x + obsWidth;
                int obsBottom = obsTop + obsHeight;
                
                // Check if collision boxes overlap
                if (catRight > obsLeft && catLeft < obsRight && catBottom > obsTop && catTop < obsBottom) {
                    collision = true;
                }
                
                if (collision) {
                    playSound("hit");
                    lives--;
                    
                    if (lives <= 0) {
                        gameOver = true;
                        playSound("gameover");
                    } else {
                        isInvincible = true;
                        invincibleTimer = 100; // Invincibility period after hit
                    }
                    
                    break;
                }
            }
        }
    }
    
    // Check powerup collisions
    for (size_t i = 0; i < powerups.size(); i++) {
        int x = powerups[i].first;
        int type = powerups[i].second;
        
        if (x >= 5 && x <= 20) {
            // Collision box for cat
            int catLeft = 11;
            int catRight = 16;
            int catTop = isDucking ? catY + 2 : catY + 1;
            int catBottom = catY + (isDucking ? 4 : 4);
            
            // Collision box for powerup
            int puLeft = x;
            int puRight = x + 5;
            int puTop = GROUND_Y - 6;
            int puBottom = GROUND_Y - 3;
            
            // Check if collision boxes overlap
            if (catRight > puLeft && catLeft < puRight && catBottom > puTop && catTop < puBottom) {
                playSound("powerup");
                
                // Apply powerup effect
                switch (type) {
                    case 0: // Milk
                        score += 50;
                        break;
                    case 1: // Fish
                        gameSpeed = max(10, gameSpeed - 5);
                        break;
                    case 2: // Catnip
                        hasDoubleJump = true;
                        powerupTimer = 300;
                        break;
                    case 3: // Yarn
                        isInvincible = true;
                        invincibleTimer = 200;
                        break;
                }
                
                powerups.erase(powerups.begin() + i);
                i--;
            }
        }
        
        if (powerups.size() > 0 && i < powerups.size() && powerups[i].first < -5) {
            powerups.erase(powerups.begin() + i);
            i--;
        }
    }
    
    // Draw UI elements
    setColor(WHITE);
    string scoreText = "Score: " + to_string(score);
    drawStringAt(scoreText, 2, 1, buffer);
    
    string levelText = "Level: " + to_string(level);
    drawStringAt(levelText, 20, 1, buffer);
    
    string livesText = "Lives: " + to_string(lives);
    drawStringAt(livesText, 35, 1, buffer);
    
    string speedText = "Speed: " + to_string(100 - gameSpeed);
    drawStringAt(speedText, WIDTH - 15, 1, buffer);
    
    // Draw powerup indicators
    if (hasDoubleJump) {
        string djText = "Double Jump: ON";
        drawStringAt(djText, 2, 2, buffer);
    }
    
    if (isInvincible) {
        string invText = "Invincible: ON";
        drawStringAt(invText, 25, 2, buffer);
    }
    
    // Print buffer
    for (int y = 0; y < HEIGHT; y++) {
        setCursorPosition(0, y);
        cout << buffer[y];
    }
    
    // Draw game over screen
    if (gameOver) {
        // Draw semi-transparent overlay
        for (int y = HEIGHT / 2 - 4; y < HEIGHT / 2 + 6; y++) {
            setCursorPosition(WIDTH / 2 - 20, y);
            setColor(RED, BLACK);
            cout << string(40, ' ');
        }
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 - 2);
        setColor(YELLOW, BLACK);
        cout << "GAME OVER!";
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2);
        cout << "Final Score: " + to_string(score);
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 + 1);
        cout << "High Score: " + to_string(max(highScore, score));
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 + 3);
        setColor(WHITE, BLACK);
        cout << "Press R to restart or Q to quit";
    }
    
    // Draw pause screen
    if (gamePaused) {
        // Draw semi-transparent overlay
        for (int y = HEIGHT / 2 - 4; y < HEIGHT / 2 + 6; y++) {
            setCursorPosition(WIDTH / 2 - 20, y);
            setColor(BLUE, BLACK);
            cout << string(40, ' ');
        }
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 - 2);
        setColor(YELLOW, BLACK);
        cout << "GAME PAUSED";
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2);
        setColor(WHITE, BLACK);
        cout << "Press P to resume";
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 + 1);
        cout << "Press R to restart";
        
        setCursorPosition(WIDTH / 2 - 15, HEIGHT / 2 + 2);
        cout << "Press Q to quit";
    }
    
    // Draw tutorial (first time only)
    if (showTutorial && score < 100) {
        setCursorPosition(WIDTH / 2 - 20, 5);
        setColor(YELLOW, BLACK);
        cout << "CONTROLS:";
        
        setCursorPosition(WIDTH / 2 - 20, 6);
        setColor(WHITE, BLACK);
        cout << "SPACE - Jump (press twice for double jump)";
        
        setCursorPosition(WIDTH / 2 - 20, 7);
        cout << "DOWN  - Duck under obstacles";
        
        setCursorPosition(WIDTH / 2 - 20, 8);
        cout << "P     - Pause game";
        
        setCursorPosition(WIDTH / 2 - 20, 9);
        setColor(LIGHTGREEN, BLACK);
        cout << "Collect powerups to gain special abilities!";
    }
    
    setColor(LIGHTGRAY, BLACK); // Reset color
}

// Function to update game state
void update() {
    if (gamePaused || gameOver) {
        return;
    }
    
    // Update score
    score++;
    levelScore++;
    
    // Check for level up
    if (levelScore >= 1000) {
        level++;
        levelScore = 0;
        playSound("levelup");
        
        // Make game harder with each level
        if (gameSpeed > 15) {
            gameSpeed -= 2;
        }
        
        // Unlock new cat at certain levels
        if (level == 3) {
            catType = 1; // Ninja cat
        } else if (level == 5) {
            catType = 2; // Space cat
        }
    }
    
    // Update animation frame
    animationFrame++;
    
    // Update day/night cycle
    timeOfDay = (timeOfDay + 1) % 1000;
    
    // Update cat jump
    if (isJumping) {
        if (jumpHeight < 8) {
            catY--;
            jumpHeight++;
        } else {
            isJumping = false;
        }
    } else if (catY < GROUND_Y - 5 && !isDucking) {
        catY++;
    }
    
    // Update powerup timers
    if (powerupTimer > 0) {
        powerupTimer--;
        if (powerupTimer == 0) {
            hasDoubleJump = false;
        }
    }
    
    if (invincibleTimer > 0) {
        invincibleTimer--;
        if (invincibleTimer == 0) {
            isInvincible = false;
        }
    }
    
    // Update obstacles
    obstacleTimer++;
    int spawnThreshold = 30 - (level * 2); // Adjust obstacle frequency based on level
    spawnThreshold = max(10, spawnThreshold); // Ensure minimum threshold
    
    if (obstacleTimer > spawnThreshold) {
        if (rand() % 10 < 7) { // 70% chance to spawn obstacle
            int type;
            
            // Occasionally spawn a boss (dog)
            if (level > 2 && score > 1000 && rand() % 20 == 0) {
                type = 5; // Dog boss
            } else {
                type = rand() % 4; // 0=small fence, 1=medium fence, 2=large fence, 3=cactus
            }
            
            obstacles.push_back({WIDTH - 1, type});
            obstacleTimer = 0;
        }
    }
    
    // Spawn clouds occasionally
    if (rand() % 100 < 2) {
        obstacles.push_back({WIDTH - 1, 4}); // 4 = cloud
    }
    
    // Spawn background elements occasionally
    if (rand() % 200 < 3) {
        int type = rand() % 2; // 0=mountain, 1=building
        backgrounds.push_back({WIDTH - 1, type});
    }
    
    // Spawn powerups occasionally
    if (rand() % 200 < 3) {
        int type = rand() % 4; // 0=milk, 1=fish, 2=catnip, 3=yarn
        powerups.push_back({WIDTH - 1, type});
    }
    
    // Move obstacles
    for (size_t i = 0; i < obstacles.size(); i++) {
        int moveSpeed;
        
        if (obstacles[i].second == 4) { // Clouds move slower
            moveSpeed = 1;
        } else if (obstacles[i].second == 5) { // Dogs move faster
            moveSpeed = 3;
        } else {
            moveSpeed = 2;
        }
        
        obstacles[i].first -= moveSpeed;
        
        if (obstacles[i].first < -20) {
            obstacles.erase(obstacles.begin() + i);
            i--;
        }
    }
    
    // Move powerups
    for (size_t i = 0; i < powerups.size(); i++) {
        powerups[i].first -= 2;
        
        if (powerups[i].first < -10) {
            powerups.erase(powerups.begin() + i);
            i--;
        }
    }
    
    // Move background elements (parallax effect)
    for (size_t i = 0; i < backgrounds.size(); i++) {
        backgrounds[i].first -= 1; // Move slower than obstacles
        
        if (backgrounds[i].first < -20) {
            backgrounds.erase(backgrounds.begin() + i);
            i--;
        }
    }
    
    // Hide tutorial after a while
    if (score > 500) {
        showTutorial = false;
    }
}

// Function to show settings menu
void showSettings() {
    system("cls");
    setColor(YELLOW);
    cout << "\n\n  SETTINGS\n\n";
    
    setColor(WHITE);
    cout << "  1. Difficulty: " << (gameSpeed <= 30 ? "Hard" : (gameSpeed <= 40 ? "Medium" : "Easy")) << "\n";
    cout << "  2. Cat Type: " << (catType == 0 ? "Normal" : (catType == 1 ? "Ninja" : "Space")) << "\n";
    cout << "  3. Show Tutorial: " << (showTutorial ? "Yes" : "No") << "\n";
    cout << "  4. Reset High Score\n";
    cout << "  5. Back to Game\n\n";
    
    cout << "  Enter your choice (1-5): ";
    
    char choice = _getch();
    
    switch (choice) {
        case '1':
            // Cycle through difficulties
            if (gameSpeed > 40) gameSpeed = 30; // Easy -> Hard
            else if (gameSpeed <= 30) gameSpeed = 40; // Hard -> Medium
            else gameSpeed = 50; // Medium -> Easy
            break;
        case '2':
            // Cycle through cat types
            catType = (catType + 1) % 3;
            break;
        case '3':
            // Toggle tutorial
            showTutorial = !showTutorial;
            break;
        case '4':
            // Reset high score
            highScore = 0;
            {
                ofstream scoreFile("highscore.txt");
                if (scoreFile.is_open()) {
                    scoreFile << 0;
                    scoreFile.close();
                }
            }
            break;
        case '5':
        default:
            break;
    }
    
    system("cls");
}

// Main function
int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    // Set up console
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setConsoleSize();
    hideCursor();
    
    // Load high score from file
    ifstream scoreFile("highscore.txt");
    if (scoreFile.is_open()) {
        scoreFile >> highScore;
        scoreFile.close();
    }
    
    // Show title screen
    system("cls");
    cout << "\n";
    
    // Display ASCII art title
    setColor(YELLOW);
    for (int i = 0; i < 9; i++) {  // Updated for new title art
        cout << "  " << titleArt[i] << "\n";
    }
    
    setColor(LIGHTGREEN);
    cout << "\n                  FENCE JUMPING EDITION\n\n";
    
    setColor(WHITE);
    cout << "             Press SPACE to jump, DOWN to duck\n";
    cout << "                Press P to pause the game\n";
    cout << "               Press any key to start...\n\n";
    
    // Display cat
    setColor(LIGHTGRAY);
    cout << "                      /\\_/\\ \n";
    cout << "                     ( o.o )\n";
    cout << "                      > ^ < \n";
    cout << "                      /   \\ \n";
    cout << "                      \\_^_/ \n";
    
    setColor(YELLOW);
    cout << "\n\n             High Score: " << highScore << "\n";
    
    setColor(LIGHTGRAY);
    _getch();
    
    system("cls");
    
    // Game loop
    while (true) {
        // Handle input
        if (_kbhit()) {
            char key = _getch();
            
            if (!gamePaused && !gameOver) {
                if (key == ' ') {
                    if (!isJumping && !isDucking && catY >= GROUND_Y - 5) {
                        isJumping = true;
                        jumpHeight = 0;
                        canDoubleJump = true;
                        playSound("jump");
                    } else if (isJumping && canDoubleJump && hasDoubleJump) {
                        // Double jump
                        jumpHeight = 0;
                        canDoubleJump = false;
                        playSound("jump");
                    }
                } else if (key == 'p' || key == 'P') {
                    gamePaused = true;
                } else if (key == 's' || key == 'S') {
                    showSettings();
                } else if (key == 0 || key == 224) { // Arrow key prefix
                    key = _getch(); // Get the actual arrow key
                    if (key == 80) { // Down arrow
                        if (!isJumping) {
                            isDucking = true;
                        }
                    }
                }
            } else if (gamePaused) {
                if (key == 'p' || key == 'P') {
                    gamePaused = false;
                } else if (key == 'r' || key == 'R') {
                    // Reset game
                    catY = GROUND_Y - 5;
                    isJumping = false;
                    isDucking = false;
                    jumpHeight = 0;
                    score = 0;
                    levelScore = 0;
                    level = 1;
                    lives = 3;
                    gameOver = false;
                    gamePaused = false;
                    obstacles.clear();
                    powerups.clear();
                    backgrounds.clear();
                    obstacleTimer = 0;
                    powerupTimer = 0;
                    gameSpeed = 50;
                    animationFrame = 0;
                    hasDoubleJump = false;
                    isInvincible = false;
                    invincibleTimer = 0;
                    system("cls");
                } else if (key == 'q' || key == 'Q') {
                    return 0;
                } else if (key == 's' || key == 'S') {
                    showSettings();
                    gamePaused = true; // Stay paused after settings
                }
            } else if (gameOver) {
                if (key == 'r' || key == 'R') {
                    // Reset game
                    catY = GROUND_Y - 5;
                    isJumping = false;
                    isDucking = false;
                    jumpHeight = 0;
                    score = 0;
                    levelScore = 0;
                    level = 1;
                    lives = 3;
                    gameOver = false;
                    obstacles.clear();
                    powerups.clear();
                    backgrounds.clear();
                    obstacleTimer = 0;
                    powerupTimer = 0;
                    gameSpeed = 50;
                    animationFrame = 0;
                    hasDoubleJump = false;
                    isInvincible = false;
                    invincibleTimer = 0;
                    system("cls");
                } else if (key == 'q' || key == 'Q') {
                    return 0;
                }
            }
        } else if (!isJumping && !gamePaused && !gameOver) {
            // Release duck when key is released
            isDucking = false;
        }
        
        // Update game state
        update();
        
        // Draw game
        draw();
        
        // Save high score if needed
        if (gameOver && score > highScore) {
            highScore = score;
            ofstream scoreFile("highscore.txt");
            if (scoreFile.is_open()) {
                scoreFile << highScore;
                scoreFile.close();
            }
        }
        
        // Control game speed
        Sleep(gameSpeed);
    }
    
    return 0;
}