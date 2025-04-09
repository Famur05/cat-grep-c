#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int flag = 1;
  char *patterns[64];
  int p_counter = 0;
  char *files[argc];
  int f_counter = 0;
  char *text[argc];
  int t_counter = 0;
  int p_counter_before;

  create_new_struct_for_flags flags =
      read_flags(argc, argv, &flag, patterns, &p_counter, files, &f_counter);
  if (flag && argc > 2) {
    additional_patterns(argv, &flags, patterns, &p_counter);
    p_counter_before = p_counter;
    add_text(argc, argv, text, &t_counter);
    read_files(argc, &flags, patterns, &p_counter, files, &f_counter, text,
               &t_counter);
    clear_patterns(&flags, patterns, &p_counter, &p_counter_before);
  }
  return 0;
}
//парсер
create_new_struct_for_flags read_flags(int argc, char *argv[], int *flag,
                                       char **patterns, int *p_counter,
                                       char *files[argc], int *f_counter) {
  static struct option long_name[] = {{"help", 0, NULL, 0}, {NULL, 0, NULL, 0}};

  create_new_struct_for_flags check = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  int current_flag;
  while ((current_flag =
              getopt_long(argc, argv, "e:ivclnhsf:o", long_name, NULL)) != -1) {
    switch (current_flag) {
      case 'e':
        check.e = 1;
        patterns[*p_counter] = optarg;
        (*p_counter)++;
        break;
      case 'i':
        check.i = 1;
        break;
      case 'v':
        check.v = 1;
        break;
      case 'c':
        check.c = 1;
        break;
      case 'l':
        check.l = 1;
        break;
      case 'n':
        check.n = 1;
        break;
      case 'h':
        check.h = 1;
        break;
      case 's':
        check.s = 1;
        break;
      case 'f':
        check.f = 1;
        files[*f_counter] = optarg;
        (*f_counter)++;
        break;
      case 'o':
        check.o = 1;
        break;
      case 0:
        fprintf(stderr,
                "Usage: grep [OPTIONS: -eivclnhsfo]... PATTERNS "
                "[FILE]...\nSearch for PATTERNS in each FILE.\n");
        *flag = 0;
        break;
      default:
        fprintf(stderr,
                "Usage: grep [OPTIONS: -eivclnhsfo]... PATTERNS [FILE]...\nTry "
                "'grep --help' for more information.\n");
        *flag = 0;
        break;
    }
  }
  return check;
}
//разбираю случай когда нет ни -e, ни -f. Чтобы дополнить паттерн
void additional_patterns(char *argv[], create_new_struct_for_flags *flags,
                         char **patterns, int *p_counter) {
  if (flags->e == 0 && flags->f == 0) {
    patterns[*p_counter] = argv[optind++];
    (*p_counter)++;
  }
}
//добавляю текст(-а) в массив текстов
void add_text(int argc, char *argv[], char *text[argc], int *t_counter) {
  for (int i = optind; i < argc; i++) {
    text[*t_counter] = argv[i];
    (*t_counter)++;
  }
}
//считываю файл, использую флаги
void read_files(int argc, create_new_struct_for_flags *flags, char **patterns,
                int *p_counter, char *files[argc], int *f_counter,
                char *text[argc], int *t_counter) {
  //в паттерн шаблонов добавляю шаблоны из файлов
  if (flags->f) {
    flag_f(argc, patterns, p_counter, files, f_counter);
  }
  //случай, когда нет паттернов
  if (*p_counter == 0) {
    exit(1);
  }
  for (int j = 0; j < *t_counter; j++) {
    FILE *file = fopen(text[j], "r");
    if (file != NULL) {
      char *line_text = NULL;
      size_t buffer_size_t = 0UL;
      regmatch_t pmatch[1];
      int counter_line = 0, coincidences = 0;
      while (getline(&line_text, &buffer_size_t, file) != EOF) {
        int flag_reg = 0;
        counter_line++;
        reg_func(flags, patterns, p_counter, line_text, pmatch, &flag_reg);
        if ((flag_reg == 1 && flags->v == 0) ||
            (flag_reg == 0 && flags->v == 1)) {
          coincidences++;
          if (flags->l == 0 && flags->c == 0) {
            if (*t_counter > 1) {
              if (flags->h == 0 && flags->o == 0) {
                printf("%s:", text[j]);
              }
              if (flags->n && flags->o == 0) {
                printf("%d:", counter_line);
              }
              if (flags->o == 1) {
                flag_o(argc, flags, patterns, p_counter, text, t_counter,
                       line_text, pmatch, j, counter_line);
              }
              if (flags->o == 0) {
                printf("%s", line_text);
              }
            } else {
              if (flags->n && flags->o == 0) {
                printf("%d:", counter_line);
              }
              if (flags->o == 1) {
                flag_o(argc, flags, patterns, p_counter, text, t_counter,
                       line_text, pmatch, j, counter_line);
              }
              if (flags->o == 0) {
                printf("%s", line_text);
              }
            }
            if (flags->o == 0) {
              size_t length_enter = strlen(line_text);
              if (line_text[length_enter - 1] != '\n') {
                printf("\n");
              }
            }
          }
        }
      }
      free(line_text);
      line_text = NULL;
      if (flags->l == 1) {
        flag_l(argc, flags, text, t_counter, coincidences, j);
      }
      if (flags->c == 1 && flags->l == 0) {
        if (*t_counter > 1 && flags->h == 0) {
          printf("%s:%d\n", text[j], coincidences);
        } else {
          printf("%d\n", coincidences);
        }
      }
      fclose(file);
    } else {
      if (flags->s == 0) {
        fprintf(stderr, "grep: %s: No such file or directory\n", text[j]);
      }
    }
  }
}

void reg_func(create_new_struct_for_flags *flags, char **patterns,
              int *p_counter, char *line_text, regmatch_t *pmatch,
              int *flag_reg) {
  regex_t regular;
  int regcomp_res;
  int regexec_res;
  for (int i = 0; i < *p_counter; i++) {
    if (flags->i)
      regcomp_res = regcomp(&regular, patterns[i],
                            REG_EXTENDED | REG_ICASE | REG_NEWLINE);
    else
      regcomp_res = regcomp(&regular, patterns[i], REG_EXTENDED | REG_NEWLINE);
    if (regcomp_res != 0) {
      fprintf(stderr, "failed to compile regular expression\n");
      exit(1);
    }
    regexec_res = regexec(&regular, line_text, 1, pmatch, 0);
    if (regexec_res == 0) {
      *flag_reg = 1;
      regfree(&regular);
      break;
    }
    regfree(&regular);
  }
}

void flag_f(int argc, char **patterns, int *p_counter, char *files[argc],
            int *f_counter) {
  for (int i = 0; i < *f_counter; i++) {
    FILE *file = fopen(files[i], "r");
    if (file != NULL) {
      char *line_file = NULL;
      size_t buffer_size_f = 0UL;
      while (getline(&line_file, &buffer_size_f, file) != EOF) {
        //если превышено максимальное количествл паттернов
        if (*p_counter >= max_patterns) {
          break;
        }
        patterns[*p_counter] = line_file;
        size_t length = strlen(patterns[*p_counter]);
        if (length > 0 && patterns[*p_counter][length - 1] == '\n') {
          patterns[*p_counter][length - 1] = '\0';
        }
        (*p_counter)++;
        line_file = NULL;
      }
      //не уверен, что нужно освобождать
      free(line_file);
      line_file = NULL;
      fclose(file);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory\n", files[i]);
      exit(1);
    }
  }
}

void flag_o(int argc, create_new_struct_for_flags *flags, char **patterns,
            int *p_counter, char *text[argc], int *t_counter, char *line_text,
            regmatch_t *pmatch, int j, int counter_line) {
  regex_t regular_o;
  char *line_o = NULL;
  line_o = line_text;
  for (int l = 0; l < *p_counter; l++) {
    if (flags->i) {
      regcomp(&regular_o, patterns[l], REG_EXTENDED | REG_ICASE | REG_NEWLINE);
    } else {
      regcomp(&regular_o, patterns[l], REG_EXTENDED | REG_NEWLINE);
    }
    while (regexec(&regular_o, line_o, 1, pmatch, 0) == 0) {
      if (*t_counter > 1) {
        if (flags->h == 0) {
          printf("%s:", text[j]);
        }
      }
      if (flags->n) {
        printf("%d:", counter_line);
      }
      fprintf(stdout, "%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
              (line_o + pmatch[0].rm_so));
      line_o += pmatch[0].rm_eo;
    }
    regfree(&regular_o);
  }
}

void flag_l(int argc, create_new_struct_for_flags *flags, char *text[argc],
            int *t_counter, int coincidences, int j) {
  if (flags->c == 0 && coincidences > 0) {
    printf("%s\n", text[j]);
  } else if (flags->c == 1 && coincidences > 0 && *t_counter > 1 &&
             flags->h == 0) {
    printf("%s:1\n", text[j]);
    printf("%s\n", text[j]);
  } else if (flags->c == 1 && coincidences > 0 &&
             (*t_counter == 1 || flags->h == 1)) {
    printf("1\n");
    printf("%s\n", text[j]);
  } else if (flags->c == 1 && coincidences == 0 &&
             (*t_counter == 1 || flags->h == 1)) {
    printf("0\n");
  } else if (flags->c == 1 && coincidences == 0 && *t_counter > 1 &&
             flags->h == 0) {
    printf("%s:%d\n", text[j], coincidences);
  }
}

void clear_patterns(create_new_struct_for_flags *flags, char **patterns,
                    int *p_counter, int *p_counter_before) {
  if (flags->f) {
    for (int i = *p_counter_before; i < *p_counter; i++) {
      free(patterns[i]);
    }
  }
}