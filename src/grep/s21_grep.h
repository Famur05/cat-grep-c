#ifndef GREP_H
#define GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_patterns 64

typedef struct name {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} create_new_struct_for_flags;

create_new_struct_for_flags read_flags(int argc, char *argv[], int *flag,
                                       char **patterns, int *p_counter,
                                       char *files[argc], int *f_counter);
void additional_patterns(char *argv[], create_new_struct_for_flags *flags,
                         char **patterns, int *p_counter);
void add_text(int argc, char *argv[], char *text[argc], int *t_counter);
void read_files(int argc, create_new_struct_for_flags *flags, char **patterns,
                int *p_counter, char *files[argc], int *f_counter,
                char *text[argc], int *t_counter);
void reg_func(create_new_struct_for_flags *flags, char **patterns,
              int *p_counter, char *line_text, regmatch_t *pmatch,
              int *flag_reg);
void flag_f(int argc, char **patterns, int *p_counter, char *files[argc],
            int *f_counter);
void flag_o(int argc, create_new_struct_for_flags *flags, char **patterns,
            int *p_counter, char *text[argc], int *t_counter, char *line_text,
            regmatch_t *pmatch, int j, int counter_line);
void flag_l(int argc, create_new_struct_for_flags *flags, char *text[argc],
            int *t_counter, int coincidences, int j);
void clear_patterns(create_new_struct_for_flags *flags, char **patterns,
                    int *p_counter, int *p_counter_before);

#endif  // GREP_H