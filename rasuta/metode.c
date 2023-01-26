#include "metode.h"

FILE *otvoriDatoteku(char *naziv) {
    FILE *fajl = fopen(naziv, "rb+");
    if(fajl == NULL ) {
        printf("\nFajl neuspesno otvoren, pokusajte ponovo...\n");
        return NULL;
    }
    printf("\nDatoteka \"%s\" je uspesno otvorena!\n\n", naziv);
    return fajl;
}


void kreirajRasutuDatoteku(char *naziv) {
    FILE *fajl = fopen(naziv, "wb");
    if(fajl == NULL) {
        printf("\n\nFAJL NIJE USPESNO POSLAT\n\n");
        return;
    }
    else {
        BAKET baket;
        fseek(fajl, 0, SEEK_SET);
        int i, j;
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {       //Postavimo sve pocetne vrednosti na neki standard za prazne karaktere ("-")
            baket.slogovi[j].duzinaKazne = 0;
            baket.slogovi[j].deleted = 0;
            strcpy(baket.slogovi[j].evidBroj, "-");
            strcpy(baket.slogovi[j].sifraZatvorenika, "-");
            strcpy(baket.slogovi[j].datumDolaska, "-");
            strcpy(baket.slogovi[j].oznakaCelije, "-");
        }
        for(i = 0; i < BAK; i++) {
            fwrite(&baket, sizeof(BAKET), 1, fajl);
        }
        printf("\n\nRASUTA DATOTEKA JE USPESNO NAPRAVLJENA\n\n");
        fclose(fajl);
    }
}


void kreirajSSDatoteku(char *naziv) {
    FILE *fajl = fopen(naziv, "wb");
    if(fajl == NULL)
        printf("\n\nFAJL NIJE USPESNO POSLAT\n\n");
    else {
        BLOK_SS blok;
        strcpy(blok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
        printf("Datoteka \"%s\" je kreirana!\n", naziv);
        fclose(fajl);
    }
}


void ispisRasute(FILE *fajl) {
    if(fajl == NULL) {
        printf("\n\nFAJL NIJE USPESNO POSLAT\n\n");
        return;
    }

    BAKET baketi[BAK];
    fseek(fajl, 0, SEEK_SET);

    printf("\n\nB S\tEvidBr  SifraZat  DatDol  OznCel  DuzKazne  STATUS\n");
    int i;
    for(i = 0; i < BAK; i++) {              //Svaki baket po redu otvaramo
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        int j;
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {   //Sve dok traje faktor baketiranja ispisujemo vrednost tog baketa
            printf("%d %d\t%s\t %s\t %s\t %s\t %d\t %d\t\n", i, j,
                   baketi[i].slogovi[j].evidBroj,
                   baketi[i].slogovi[j].sifraZatvorenika,
                   baketi[i].slogovi[j].datumDolaska,
                   baketi[i].slogovi[j].oznakaCelije,
                   baketi[i].slogovi[j].duzinaKazne,
                   baketi[i].slogovi[j].deleted);
        }
    }


}

void ispisSS(FILE *fajl) {
    if(fajl == NULL) {
        printf("\n\nFAJL NIJE USPESNO POSLAT\n\n");
        return;
    }

    BLOK_SS blok;
    fseek(fajl, 0, SEEK_SET);
    int rbBloka = 0;
    printf("\nBL SL   EvidBr   SifZat   DatVremDol     Celija  Kazna\n");

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("B%d S%d *\n", rbBloka, i);
                break;
            }
            if(!blok.slogovi[i].deleted) {
                printf("B%d S%d %8s %7s %02d-%02d-%04d %02d:%02d %7s %6d\n", i, rbBloka,
           blok.slogovi[i].evidBroj,
           blok.slogovi[i].sifraZatvorenika,
           blok.slogovi[i].datumVremeDolaska.dan,
           blok.slogovi[i].datumVremeDolaska.mesec,
           blok.slogovi[i].datumVremeDolaska.godina,
           blok.slogovi[i].datumVremeDolaska.sati,
           blok.slogovi[i].datumVremeDolaska.minuti,
           blok.slogovi[i].oznakaCelije,
           blok.slogovi[i].duzinaKazne);
            }
        }
        rbBloka++;
    }

}

int nadjiSlobodanBaket(int adresaPunog, FILE *fajl) {
    BAKET baketi[BAK];
    fseek(fajl, 0, SEEK_SET);
    int i;

    for(i = 0; i < BAK; i++)
        fread(&baketi[i], sizeof(BAKET), 1, fajl);


    for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
        if(baketi[adresaPunog].slogovi[i].deleted != 1)
            return adresaPunog;

    adresaPunog = nadjiSlobodanBaket((adresaPunog+KORAK)%BAK, fajl);
    return adresaPunog;
}

void konverzija(FILE *fajlSS, FILE *fajlRasuta) {
    if(fajlSS == NULL || fajlRasuta == NULL) {
        printf("\n\nNEUSPESNO OTVORENI FAJLOVI\n\n");
        return;
    }

    BAKET baketi[BAK];
    fseek(fajlRasuta, 0, SEEK_SET);
    int i;

    for(i = 0; i < BAK; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajlRasuta);
    }

    BLOK_SS blok;
    fseek(fajlSS, 0, SEEK_SET);

    while(fread(&blok, sizeof(BLOK_SS), 1, fajlSS)) {
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
                return;

            int id = atoi(blok.slogovi[i].evidBroj);
            int adresa = id%BAK;    //Trazimo adresu gde treba da upisujemo evidencioni broj u rasutu datoteku
            int j;

            for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
                if(baketi[adresa].slogovi[FAKTOR_BAKETIRANJA-1].deleted != 0)   //U slucaju da nadjemo mesto za tog zatvorenika u jednom baketu a svako mesto je zauzeto (znaci da je ovaj podatak PREKORACILAC) koristimo funkciju nadjiSlobodanBlok koja trazi da li postoji jos slobodnih mesta
                    adresa = nadjiSlobodanBaket(adresa, fajlRasuta);

                if(baketi[adresa].slogovi[j].deleted == 0) {            //U slucaju da nadjemo mesto za tog zatvorenika i to mesto je slobodno (ILI JE PREKORACILAC NASAO SEBI MESTO) ubacujemo zatvorenika
                    baketi[adresa].slogovi[j].deleted = 1;              //Deleted prelazi iz stanja EMPTY u stanje ACTIVE
                    baketi[adresa].slogovi[j].duzinaKazne = blok.slogovi[i].duzinaKazne;
                    strcpy(baketi[adresa].slogovi[j].evidBroj, blok.slogovi[i].evidBroj);
                    strcpy(baketi[adresa].slogovi[j].sifraZatvorenika, blok.slogovi[i].sifraZatvorenika);
                    sprintf(baketi[adresa].slogovi[j].datumDolaska, "%d-%d-%d %d:%d", blok.slogovi[i].datumVremeDolaska.dan, blok.slogovi[i].datumVremeDolaska.mesec, blok.slogovi[i].datumVremeDolaska.godina, blok.slogovi[i].datumVremeDolaska.sati, blok.slogovi[i].datumVremeDolaska.minuti);
                    //strcpy(baketi[adresa].slogovi[j].datumDolaska, "-");
                    strcpy(baketi[adresa].slogovi[j].oznakaCelije, blok.slogovi[i].oznakaCelije);
                    break;      //Upisali smo vrednost tog clana, nema potrebe da prolazimo kroz celu RASUTU datoteku (moze cak doci i do gresaka!!!)
                }
            }

            fseek(fajlRasuta, adresa*sizeof(BAKET), SEEK_SET);      //adresa*baket = baket u kome se nalazi podatak koji smo upisali
            fwrite(&baketi[adresa], sizeof(BAKET), 1, fajlRasuta);
        }

    }

}

int postojiSifra(FILE *fajl, int sifra) {
    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BAK];
    int i, j;

    for(i = 0; i < BAK; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++)
            if(sifra == baketi[i].slogovi[j].sifraZatvorenika)
                return -1;

    }

    return 0;
}

void unesiRasutiSlog(FILE *fajl, SLOG *slog) {
    if(fajl == NULL)
        return;

    int id = atoi(slog->sifraZatvorenika)%BAK;
    int faktor = postojiSifra(fajl, atoi(slog->sifraZatvorenika));

    if(faktor == -1)
        return;

    BAKET baketi[BAK];
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


SLOG *pronadjiSlog(FILE *fajl, char *evidBroj, int tip) { //FALSE - SERIJSKA / TRUE - SEKVENCIJALNA
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BLOK_SS blok;

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
                return NULL;
            if(tip != 0 && atoi(blok.slogovi[i].evidBroj) > atoi(evidBroj)) //Ako je sekvencijalna datoteka, takodje moramo da proverimo da li je evidencioni broj koji je poslat manji od onog sto trenutno gledamo u datoteci, samim tim ne postoji takav u datoteci
                return NULL;
            if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted) {
                SLOG_SS *slog = (SLOG *) malloc(sizeof(SLOG_SS));
                memcpy(slog, &blok.slogovi[i], sizeof(SLOG_SS));
                return slog;
            }
        }
    }

    return NULL;
}

void unesiSerSlog(FILE *fajl, SLOG_SS *slog) {
    if(fajl == NULL)
        return;

    SLOG_SS *slogStari = pronadjiSlog(fajl, slog->evidBroj, 0);
    if(slogStari != NULL) {
        printf("\nVec postoji ovakav clan!\n");
        return;
    }

    BLOK_SS blok;
    fseek(fajl, -sizeof(BLOK_SS), SEEK_END);
    fread(&blok, sizeof(BLOK_SS), 1, fajl);

    int i;
    for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
        if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
            memcpy(&blok.slogovi[i], slog, sizeof(SLOG_SS));
            i++;
            break;
        }
    }

    if(i < FAKTOR_BLOKIRANJA) {
        strcpy(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
        fflush(fajl);
    }
    else {
        fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
        BLOK_SS noviBlok;
        strcpy(noviBlok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&noviBlok, sizeof(BLOK_SS), 1, fajl);
    }

    if(ferror(fajl))
        printf("Greska.\n");
    else
        printf("\nUSPESAN UPIS\n");
}

void unesiSekSlog(FILE *fajl, SLOG_SS *slog) {
    if(fajl == NULL)
        return;

    SLOG_SS noviSlog;
    memcpy(&noviSlog, slog, sizeof(SLOG_SS));
    BLOK_SS blok;
    fseek(fajl, 0 , SEEK_SET);

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
                memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG_SS));
                if(i != FAKTOR_BLOKIRANJA - 1) {
                    strcpy(blok.slogovi[i+1].evidBroj, OZNAKA_KRAJA_DATOTEKE);
                    fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                    printf("\nNovi slog dodat u datoteci.\n");
                    return;
                }
                else {
                    fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                    BLOK_SS noviBlok;

                    strcpy(noviBlok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
                    fwrite(&noviBlok, sizeof(BLOK_SS), 1 , fajl);
                    printf("\nNovi slog dodat u datoteci.\n");
                    printf("Dodat novi blok.");
                    return;
                }
            }
            else if(strcmp(blok.slogovi[i].evidBroj, noviSlog.evidBroj) == 0) {
                if(!blok.slogovi[i].deleted)
                    printf("Slog ne postoji.");
                else {
                    memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG_SS));
                    fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                    printf("\nNovi slog je evidentiran.\nPrepisan preko logicki izabranog.\n");
                }
            }
            else if(atoi(blok.slogovi[i].evidBroj) > atoi(noviSlog.evidBroj)) {
                SLOG_SS tmp;
                memcpy(&tmp, &blok.slogovi[i], sizeof(SLOG_SS));
                memcpy(&blok.slogovi[i], &noviSlog, sizeof(SLOG_SS));
                memcpy(&noviSlog, &tmp, sizeof(SLOG_SS));

                if(i == FAKTOR_BLOKIRANJA - 1) {
                    fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                    fflush(fajl);
                }
            }
        }
    }

}

void azurirajSlogRasuta(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BAK];
    int i, j;
    for(i = 0; i < BAK; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            if(strcmp(baketi[i].slogovi[j].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
                return;
            if(strcmp(baketi[i].slogovi[j].evidBroj, evidBroj) == 0 && baketi[i].slogovi[j].deleted == 1) {
                strcpy(baketi[i].slogovi[j].oznakaCelije, oznakaCelije);
                baketi[i].slogovi[j].duzinaKazne = duzinaKazne;
                fseek(fajl, -sizeof(BAKET), SEEK_CUR);
                fwrite(&baketi[i], sizeof(BAKET), 1, fajl);
                fflush(fajl);
                printf("\nSlog izmenjen\n");
                return;
            }
        }
    }
    printf("\nSlog nije izmenjen.\n");
}

void azurirajSlogSerijska(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BLOK_SS blok;

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0)
                return;
            if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted) {
                strcpy(blok.slogovi[i].oznakaCelije, oznakaCelije);
                blok.slogovi[i].duzinaKazne = duzinaKazne;
                fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                fflush(fajl);
                printf("Slog izmenjen.\n");
                return;
            }
        }
    }
}

void azurirajSlogSekvencijalna(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne) {
    if(fajl == NULL)
        return;

    fseek(fajl, 0, SEEK_SET);
    BLOK_SS blok;

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0 || atoi(blok.slogovi[i].evidBroj) > atoi(evidBroj))
                return;
            if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted) {
                strcpy(blok.slogovi[i].oznakaCelije, oznakaCelije);
                blok.slogovi[i].duzinaKazne = duzinaKazne;
                fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                fflush(fajl);
                printf("Slog izmenjen.\n");
                return;
            }
        }
    }
}


void obrisiSlogRasutaLogicki(FILE *fajl, char *evidBroj) {
    if(fajl == NULL)
        return;
    fseek(fajl, 0, SEEK_SET);
    BAKET baketi[BAK];
    int i, j, k = 0;

    for(i = 0; i < BAK; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajl);
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
            if(strcmp(baketi[i].slogovi[j].evidBroj, evidBroj) == 0) {
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
        printf("\nSlog je vec obrisan...\n");
    else
        printf("\nSlog ne postoji...\n");
}


void obrisiSlogSSLogicki(FILE *fajl, char *evidBroj) {
    if(fajl == NULL)
        return;
    fseek(fajl, 0, SEEK_SET);
    int i;
    BLOK_SS blok;

    while(fread(&blok, sizeof(BLOK_SS), 1, fajl)) {
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("\nSlog ne postoji.");
                return;
            }
            if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted) {
                blok.slogovi[i].deleted = 1;
                fseek(fajl, -sizeof(BLOK_SS), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK_SS), 1, fajl);
                fflush(fajl);

                printf("\nSlog je logicki obrisan.\n");
                return;
            }
        }
    }
}

