#include "metode.h"

int main() {

    int trajanje = 1;
    int unosKorisnik;
    FILE *fajlRasuta = NULL;
    FILE *fajlSS = NULL;
    int brojac = 0;             //Brojac sluzi za preglednost koda, ako predje vrednost 0, prvi if u whileu vrsi pauziranje ispisa i onda cisti ekran

    while(trajanje) {
        if(brojac > 0) {
            system("PAUSE");
            system("CLS");
        }

        printf("Odaberite opciju:\n\n");
        printf("\t1 -> Otvaranje Datoteke\n");
        printf("\t2 -> Kreiraj Rasutu Datoteku\n");
        printf("\t3 -> Kreiraj Sekvencijalnu/Serijsku Datoteku\n");
        printf("\t4 -> Ispisi Rasutu Datoteku\n");
        printf("\t5 -> Ispisi Serijsku/Sekvencijalnu Datoteku\n");
        printf("\t6 -> Konverzija iz Serijske/Sekvencijalne u Rasutu\n");
        printf("\t7 -> Dodaj novi slog u Rasutu Datoteku\n");
        printf("\t8 -> Dodaj novi slog u Serijsku Datoteku\n");
        printf("\t9 -> Dodaj novi slog u Sekvencijalnu Datoteku\n");
        printf("\t10 -> Azuriranje Sloga u Rasutoj Datoteci\n");
        printf("\t11 -> Azuriranje Sloga u Serijskoj Datoteci\n");
        printf("\t12 -> Azuriranje Sloga u Sekvencijalnoj Datoteci\n");
        printf("\t13 -> Brisanje Sloga Logicki u Rasutoj Datoteci\n");
        printf("\t14 -> Brisanje Sloga Logicki u Sekvencijalnoj/Serijskoj Datoteci\n");
        printf("\t0 -> Izlaz\n\n");
        if(fajlRasuta == NULL)
            printf("RASUTA DATOTEKA NIJE OTVORENA\n");
        if(fajlSS == NULL)
            printf("SERIJSKA/SEKVENCIJALNA DATOTEKA NIJE OTVORENA\n\n\nUnos:\t");
        scanf("%d", &unosKorisnik);

        switch(unosKorisnik) {
            case 1: {
                if(fajlRasuta != NULL)
                    fclose(fajlRasuta);
                if(fajlSS != NULL)
                    fclose(fajlSS);

                char nazivRasuta[20];
                printf("\nUnesite naziv rasute datoteke za otvaranje:\t");
                scanf("%s", nazivRasuta);
                fajlRasuta = otvoriDatoteku(nazivRasuta);
                char nazivSS[20];
                printf("\nUnesite naziv sekvencijalne/serijske datoteke za otvaranje:\t");
                scanf("%s", nazivSS);
                fajlSS = otvoriDatoteku(nazivSS);
                break;
            }
            case 2: {
                char naziv[20];
                printf("Unesite ime rasute datoteke za kreiranje:\t");
                scanf("%s", naziv);
                kreirajRasutuDatoteku(naziv);
                break;
            }
            case 3: {
                char naziv[20];
                printf("Unesite ime sekvencijalne/serijske datoteke za kreiranje:\t");
                scanf("%s", naziv);
                kreirajSSDatoteku(naziv);
                break;
            }
            case 4: {
                ispisRasute(fajlRasuta);
                break;
            }
            case 5: {
                ispisSS(fajlSS);
                break;
            }
            case 6: {
                konverzija(fajlSS, fajlRasuta);
                break;
            }
            case 7: {
                SLOG slog;
                printf("\nEvidencioni Broj:\t");
                scanf("%s", &slog.evidBroj);
                printf("\nDuzina Kazne:\t");
                scanf("%d", &slog.duzinaKazne);
                printf("\nDatum Dolaska:\t");
                scanf("%s", slog.datumDolaska);
                printf("\nOznaka Celije:\t");
                scanf("%s", slog.oznakaCelije);
                printf("\nSifra Zatvorenika\t");
                scanf("%s", slog.sifraZatvorenika);
                slog.deleted = 1;
                unesiRasutiSlog(fajlRasuta, &slog);
                break;
            }
            case 8: {
                SLOG_SS slog;
                printf("\nEvid Broj:\t");
                scanf("%s", slog.evidBroj);
                printf("\nSifra Zatvorenika:\t");
                scanf("%s", slog.sifraZatvorenika);
                printf("\nDatum i vreme dolaska:\t");
                scanf("%d-%d-%d %d:%d",
                      &slog.datumVremeDolaska.dan,
                      &slog.datumVremeDolaska.mesec,
                      &slog.datumVremeDolaska.godina,
                      &slog.datumVremeDolaska.sati,
                      &slog.datumVremeDolaska.minuti);
                printf("\nOznaka celije:\t");
                scanf("%s", slog.oznakaCelije);
                printf("\nDuzina kazne:\t");
                scanf("%d", &slog.duzinaKazne);
                slog.deleted = 0;
                unesiSerSlog(fajlSS, &slog);
                break;
            }
            case 9: {
                SLOG_SS slog;
                printf("\nEvidencioni Broj:\t");
                scanf("%s", slog.evidBroj);
                printf("\nSifra zatvorenika:\t");
                scanf("%s", slog.sifraZatvorenika);
                printf("\nDatum i vreme dolaska (dd-mm-YYYY HH:mm):\t");
                scanf("%d-%d-%d %d:%d", &slog.datumVremeDolaska.dan, &slog.datumVremeDolaska.mesec, &slog.datumVremeDolaska.godina, &slog.datumVremeDolaska.sati, &slog.datumVremeDolaska.minuti);
                printf("\nOznaka celije:\t");
                scanf("%s", slog.oznakaCelije);
                printf("\nDuzina kazne:\t");
                scanf("%d", &slog.duzinaKazne);
                slog.deleted = 0;
                unesiSekSlog(fajlSS, &slog);
                break;
            }
            case 10: {
                char evidBroj[8+1];
                printf("Unesite evid. broj sloga koji azurirate: ");
                scanf("%s", evidBroj);
                char oznakaCelije[5+1];
                printf("Unesite novu oznaku celije: ");
                scanf("%s", oznakaCelije);
                int duzinaKazne;
                printf("Unesite novu duzinu kazne: ");
                scanf("%d", &duzinaKazne);
                azurirajSlogRasuta(fajlRasuta, evidBroj, oznakaCelije, duzinaKazne);
                break;
            }
            case 11: {
                char evidBroj[8+1];
                printf("Unesite evid. broj sloga koji azurirate: ");
                scanf("%s", evidBroj);
                char oznakaCelije[5+1];
                printf("Unesite novu oznaku celije: ");
                scanf("%s", oznakaCelije);
                int duzinaKazne;
                printf("Unesite novu duzinu kazne: ");
                scanf("%d", &duzinaKazne);
                azurirajSlogSerijska(fajlSS, evidBroj, oznakaCelije, duzinaKazne);
                break;
            }
            case 12: {
                char evidBroj[8+1];
                printf("Unesite evid. broj sloga koji azurirate: ");
                scanf("%s", evidBroj);
                char oznakaCelije[5+1];
                printf("Unesite novu oznaku celije: ");
                scanf("%s", oznakaCelije);
                int duzinaKazne;
                printf("Unesite novu duzinu kazne: ");
                scanf("%d", &duzinaKazne);
                azurirajSlogSekvencijalna(fajlSS, evidBroj, oznakaCelije, duzinaKazne);
                break;
            }
            case 13: {
                char evidBroj[8+1];
                printf("\nUnesi evidencioni broj za logicko brisanje:\t");
                scanf("%s", evidBroj);
                obrisiSlogRasutaLogicki(fajlRasuta, evidBroj);
                break;
            }
            case 14: {
                char evidBroj[8+1];
                printf("\nUnesi evidencioni broj za logicko brisanje:\t");
                scanf("%s", evidBroj);
                obrisiSlogSSLogicki(fajlSS, evidBroj);
                break;
            }
            case 0: {
                trajanje = 0;
                if(fajlRasuta != NULL)
                    fclose(fajlRasuta);
                if(fajlSS != NULL)
                    fclose(fajlSS);
            }
        }

        brojac++;

    }

    printf("\n***USPESAN IZLAZ***\n\n");
    return 0;
}
