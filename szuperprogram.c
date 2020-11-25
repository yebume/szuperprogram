#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef enum MUFALY {Action,Adventure,Comedy,Drama,Slice_of_Life,Fantasy,Magic,Supernatural,Horror,Mystery,Psychological,Romance,SciFi}MUFALY;
typedef enum EVSZAK {SPRING,SUMMER,FALL,WINTER}EVSZAK;
typedef enum ALAP   {Light_novel,Visual_novel,Manga,Muisc}ALAP;



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



User *users_head;
User *users_tail;
Anime *animes_head;
Anime *animes_tail;



void push_anime(Anime NEW, Anime *tail)
{
    Anime *newp = (Anime*)malloc(sizeof(Anime));
    newp ->next = tail ->prev ->next;
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

void anime2list(void)
{
    FILE *fp =  fopen("animek.txt", "r");
    if (fp == NULL) return;
    Anime *head = (Anime*)malloc(sizeof(Anime));
    Anime *tail = (Anime*)malloc(sizeof(Anime));
    head ->next = tail;
    tail ->prev = head;
    tail ->next = NULL;
    head ->prev = NULL;
    Anime NEW;
    while(fscanf(fp,"#%d,%[^,],%[^,],%d,%d,%d,%d,%d",&NEW.ID, NEW.name, NEW.studio, &NEW.genre, &NEW.base, &NEW.year, &NEW.season, &NEW.EP)==8)
    {
        push_anime(NEW, tail);
    }
    animes_head = head;
    animes_tail = tail;
}



void fesulista(User_data elemek, User_data *datahead, User_data *datatail)
{
    User_data *listelem = (User_data*)malloc(sizeof(User_data));   
    listelem ->ID = elemek.ID;
    listelem ->seen = elemek.seen;

    listelem ->next = datatail;
    listelem ->prev = datatail ->prev;
    datatail ->prev ->next = listelem;
    datatail ->prev = listelem;
}

void push_user(User UNEW, User *utail)
{
    User *newp = (User*)malloc(sizeof(User));


    newp ->next = utail;
    newp ->prev = utail ->prev;
    utail ->prev ->next = newp;
    utail ->prev = newp;

    strcpy(newp ->USER, UNEW.USER);

    FILE *fptr =  fopen("profilok.txt", "r");
    if (fptr == NULL) return;

    User_data elemek;
    User_data *datahead = (User_data*)malloc(sizeof(User_data));
    User_data *datatail = (User_data*)malloc(sizeof(User_data));

    newp ->datas = datahead;

    datahead ->prev = NULL;
    datahead ->next = datatail;
    datatail ->prev = datahead;
    datatail ->next = NULL;

    while (fscanf(fptr,"#%d,%d",&elemek.seen, &elemek.ID))
    {
        fesulista(elemek, datahead, datatail);
    }

    datahead ->next ->prev = NULL;
    newp ->datas =datahead ->next;
    free(datahead);
    datatail ->prev -> next = NULL;
    free(datahead);
}

void userdata2list(void)
{
    FILE *fp =  fopen("profilok.txt", "r");
    if (fp == NULL) return;
    User *head = (User*)malloc(sizeof(User));
    User *tail = (User*)malloc(sizeof(User));

    head ->next = tail;
    tail ->prev = head;

    tail ->next = NULL;
    head ->prev = NULL;

    User NEW;

    while(fscanf(fp,"%[^,]", NEW.USER)==1)
    {
        push_user(NEW, tail);
    }
    users_head = head;
    users_tail = tail;
}

void users_kilistazasa(void)
{
    printf("Létező profilok:\n");
    User *curr = users_head ->next;

    while(curr ->next != NULL)                                                       
    {
        printf("%s ", curr ->USER);
        curr = curr->next;
    }
}



void user_add()
{
    printf("Mi legyen a felhasználó neved?\n(maximum 25 karakter és vessző nem lehet benne)\n");
    char nev[26];
    scanf("%s", nev);
}

void user_del()
{

}

void user_edit()
{

}

void user_login()
{

}


void user_menu(void)
{
    typedef enum COMMAND {add,del,edit,login,exit}COMMAND;       //0, 1, 2, 3, 4
    printf("Mit szeretnél csinálni? (add, del, edit, login, exit)");
    COMMAND mi = -1;
    while (mi!=0 || mi!=1 || mi!=2 || mi!=3 || mi!=4)
    {
        scanf("%d", &mi);
        if (mi!=0 || mi!=1 || mi!=2 || mi!=3 || mi!=4)
            printf("Kérlek írj be valid kommandot");
    }

    switch (mi)
    {
        case add: user_add(); break;
        case del: user_del(); break;
        case edit: user_edit(); break;
        case login: user_login(); break;
        case exit: return;
        default: break;
    }
}



int main(void)
{
    userdata2list();
    anime2list();
    users_kilistazasa();
    printf("%s", users_head->USER);
    //user_menu();
}