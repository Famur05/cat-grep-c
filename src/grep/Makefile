CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_grep

test_grep_func:
	$(CC) $(FLAGS) s21_grep.c -o test_grep/s21_grep
	cd test_grep/ && bash test_func_grep1.sh

test_grep_leak:
	$(CC) $(FLAGS) s21_grep.c -o test_grep/s21_grep
	bash test_grep/test_leak_grep.sh

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

clean:
	rm -rf *.o *.log s21_grep test_grep/s21_grep test_grep/*.log

rebuild: clean all