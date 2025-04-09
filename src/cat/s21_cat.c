#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int flag = 1;
  create_new_struct_for_flags flags = read_flags(argc, argv, &flag);
  if (flag) {
    read_files(argc, argv, &flags);
  }
  return 0;
}
//парсер
create_new_struct_for_flags read_flags(int argc, char *argv[], int *flag) {
  //структура длинных флагов
  static struct option long_name[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'n'},
                                      {"squeeze-blank", 0, NULL, 's'},
                                      {"help", 0, NULL, 0},
                                      {NULL, 0, NULL, 0}};
  //обнуляю счетчик
  create_new_struct_for_flags check = {0, 0, 0, 0, 0, 0};
  //определяю введенные флаги
  int current_flag;
  while ((current_flag =
              getopt_long(argc, argv, "+benstvET", long_name, NULL)) != -1) {
    switch (current_flag) {
      case 'b':
        check.b = 1;
        break;
      case 'e':
        check.e = 1;
        check.v = 1;
        break;
      case 'n':
        check.n = 1;
        break;
      case 's':
        check.s = 1;
        break;
      case 't':
        check.t = 1;
        check.v = 1;
        break;
      case 'v':
        check.v = 1;
        break;
      case 'E':
        check.e = 1;
        break;
      case 'T':
        check.t = 1;
        break;
      case 0:
        fprintf(stderr,
                "Usage: cat [OPTIONS: -benstv]... [FILE]...\nConcatenate "
                "FILE(s) to standard output.\n");
        *flag = 0;
        break;
      default:
        fprintf(stderr, "Try 'cat --help' for more information.\n");
        *flag = 0;
        break;
    }
  }
  return check;
}
//считываю файл, использую флаги
void read_files(int argc, char *argv[], create_new_struct_for_flags *flags) {
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      char past_symbol;
      int current_symbol = '\n';
      int count_enter = 0;
      int counter = 1;
      while (1) {
        past_symbol = current_symbol;
        current_symbol = fgetc(file);
        if (feof(file)) {
          break;
        }
        //счетчик enter
        if (past_symbol == '\n') {
          if (current_symbol == '\n') {
            count_enter++;
          } else {
            count_enter = 0;
          }
        }
        //флаги
        if (flags->s && count_enter > 1) {
          continue;
        }
        if (flags->b && past_symbol == '\n' && current_symbol != '\n') {
          printf("%6d\t", counter++);
        }
        if (flags->n && flags->b == 0 && past_symbol == '\n') {
          printf("%6d\t", counter++);
        }
        if (flags->t && current_symbol == '\t') {
          printf("^I");
          past_symbol = current_symbol;
          continue;
        }
        if (flags->v && current_symbol != '\t' && current_symbol != '\n') {
          if (current_symbol >= 0 && current_symbol <= 31) {
            printf("^%c", current_symbol + 64);
            continue;
          } else if (current_symbol == 127) {
            printf("^?");
            continue;
          }
        }
        if (flags->e && current_symbol == '\n') {
          printf("%c", '$');
        }
        putc(current_symbol, stdout);
      }
      fclose(file);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    }
  }
}
