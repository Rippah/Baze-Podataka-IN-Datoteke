#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#include "strukture.h"

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraTran);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG *slog);
void obrisiSlogLogicki(FILE *fajl, int sifraTran);
void pronadjiNajveciPrenos(FILE *fajl);
void povecajTransakcijuPoID(FILE *fajl, char *id);
void prosekPoID(FILE *fajl, char *id);
