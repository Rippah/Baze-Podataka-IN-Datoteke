#include "metode.h"


FILE *otvoriDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "rb+");
    if(fajl == NULL)
        printf("\nNije dobro unet naziv.\n");
    else
        printf("\nUspesno otvorena datoteka.\n");
    return fajl;
}


void kreirajDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "wb");
    if(fajl == NULL)
        printf("Greska");
    else {
        printf("Uspesno kreirana datoteka.");
        BLOK blok;
        blok.slogovi[0].sifraTran = OZNAKA_KRAJA_DATOTEKE;
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        fclose(fajl);
    }
}


SLOG *pronadjiSlog(FILE *fajl, int sifraTran) {
    if(fajl == NULL)
        return NULL;

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == OZNAKA_KRAJA_DATOTEKE || blok.slogovi[i].sifraTran > sifraTran)
                return NULL;
            else if(blok.slogovi[i].sifraTran == sifraTran) {
                if(!blok.slogovi[i].obrisan) {
                    SLOG *slog = (SLOG*) malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                    return slog;
                }
            }
        }
    }
    return NULL;
}


void dodajSlog(FILE *fajl, SLOG *slog) {
    if(fajl == NULL)
        return;

    SLOG noviSlog;
    memcpy(&noviSlog, slog, sizeof(SLOG));

    BLOK blok;
    fseek(fajl, 0 , SEEK_SET);

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == OZNAKA_KRAJA_DATOTEKE) {
                memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG));
                if(i != FAKTOR_BLOKIRANJA - 1) {
                    blok.slogovi[i+1].sifraTran = OZNAKA_KRAJA_DATOTEKE;
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);
                    printf("\nUpisan novi slog.\n");
                    return;
                }
                else {
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);

                    BLOK noviBlok;
                    noviBlok.slogovi[0].sifraTran = OZNAKA_KRAJA_DATOTEKE;
                    fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
                    printf("\nNovi slog i blok dodat u datoteci.\n");
                    return;
                }
            }
            else if(blok.slogovi[i].sifraTran == noviSlog.sifraTran) {
                if(!blok.slogovi[i].obrisan) {
                    printf("\nSlog postoji.\n");
                    return;
                }
                else {
                    memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG));
                    fseek(fajl, -sizeof(SLOG), SEEK_CUR);
                    fwrite(&blok.slogovi[i], sizeof(SLOG), 1, fajl);
                    printf("\nDodat novi slog na mesto obrisanog.\n");
                }
            }
            else if(blok.slogovi[i].sifraTran > noviSlog.sifraTran) {
                SLOG tmp;
                memcpy(&tmp, &blok.slogovi[i], sizeof(SLOG));
                memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG));
                memcpy(&noviSlog, &tmp, sizeof(SLOG));

                if(i == FAKTOR_BLOKIRANJA - 1) {
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);
                    fflush(fajl);
                }

            }
        }
    }

}


void ispisiSveSlogove(FILE *fajl) {
    if(fajl == NULL) {
        printf("Prazan fajl.");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == OZNAKA_KRAJA_DATOTEKE) {
                printf("S%d *\n", i);
                return;
            }
            else if(!blok.slogovi[i].obrisan) {
                printf("S%d \t", i);
                ispisiSlog(&blok.slogovi[i]);
            }
        }
    }
}


void ispisiSlog(SLOG *slog) {
    printf("%d\t%s\t%s\t%d\t%s\t%.2f\n", slog->sifraTran, slog->idSluz, slog->idKlij, slog->brRacuna, slog->datumTran, slog->kolPren);
}


void obrisiSlogLogicki(FILE *fajl, int sifraTran) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == sifraTran) {
                if(blok.slogovi[i].obrisan == 1)
                    return;
                blok.slogovi[i].obrisan = 1;
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                printf("\nObrisan\n");
                return;
            }
        }
    }
}


void pronadjiNajveciPrenos(FILE *fajl) {
    if(fajl == NULL)
        return;

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    int i;
    float max = 0;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(max < blok.slogovi[i].kolPren && !blok.slogovi[i].obrisan)
                max = blok.slogovi[i].kolPren;
        }
    }
    printf("\nNajveci prenos je:\t%.2f", max);
}


void povecajTransakcijuPoID(FILE *fajl, char *id) {
    if(fajl == NULL)
        return;

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    int i;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == OZNAKA_KRAJA_DATOTEKE)
                return;
            if(strcmp(blok.slogovi[i].idKlij, id) == 0) {
                blok.slogovi[i].kolPren = 1.1*blok.slogovi[i].kolPren;
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
            }
        }
    }
}


void prosekPoID(FILE *fajl, char *id) {
    if(fajl == NULL)
        return;

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    int i, j = 0;
    float prosek;

    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(blok.slogovi[i].sifraTran == OZNAKA_KRAJA_DATOTEKE) {
                prosek = prosek/j;
                printf("\nProsek je: %.2f", prosek);
                return;
            }
            if(strcmp(blok.slogovi[i].idSluz, id) == 0) {
                prosek += blok.slogovi[i].kolPren;
                j++;
            }
        }
    }
}
