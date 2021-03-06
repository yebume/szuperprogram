
//A file az alabbi modon vannak beallitva: "NAGYHAZI/xyz.txt" !!!
// Admin módba belépni akkor lehet ha logineltél és beírod hogy 42

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef enum MUFALY {Action,Adventure,Comedy,Drama,Slice_of_Life,Fantasy,Magic,Supernatural,Horror,Mystery,Psychological,Romance,Sci_Fi}MUFALY;
typedef enum EVSZAK {SPRING,SUMMER,FALL,WINTER}EVSZAK;
typedef enum ALAP   {Light_novel,Visual_novel,Manga,Muisc}ALAP;
typedef enum COMMAND {ADD = 1,DEL,EDIT,LOGIN,EXIT}COMMAND;       //0, 1, 2, 3, 4
typedef enum ANIMCOMM {HOZZAAD = 1,TOROL,PLUS,KILEP,ADMINMENU=42}ANIMCOMM;
typedef enum ADMIN {DELETE, OUT}ADMIN;

const char* mufaly(MUFALY x) 
{
   switch (x) 
   {
    case Action: return "Action";
    case Adventure: return "Adventure";
    case Comedy: return "Comedy";
    case Drama: return "Drama";
    case Slice_of_Life: return "Slice_of_Life";
    case Fantasy: return "Fantasy";
    case Magic: return "Magic";
    case Supernatural: return "Supernatural";
    case Horror: return "Horror";
    case Mystery: return "Mystery";
    case Psychological: return "Psychological";
    case Romance: return "Romance";
    case Sci_Fi: return "Sci_Fi";
   }
}
const char* evszak(EVSZAK x) 
{
   switch (x) 
   {
      case SPRING: return "SPRING";
      case SUMMER: return "SUMMER";
      case FALL: return "FALL";
      case WINTER: return "WINTER";
   }
}
const char* alap(ALAP x) 
{
   switch (x) 
   {
      case Light_novel: return "Light_novel";
      case Visual_novel: return "Visual_novel";
      case Manga: return "Manga";
      case Muisc: return "Muisc";
   }
}

typedef struct Anime
{
    int ID;
    char name[31];
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
    head ->ID = 0;
    tail ->ID = 0;
    head ->next = tail;
    tail ->prev = head;
    tail ->next = NULL;
    head ->prev = NULL;
    Anime NEW;
    while(fscanf(fpa,"#%d,%[^,],%[^,],%d,%d,%d,%d,%d\n",&NEW.ID, NEW.name, NEW.studio, &NEW.genre, &NEW.base, &NEW.year, &NEW.season, &NEW.EP)==8)
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

int anime_validity(int sorszam,Anime* animes_head)
{
    Anime *curr = animes_head->next;
    while (curr ->next != NULL)
    {
        if (curr ->ID == sorszam)
            return 1;
        curr = curr->next;
    }
    return 0;
}
Anime *anime_search(int id, Anime *animes_head)
{
    Anime *curr = animes_head->next;
    while(curr->ID != id)
    {
        curr = curr ->next;
    }
    return curr;
}
int most_pop_anim(User *users_head,  Anime *animes_head, Anime *animes_tail)
{
    Anime *animcurr = animes_head ->next;

    int db = animes_tail->prev->ID;

    int *nepszeruseg = (int*) malloc(db * sizeof(int)); // tomb amiben benne van, hogy melyik animet hanyan lattak, id szerint

    for (int i = 0; i < db; i++)                        // tomb kinullazasa
    {
        nepszeruseg[i] = 0;
    }


    User *usercurr = users_head->next;
    animcurr = animes_head ->next;

    while(usercurr ->next != NULL)          //minden felhasznalon vegigmenni
    {
        User_data *udcurr = usercurr ->datas ->next;

        while (udcurr ->next != NULL)       //minden animen vegigmenni, amit lattak
        {
            for (int i = 0; i < db; i++)       //adatot listaba tolteni
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
    for (int i = 0; i < db; i++)            // max megkeresese
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
    printf("\n\n");
    printf("#  ID                     Anime neve                    Studio          Mufaly            Alap   Ev     Evszak   EP Latott\n");
    while(curr_user ->next != NULL)
    {
        Anime * curr_anime = anime_search(curr_user->ID, animes_head);
        printf("#%4d %30s %25s %15s %15s %4d %10s %4d   %4d\n", curr_user->ID, curr_anime->name, curr_anime->studio, mufaly(curr_anime->genre), alap(curr_anime->base), curr_anime->year, evszak(curr_anime->season), curr_anime->EP, curr_user->seen);
        curr_user = curr_user->next;
    }
        int pop_anim = most_pop_anim(users_head, animes_head, animes_tail);
        Anime *mostpop = anime_search(pop_anim, animes_head);
        printf("\nA legnepszerubb anime: %s\n\n", mostpop->name);
    return;
}
void print_animemenu(void) {
    printf("\t%d. Anmie hozzaadasa\n", HOZZAAD);
    printf("\t%d. Anime levetele a listarol\n", TOROL);
    printf("\t%d. Anime latott reszek allitasa\n", PLUS);
    printf("\t%d. Kilepes\n", KILEP);
}

void print_mufalyok()
{

    printf("\t%d. Action\n", Action);
    printf("\t%d. Adventure\n", Adventure);
    printf("\t%d. Comedy\n", Comedy);
    printf("\t%d. Drama\n", Drama);
    printf("\t%d. Slice_of_Life\n", Slice_of_Life);
    printf("\t%d. Fantasy\n", Fantasy);
    printf("\t%d. Magic\n", Magic);
    printf("\t%d. Supernatural\n", Supernatural);
    printf("\t%d. Horror\n", Horror);
    printf("\t%d. Mystery\n", Mystery);
    printf("\t%d. Psychological\n", Psychological);
    printf("\t%d. Romance\n", Romance);
    printf("\t%d. Sci_Fi\n", Sci_Fi);
    return;
}
void print_alapok()
{
    printf("\t%d. Light_novel\n", Light_novel);
    printf("\t%d. Visual_novel\n", Visual_novel);
    printf("\t%d. Manga\n", Manga);
    printf("\t%d. Muisc\n", Muisc);
}
void print_evszakok()
{
    printf("\t%d. SPRING\n", SPRING);
    printf("\t%d. SUMMER\n", SUMMER);
    printf("\t%d. FALL\n", FALL);
    printf("\t%d. WINTER\n", WINTER);
}

int newid(Anime *animes_head, Anime *animes_tail)
{
    Anime *curr = animes_head;
    while (curr ->next ->next != NULL)
    {
        if(curr ->ID +1 != curr ->next ->ID)
            return (curr ->ID + 1);
        curr = curr ->next;
    }
    return animes_tail->prev->ID + 1;
}

void lista_add(User *name, Anime *animenev)
{
    User_data *curr = name ->datas;

    while (curr->next != NULL && curr ->ID < animenev ->ID)
    {
        curr = curr ->next;
    }

    curr = curr ->prev;
    User_data *newp = (User_data*)malloc(sizeof(User_data));
    newp ->ID = animenev ->ID;
    newp ->seen = 0;

    newp ->next = curr->next;
    newp ->prev = curr;
    curr ->next ->prev = newp;
    curr ->next =newp;
    return;
}
void anime_beszur(User *name, Anime *animes_head, Anime *animes_tail)
{
    int newID = newid(animes_head, animes_tail);

    printf("Mi az anime neve? (max 30 karakter es nem lehet benne vesszo)\n");
    char newname[31];
    scanf("%s",newname);

    printf("Melyik stodio adta ki az animet? (max 25 karakter es nem lehet benne vesszo)\n");
    char newstudio[26];
    scanf("%s",newstudio);
    printf("Mi az anime mufaja?\n");
    int newgenre = -1;
    while  (newgenre < 0 || 12 < newgenre)
    {
        print_mufalyok();
        scanf("%d", &newgenre);
    }

    printf("Min alapul az anime?\n");
    int newbase = -1;
    while(newbase < 0 || 3 < newbase)
    {
        print_alapok();
        scanf("%d", &newbase);
    }

    printf("Mikor jelent meg?\n");
    int newyear;
    scanf("%d", &newyear);

    printf("Melyik evszakban jelent meg?\n");
    int newseason;
    while(newseason < 0 || 3 < newseason)
        {
            print_evszakok();
            scanf("%d", &newseason);
        }

    printf("Hany epizodja van?\n");
    int newEP;
    scanf("%d", &newEP);


    Anime *newp = (Anime*)malloc(sizeof(Anime));
    newp ->ID = newID;
    strcpy(newp ->name, newname);
    strcpy(newp ->studio, newstudio);
    newp ->genre = newgenre;
    newp ->base = newbase;
    newp ->year = newyear;
    newp ->season = newseason;
    newp ->EP = newEP;



    Anime* behelyez = animes_head;
    while ((behelyez ->next != NULL) && (behelyez ->ID + 1 == behelyez ->next ->ID))
    {
        behelyez = behelyez ->next;
    }
    
    newp ->next = behelyez ->next;
    newp ->prev = behelyez;
    behelyez ->next ->prev = newp;
    behelyez ->next = newp;
    return;
}

void anime_add(User *name, Anime *animes_head, Anime *animes_tail)
{
    Anime *curr = animes_head ->next;
    printf("\n\n\n");
    printf("#  ID                     Anime neve                    Studio          Mufaly            Alap   Ev     Evszak   EP\n");
    while (curr ->next != NULL)
    {
        printf("#%4d %30s %25s %15s %15s %4d %10s %4d\n", curr->ID, curr->name, curr->studio, mufaly(curr->genre), alap(curr->base), curr->year, evszak(curr->season), curr->EP);
        curr = curr ->next;
    }
    printf("\n\nItt van az anime amit keresel?(1/0)\n");
    int yes;
    scanf("%d", &yes);
    if (yes == 1)
    {
        printf("Mi a sorrszama?\n");
        int sorszam;
        scanf("%d",&sorszam);

        if (anime_validity(sorszam, animes_head) == 0)
        {
            printf("\nIlyen anime nincsen!\n");
            return;
        }

        Anime* animenev = anime_search(sorszam, animes_head);
        lista_add(name ,animenev);
        return;
    }
    anime_beszur(name, animes_head, animes_tail);
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
    printf("Melyik sorszamu animet szeretned kitorolni a listadbol?\n");
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
    printf("\nTe nem lattal ilyen animet!\n\n");
    return;
}
void anime_plus(User *name)
{
    int id;
    int latott;
    printf("Melyik szamu animebol lattal?\n");
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
        printf("\nTe nem lattal ilyen animet!\n");
        return;
    }
    printf("Hany reszt lattal belole?\n");
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


void admin_del(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    Anime *curr = animes_head ->next;

    printf("\n\n\n");
    while (curr ->next != NULL)
    {
        printf("#%4d %30s %25s %15d %15d %d %10d %4d\n", curr->ID, curr->name, curr->studio, curr->genre, curr->base, curr->year, curr->season, curr->EP);
        curr = curr ->next;
    }
    printf("\n\nMit?\n");
    int n;
    scanf("%d", &n);

    User *currentuser = users_head ->next;
    while(currentuser ->next != NULL)
    {
        User_data *currentdata = currentuser ->datas ->next;
        while (currentdata ->next != NULL)
        {
            if(n == currentdata ->ID)
            {
                currentdata ->prev ->next = currentdata ->next;
                currentdata ->next ->prev = currentdata ->prev;
                free(currentdata);
                break;
            }
            currentdata = currentdata ->next;
        }
        currentuser = currentuser->next;
    }

    Anime *animcurr = animes_head;
    while (animcurr ->next != NULL && animcurr != NULL)
    {
        if (animcurr ->ID == n)
        {
            animcurr ->prev ->next = animcurr ->next;
            animcurr ->next ->prev = animcurr ->prev;
            free(animcurr);
            return;
        }
        animcurr = animcurr ->next;
    }
    
    return;
}
void print_adminmenu()
{
    printf("\t%d. DEL\n", DELETE);
    printf("\t%d. EXIT\n", OUT);
    
}
void admin(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    int selection = 0;
    while (selection != EXIT)
    {
        print_adminmenu();
        scanf("%d", &selection);

        switch (selection)
        {
            case DELETE: admin_del(users_head, users_tail, animes_head, animes_tail); break;
            case OUT: return;
            default:printf("Unknown option.\n"); break;
        }
    }
}

void anime_menu(User *name, User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    int selection = 0;
    while (selection != EXIT)
    {
        print_animes(name, users_head, users_tail, animes_head, animes_tail);
        print_animemenu();
        scanf("%d", &selection);

        switch (selection)
        {
            case HOZZAAD: anime_add(name, animes_head, animes_tail); break;
            case TOROL: anime_del(name); break;
            case PLUS: anime_plus(name); break;
            case ADMINMENU: admin(users_head, users_tail, animes_head, animes_tail); break;
            case KILEP: printf("\nKilpes a felhasznalokhoz...\n"); return;
            default:printf("Unknown option.\n"); break;
        }
    }
}


void users_kilistazasa(User *users_head)
{
    printf("\nLetezo profilok:\n\n");
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

    while (strcmp(nevbuffer, curr ->USER)!=0)
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
void pop_data(User *users_head, char nevbuffer[])
{
    User *curr = users_head->next;

    while (strcmp(nevbuffer, curr ->USER)==0)
    {
        curr = curr ->next;
    }

    User_data *current = curr ->datas;
    User_data *nextdata;

    while(current->next != NULL)
    {
        nextdata = current ->next;
        free(current);
        current = nextdata;
    }
    curr ->datas = NULL;
    return;
    
}

void user_add(User *users_head, User *users_tail)
{
    printf("\nMi legyen a felhasznalo neved?\n(maximum 25 karakter es vesszo meg ekezet nem lehet benne)\n");
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
    printf("\n\nMelyik felhasznlot szeretned torolni?\n\n");
    users_kilistazasa(users_head);
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    if (user_search(users_head, nevbuffer) == NULL)
    {
        printf("\nNem valid felhasznalo!\n");
        return;
    }
    printf("\nBiztos? (1/0)\n");
    int sure;

    scanf("%d", &sure);

    if (sure == 0) return;
    pop_data(users_head, nevbuffer);
    pop_user(users_head, nevbuffer);
    printf("\n");
    return;
}
void user_edit(User *users_head)
{
    users_kilistazasa(users_head);
    printf("\nMelyik felhasznlo nevet szeretned modositani?\n");
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
    printf("\nMire szeretned modositani? (Vesszo es ekezet nem lehet benne es maximum 25 karakter lehet)\n");
    char nevbuffer[26];
    scanf("%s",nevbuffer);
    strcpy(valid ->USER, nevbuffer);
    return;
}
void user_login(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    users_kilistazasa(users_head);
    printf("\nMelyik felhasznaloba szeretnel bejelentkezni?\n");
    char nevbuffer[26];
    scanf("%s", nevbuffer);
    User *name = user_search(users_head, nevbuffer);
    if(name == NULL)
    {
        printf("Ilyen felhasznalonev nem talalhato a rendszerben!\n\n");
        return;
    }
    anime_menu(name, users_head, users_tail, animes_head, animes_tail);
}

void save_data(User *users_head, User *users_tail, Anime *animes_head, Anime *animes_tail)
{
    FILE *animek =  fopen("NAGYHAZI/animek.txt", "w");
    if (animek == NULL)
    {
        printf("File open error\n");
        return;
    }

    FILE *profilok =  fopen("NAGYHAZI/profilok.txt", "w");
    if (profilok == NULL)
    {
        printf("File open error\n");
        return;
    }

    Anime *curranime = animes_head ->next;
    while (curranime ->next != NULL)
    {
        fprintf(animek, "#%d,%s,%s,%d,%d,%d,%d,%d\n", curranime ->ID, curranime ->name, curranime ->studio, curranime ->genre, curranime ->base, curranime ->year, curranime->season, curranime ->EP);
        curranime = curranime ->next;
    }
    
    User *curruser = users_head ->next;
    while (curruser ->next != NULL)
    {
        fprintf(profilok, "%s", curruser ->USER);
        User_data *currdata = curruser ->datas ->next;
        while (currdata ->next != NULL)
        {
             fprintf(profilok,",#%d,%d", currdata ->ID, currdata ->seen);
            currdata = currdata ->next;
        }
        fprintf(profilok,"\n");
        curruser = curruser ->next;
    }
    fclose(animek);
    fclose(profilok);
    return;
}


void print_usermenu(void) {
    printf("\nMit szeretnel csinalni?\n\n");
    printf("\t%d. Felhasznalo letrehozasa\n", ADD);
    printf("\t%d. Felhasznalo torlese\n", DEL);
    printf("\t%d. Felhasznalonev modositasa\n", EDIT);
    printf("\t%d. Login\n", LOGIN);
    printf("\t%d. Kilepes\n", EXIT);
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
            case EXIT: printf("Viszlat!"); save_data(users_head, users_tail, animes_head, animes_tail); return;
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


    return 0;
}
