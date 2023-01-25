#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strukture.h"

FILE *otvoriDatoteku(char *naziv);
void kreirajRasutuDatoteku(char *naziv);
void kreirajSSDatoteku(char *naziv);
void ispisRasute(FILE *fajl);
void ispisSS(FILE *fajl);
int nadjiSlobodanBaket(int adresaPunog, FILE *fajlRasuta);
void konverzija(FILE *fajlSS, FILE *fajlRasuta);
void unesiRasutiSlog(FILE *fajl, SLOG *slog);
void unesiSerSlog(FILE *fajl, SLOG_SS *slog);
void unesiSekSlog(FILE *fajl, SLOG_SS *slog);
SLOG *pronadjiSlog(FILE *fajl, char *evidBroj, int tip);
void azurirajSlogRasuta(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne);
void azurirajSlogSerijska(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne);
void azurirajSlogSekvencijalna(FILE *fajl, char *evidBroj, char *oznakaCelije, int duzinaKazne);
void obrisiSlogRasutaLogicki(FILE *fajl, char *evidBroj);
void obrisiSlogSSLogicki(FILE *fajl, char *evidBroj);
