#define BAK 7
#define FAKTOR_BAKETIRANJA 3             //Faktor Baketiranja za Rasutu datoteku
#define FAKTOR_BLOKIRANJA 3            //Faktor Blokiranja za Sekvencijalnu/Serijsku datoteku
#define OZNAKA_KRAJA_DATOTEKE "********"
#define KORAK 1                     //Sluzi za trazenje narednog bloka


typedef struct Slog {
    char evidBroj[9];
    char sifraZatvorenika[8];
    char datumDolaska[20];
    char oznakaCelije[6];
    int duzinaKazne;
    int deleted;                    //Mogli smo i boolean ovde -> [ 0 - EMPTY / 1 - ACTIVE / 2 - DELETED ]
} SLOG;

typedef struct Baket {
    SLOG slogovi[FAKTOR_BAKETIRANJA];
} BAKET;

typedef struct DateTime {
    int dan;
    int mesec;
    int godina;
    int sati;
    int minuti;
} DATETIME;

typedef struct SlogSS {
    char evidBroj[8+1];
    char sifraZatvorenika[7+1];
    DATETIME datumVremeDolaska;
    char oznakaCelije[5+1];
    int duzinaKazne;
    int deleted;                    //Klasicno za SS datoteke
} SLOG_SS;

typedef struct BlokSS {
    SLOG_SS slogovi[FAKTOR_BLOKIRANJA];
} BLOK_SS;                          //Blokovi iz koje iscitavamo datoteke

