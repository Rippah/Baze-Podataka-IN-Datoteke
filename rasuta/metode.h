#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strukture.h"

FILE *otvoriDatoteku(char *naziv);
void kreirajRasutuDatoteku(char *naziv);
void kreirajSSDatoteku(char *naziv);
void ispisRasute(FILE *fajl);
void ispisSS(FILE *fajl);
void konverzija(FILE *fajlSS, FILE *fajlRasuta);
