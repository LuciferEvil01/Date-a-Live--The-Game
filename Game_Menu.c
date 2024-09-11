#include "Main.h"
#include "Game_Menu.h"
#include "Game_logic.h"
#include "threads.h"

#define Option_Number 2

struct Options options[Option_Number];
int option= 0;


void Init_Options()
{
    options[0].c = COLS/4;
    options[0].r = LINES/2;
    options[1].c = options[0].c;
    options[1].r = options[0].r + 3;
}

void drawOptions()
{
    pthread_mutex_lock(&lock);
    attron(COLOR_PAIR(5));
    mvprintw(options[0].r, options[0].c, "New Game");
    mvprintw(options[1].r, options[0].c, "Exit");
    attroff(COLOR_PAIR(5));
    refresh();
    pthread_mutex_unlock(&lock);

}

void erasePointer()
{
    pthread_mutex_lock(&lock);

    attron(COLOR_PAIR(5));
    mvprintw(options[option].r, options[option].c -2, "  ");
    attroff(COLOR_PAIR(5));
    refresh();
    pthread_mutex_unlock(&lock);

}

void drawPointer()
{
    pthread_mutex_lock(&lock);

    attron(COLOR_PAIR(5));
    mvprintw(options[option].r, options[option].c -2, "->");
    attroff(COLOR_PAIR(5));
    refresh();
    pthread_mutex_unlock(&lock);

}

void drawTitle()
{
    pthread_mutex_lock(&lock);

    attron(COLOR_PAIR(5));
    mvprintw(2, COLS/2 - 7, "MATCOM INVASION");
    //char *text = "MATCOM INVASION";
    //draw_text(5, 5, text);
    attroff(COLOR_PAIR(5));
    refresh();
    pthread_mutex_unlock(&lock);

}

void Select_Options()
{
    int event;
    while(1)
    {
        event = getch();
        pthread_mutex_unlock(&lock);
        erasePointer();
        switch (event)
        {
            case KEY_UP:
                if(option <= 0) break;
                option--;
                break;
            case KEY_DOWN:
                if(option >= Option_Number - 1) break;
                option++;
                break;
            default:
                break;
        }
        if(event == 32) break;
        drawPointer();
        refresh();
        napms(50);
    }

    if(option == 0)
    {
        Init_Game();
        Update_Game();
    }
    if(option == 1)
    {
        endwin();
        exit(0);
    }
}


void Screen(char* text)
{
    clear();
    attron(COLOR_PAIR(5));
    Draw_Text(10, 10,5,text);
    attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(5));
    mvprintw(30, 10, "Press spacebar to go back to menu...");
    attroff(COLOR_PAIR(5));
    refresh();

    int key;

    while(1)
    {
        key = getch();
        pthread_mutex_unlock(&lock);
        if(key == 32) break;
    }
}


void Menu()
{
    Init_Options();
    clear();
    drawTitle();
    drawOptions();
    drawPointer();
    Select_Options();
}