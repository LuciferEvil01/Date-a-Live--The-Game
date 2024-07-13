#ifndef Game_Logic
#define Game_Logic
#define true 1
#define false 0

// Enum
typedef enum
{
    Init,
    Playing,
    Game_Over,
    Paused,
    Cinematic,
}Game_State;
typedef enum
{
    Spirit,
    AST,
    DEM,

}Enemy_Team;
typedef enum
{
    True,
    False,
}Bool;
// Struct
typedef struct
{
    float x;
    float y;
}Entity;
typedef struct
{
    Entity Entity;
    Enemy_Team Team;
    Bool Active;
    int Health;
    int Speed;

}Enemy;
typedef struct
{
    Entity Entity;
    int Health;
    int Score;
}Player;
typedef struct
{
    Entity Entity;
    Bool Active;
    int Speed;
    int Direction;
}Bullet;
typedef struct
{
    int Level;
    int Enemy_Row;
    int Enemy_Column;
    int Speed;
}Level;

// Function
void Init_Game();
void Update_Game();
void Check_Collisions();
void Update_Enemy();
void Update_Player();
void Update_Bullet();
void Resume_Game();
void Paused_Game();
void Game_Over();

#endif