#include "cat.h"


int main(int argc, char **argv) {
  flags flag = {0};

  parse_file(argc, argv, &flag);
  for (int i = optind; i < argc; i++) {
    print_file(argv[i], flag);
  }
  return 0;
}
FILE *open_file(char *name) {
  FILE *file = fopen(name, "r");
  if (file == NULL) {
    fprintf(stderr, "File not found");
  }
  return file;
}
void print_file(char *name, flags flag) {
  FILE *file = open_file(name);
  int c = 0;
  int count_line = 1;
  int empty_line = 0;
  int new_line = 1;
  int prev_line = '\n';
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n' && prev_line == '\n') {
      empty_line++;
    } else {
      empty_line = 0;
    }
    if (!(flag.squeeze && empty_line > 1)) {
      if (flag.number && new_line) {
        printf("%6d\t", count_line++);
        new_line = 0;
      }
      if (flag.end_line && flag.number_nonblack && c == '\n' &&
          prev_line == '\n') {
        printf("      \t");
      }
      if (flag.number_nonblack && c != '\n' && new_line) {
        printf("%6d\t", count_line++);
        new_line = 0;
      }
      if (flag.tab && c == '\t') {
        printf("^");
        c = 'I';
      }
      if (c == '\n' && flag.end_line) {
        printf("$");
      }
      if (flag.v && c != '\n' && c != '\t') {
        if (c > 127 && c < 160) {
          printf("M-^");
          c -= 64;
        }
        if (c >= 0 && c <= 31) {
          printf("^");
          c += 64;
        } else if (c == 127) {
          printf("^");
          c = '?';
        }
      }
      if (c == '\n') {
        new_line = 1;
      }
      putchar(c);
      prev_line = c;
    }
  }
  fclose(file);
}

void parse_file(int argc, char **argv, flags *flag) {
  char *short_opt = "beEnstTv";
  struct option long_opt[] = {{"number-nonblank", no_argument, NULL, 'b'},
                              {"number", no_argument, NULL, 'n'},
                              {"squeeze-blank", no_argument, NULL, 's'}};

  int res;
  while ((res = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
    switch (res) {
      case 'e':
        flag->v = 1;
        flag->end_line = 1;
        break;
      case 'E':
        flag->end_line = 1;
        break;
      case 's':
        flag->squeeze = 1;
        break;
      case 'b':
        flag->number_nonblack = 1;
        flag->number = 0;
        break;
      case 'n':
        if (!flag->number_nonblack) {
          flag->number = 1;
        }
        break;
      case 't':
        flag->tab = 1;
        flag->v = 1;
        break;
      case 'T':
        flag->tab = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case '?':
        fprintf(stderr, "Unknown option: -%c\n", optopt);
        break;
      default:
        fprintf(stderr, "usage: %s [-ebnts] [file ...]\n", argv[0]);
        break;
    }
  }
}
