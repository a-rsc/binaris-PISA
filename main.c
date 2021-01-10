/*
    Alumne: Álvaro Rodríguez Santa Cruz

    Professors: Claudina Riaza
    Assignatura: Programació Estructurada - Fitxers Binaris

    DAM 2 / EDUCEM

    Data: 24/12/2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Añade paquete de idiomas
#include <locale.h>
#include <windows.h>

/*
    INFORME PISA
*/

// --------------------------------------------------------------------------------------- CONSTANTS

#define MAX_STRING 256 // longitud de la cadena
#define END_OF_STRING '\0' // Caràcter final de cadena
#define MAX_MENU 7 // Opcions de menú

#define MAX_COUNTRY 4 // longitud de la cadena country
#define MAX_YEAR 5 // longitud de la cadena year

// --------------------------------------------------------------------------------------- FI CONSTANTS

// --------------------------------------------------------------------------------------- PROTOTIPS FUNCIONS

typedef struct
{
    char index;
    char desc[MAX_STRING];
}tMenu;

typedef struct
{
    char country[MAX_COUNTRY];
    char knowledge[MAX_STRING];
    char year[MAX_YEAR];
    int index;
}tPisa;

int showMenu();
char getChar();
int get_integer_interval(int min, int max);
void entrarCadena(char str[], int size);
bool searchOption(char *option, tMenu menu[MAX_MENU]);
char* getOption(char option, tMenu menu[MAX_MENU]);
bool deleteCRLF(char cad[MAX_STRING]);
void pathFilename(char cad[MAX_STRING], char dir[MAX_STRING], char filename[MAX_STRING]);
FILE *obrir(char filename[MAX_STRING], char mode[4], bool printWarning);
bool import(char filename[MAX_STRING], char filenamecopy[MAX_STRING]);
tPisa llegirRegistre(char cad[MAX_STRING]);
int showData(char filename[MAX_STRING]);
bool copyFile(char filename[MAX_STRING], char copy[MAX_STRING]);
int showDataByCountry(char filename[MAX_STRING]);
int showDataByKnowledge(char filename[MAX_STRING]);
int showDataByYear(char filename[MAX_STRING]);
int showDataByIndex(char filename[MAX_STRING]);
int calcularqttPisas(char filename[MAX_STRING]);

// --------------------------------------------------------------------------------------- FI PROTOTIPS FUNCIONS

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ MAIN

int main()
{
// --------------------------------------------------------------------------------------- DECLARACIO VARIABLES

    char option,
        cad[MAX_STRING], pisadatFile[MAX_STRING], dir[MAX_STRING], filename[MAX_STRING];

    strcpy(cad, "C:\\foo\\foo\\baz.txt");
    // strcpy(cad, "");

    pathFilename(cad, dir, filename);

    printf("%s\n", dir);
    printf("%s\n", filename);

    return 0;

// --------------------------------------------------------------------------------------- FI DECLARACIO VARIABLES

    setlocale(LC_ALL, "spanish"); // Cambiar locale - Suficiente para máquinas Linux

    strcpy(pisadatFile, "pisadat.dat");

    remove(pisadatFile);

    option = showMenu();

    while(option != 'Z')
    {
        switch(option)
        {
            case 'A':

                printf(" Introdueix un nom d'arxiu, el qual s'importaran les dades PISA [Per defecte: PisaUE.txt]: ");
                entrarCadena(cad, MAX_STRING);

                pathFilename(cad, dir, filename);

                if(strcmp(".", filename) == 0) { strcpy(filename, "PisaUE.txt");}

                if(import(filename, pisadatFile)) {
                    printf("\n La importacio s'ha realitzat amb èxit.\n");
                }else {
                    printf("\n No s'ha realitzat la importació.\n");
                }
                break;
            case 'B':
                if(showData(pisadatFile) == 0) {
                    printf(" No hi ha cap llista a l'arxiu.\n");
                }
                break;
            case 'C':
                if(showDataByCountry(pisadatFile) == 0) {
                    printf(" No hi ha cap llista a l'arxiu.\n");
                }
                break;
            case 'D':
                if(showDataByKnowledge(pisadatFile) == 0) {
                    printf(" No hi ha cap llista a l'arxiu.\n");
                }
                break;
            case 'E':
                if(showDataByYear(pisadatFile) == 0) {
                    printf(" No hi ha cap llista a l'arxiu.\n");
                }
                break;
            case 'F':
                if(showDataByIndex(pisadatFile) == 0) {
                    printf(" No hi ha cap llista a l'arxiu.\n");
                }
                break;
            default:
                break;
        }

        printf("\n Premeu qualsevol tecla per continuar...\n");
        getch();
        system("cls");

        option = showMenu();
    }

    return 0;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ FI MAIN

int showMenu()
{
    tMenu menu[MAX_MENU] = {
        {'A', "Importar dades PISA"},                   // código ASCII - A: 65 / a: 97
        {'B', "Mostrar dades PISA sense ordenar"},      // código ASCII - B: 66 / b: 98
        {'C', "Mostrar dades PISA ordenats per pais"},  // código ASCII - C: 67 / c: 99
        {'D', "Mostrar dades PISA ordenats per prova"}, // código ASCII - D: 68 / d: 100
        {'E', "Mostrar dades PISA ordenats per any"},   // código ASCII - E: 69 / e: 101
        {'F', "Mostrar dades PISA ordenats per index"}, // código ASCII - F: 70 / f: 102
        {'Z', "Sortir"}                                 // código ASCII - Z: 90 / z: 122
    };
    char option;
    int i;

    printf("\n    INFORME PISA:\n");
    printf(" ----------------\n");

    for(i = 0; i < MAX_MENU; i++)
    {
        printf(" %c - %s\n", menu[i].index, menu[i].desc);
        if(i != 0 && (i+1)%4 == 0) {
            printf(" ----------------\n");
        }
    }

    printf("\n");

    do
    {
        printf(" Escollir opcio: ");
        option = getChar();
    }while(!searchOption(&option, menu));

    printf("\n Ha escollit l'opcio: %c - %s\n\n", option, getOption(option, menu));

    return option;
}

char getChar()
{
    char c;

    fflush(stdin); // Its purpose is to clear (or flush) the output buffer and move the buffered data to console (in case of stdout) or disk (in case of file output stream).
    scanf("%c", &c);
    fflush(stdin); // Its purpose is to clear (or flush) the output buffer and move the buffered data to console (in case of stdout) or disk (in case of file output stream).

    return c;
}

int get_integer_interval(int min, int max)
{
    int a;

    scanf("%d", &a);

    while(a<min || a>max)
    {
        printf(" Introdueix un nombre compres en l'interval [%d, %d]: ", min, max);
        scanf("%d",&a);
    }

    return a;
}

void entrarCadena(char str[], int size)
{
    fflush(stdin);
    fgets(str, size, stdin);
    if(str[strlen(str)-1]=='\n') str[strlen(str)-1]=END_OF_STRING;
}

bool searchOption(char *option, tMenu menu[MAX_MENU])
{
    bool found = false;
    int i = 0;

    while(i < MAX_MENU && !found) {
        if(*option == menu[i].index || *option == (char)(menu[i].index+32)) {
            found = true;
            *option = menu[i].index;
        }else {
            i++;
        }
    }

    return found;
}

char* getOption(char option, tMenu menu[MAX_MENU])
{
    bool found = false;
    char *desc[MAX_STRING];
    int i = 0;

    while(i < MAX_MENU && !found)
    {
        if(option == menu[i].index || option == (char)(menu[i].index+32)) {
            found = true;
            *desc = menu[i].desc;
        }else {
            i++;
        }
    }

    return *desc;
}

bool deleteCRLF(char cad[MAX_STRING])
{
    bool deleted = false;
    int length = strlen(cad);

    if (cad[length-1] == '\n')
    {
        cad[length-1] = END_OF_STRING;
        deleted = true;
    }

    return deleted;
}

void pathFilename(char cad[MAX_STRING], char dir[MAX_STRING], char filename[MAX_STRING])
{
    char ts1[MAX_STRING], ts2[MAX_STRING];

    strcpy(ts1, strdup(cad));
    strcpy(ts2, strdup(cad));

    strcpy(dir, dirname(ts1));
    strcpy(filename, basename(ts2));
}

FILE *obrir(char filename[MAX_STRING], char mode[4], bool printWarning)
{
    FILE *f;

    f=fopen(filename, mode);

    if(f==NULL && printWarning)
    {
        if(
            strcmpi(mode, "r")==0 || strcmpi(mode, "rt")==0 || strcmpi(mode, "r+")==0 || strcmpi(mode, "rt+")==0 ||
            strcmpi(mode, "rb")==0 || strcmpi(mode, "rb+")==0
        )
            printf(" El fitxer %s no existeix!\n", filename);
        else if(
             strcmpi(mode, "w")==0 || strcmpi(mode, "wt")==0 || strcmpi(mode, "w+")==0 || strcmpi(mode, "wt+")==0 ||
             strcmpi(mode, "a")==0 || strcmpi(mode, "at")==0 || strcmpi(mode, "a+")==0 || strcmpi(mode, "at+")==0 ||
             strcmpi(mode, "wb")==0 || strcmpi(mode, "wb+")==0 ||
             strcmpi(mode, "ab")==0 || strcmpi(mode, "ab+")==0
        )
            printf(" No s'ha pogut obrir/crear el fitxer %s!\n", filename);
        else
            printf(" El paràmetre de fitxer és incorrecte!\n");
    }

    return f;
}

bool import(char filename[MAX_STRING], char filenamecopy[MAX_STRING])
{
    FILE *f, *copy;
    tPisa pisa;
    bool copied = false;
    char cad[MAX_STRING];

    if((f=obrir(filename, "r", false)) && (copy=obrir(filenamecopy, "wb", false)))
    {
        while (feof(f)==0)
        {
            fgets(cad, MAX_STRING, f);
            deleteCRLF(cad);

            pisa = llegirRegistre(cad);

            fwrite(&pisa, sizeof(tPisa), 1, copy);
        }

        copied = true;
        fflush(copy);
        fclose(f);
        fclose(copy);
    }

    return copied;
}

tPisa llegirRegistre(char cad[MAX_STRING])
{
    tPisa pisa;

    int i = 0, index[4] = {}, count = 0;
    char registre[4][MAX_STRING];

    while(cad[i] != END_OF_STRING)
    {
        if(cad[i] != '"' && cad[i] != ',')
        {
            registre[count][index[count]] = cad[i];
            index[count]++;
        }
        else if(cad[i] == ',')
        {
            count++;
        }
        i++;
    }

    for(i = 0; i < count; i++)
    {
        registre[i][index[i]] = END_OF_STRING;
    }

    strcpy(pisa.country, registre[0]);
    strcpy(pisa.knowledge, registre[1]);
    strcpy(pisa.year, registre[2]);
    pisa.index = atoi(registre[3]);

    return pisa;
}

// Imprimeix i retorna el número de registres en el fitxer
int showData(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa;
    int i = 0;

    if(f=obrir(filename, "rb", false))
    {
        fread(&pisa, sizeof(tPisa), 1, f);
        while (feof(f)==0)
        {
            printf(" %.2d - Pais: %s * Prova: %s * Any: %s * Index: %d\n", ++i, pisa.country, pisa.knowledge, pisa.year, pisa.index);
            fread(&pisa, sizeof(tPisa), 1, f);
        }

        fclose(f);
    }

    return i;
}

bool copyFile(char filename[MAX_STRING], char filenamecopy[MAX_STRING])
{
    FILE *f, *copy;
    tPisa pisa;
    bool copied = false;

    if((f=obrir(filename, "rb", false)) && (copy=obrir(filenamecopy, "wb", false)))
    {
        fread(&pisa, sizeof(tPisa), 1, f);
        while (feof(f)==0)
        {
            fwrite(&pisa, sizeof(tPisa), 1, copy);
            fread(&pisa, sizeof(tPisa), 1, f);
        }

        copied = true;
        fflush(copy);
        fclose(f);
        fclose(copy);
    }

    return copied;
}

int showDataByCountry(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa, auxPisa;
    char auxFileName[MAX_STRING];
    int i = 0, j = 0, qttPisas = 0;

    strcpy(auxFileName, "auxiliar.dat");
    copyFile(filename, auxFileName);

    if(f=obrir(auxFileName, "rb+", false))
    {
        qttPisas = calcularqttPisas(filename);

        for (i = 1; i < qttPisas; i++)
        {
            rewind(f);

            for (j = 0; j < (qttPisas-i); j++)
            {
                fread(&pisa, sizeof(tPisa), 1, f);
                fread(&auxPisa, sizeof(tPisa), 1, f);

                if (strcmpi(pisa.country, auxPisa.country) > 0) {

                    fseek(f, -2*sizeof(tPisa), SEEK_CUR);
                    fwrite(&auxPisa, sizeof(tPisa), 1, f);
                    fwrite(&pisa, sizeof(tPisa), 1, f);
                }

                fseek(f, -sizeof(tPisa), SEEK_CUR);
            }
        }

        fflush(f);
        fclose(f);

        showData(auxFileName);
        remove(auxFileName);
    }

    return qttPisas;
}

int showDataByKnowledge(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa, auxPisa;
    char auxFileName[MAX_STRING];
    int i = 0, j = 0, qttPisas = 0;

    strcpy(auxFileName, "auxiliar.dat");
    copyFile(filename, auxFileName);

    if(f=obrir(auxFileName, "rb+", false))
    {
        qttPisas = calcularqttPisas(filename);

        for (i = 1; i < qttPisas; i++)
        {
            rewind(f);

            for (j = 0; j < (qttPisas-i); j++)
            {
                fread(&pisa, sizeof(tPisa), 1, f);
                fread(&auxPisa, sizeof(tPisa), 1, f);

                if (strcmpi(pisa.knowledge, auxPisa.knowledge) > 0) {

                    fseek(f, -2*sizeof(tPisa), SEEK_CUR);
                    fwrite(&auxPisa, sizeof(tPisa), 1, f);
                    fwrite(&pisa, sizeof(tPisa), 1, f);
                }

                fseek(f, -sizeof(tPisa), SEEK_CUR);
            }
        }

        fflush(f);
        fclose(f);

        showData(auxFileName);
        remove(auxFileName);
    }

    return qttPisas;
}

int showDataByYear(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa, auxPisa;
    char auxFileName[MAX_STRING];
    int i = 0, j = 0, qttPisas = 0;

    strcpy(auxFileName, "auxiliar.dat");
    copyFile(filename, auxFileName);

    if(f=obrir(auxFileName, "rb+", false))
    {
        qttPisas = calcularqttPisas(filename);

        for (i = 1; i < qttPisas; i++)
        {
            rewind(f);

            for (j = 0; j < (qttPisas-i); j++)
            {
                fread(&pisa, sizeof(tPisa), 1, f);
                fread(&auxPisa, sizeof(tPisa), 1, f);

                if (strcmpi(pisa.year, auxPisa.year) > 0) {

                    fseek(f, -2*sizeof(tPisa), SEEK_CUR);
                    fwrite(&auxPisa, sizeof(tPisa), 1, f);
                    fwrite(&pisa, sizeof(tPisa), 1, f);
                }

                fseek(f, -sizeof(tPisa), SEEK_CUR);
            }
        }

        fflush(f);
        fclose(f);

        showData(auxFileName);
        remove(auxFileName);
    }

    return qttPisas;
}

int showDataByIndex(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa, auxPisa;
    char auxFileName[MAX_STRING];
    int i = 0, j = 0, qttPisas = 0;

    strcpy(auxFileName, "auxiliar.dat");
    copyFile(filename, auxFileName);

    if(f=obrir(auxFileName, "rb+", false))
    {
        qttPisas = calcularqttPisas(filename);

        for (i = 1; i < qttPisas; i++)
        {
            rewind(f);

            for (j = 0; j < (qttPisas-i); j++)
            {
                fread(&pisa, sizeof(tPisa), 1, f);
                fread(&auxPisa, sizeof(tPisa), 1, f);

                if (pisa.index < auxPisa.index) {

                    fseek(f, -2*sizeof(tPisa), SEEK_CUR);
                    fwrite(&auxPisa, sizeof(tPisa), 1, f);
                    fwrite(&pisa, sizeof(tPisa), 1, f);
                }

                fseek(f, -sizeof(tPisa), SEEK_CUR);
            }
        }

        fflush(f);
        fclose(f);

        showData(auxFileName);
        remove(auxFileName);
    }

    return qttPisas;
}

int calcularqttPisas(char filename[MAX_STRING])
{
    FILE *f;
    tPisa pisa;
    int i = 0;

    if(f=obrir(filename, "rb", false))
    {
        fread(&pisa, sizeof(tPisa), 1, f);
        while (feof(f)==0)
        {
            ++i;
            fread(&pisa, sizeof(tPisa), 1, f);
        }

        fclose(f);
    }

    return i;
}
