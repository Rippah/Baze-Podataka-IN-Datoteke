#include "metode.h"

int main()
{
    int trajanje = 1, unosKorisnik, brojac = 0;
    FILE *fajlRasuta = NULL;

    while(trajanje) {
        if(brojac > 0) {
            system("PAUSE");
            system("CLS");
        }
        brojac++;

        printf("\n1.Otvori Rasutu");
        printf("\n2.Kreiraj Rasutu");
        printf("\n3.Unos novog sloga (Rasuta)");
        printf("\n4.Trazenje po kljucu (Rasuta)");
        printf("\n5.Ispis slogova (Rasuta)");
        printf("\n6.Logicko brisanje (Rasuta)");
        printf("\n7.Najveca transakcija");
        printf("\n8.Transformacija Rasute u Serijsku\n\n");
        printf("\n0.Izlaz\n\n");

        if(fajlRasuta == NULL)
            printf("Rasuta: NEOTVORENA\n\n");

        scanf("%d", &unosKorisnik);

        switch(unosKorisnik) {
            case 1: {
                if(fajlRasuta != NULL)
                    fclose(fajlRasuta);

                char filename[20];
                printf("\nUnesite ime datoteke:\t");
                scanf("%s", filename);
                fajlRasuta = otvoriDatoteku(filename);
                break;
            }
            case 2: {
                char filename[20];
                printf("\nUnesite ime datoteke:\t");
                scanf("%s", filename);
                kreirajDatoteku(filename);
                break;
            }
            case 3: {
                SLOG slog;
                printf("\nUnesi sifru transakcije:\n");
                scanf("%d", &slog.sifra);
                printf("\nUnesi idSluz:\n");
                scanf("%s", slog.idSluz);
                printf("\nUnesi datum:\n");
                scanf("%s", slog.datumTran);
                printf("\nUnesi kolPren:\n");
                scanf("%f", &slog.kolPren);
                printf("\nUnesi idKlij:\n");
                scanf("%s", slog.idKlij);
                printf("\nUnesi racun klijenta:\n");
                scanf("%d", &slog.brRacun);
                slog.deleted = 1;
                unesiSlog(fajlRasuta, &slog);
                break;
            }
            case 4: {
                int sifra;
                printf("\nUnesite sifru transakcije:\t");
                scanf("%d", &sifra);
                trazenjePoKljucu(fajlRasuta, sifra);
                break;
            }
            case 5: {
                ispisSvihSlogova(fajlRasuta);
                break;
            }
            case 6: {
                int sifra;
                printf("\nUnesite sifru za brisanje:\t");
                scanf("%d", &sifra);
                logickoBrisanje(fajlRasuta, sifra);
                break;
            }
            case 7: {
                ispisiNajvecuTransakciju(fajlRasuta);
                break;
            }
            case 8: {
                konverzija(fajlRasuta);
                break;
            }
            case 0: {
                trajanje = 0;
                printf("\nMars napolje\n");
            }
        }
    }

    return 0;
}
