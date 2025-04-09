# Bash Utils: cat and grep

This project implements basic Bash utilities for text file processing: `cat` and `grep`. These utilities are developed in C and follow the POSIX standards, with support for flags used in real Bash utilities.

## Description

- **cat**: A utility to display file contents, concatenate files, and create new files. It supports flags for line numbering, collapsing empty lines, and displaying special characters like tab and end-of-line symbols.
  
- **grep**: A utility to search for lines matching regular expressions. It supports flags for inverting search results, counting matches, and displaying only matching files.

## Technical Details

- **Language**: C (C11)
- **Standard**: POSIX.1-2017
- **Style**: Google C++ Style Guide
- **Build**: Makefile to compile `s21_cat` and `s21_grep`
- **Libraries**: pcre or regex for regular expression handling

## Project Structure

- `src/cat/`: Source files for the `cat` utility
- `src/grep/`: Source files for the `grep` utility

## Requirements

- `gcc` (C compiler)
- Support for regular expressions using either the pcre or regex library
