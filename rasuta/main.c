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
        printf("\t3 -> Ispisi Rasutu Datoteku\n");
        printf("\t4 -> Konverzija iz Serijske u Rasutu\n");
        printf("\t0 -> Izlaz\n\n");
        if(fajlRasuta == NULL)
            printf("RASUTA DATOTEKA NIJE OTVORENA\n");
        if(fajlSS == NULL)
            printf("SERIJSKA/SEKVENCIJALNA DATOTEKA NIJE OTVORENA\n\n\nUnos:\t");
        scanf("%d", &unosKorisnik);

        switch(unosKorisnik) {
            case 1: {

                break;
            }
            case 2: {

                break;
            }
            case 3: {

                break;
            }
            case 4: {

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
