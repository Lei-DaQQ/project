#include <immintrin.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define LEN 1L << 22       // the maximum length of array
#define MAX_GET_NUM 100000 // the maximum numbers to get
#define EPS 1e-6

#define PRINTF 0

// store the result:
#define max_run_count 200
#define methods 256
// const int max_run_count = 100;
// const int methods = 4;

#define Giga 1e9
#define Mega 1e6
#define Unit Giga

enum AVX_TYPE
{
    AVX2,
    AVX512
};

enum AVX_METHODS{
    AVX2_methods = 16,
    AVX512_methods = 256
};

enum Average_times{
    ten = 10,
    twenty = 20
};
