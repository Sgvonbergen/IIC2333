#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

lists* get_convinations(int len,int level);

list* optimum(int len, int level);

int score(list* l);
