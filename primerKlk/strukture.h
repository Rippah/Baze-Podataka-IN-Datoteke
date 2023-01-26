#define FAKTOR_BLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1


typedef struct Slog {
    int sifraTran;
    char idSluz[7];
    char datumTran[11]; //YYYY-MM-DD
    float kolPren;
    char idKlij[7];
    int brRacuna;
    int obrisan; //1 - obrisan, 0 - nije obrisan
} SLOG;

typedef struct Blok {
    SLOG slogovi[FAKTOR_BLOKIRANJA];
} BLOK;
