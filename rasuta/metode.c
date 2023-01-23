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
    if(fajl == NULL)
        printf("\n\nFAJL NIJE USPESNO POSLAT\n\n");
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

int nadjiSlobodanBaket(int adresaPunog, FILE *fajlRasuta) {
    int adresaSledeceg = adresaPunog;
    BAKET baketi[BAK];
    fseek(fajlRasuta, 0, SEEK_SET);
    int i;

    for(i = 0; i < BAK; i++) {
        fread(&baketi[i], sizeof(BAKET), 1, fajlRasuta);
    }

    adresaSledeceg = (adresaPunog + KORAK)%BAK; //Trazimo da li postoji slobodan slot

    if(baketi[adresaSledeceg].slogovi[FAKTOR_BAKETIRANJA-1].deleted != 0)   //Ako u tom slotu se nalaze podaci, moramo da trazimo dalje
        adresaSledeceg = nadjiSlobodanBaket(adresaSledeceg, fajlRasuta);

    return adresaSledeceg;
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
                    if(baketi[adresa].slogovi[FAKTOR_BAKETIRANJA-1].deleted != 0)   //U slucaju da nadjemo mesto za tog zatvorenika a to mesto je zauzeto (znaci da je ovaj podatak PREKORACILAC) koristimo funkciju nadjiSlobodanBlok koja trazi da li postoji jos slobodnih mesta
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


void unesiRasutiSlog(FILE *fajl, SLOG *slog) {
    int indeks = nadjiSlobodanBaket(atoi(slog->evidBroj)%BAK, fajl);

    BAKET baketi[BAK];
    fseek(fajl, indeks*sizeof(BAKET), SEEK_SET);
    int j;
    for(j = 0; j < FAKTOR_BAKETIRANJA; j++) {
        if(baketi[indeks].slogovi[j].deleted != 0) {
            memcpy(&baketi[indeks].slogovi[j], slog, sizeof(SLOG));
            break;
        }
    }
    fseek(fajl, -sizeof(SLOG), SEEK_CUR);
    fwrite(&baketi[indeks].slogovi[j], sizeof(SLOG), 1, fajl);
}



