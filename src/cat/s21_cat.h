#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct name {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} create_new_struct_for_flags;

create_new_struct_for_flags read_flags(int argc, char *argv[], int *flag);
void read_files(int argc, char *argv[], create_new_struct_for_flags *flags);

#endif  // CAT_H