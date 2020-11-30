#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef enum MUFALY {Action,Adventure,Comedy,Drama,Slice_of_Life,Fantasy,Magic,Supernatural,Horror,Mystery,Psychological,Romance,SciFi}MUFALY;
typedef enum EVSZAK {SPRING,SUMMER,FALL,WINTER}EVSZAK;
typedef enum ALAP   {Light_novel,Visual_novel,Manga,Muisc}ALAP;
typedef enum COMMAND {ADD = 1,DEL,EDIT,LOGIN,EXIT}COMMAND;       //0, 1, 2, 3, 4
typedef enum ANIMCOMM {HOZZAAD = 1,TOROL,PLUS,KILEP}ANIMCOMM;


typedef struct Anime
{
    int ID;
    char name[26];
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
User *push_user(User UNEW, User *utail)
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


Anime *anime_search(int ID, Anime *animes_head)
{
    Anime *curr = animes_head->next;
    while(curr->ID != ID)
    {
        curr = curr ->next;
    }
    return curr;
}

int most_pop_anim(User *users_head,  Anime *animes_head)
{
    Anime *animcurr = animes_head ->next;
    int db = 0;
    while(animcurr->next != NULL)       //?db anime van
    {
        db++;
        animcurr = animcurr ->next;
    }


    int *nepszeruseg = (int*) malloc(db * sizeof(int)); // tomb amiben benne van, hogy melyik animet hányan látták, id szerint

    for (int i = 0; i < db; i++)                        // tomb kinullázása
    {
        nepszeruseg[i] = 0;
    }


    User *usercurr = users_head->next;
    animcurr = animes_head;

    while(usercurr ->next != NULL)          //minden felhasználón végigmenni
    {
        User_data *udcurr = usercurr ->datas ->next;

        while (udcurr ->next != NULL)       //minden animén végigmenni, amit láttak
        {
            for (int i = 0; i < db; i++)       //adatot listába tölteni
            {
                if(i == udcurr ->ID)
                    nepszeruseg[i] += 1;
            }
            udcurr = udcurr ->next;
        }
    usercurr = usercurr ->next;
    }

    int maxseen = 0;
    int maxindex = 0;
    for (int i = 0; i < db; i++)            // max megkeresése
    {
        if (nepszeruseg[i] > maxseen)
        {
            maxindex = i;
            maxseen = nepszeruseg[i];
        }
    }
    return maxseen;
}

void print_animes(User *name, User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    User_data *curr_user = name->datas->next;

    while(curr_user ->next != NULL)
    {
        Anime * curr_anime = anime_search(curr_user->ID, animes_head);
        printf("#%4d %25s %25s %15d %15d %d %10d %4d %4d\n", curr_user->ID, curr_anime->name, curr_anime->studio, curr_anime->genre, curr_anime->base, curr_anime->year, curr_anime->season, curr_anime->EP, curr_user->seen);
        curr_user = curr_user->next;
    }
        int pop_anim = most_pop_anim(users_head, animes_head);
        Anime *mostpop = anime_search(pop_anim, animes_head);
        printf("\nA legnépszerübb anime: %s", mostpop->name);
    return;
}
void print_animemenu(void) {
    printf("\t%d. Anmie hozzáadása\n", HOZZAAD);
    printf("\t%d. Anime levétele a listáról\n", TOROL);
    printf("\t%d. Anime levétele a listáról\n", PLUS);
    printf("\t%d. Kilépés\n", KILEP);
}

Anime* fiftypercent(char nevbuffer[], Anime *animes_head)
{
    Anime *curr = animes_head->next;

    while(curr->next != NULL)
    {
        int same = 0;
        char animebuff[26];
        strcpy(animebuff, curr->name);
        int a = strlen(nevbuffer);
        int b = strlen(animebuff);
        if(b < a) a=b;
        for (int i = 0; i < a; i++)
        {
            if(animebuff[i] == nevbuffer[i])
                same++;
        }
        if (a/2 <= same)
            return curr;
        curr = curr ->next;
    }
    return NULL;
}

void lista_add()
{

}

void anime_beszur()
{
    
}

void anime_add(Anime *animes_head)
{
    printf("Mi az anime címe, amit bele szeretnél rakni a listádba? (Max 25 karakter, és nem lehet benne vessző)");
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    Anime *ajanlas = fiftypercent(nevbuffer, animes_head);
    if (ajanlas != NULL)
    {
        printf("%s-re gondoltál? (1/0)", ajanlas ->name);
        int logikai;
        scanf("%d", &logikai);
        // if (logikai == 1)
        //     lista_add();
    }
    // anime_beszur();
    return;
}

void anime_poplist(User_data *dataelem)
{
    User_data *kov = dataelem ->next;
    User_data *elozo = dataelem ->prev;
    elozo ->next = kov;
    kov ->prev = elozo;
    free(dataelem);
    return;
}
void anime_del(User *name)
{
    int torlendo;
    printf("Melyik sorszámú animét szeretnéd kitörölni a listádból? ");
    scanf("%d", &torlendo);

    User_data *curr = name ->datas->next;
    while (curr ->next != NULL)
    {
        if(curr->ID == torlendo)
        {
            anime_poplist(curr);
            return;
        }
        curr = curr ->next;
    }
    printf("Te nem láátál ilyen animét!");
    return;
}

void anime_plus(User *name)
{
    int id;
    int latott;
    printf("Melyik számú animéből láttál?");
    scanf("%d", &id);
    int talal = 0;
    User_data *curr = name ->datas->next;
    while(curr ->next != NULL)
    {
        if(curr->ID == id)
        {
            talal = 1;
            break;
        }
        curr = curr->next;
    }
    if (talal == 0)
    {
        printf("Te nem láttál ilyen animét!");
        return;
    }
    printf("Hány részt láttál belőle?");
    scanf("%d", &latott);

    curr = name ->datas->next;
    while(curr ->next != NULL)
    {
        if(curr->ID == id)
        {
            curr->seen = latott;
            break;
        }
        curr = curr->next;
    }
    return;
}


void anime_menu(User *name, User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    print_animes(name, users_head, users_tail, animes_head, animes_tail);

    int selection = 0;
    while (selection != EXIT)
    {
        print_animemenu();
        scanf("%d", &selection);

        switch (selection)
        {
            case HOZZAAD: anime_add(animes_head); break;
            case TOROL: anime_del(name); break;
            case PLUS: anime_plus(name); break;
            case KILEP: printf("\nKilpés a felhasználókhoz...\n"); return;
            default:printf("Unknown option.\n"); break;
        }
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

void pop_user(User *users_head, char nevbuffer[])
{
    User *curr = users_head->next;

    while (strcmp(nevbuffer, curr ->USER)==0)
    {
        curr = curr ->next;
    }

    curr ->prev ->next = curr -> next;
    curr ->next ->prev = curr ->prev;
    free(curr);
}

User *user_search(User *users_head, char nevbuffer[])
{
    User *curr = users_head->next;

    while (curr->next != NULL)
    {
        if(strcmp(curr->USER, nevbuffer)==0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}


void user_add(User *users_head, User *users_tail)
{
    printf("\nMi legyen a felhasználó neved?\n(maximum 25 karakter és vessző meg ékezet nem lehet benne)\n");
    User nevbuff;
    scanf("%s", nevbuff.USER);

    User *felhasznalo = push_user(nevbuff, users_tail);

    User_data *datahead = (User_data*)malloc(sizeof(User_data));
    User_data *datatail = (User_data*)malloc(sizeof(User_data));
    datahead ->prev = NULL;
    datahead ->next = datatail;
    datatail ->prev = datahead;
    datatail ->next = NULL;
    felhasznalo ->datas = datahead;

    users_kilistazasa(users_head);
    return;
}

void user_del(User *users_head)
{
    printf("\nMelyik felhasznlót szeretnéd törölni (BACK)?\n");
    users_kilistazasa(users_head);
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    if (user_search(users_head, nevbuffer) == NULL)
    {
        printf("Nem valid felhasználó!");
        return;
    }
    printf("\nBiztos? (1/0)\n");
    int sure;
    while(scanf("%d", &sure)==1)
    {
        if(sure !=1 || sure != 0) break;
    }
    if (sure == 1) return;
    if (sure == 0) pop_user(users_head, nevbuffer);
}

void user_edit(User *users_head)
{
    users_kilistazasa(users_head);
    printf("\nMelyik felhasznló nevét szeretnéd módosítani?\n");
    int i;
    char melyik[26];
    scanf("%s", melyik);
    User *valid = user_search(users_head, melyik);
    if (valid == NULL) return;

    printf("Biztos? (1/0)");
    while(scanf("%d",&i))
    {
        if(i==0) return;
        if(i==1) break;
    }
    printf("\nMire szeretnéd módosítani? (Vessző és ékezet nem lehet benne és maximum 25 karakter lehet)\n");
    char nevbuffer[26];
    scanf("%s",nevbuffer);
    strcpy(valid ->USER, nevbuffer);
    return;
}

void user_login(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    users_kilistazasa(users_head);
    printf("Melyik felhasználóba szeretnél bejelentkezni?");
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    User *name = user_search(users_head, nevbuffer);
    if(name == NULL)
    {
        printf("Ilyen felhasználónév nem található a rendszerben!");
        return;
    }
    anime_menu(name, users_head, users_tail, animes_head, animes_tail);
}




void print_usermenu(void) {
    printf("\nMit szeretnél csinálni?\n\n");
    printf("\t%d. Felhasználó létrehozása\n", ADD);
    printf("\t%d. Felhasználó törlése\n", DEL);
    printf("\t%d. Felhasználónév módosítása\n", EDIT);
    printf("\t%d. Login\n", LOGIN);
    printf("\t%d. Kilépés\n", EXIT);
}

void user_menu(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
int selection = 0;
    while (selection != EXIT)
    {
        print_usermenu();
        scanf("%d", &selection);

        switch (selection)
        {
            case ADD: user_add(users_head, users_tail); break;
            case DEL: user_del(users_head); break;
            case EDIT: user_edit(users_head); break;
            case LOGIN: user_login(users_head, users_tail, animes_head, animes_tail); break;
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

    user_menu(users_head, users_tail, animes_head, animes_tail);
    //save_data(users_head, users_tail, animes_head, animes_tail);
    print_animes(users_head->next, users_head, users_tail, animes_head, animes_tail);
    return 0;
}
