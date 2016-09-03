#include <stdio.h>
#include <ctype.h>
#include "puzzle.h"
#include "hashmap.h"

void parse_puzzle_from_file(const char file_name[], Puzzle* puzzle);
void parse_dictionary_from_file(const char file_name[], HashMap* dictionary);
