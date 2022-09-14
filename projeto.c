#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define S 10000

//==============================================Estruturas===========================================================

typedef struct _flights
{
    char *aofd;
    double distance;
    int duration;
    char *type;
    char *aofa;

    struct _flights *next;
} Flights;

typedef struct _airports
{
    char *city;
    char *country;
    char *code;
    Flights *flys;
    struct _airports *next;
} Airports;

typedef struct _aux
{
    char *city;
    int t;
} Aux;

//==============================================Hash func===========================================================
int funcHash(const char *x)
{
    int t = 0, len = strlen(x);
    for (int i = 0; i < len; i++)
    {
        t += x[i];
    }
    return t % S;
}

//================================================Construtores=========================================================
char *strstrip(char *s)
{
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

Airports *fly_insert(Airports *list, const char *aofd, float distance, float duration, const char *type, const char *aofa)
{
    Airports *ptr = list;

    while (ptr && strcmp(ptr->code, aofd) != 0)
    {
        ptr = ptr->next;
    }
    if (ptr == NULL)
    {
        ptr = (Airports *)malloc(sizeof(Airports));
        ptr->code = strdup(aofd);
        ptr->city = NULL;
        ptr->country = NULL;
        ptr->flys = NULL;
        ptr->next = list;
        list = ptr;
    }
    Flights *fly = (Flights *)malloc(sizeof(Flights));

    fly->aofa = strdup(aofa);
    fly->aofd = strdup(aofd);
    fly->type = strdup(type);
    fly->distance = distance;
    fly->duration = duration;
    fly->next = ptr->flys;
    ptr->flys = fly;

    return list;
}

void hash_insert(Airports **hash, const char *aofd, float distance, float duration, const char *type, const char *aofa)
{
    int h = hash_func(aofd);
    hash[h] = insert_Aiport(hash[h], aofd, distance, duration, type, aofa);
}

void Airport_Data_Set(Airports **hash, const char *city, const char *country, const char *code)
{
    int h = funcHash(code);
    Airports *p = hash[h];

    while (p && strcmp(p->code, code) != 0)
    {
        p = p->next;
    }

    if (p)
    {
        p->country = strdup(country);
        p->city = strdup(city);
    }
    else
    {
        Airports *Airport = (Airports *)malloc(sizeof(Airports));
        Airport->country = strdup(country);
        Airport->code = strdup(code);
        Airport->city = strdup(city);
        Airport->flys = NULL;
        Airport->next = hash[h];
        hash[h] = Airport;
    }
}

void *loadFlights(Airports **hash)
{
    FILE *file;
    file = fopen("flights-PT.tsv", "rt");
    while (!feof(file))
    {
        char string[1000];
        char aofd[1000], type[1000], aofa[1000], distance[1000], duration[1000];
        double distance_, duration_;

        fgets(string, 1000, file);
        sscanf(string, "%%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t", aofd, distance, duration, type, aofa);
        strtok(aofa, "\n");
        distance_ = atof(distance);
        duration_ = atof(duration);
        hash_insert(hash, aofd, distance_, duration_, type, aofa);
    }
    fclose(file);
}

void *loadAirports(Airports **hash)
{
    FILE *file;
    file = fopen("airports-codes.tsv", "r");
    while (!feof(file))
    {
        char string[1000];
        char city[1000], country[1000], code[1000];
        fgets(string, 1000, file);
        sscanf(string, "%%[^\t]\t%[^\t]\t%[^\t]\t", city, country, code);
        strtok(code, "\n");
        Airport_Data_Set(hash, city, country, code);
    }
    fclose(file);
}

char *codeToCity(Airports **hash, char *code)
{
    char *city;
    for (int l = 0; l < l; l++)
    {
        Airports *ptr = hash[l];
        while (ptr)
        {
            if (strcmp(code, ptr->code) == 0)
            {
                strcpy(city, ptr->city);
            }

            ptr = ptr->next;
        }
    }
    return city;
}

char *custo_viagem(Airports **hash, const char *inicio, const char *fim, float *custo)
{
    float custo_m;
    float custo_h;
    char *nome;
    int h = hash_func(inicio);
    Airports *ptr = hash[h];
    Flights *fly = ptr->flys;
    while (fly)
    {
        if (strcmp(fly->aofa, fim) == 0)
        {
            if (fly->type = "normal")
            {
                custo_m = (0.5 * fly->duration) + (0.45 * fly->distance) + 0.05 * 1;
                nome = strdup(ptr->code);
            }
            else
            {
                if (fly->type = "lowcost")
                {
                    custo_m = (0.5 * fly->duration) + (0.45 * fly->distance) + (0.05 * 0);
                    nome = strdup(ptr->code);
                }
            }
        }
        *custo = custo_m;
        fly = fly->next;
    }

    bool on_list(Airports * list, const char *code)
    {
        bool search = false;
        for (; list && !search; list = list->next)
        {
            if (strcmp(code, list->code) == 0)
                search = true;
        }
        return search;
    }

    bool on_hash(Airports * *hash, char *code)
    {
        int h = hash_func(code);
        return on_list(*(hash + h), code);
    }

    bool check(Airports * *hash, const char *aofd, const char *aofa, const char *type)
    {
        bool positive = false;
        Flights *fly = NULL;
        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                fly = ptr->flys;
                while (fly)
                {
                    if (strcmp(fly->aofa, aofa) == 0 && strcmp(fly->aofd, aofd) == 0 && strcmp(fly->type, type) == 0)
                    {
                        positive = true;
                    }
                    fly = fly->next;
                }
                ptr = ptr->next;
            }
        }
        return positive;
    }

    void Q3(Airports * *hash)
    {

        Aux aux[S];
        for (int o = 0; o < S; o++)
        {
            aux[o].city = strdup(" ");
            aux[o].t = 0;
        }

        int most = 0;
        char *city;
        bool vef = false;

        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {

                Flights *fly = ptr->flys;
                while (fly)
                {
                    city = codeToCity(hash, fly->aofa);
                    if (i = 0)
                    {
                        aux[i].city = strdup(city);
                    }
                    for (int e = 0; e < S; e++)
                    {
                        if (strcmp(aux[e].city, city) == 0)
                        {
                            vef = true;
                        }
                    }
                    if (vef != true)
                    {
                        char q = " ";
                        for (int e = 0; e < S; e++)
                        {
                            if (strcmp(aux[e].city, q) == 0)
                            {
                                aux[e].city = strdup(city);
                            }
                        }
                    }

                    for (int x = 0; x < S; x++)
                    {
                        if (strcmp(aux[x].city, city) == 0)
                        {
                            aux[x].t = aux[x].t++;
                        }
                    }

                    fly = fly->next;
                }

                ptr = ptr->next;
            }
        }

        for (int u = 0; u < S; u++)
        {
            if (u == 0)
            {
                most = aux[u].t;
                strcpy(city, aux[u].city);
            }
            else if (most < aux[u].t)
            {
                most = aux[u].t;
                strcpy(city, aux[u].city);
            }
        }

        printf("Cidade com mais chegadas!\n");
        printf("Cidade: %s\n", city);
        printf("Chegadas: %d\n", most);
        printf("-------------------------------\n");
        printf("Prima qualquer tecla para voltar ao menu.");
        getchar();
        system("cls || clear");
        Menu(hash);
    }

    void Q2(Airports * *hash)
    {

        Aux aux[S];
        for (int o = 0; o < S; o++)
        {
            aux[o].city = strdup(" ");
            aux[o].t = 0;
        }

        int most = 0;
        int tmp = 0;
        char *city;
        bool vef = false;

        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                if (i = 0)
                {
                    aux[i].city = strdup(ptr->city);
                }
                for (int e = 0; e < S; e++)
                {
                    if (strcmp(aux[e].city, ptr->city) == 0)
                    {
                        vef = true;
                    }
                }
                if (vef != true)
                {
                    char q = " ";
                    for (int e = 0; e < S; e++)
                    {
                        if (strcmp(aux[e].city, q) == 0)
                        {
                            aux[e].city = strdup(ptr->city);
                        }
                    }
                }
                Flights *fly = ptr->flys;
                while (fly)
                {
                    if (i = 0)
                    {
                        aux[i].t = aux[i].t++;
                    }
                    else
                    {
                        for (int x = 0; x < S; x++)
                        {
                            if (strcmp(aux[x].city, ptr->city) == 0)
                            {
                                aux[x].t = aux[x].t++;
                            }
                        }
                    }
                    fly = fly->next;
                }

                ptr = ptr->next;
            }
        }

        for (int u = 0; u < S; u++)
        {
            if (u == 0)
            {
                most = aux[u].t;
                strcpy(city, aux[u].city);
            }
            else if (most < aux[u].t)
            {
                most = aux[u].t;
                strcpy(city, aux[u].city);
            }
        }
        printf("Cidade com mais partidas!\n");
        printf("Cidade: %s\n", city);
        printf("Partidas: %d\n", most);
        printf("-------------------------------\n");
        printf("Prima qualquer tecla para voltar ao menu.");
        getchar();
        system("cls || clear");
        Menu(hash);
    }

    void Q4(Airports * *hash)
    {
        int size = S, y = 0, counter[size];
        char *country;
        char *aeroportos[size];

        for (int x = 0; x < size; x++)
        {
            counter[x] = 0;
            aeroportos[x] = strdup(" ");
        }

        for (int z = 0; z < S; z++)
        {
            Airports *ptr = hash[z];
            while (ptr)
            {
                if (strcmp(ptr->country, country) == 0)
                {
                    aeroportos[z] = strdup(ptr->code);
                    z++;
                    break;
                }
                ptr = ptr->next;
            }
        }

        int k = 0;
        int x = 0;
        for (int y = 0; y < S; y++)
        {
            Airports *ptr = hash[y];
            while (ptr)
            {
                for (int x = 0; x < size; x++)
                {
                    if (strcmp(ptr->code, aeroportos[x]) == 0)
                    {
                        counter[k]++;
                    }
                }
                ptr = ptr->next;
                k++;
            }
        }

        for (int x = 0; x < size; x++)
        {
            if (counter[x] != 0)
            {
                printf("%d ", counter[x]);
            }
        }
    }

    void Q5(Airports * *hash)
    {
        char *p;
        char *c;
        char *typen = "normal";
        char *typel = "lowcost";
        double custon, custol;
        printf("Codigo do aeroporto de partida: ");
        scanf(" %s\n", &p);
        printf("Codigo do aeroporto de chegada: ");
        scanf(" %s\n", &c);

        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                if (strcmp(ptr->code, p) == 0)
                {
                    Flights *fly = ptr->flys;
                    while (fly)
                    {
                        if (strcmp(fly->type, typen) == 0)
                        {
                            custon = (0.5 * fly->duration) + (0.45 * fly->distance) + 0.05 * 1;
                        }
                        else if (strcmp(fly->type, typel) == 0)
                        {
                            custol = (0.5 * fly->duration) + (0.45 * fly->distance) + 0.05 * 0;
                        }
                        fly = fly->next;
                    }
                }
                ptr = ptr->next;
            }
        }

        if (custon > custol)
        {
            printf("A viagem com menor custo é a Lowcost!");
        }
        else if (custon < custol)
        {
            printf("A viagem com menor custo é a Normal!");
        }
        else
        {
            printf("As viagens tem o mesmo custo!");
        }

        printf("-------------------------------\n");
        printf("Prima qualquer tecla para voltar ao menu.");
        getchar();
        system("cls || clear");
        Menu(hash);
    }

    void Q6(Airports * *hash)
    {
        Aux aux[S];
        for (int o = 0; o < S; o++)
        {
            aux[o].city = strdup(" ");
            aux[o].t = 0;
        }

        Aux aux2[S];
        for (int o = 0; o < S; o++)
        {
            aux2[o].city = strdup(" ");
            aux2[o].t = 0;
        }

        char *p;
        char *c;
        char *typen = "normal";
        char *typel = "lowcost";
        char *code;
        double custon, custol;
        printf("Cidade de partida: ");
        scanf(" %s\n", &p);
        printf("Cidade de chegada: ");
        scanf(" %s\n", &c);

        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                if (strcmp(ptr->city, p) == 0)
                {
                    aux[i].city = strdup(ptr->code);
                }
            }
        }

        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                if (strcmp(ptr->city, c) == 0)
                {
                    aux2[i].city = strdup(ptr->code);
                }
            }
        }
        for (int y = 0; y < S; y++)
        {
            for (int i = 0; i < S; i++)
            {
                Airports *ptr = hash[i];
                while (ptr)
                {
                    if (strcmp(ptr->code, aux[y].city) == 0)
                    {
                        Flights *fly = ptr->flys;
                        while (fly)
                        {
                            for (int k = 0; k < S; k++)
                            {
                                if (strcmp(fly->aofa, aux2[k].city) == 0)
                                    if (strcmp(fly->type, typen) == 0)
                                    {
                                        custon = (0.5 * fly->duration) + (0.45 * fly->distance) + 0.05 * 1;
                                    }
                                    else if (strcmp(fly->type, typel) == 0)
                                    {
                                        custol = (0.5 * fly->duration) + (0.45 * fly->distance) + 0.05 * 0;
                                    }
                            }
                            fly = fly->next;
                        }
                    }
                }
                ptr = ptr->next;
            }
        }

        if (custon > custol)
        {
            printf("A viagem com menor custo é a Lowcost!");
        }
        else if (custon < custol)
        {
            printf("A viagem com menor custo é a Normal!");
        }
        else
        {
            printf("As viagens tem o mesmo custo!");
        }

        printf("-------------------------------\n");
        printf("Prima qualquer tecla para voltar ao menu.");
        getchar();
        system("cls || clear");
        Menu(hash);
    }

    /*void Q7(Airports * *hash)
    {
        char *aofd;
        char *aofa;
        char *type;
        float distance, duration;
        int cont = 0;
        Flights *fly = NULL;
        for (int i = 0; i < S; i++)
        {
            Airports *ptr = hash[i];
            while (ptr)
            {
                fly = ptr->flys;
                while (fly)
                {
                    if (strcmp(fly->aofa, aofa) == 0 && strcmp(fly->aofd, aofd) == 0 && strcmp(fly->type, type) == 0)
                    {
                        cont++;
                        printf("Diga a distancia -> ");
                        scanf("%f", &distance);
                        printf("Diga a Duration -> ");
                        scanf("%f", &duration);
                        fly->distance = distance;
                        fly->duration = duration;
                    }
                    fly = fly->next;
                }
                ptr = ptr->next;
            }
        }

        if (cont == 0)
        {
            printf("Nao encontrado\n");
        }

        printf("-------------------------------\n");
        printf("Prima qualquer tecla para voltar ao menu.");
        getchar();
        system("cls || clear");
        Menu(hash);
    }*/

    void Q8(Airports * *hash)
    {
        char *city, *code, *country;
        bool positive;
        int counter = 0, counter_cidade = 0;

        for (int i = 0; i < S; i++)
        {
            if (counter < 0)
            {
                break;
            }
            Airports *ptr = hash[i];
            while (ptr)
            {
                if (strcmp(ptr->city, city) == 0)
                {
                    counter_cidade++;
                    if (counter == 0)
                    {
                        positive = on_hash(hash, code);
                        if (positive == false && counter == 0)
                        {
                            hash_insert(hash, code, 0, 0, NULL, NULL);
                            Airport_Data_Set(hash, city, ptr->country, code);
                            counter++;
                        }
                        else
                        {
                            printf("Ja existe\n");
                        }
                    }
                }
                ptr = ptr->next;
            }
        }

        if (counter_cidade == 0)
        {
            printf("Essa cidade nao existe\n");
        }
    }

    void Q9(Airports * *hash)
    {
        char *aofd, *aofa, *type;
        float duration, distance;
        int typef = 0;
        bool positive, end, conection;
        printf("Indica a distancia: ");
        scanf("%f", &distance);
        printf("Indica a duracao: ");
        scanf("%f", &duration);

        printf("Qual o tipo de voo (0 - Normal | 1 - Lowcost ) ");
        scanf("%d", &typef);
        switch (typef)
        {
        case 0:
            type = strdup("normal");
            break;
        case 1:
            type = strdup("lowcost");
            break;
        }

        conection = check(hash, aofd, aofa, type);

        if (conection == false)
        {
            positive = on_hash(hash, aofd);

            if (positive == true)
            {
                end = on_hash(hash, aofa);
                if (end == true)
                {
                    if (strcmp(aofd, aofa) != 0)
                    {
                        hash_insert(hash, aofd, distance, duration, type, aofa);
                    }
                    else
                    {
                        printf("Impossivel levantar e aterrar no mesmo Airport\n");
                    }
                }
                else
                {
                    printf("Esse Airport de chegada nao existe\n");
                }
            }
            else
            {
                printf("Esse Airport de partida nao existe\n");
            }
        }
        else
        {
            printf("Essa ligacao ja existe\n");
        }
    }
    void Q10(Airports * *hash)
    {

        char *city, *code, *country;
        bool positive;
        int counter = 0, counter_cidade = 0;

        for (int i = 0; i < S; i++)
        {
            if (counter < 0)
            {
                break;
            }

            Airports *ptr = hash[i];
            while (ptr)
            {
                if (strcmp(ptr->city, city) == 0)
                {
                    counter_cidade++;
                    if (counter == 0)
                    {
                        positive = on_hash(hash, code);
                        if (positive == false && counter == 0)
                        {
                            hash_insert(hash, code, 0, 0, NULL, NULL);
                            Airport_Data_Set(hash, city, ptr->country, code);
                            counter++;
                        }
                        else
                        {
                            printf("Ja existe\n");
                        }
                    }
                }
                ptr = ptr->next;
            }
        }

        if (counter_cidade == 0)
        {
            printf("Essa cidade nao existe\n");
        }
    }

    void Menu(Airports * *hash)
    {

        int option = 0;

        printf("1 - Todas as cidades de destino possiveis a partir de uma dada cidade de origem.");                                         //[NOT DONE]
        printf("\n2 - A cidade com mais partidas diferentes para cidades distintas (Apenas Voos Diretos)");                                 //[DONE]
        printf("\n3 - A cidade com mais chegadas diferentes a partir de cidades distintas (Apenas Voos Diretos)");                          //[DONE]
        printf("\n4 - O aeroporto mais internacional de um dado pais, ou seja, aquele que tem mais ligacoes (sem escalas) com outro pais"); //[NOT DONE]
        printf("\n5 - A melhor opcao de voo entre dois aeroportos (Menor Custo)");                                                          //[DONE]
        printf("\n6 - A melhor opcao de voo entre duas cidades (Menor Custo)");                                                             //[DONE]
        printf("\n7 - Alterar distancia e duracao de uma dada ligacao");                                                                    //[DONE]
        printf("\n8 - Adicionar ou remover um dado aeroporto de uma cidade especifica");                                                    //[NOT DONE]
        printf("\n9 - Adicionar uma nova ligacao entre dois aeroportos");                                                                   //[NOT DONE]
        printf("\n10 - Adicionar uma nova cidade");                                                                                         //[NOT DONE]
        printf("\n11 - Exit Program");                                                                                                      // OPTION 11 [DONE]
        printf("\nOption: ");
        scanf("%d", &option);

        switch (option)
        {

        case 1:

            system("clear || cls");
            //Q1(hash);
            break;

        case 2:
            system("clear || cls");
            Q2(hash);
            break;

        case 3:
            system("clear || cls");
            Q3(hash);
            break;

        case 4:
            system("clear || cls");
            Q4(hash);
            break;

        case 5:
            system("clear || cls");
            Q5(hash);
            break;

        case 6:
            system("clear || cls");
            Q6(hash);
            break;

        case 7:
            system("clear || cls");
            //Q7(hash);
            break;

        case 8:
            system("clear || cls");
            //Q8(hash);
            break;

        case 9:
            system("clear || cls");
            Q9(hash);
            break;

        case 10:
            system("clear || cls");
            Q10(hash);
            break;

        default:

            system("pause");

            exit(0);

            break;
        }
    }
    void main(Airports * *hash)
    {
        Airports **airhash;
        printf("Loading Data");
        airhash = (Airports **)calloc(S, sizeof(Airports *));
        loadFlights(airhash);
        loadAirports(airhash);
        system("clear || cls");
        Menu(airhash);
    }
}
