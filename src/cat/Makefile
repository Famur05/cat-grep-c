CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_cat

test_cat_func:
	$(CC) $(FLAGS) s21_cat.c -o test_cat/s21_cat
	bash test_cat/test_func_cat.sh

test_cat_leak:
	$(CC) $(FLAGS) s21_cat.c -o test_cat/s21_cat
	bash test_cat/test_leak_cat.sh

s21_cat:
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf *.o *.log s21_cat test_cat/s21_cat

rebuild: clean all