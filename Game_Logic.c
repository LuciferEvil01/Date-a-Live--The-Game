# include "Game_logic.h"
# include "Main.h"
# include "Threads.h"
# define Max_Bombs  1000
# define Max_Shoot  10

struct Player  tank ;
struct alien   aliens[30];
struct shoot   Shoot[Max_Shoot];
struct bomb    bombs[Max_Bombs];
struct options settings;

int input ,loops=0 ,currentshots=0, currentbombs=0, currentaliens=30;
int  score=0, win=-1;

char tellscore[30];


void cleanPosition(int c, int r)
{
    pthread_mutex_lock(&lock);

    attron(COLOR_PAIR(2));
    mvaddch(r, c, '  ');
    attroff(COLOR_PAIR(2));
    refresh();
    pthread_mutex_unlock(&lock);

}
void Draw(int c,int r,int index, char Ch)
{
    pthread_mutex_lock(&lock);
    attron(COLOR_PAIR(index));
    mvaddch(r, c, Ch);
    attroff(COLOR_PAIR(index));
    //refresh();
    pthread_mutex_unlock(&lock);
}
void Draw_Text(int c,int r,int index,char Text[])
{
    pthread_mutex_lock(&lock);
    attron(COLOR_PAIR(index));
    mvprintw(r, c,"%s", Text);
    attroff(COLOR_PAIR(index));
    //refresh();
    pthread_mutex_unlock(&lock);
}


void Default_Setting()
{
    settings.overall = 15000;
    settings.alien = 12;
    settings.shots = 3;
    settings.bombs = 10;
    settings.bombchance = 5;
}
void Init_Players()
{
    tank.r = LINES - 5;
    tank.c = COLS / 2;
    tank.munitions=3;
    tank.ch = '^';
}
void Init_Aliens()
{
    for (int i=0; i<30; ++i)
    {
        int row =3;
        int dist=  (row-1)*10;
        aliens[i].r = row;
        aliens[i].c = (i-dist)*3;
        aliens[i].pr = 0;
        aliens[i].pc = 0;
        aliens[i].ch = '#';
        aliens[i].alive = 1;
        aliens[i].direction = 'r';
    }
}
void Init_Bullets()
{
   /* Set shot settings */
    for (int i=0; i<3; ++i)
    {
        Shoot[i].r=0;
        Shoot[i].c=0;
        Shoot[i].active = 0;
        Shoot[i].ch = 'o';
    }
   /* Set bomb settings */
    for (int i=0; i<Max_Bombs; ++i)
    {
        bombs[i].active = 0;
        bombs[i].ch = '*';
        bombs[i].loop = 0;
    }
}
void Init_Visual()
{
    clear();
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);
    srand(time(NULL));
    WINDOW *game_window = newwin(LINES, COLS, 0, 0);
    Draw(tank.c,tank.r,5,tank.ch);
}

void Init_Game()
{
    Default_Setting();
    Init_Players();
    Init_Aliens();
    Init_Bullets();
    Init_Visual();
}


void Life_Control()
{
    tank.LP--;
    if(tank.LP <= 0) restart = true;
}
void Check_Collision()
{
    
}
void Move_Players(int direction)
{
    switch (direction)
    {
    case 0:
        if(tank.c - 3 < 1) break;
        tank.c += -3;
        break;
    case 1:
        if(tank.c + 3 > COLS - 2) break;
        tank.c += 3;
        break;
    }
}

void* Information(void* threads)
{
    Draw_Text( 15, LINES-1,5,"     Munition: "+tank.munitions);
    Draw_Text(0,LINES-1,5,"Lifes: "+tank.LP);
    Draw_Text (COLS-30,0,5,"m = Menu  Esc = Exit");
    Draw_Text ((COLS/2)-9,0,5,"Matcom-Invasion");
    Draw_Text (1,0,5,"SCORE: "+score);
    if(tank.munitions<=Max_Shoot) tank.munitions=tank.munitions+1;
    napms(333);
}
void* Move_Alien(void* thread)
{
    while (1)
    {
        for (int i=0; i<30; ++i)
        {
            struct alien* alien=&aliens[i];
            if (alien->alive == 1)
            {
                cleanPosition(alien->pc,alien->pr);
                Draw(alien->c,alien->r,5,alien->ch);
                alien->pr = alien->r;
                alien->pc = alien->c;
                /* Check if alien should drop bomb */
                int random_number = (random() % 10) + 1;
                if ((settings.bombchance - random_number) >= 0 )
                {
                    for (int j=0; j<Max_Bombs; ++j)
                    {
                        if (bombs[j].active == 0)
                        {
                            bombs[j].active = 1;
                            ++currentbombs;
                            bombs[j].r = alien->r + 1;
                            bombs[j].c = alien->c;
                            break;
                        }
                    }
                }
                /* Set alien's next position */
                if (alien->direction == 'l')
                    --alien->c;
                else if (alien->direction == 'r')
                    ++alien->c;
                /* Check alien's next positions */
                if (alien->c == COLS - 2)
                {
                    ++alien->r;
                    alien->direction = 'l';
                }
                else if (alien->c == 0)
                {
                    ++alien->r;
                    alien->direction = 'r';
                }
            }
        }
        if(restart == true) pthread_exit(NULL);
        napms(333);
    }

}
void* Move_Bombs(void* thread)
{
    while(1)
    {
        for (int i=0; i<Max_Bombs; ++i)
    {
        struct bomb* bomb= &bombs[i];
        if (bomb->active == 1)
        {
            if (bomb->r < LINES-4)
            {
                if (bomb->loop != 0)
                {
                    cleanPosition(bomb->c,bomb->r-1);
                }
                else
                    ++bomb->loop;
                Draw(bomb->c,bomb->r,5,bomb->ch);
                ++bomb->r;
            }
            else
            {
                bomb->active = 0;
                bomb->loop = 0;
                cleanPosition(bomb->c,bomb->r-1);
            }
            if(bomb->c==tank.c && bomb->r==tank.r)
            {
                Life_Control();
                break;
            }
            for (int i = 0; i < Max_Shoot; i++)
            {
                if(bomb->c==Shoot[i].c && bomb->r==Shoot[i].r && Shoot[i].active==1)
                {
                    cleanPosition(bomb->c,bomb->r);
                    bomb->active=0;
                    Shoot[i].active=0;
                    refresh();
                }
            }
        }
    }
        if(restart == true) pthread_exit(NULL);
        napms(100);
    }
}
void* Move_Shot(void* thread)
{
    while(1)
    {
        for (int i=0; i<Max_Shoot; ++i)
        {
            struct shoot* shot = &Shoot[i];
            if (shot->r < 2)
            {
                shot->active=0;
                cleanPosition(shot->c,shot->r);
            }
            if (shot->active == 1)
            {
                cleanPosition(shot->c,shot->r+1);
                Draw(shot->c,shot->r,5,shot->ch);
                for (int j=0; j<30; ++j)
                {
                    if (aliens[j].r == shot->r && aliens[j].c == shot->c && aliens[j].alive==1)
                    {
                        score += 20;
                        aliens[j].alive = 0;
                        shot->active = 0;
                        cleanPosition(shot->c,shot->r);
                        break;
                    }
                }
                if(shot->active==0) continue;
            }
            shot->r=shot->r-1;
            if(shot->r<0) shot->active=0;
            refresh();
        }
        if(restart == true) pthread_exit(NULL);
        napms(50);
    }
}
void* Imput(void* thread)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        //flushinp();
        int event = getch();
        // Procesar el evento de teclado
        switch (event)
        {
            case KEY_LEFT:
                pthread_mutex_unlock(&lock);
                cleanPosition(tank.c,tank.r);
                Move_Players(0);
                Draw(tank.c,tank.r,5,tank.ch);
                break;
            case KEY_RIGHT:// Mover la nave hacia la derecha
                pthread_mutex_unlock(&lock);
                cleanPosition(tank.c,tank.r);
                Move_Players(1);
                Draw(tank.c,tank.r,5,tank.ch);
                break;
            case 27:
                pthread_mutex_unlock(&lock);
                restart=true;
                break;
            case 32:
                pthread_mutex_unlock(&lock);
                if(tank.munitions>0)
                {
                    tank.munitions--;
                    for(int i=0 ;i<Max_Shoot;i++)
                    {
                        if(Shoot[i].active==0)
                        {
                            Shoot[i].active=1;
                            Shoot[i].c=tank.c;
                            Shoot[i].r=tank.r-2;
                            break;
                        }
                    }
                }
                break;
            default:
                pthread_mutex_unlock(&lock);
                break;
            // ...
        }
        refresh();
        if(restart == true) pthread_exit(NULL);
        napms(10);
    }
}

void Update_Game()
{
    Add_Thread(&Move_Alien);
    Add_Thread(&Move_Shot);
    Add_Thread(&Move_Bombs);
    Add_Thread(&Imput);
    Add_Thread(&Information);
}


