#ifndef CAT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int end_line;         // e
  int number_nonblack;  // b
  int number;           // n
  int tab;              // t
  int v;                // v
  int squeeze;          // s
} flags;

void print_file(char *name, flags flag);
void parse_file(int argc, char **argv, flags *flag);
FILE *open_file(char *name);

#endif