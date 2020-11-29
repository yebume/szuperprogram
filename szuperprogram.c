#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef enum MUFALY {Action,Adventure,Comedy,Drama,Slice_of_Life,Fantasy,Magic,Supernatural,Horror,Mystery,Psychological,Romance,SciFi}MUFALY;
typedef enum EVSZAK {SPRING,SUMMER,FALL,WINTER}EVSZAK;
typedef enum ALAP   {Light_novel,Visual_novel,Manga,Muisc}ALAP;
typedef enum COMMAND {ADD = 1,DEL,EDIT,LOGIN,EXIT}COMMAND;       //0, 1, 2, 3, 4



typedef struct Anime
{
    int ID;
    char name[51];
    char studio[26];
    MUFALY genre;
    ALAP base;
    int year;
    EVSZAK season;
    int EP;
    struct Anime *next;
    struct Anime *prev;
}Anime;

typedef struct User_data
{
    int ID;
    int seen;
    struct User_data *next;
    struct User_data *prev;
}User_data;

typedef struct User
{
    char USER[26];
    User_data *datas;
    struct User *next;
    struct User *prev;
}User;



void push_anime(Anime NEW, Anime *tail)
{
    Anime *newp = (Anime*)malloc(sizeof(Anime));
    newp ->next = tail;
    newp ->prev = tail ->prev;
    tail ->prev ->next = newp;
    tail ->prev = newp;

    newp ->ID = NEW.ID;
    strcpy(newp ->name, NEW.name);
    strcpy(newp ->studio, NEW.studio);
    newp ->genre = NEW.genre;
    newp ->base = NEW.base;
    newp ->year = NEW.year;
    newp ->season = NEW.season;
    newp ->EP = NEW.EP;
}

void anime2list(Anime **animes_head, Anime **animes_tail)
{
    FILE *fpa =  fopen("NAGYHAZI/animek.txt", "r");
    if (fpa == NULL)
    {
        printf("File open error\n");
        return;
    } 
    Anime *head = (Anime*)malloc(sizeof(Anime));
    Anime *tail = (Anime*)malloc(sizeof(Anime));
    head ->next = tail;
    tail ->prev = head;
    tail ->next = NULL;
    head ->prev = NULL;
    Anime NEW;
    while(fscanf(fpa,"#%d,%[^,],%[^,],%d,%d,%d,%d,%d",&NEW.ID, NEW.name, NEW.studio, &NEW.genre, &NEW.base, &NEW.year, &NEW.season, &NEW.EP)==8)
    {
        push_anime(NEW, tail);
    }
    *animes_head = head;
    *animes_tail = tail;
        if (fclose(fpa) != 0)
    {
        printf("File close error\n");
        return;
    }
}

void fesulista(User_data elemek, User_data *datahead, User_data *datatail)
{
    User_data *newp = (User_data*)malloc(sizeof(User_data));

    newp ->next = datatail;
    newp ->prev = datatail ->prev;
    datatail ->prev ->next = newp;
    datatail ->prev = newp;

    newp ->ID = elemek.ID;
    newp ->seen = elemek.seen;
}

User *push_user(User UNEW, User *utail) //működnie kellene
{
    User *newp = (User*)malloc(sizeof(User));

    newp ->next = utail;
    newp ->prev = utail ->prev;
    utail ->prev ->next = newp;
    utail ->prev = newp;

    strcpy(newp ->USER, UNEW.USER);
    return newp;
}

void userdata2list(User **users_head, User **users_tail)
{
    FILE *fp =  fopen("NAGYHAZI/profilok.txt", "r");
    if (fp == NULL)
    {
        printf("File open error\n");
        return;
    } 

    User *head = (User*)malloc(sizeof(User));
    User *tail = (User*)malloc(sizeof(User));

    head ->next = tail;
    tail ->prev = head;
    tail ->next = NULL;
    head ->prev = NULL;

    User NEW;

    while(fscanf(fp,"%[^,]", NEW.USER)==1)
    {
        User *curr = push_user(NEW, tail);

        User_data *datahead = (User_data*)malloc(sizeof(User_data));
        User_data *datatail = (User_data*)malloc(sizeof(User_data));

        NEW.datas = datahead;
        datahead ->prev = NULL;
        datahead ->next = datatail;
        datatail ->prev = datahead;
        datatail ->next = NULL;

        curr->datas = datahead;
        User_data elemek;
        while (fscanf(fp,",#%d,%d",&elemek.ID, &elemek.seen)==2)
        {
            fesulista(elemek, datahead, datatail);
        }
        fscanf(fp,"\n");
    }
    *users_head = head;
    *users_tail = tail;
    if (fclose(fp) != 0)
    {
        printf("File close error\n");
        return;
    }
}




void users_kilistazasa(User *users_head)
{
    printf("\nLétező profilok:\n\n");
    User *curr = users_head ->next;

    while(curr ->next != NULL)                                                       
    {
        printf("%s\n", curr ->USER);
        curr = curr->next;
    }
}

int user_valid(User *users_head, char nevbuffer[])
{
    User *curr = users_head ->next;
    while (curr ->next !=NULL)
    {
        if(strcmp(nevbuffer, curr->USER)==0) return 1;
        curr == curr ->next;
    }
    return 0;
}

//void anime_pop();

void pop_user(User *users_head, char nevbuffer[])
{
    User *curr = users_head->next;
    
    while (strcmp(nevbuffer, curr ->USER)==0)
    {
        curr = curr ->next;
    }
    
    curr ->prev ->next = curr -> next;
    curr ->next ->prev = curr ->prev;
    //anime_pop();
    free(curr);
}



void user_add(User *users_head)
{
    printf("\nMi legyen a felhasználó neved?\n(maximum 25 karakter és vessző nem lehet benne)\n");
    char nev[26];
    scanf("%s", nev);
}

void user_del(User *users_head)
{
    printf("\nMelyik felhasznlót szeretnéd törölni (BACK)?\n");
    users_kilistazasa(users_head);
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    if (user_valid(users_head, nevbuffer) == 0) 
    {
        printf("Nem valid felhasználó!");
        return;
    }
    printf("\nBiztos? (y/n)\n");
    char sure = '0';
    while(sure !='n' || sure != 'y')
    {
        scanf("%c", &sure);
    }
    if (sure == 'n') return;
    if (sure == 'y') pop_user(users_head, nevbuffer);
}

void user_edit(User *users_head)
{
    users_kilistazasa(users_head);
}

void user_login(User *users_head)
{
    users_kilistazasa(users_head);
}




void print_menu(void) {
    printf("\nMit szeretnél csinálni?\n\n");
    printf("\t%d. Felhasználó létrehozása\n", ADD);
    printf("\t%d. Felhasználó törlése\n", DEL);
    printf("\t%d. Felhasználónév módosítása\n", EDIT);
    printf("\t%d. Felhasználó létrehozása\n", LOGIN);
    printf("\t%d. Kilépés\n", EXIT);
}

void user_menu(User *users_head)
{
int selection = 0;
    while (selection != EXIT)
    {
        print_menu();
        scanf("%d", &selection);

        switch (selection)
        {
            case ADD: user_add(users_head); break;
            case DEL: user_del(users_head); break;
            case EDIT: user_edit(users_head); break;
            case LOGIN: user_login(users_head); break;
            case EXIT: printf("Viszlát!"); return;
            default:printf("Unknown option.\n"); break;
        }
    }
}


int main(void)
{
    User *users_head;
    User *users_tail;
    userdata2list(&users_head, &users_tail);

    Anime *animes_head;
    Anime *animes_tail;
    anime2list(&animes_head, &animes_tail);

    //user_menu(users_head);
    printf("\n\n%s %d %d", users_head->next->USER, users_head->next->datas->next->ID, users_head->next->datas->next->seen);
    return 0;
}
