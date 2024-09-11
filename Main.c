#include "Game_logic.h"
#include "Game_Menu.h"
#include "Threads.h"
#include "Main.h"
pthread_mutex_t lock;
void Init_Main()
{
    pthread_mutex_init(&lock, NULL);
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);

    start_color();
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(1, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_YELLOW);
    init_color(COLOR_YELLOW, 404, 268, 132);
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_CYAN, COLOR_CYAN);
    init_color(COLOR_YELLOW + 1, 1000, 500, 0);
    init_pair(9, COLOR_YELLOW + 1,COLOR_YELLOW + 1);
}

void Game_Control()
{
    while(1)
    {
        if(restart)
            {
                napms(1100);
                killAll();
                restart = false;
                break;
            }
    }

    char* Result="You Win";
    if(Score != 600) Result="You Lose";
    Score=0;
    Screen(Result);
}

int main()
{
    Init_Main();
    while(1)
    {
        Menu();
        Game_Control();
    }
    pthread_mutex_destroy(&lock);
    endwin();
    return 0;
}
