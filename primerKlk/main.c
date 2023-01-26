#include <stdio.h>
#include <stdlib.h>
#include "metode.h"

int main()
{
    int trajanje = 1, brojac = 0, unosKorisnik;
    FILE *fajl = NULL;

    while(trajanje) {
        if(brojac > 0) {
            system("PAUSE");
            system("CLS");
        }
        brojac++;

        printf("\n1.Otvori Datoteku\n");
        printf("\n2.Pronadji Slog\n");
        printf("\n3.Dodaj Slog\n");
        printf("\n4.Ispisi Slogove\n");
        printf("\n5.Obrisi Slog Logicki\n");
        printf("\n6.Kreiraj Datoteku\n");
        printf("\n7.Pronadji najvecu kolicinu\n");
        printf("\n8.Povecaj transakciju po ID\n");
        printf("\n9.Nadji prosek transakcija po ID\n");
        printf("\n0.Izadji iz programa\n\n\n");

        if(fajl == NULL)
            printf("\nFajl nije otvoren.\n\n");

        scanf("%d", &unosKorisnik);

        switch(unosKorisnik) {
            case 1: {
                char filename[20];
                printf("\nUnesi ime datoteke:\t");
                scanf("%s", filename);
                fajl = otvoriDatoteku(filename);
                break;
            }
            case 2: {
                int sifraTran;
                printf("\nUnesite sifru transakcije:\t");
                scanf("%d", &sifraTran);
                SLOG *slog = pronadjiSlog(fajl, sifraTran);
                if(slog == NULL)
                    printf("\nSlog ne postoji.\n");
                else
                    ispisiSlog(slog);
                break;
            }
            case 3: {
                SLOG slog;
                printf("\nUnesi sifru transakcije:\n");
                scanf("%d", &slog.sifraTran);
                printf("\nUnesi idSluz:\n");
                scanf("%s", slog.idSluz);
                printf("\nUnesi datum:\n");
                scanf("%s", slog.datumTran);
                printf("\nUnesi kolPren:\n");
                scanf("%f", &slog.kolPren);
                printf("\nUnesi idKlij:\n");
                scanf("%s", slog.idKlij);
                printf("\nUnesi racun klijenta:\n");
                scanf("%d", &slog.brRacuna);
                slog.obrisan = 0;
                dodajSlog(fajl, &slog);
                break;
            }
            case 4: {
                ispisiSveSlogove(fajl);
                break;
            }
            case 5: {
                int sifraTran;
                printf("\nUnesi sifru transakcije:\n");
                scanf("%d", &sifraTran);
                obrisiSlogLogicki(fajl, sifraTran);
                break;
            }
            case 6: {
                char filename[20];
                printf("\nUnesite ime datoteke:\t");
                scanf("%s", filename);
                kreirajDatoteku(filename);
                break;
            }
            case 7: {
                pronadjiNajveciPrenos(fajl);
                break;
            }
            case 8: {
                printf("\nUnesite id ciju transakciju zelite da izmenite za 10%:\t");
                char id[7];
                scanf("%s", id);
                povecajTransakcijuPoID(fajl, &id);
                break;
            }
            case 9: {
                printf("\nUnesite id:\t");
                char id[7];
                scanf("%s", id);
                prosekPoID(fajl, &id);
                break;
            }
            case 0: {
                trajanje = 0;
                if(fajl != NULL)
                    fclose(fajl);
                printf("\nMars napolje\n");
            }
        }

    }

    return 0;
}
