#include "metode.h"

FILE *otvoriDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "rb+");
    if(fajl == NULL) {
        printf("\nNije otvorena datoteka.\n");
        return NULL;
    }
    printf("\nDatoteka otvorena.");
    return fajl;
}


void kreirajDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "wb");

    if(fajl == NULL) {
        printf("\nDatoteka ne postoji.");
        return;
    }
    BAKET baket;
    int i, j;
    fseek(fajl, 0, SEEK_SET);
    for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
        baket.slogovi[j].brRacun = 0;
        baket.slogovi[j].deleted = 0;
        baket.slogovi[j].kolPren = 0;
        baket.slogovi[j].sifra = 0;
        strcpy(baket.slogovi[j].datumTran, "-");
        strcpy(baket.slogovi[j].idKlij, "-");
        strcpy(baket.slogovi[j].idSluz, "-");
    }
    for(i = 0; i < BROJ_BAKETA; i++)
        fwrite(&baket, sizeof(BAKET), 1, fajl);
    printf("\nUspesno upisan novi fajl.");
    fclose(fajl);
}

int nadjiSlobodanBaket(int adresaPunog, FILE *fajl) {
    BAKET baketi[BROJ_BAKETA];
    fseek(fajl, 0, SEEK_SET);
    int i;

    for(i = 0; i < BROJ_BAKETA; i++)
        fread(&baketi[i], sizeof(BAKET), 1, fajl);


    for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
        if(baketi[adresaPunog].slogovi[i].deleted != 1)
            return adresaPunog;

    adresaPunog = nadjiSlobodanBaket((adresaPunog+KORAK)%BROJ_BAKETA, fajl);
    return adresaPunog;
}


int postojiSifra(FILE *fajl, int sifra) {
    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BROJ_BAKETA];
    int i, j;

    for(i = 0; i < BROJ_BAKETA; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++)
            if(sifra == baketi[i].slogovi[j].sifra)
                return -1;

    }

    return 0;
}


void unesiSlog(FILE *fajl, SLOG *slog) {
    if(fajl == NULL)
        return;

    int id = slog->sifra%BROJ_BAKETA;
    int faktor = postojiSifra(fajl, slog->sifra);

    if(faktor == -1)
        return;

    BAKET baketi[BROJ_BAKETA];
    fseek(fajl, id*sizeof(BAKET), SEEK_SET);
    int j;

    for(j = 0; j < FAKTOR_BAKETIRANJA; j++)
        if(baketi[id].slogovi[FAKTOR_BAKETIRANJA - 1].deleted == 1)
            id = nadjiSlobodanBaket(id, fajl);

    fread(&baketi[id], sizeof(BAKET), 1, fajl);
    for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
        if(baketi[id].slogovi[j].deleted != 1) {
            memcpy(&baketi[id].slogovi[j], slog, sizeof(SLOG));
            fseek(fajl, -(FAKTOR_BAKETIRANJA-j)*sizeof(SLOG), SEEK_CUR);
            fwrite(&baketi[id].slogovi[j], sizeof(SLOG), 1, fajl);
            printf("\nUpisan u baket %d\n", id);
            return;
        }
    }

}


void trazenjePoKljucu(FILE *fajl, int sifra) {
    if(fajl == NULL)
        return;

    BAKET baketi[BROJ_BAKETA];
    int i, j;
    fseek(fajl, 0, SEEK_SET);

    for(i = 0; i < BROJ_BAKETA; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            if(sifra == baketi[i].slogovi[j].sifra) {
                printf("\n***BAKET %d***\n\n", i+1);
                printf("\n\nSLOGOVI\tSIFRA \t IDSLUZ \t DATUM \t KOLICINA \t IDKLIJ \t RACUN\n");
                ispisSloga(&baketi[i].slogovi[j]);
            }
        }
    }
}

void ispisPrazan() {
    printf("%d \t %7s \t %10s \t %.2f \t %7s \t %d\n", 0, "-", "-", 0., "-", 0);
}


void ispisSloga(SLOG *slog) {
    printf("%d \t %7s \t %10s \t %.2f \t %7s \t %d\n",
           slog->sifra, slog->idSluz, slog->datumTran, slog->kolPren, slog->idKlij, slog->brRacun);
}

void ispisSvihSlogova(FILE *fajl) {
    if(fajl == NULL)
        return;
    BAKET baketi[BROJ_BAKETA];
    fseek(fajl, 0, SEEK_SET);

    int i, j;
    for(i = 0; i < BROJ_BAKETA; i++) {
        printf("\n***BAKET %d***\n\n", i+1);
        printf("\n\nSLOGOVI\tSIFRA \t IDSLUZ \t DATUM \t KOLICINA \t IDKLIJ \t RACUN\n");
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            printf("Slog %d:\t", j);
            if(baketi[i].slogovi[j].deleted == 2)
                ispisPrazan();
            else
                ispisSloga(&baketi[i].slogovi[j]);
        }
    }
}


void logickoBrisanje(FILE *fajl, int sifra) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BROJ_BAKETA];
    int i, j, k = 0;

    for(i = 0; i < BROJ_BAKETA; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            if(sifra == baketi[i].slogovi[j].sifra) {
                if(baketi[i].slogovi[j].deleted == 1) {
                    baketi[i].slogovi[j].deleted = 2;
                    fseek(fajl, -sizeof(BAKET), SEEK_CUR);
                    fwrite(&baketi[i], sizeof(BAKET), 1, fajl);
                    fflush(fajl);
                    printf("\nPronadjen slog...\n");
                    return;
                }
                else if(baketi[i].slogovi[j].deleted == 2)
                    k = 1;
            }
        }
    }
    if(k == 1)
        printf("Slog je vec obrisan...\n");
    else
        printf("Slog ne postoji...\n");
}


void ispisiNajvecuTransakciju(FILE *fajl) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BROJ_BAKETA];
    int i, j;
    int imax, jmax;
    float max = 0;

    for(i = 0; i < BROJ_BAKETA; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++)
            if(max < baketi[i].slogovi[j].kolPren) {
                max = baketi[i].slogovi[j].kolPren;
                imax = i;
                jmax = j;
            }
    }

    ispisSloga(&baketi[imax].slogovi[jmax]);

}


void konverzija(FILE *fajlRasuta) {
    if(fajlRasuta == NULL)
        return;

    FILE *fajlSerijska = fopen("izvestaj.bin", "wb");

    BAKET baketi[BROJ_BAKETA];
    fseek(fajlRasuta, 0, SEEK_SET);
    int i, j, k = 0, l, hit = 0;

    BLOK blok;
    strcpy(blok.slogovi[0].idKlij, "********"); //Postavis pocetnu vrednost datoteke
    fseek(fajlRasuta, 0, SEEK_SET);


    for(i = 0; i < BROJ_BAKETA; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajlRasuta);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            if(baketi[i].slogovi[j].deleted == 1 && baketi[i].slogovi[j].sifra != 0) {      //Ako smo nasli vrednost
                for(l = 0; l <=k; l++)                                                      //Prolazimo kroz ceo blok
                    if(strcmp(blok.slogovi[l].idKlij, baketi[i].slogovi[j].idKlij) == 0) {  //I ako naidje na isti idKlijenta
                        blok.slogovi[l].ukuPren += baketi[i].slogovi[j].kolPren;            //Dodaj na ukupnu kolicinu
                        hit = 1;                                                            //Ako je nasao istog, nema poente da upisujemo opet idKlijenta
                    }
                if(hit == 0) {                                                              //Tako da ga ovaj hit cuva od toga
                    strcpy(blok.slogovi[k].idKlij, baketi[i].slogovi[j].idKlij);
                    blok.slogovi[k].ukuPren = baketi[i].slogovi[j].kolPren;
                    k++;
                }
                hit = 0;
                if(k >= FAKTOR_BLOKIRANJA) {                                                //Ako imamo vise od 4 sloga u bloku, treba napisati novi blok
                    fwrite(&blok, sizeof(BLOK), 1, fajlSerijska);
                    BLOK noviBlok;
                    strcpy(noviBlok.slogovi[0].idKlij, "********");
                    fwrite(&noviBlok, sizeof(BLOK), 1, fajlSerijska);
                    fseek(fajlSerijska, -sizeof(BLOK), SEEK_CUR);
                    k = 0;                                                                  //Brojac nazad na nulu da bi opet to uradili
                }
            }
        }
    }

    if(k != 0) {                                                                           //Ako je brojac ostao u opsegu od 1 do FAKTOR_BLOKIRANJA - 1
        strcpy(blok.slogovi[k].idKlij, "********");                                        //Upisujemo ostatak u datoteku
        fseek(fajlSerijska, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajlSerijska);

    }

    fclose(fajlSerijska);
    printf("\n\n\tID Klijenta \t Ukupna Kolicina Transakcija");
    fajlSerijska = fopen("izvestaj.bin", "rb+");
    ispisSvihSlogovaSerijska(fajlSerijska);
    fclose(fajlSerijska);

}

void ispisSvihSlogovaSerijska(FILE *fajl) {
    if(fajl == NULL)
        return;
    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    printf("\n\n");
    while(fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].idKlij, "********") == 0) {
                printf("Slog %d: *\n", i+1);
                break;
            }
            printf("Slog %d: %s \t\t %f\n", i+1, blok.slogovi[i].idKlij, blok.slogovi[i].ukuPren);
        }
    }
}
