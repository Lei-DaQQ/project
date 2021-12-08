#include "define.h"
extern int kase; // 100 ~ 100000 + 100
extern struct timeval t1, t2;

extern double array[LEN];
extern double trash_array[LEN];

extern int idex[MAX_GET_NUM]; // randomly index
// // __attribute__ ((aligned (32))) double res[MAX_GET_NUM] = {0};
extern double res[MAX_GET_NUM];  // numbers using gather
extern double nres[MAX_GET_NUM]; // numbers without gather

extern int get_num[max_run_count];
extern double GBS[max_run_count][methods];

//2021/11/26
// extern char avx2_miss_hit[16][5];
// extern char avx512_miss_hit[256][9];

extern char **cacheline_hit_miss_status;

double getrandomd(int max, int min);
int rand_int(int min, int max);
int flush_cache();

double general_sequentially(int num);
double general_randomly0(int num);
double general_randomly1(int num);

// double gather_by_index(int num);

void verify(int num);

void initialize();

void gen_ind(int AVX, char *type, int len);
void gen_miss_hit(int avx);

void store_as_csv_v1(char *csv_name);
void store_as_csv_v2(char *csv_name, int len);

void start_to_test_v1(int avx);
void start_to_test_v2(int avx);