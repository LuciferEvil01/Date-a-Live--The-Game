

#ifndef InvaderStruct
#define InvaderStruct

extern int Score;
typedef struct Player{
	int r,c,LP,munitions;
	char ch;
};

typedef struct alien {
	int r,c;
	int pr,pc;
	int alive; /* 1=alive 0=destroyed */
	char direction,move; /* 'l'=left 'r'=right */
	char ch;
};

typedef struct shoot {
	int r,c;
	int active; /* 1=active 0=inactive */
	char ch;
};

typedef struct bomb {
	int r,c;
	int active; /* 1=active 0=inactive */
	int loop; /* used to prevent alien from flashing when bomb is dropped */
	char ch;
};

typedef struct options {
	int overall,alien,shots,bombs,bombchance;
};

void Init_Game();
void Update_Game();
void Draw_Text(int c,int r,int index,char Text[]);
#endif
