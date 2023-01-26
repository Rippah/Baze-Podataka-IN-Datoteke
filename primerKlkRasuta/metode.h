#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struktura.h"

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
void unesiSlog(FILE *fajl, SLOG *slog);
void trazenjePoKljucu(FILE *fajl, int sifra);
void ispisSvihSlogova(FILE *fajl);
void ispisSloga(SLOG *slog);
void logickoBrisanje(FILE *fajl, int sifra);
void ispisiNajvecuTransakciju(FILE *fajl);
void konverzija(FILE *fajlRasuta);
void ispisSvihSlogovaSerijska(FILE *fajl);
