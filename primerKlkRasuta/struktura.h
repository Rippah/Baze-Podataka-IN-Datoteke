#define FAKTOR_BAKETIRANJA 3
#define BROJ_BAKETA 7
#define KORAK 2 //Ostatak pri deljenju
#define FAKTOR_BLOKIRANJA 4

typedef struct Slog {
    int sifra;
    char idSluz[7];
    char datumTran[11];
    float kolPren;
    char idKlij[7];
    int brRacun;
    int deleted; //0 - EMPTY, 1 - ACTIVE, 2 - DELETED
} SLOG;

typedef struct Baket {
    SLOG slogovi[FAKTOR_BAKETIRANJA];
} BAKET;

typedef struct SlogSer {
    char idKlij[7];
    float ukuPren; //tog idKlij (samo > 100000)
    int deleted;
} SLOGSER;

typedef struct Blok {
    SLOGSER slogovi[FAKTOR_BLOKIRANJA];
} BLOK;
