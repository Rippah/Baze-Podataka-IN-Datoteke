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
        printf("\t6 -> Konverzija iz Serijske u Rasutu\n");
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
