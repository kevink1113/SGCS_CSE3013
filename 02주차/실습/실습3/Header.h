#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define BNUM 256
#define LIMIT 72

void Remove_Blanks_At_The_End( char *line );
void Print_Line( char *line, int *Count, int *B_Flag );
void Get_Blanks_Chars( char *line, int Start, int *N_Blanks, int *N_Chars );
