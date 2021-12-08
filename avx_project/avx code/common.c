#include "define.h"

struct timeval t1, t2;

double array[LEN] = {0};
double trash_array[LEN] = {0};

int idex[MAX_GET_NUM] = {0};	// randomly index
								// __attribute__ ((aligned (32))) double res[MAX_GET_NUM] = {0};
double res[MAX_GET_NUM] = {0};	// numbers using gather
double nres[MAX_GET_NUM] = {0}; // numbers without gather

int get_num[max_run_count] = {0};
double GBS[max_run_count][methods] = {0};
// double GBS[max_run_count][methods] ;
int kase = 0; // index

char **cacheline_hit_miss_status;


// char avx2_miss_hit[16][5] = {0};
// char avx512_miss_hit[256][9] = {0};

double general_by_index(int num)
{
	double time = 0.0;

	gettimeofday(&t1, NULL);
	for (int i = 0; i < num; i++)
	{
		nres[i] = array[idex[i]];
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\ngeneral by index: read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);
	return num / Unit / time * 1000 * 8;
}

void gen_miss_hit(int avx)
{
	int a = 0;
	if (avx == AVX2)
	{
		//avx2
		while (a < 16)
		{
			int t = a;
			int pos = 3;
			while (pos >= 0)
			{
				// avx2_miss_hit[a * AVX2_methods + pos] = (t & 1) + '0';
				cacheline_hit_miss_status[a][pos] = (t & 1) + '0';
				t >>= 1;
				pos--;
			}
			// avx2_miss_hit[a * AVX2_methods + 4] = 0;

			cacheline_hit_miss_status[a][4] = 0;
			// printf("%s\n", avx2_miss_hit[a]);

			a += 1;
		}
	}
	else if (avx == AVX512)
	{
		while (a < 256)
		{
			int t = a;
			int pos = 7;
			while (pos >= 0)
			{
				// avx512_miss_hit[a * AVX512_methods + pos] = (t & 1) + '0';

				cacheline_hit_miss_status[a][pos] = (t & 1) + '0';
				t >>= 1;
				pos--;
			}
			// avx512_miss_hit[a * AVX512_methods + 4] = 0;

			cacheline_hit_miss_status[a][8] = 0;
			// printf("%s\n", cacheline_hit_miss_status[a]);
			a += 1;
		}
	}
}

double getrandomd(int max, int min)
{
	double m1 = (double)(rand() % 101) / 101;
	min++;
	double m2 = (double)((rand() % (max - min + 1)) + min);
	m2--;
	return m1 + m2;
}

int rand_int(int min, int max)
{
	return (int)(1.0 * rand() / RAND_MAX * (max - min) + min);
}

int flush_cache()
{
	//meaningless operation:
	double sum = 0.0;
	for (int i = 0; i < LEN; i += 8)
	{
		sum += trash_array[i];
	}
	if (sum > 100000)
		return 1;
	return 0;
}

double general_sequentially(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	// without gather
	gettimeofday(&t1, NULL);
	for (int i = 0; i < num; i++)
	{
		nres[i] = array[i];
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\nwithout gather, sequentially read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);
	return num / Unit / time * 1000 * 8;
}

double general_randomly0(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	//without gather
	gettimeofday(&t1, NULL);
	for (int i = 0; i < num; i++)
	{
		nres[i] = array[idex[i]];
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\nwithout gather randomly read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);
	return num / Unit / time * 1000 * 8;
}

double general_randomly1(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	//without gather
	gettimeofday(&t1, NULL);

	// for (int i = 0, pos = 0; i < num / 4; i++, pos += 8)
	// {
	// 	nres[pos] = array[pos];
	// 	pos += 8;
	// 	nres[pos] = array[pos];
	// 	pos += 8;
	// 	nres[pos] = array[pos];
	// 	pos += 8;
	// 	nres[pos] = array[pos];
	// }
	for (int i = 0, pos = 0; i < num / 4; i++, pos += 32)
	{
		nres[pos] = array[pos];
		nres[pos + 8] = array[pos + 8];
		nres[pos + 16] = array[pos + 16];
		nres[pos + 24] = array[pos + 24];
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\nwithout gather randomly read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);
	return num / Unit / time * 1000 * 8;
}

void verify(int num)
{
	//verification:
	printf("\n");
	double sum_g = 0;
	for (int i = 0; i < num; i++)
	{
		sum_g += res[i];
	}
	// printf("sum of ' gather': %f\n", sum_g);

	double sum_n = 0;
	for (int i = 0; i < num; i++)
	{
		sum_n += nres[i];
	}
	// printf("sum of 'general': %f\n", sum_n);

	if (fabs(sum_g - sum_n) >= EPS)
	{
		printf("don't pass! exit!\n");
		printf("sum of ' gather': %f\n", sum_g);
		printf("sum of 'general': %f\n", sum_n);
		exit(0);
	}
}

void initialize()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < LEN; ++i)
	{
		array[i] = getrandomd(10000, -10000);
	}

	for (int i = 0; i < LEN; ++i)
	{
		trash_array[i] = getrandomd(100, -100);
	}

	// for (int i = 0; i < MAX_GET_NUM; ++i)
	// {
	// 	idex[i] = rand_int(0, LEN - 1);
	// }
	cacheline_hit_miss_status = (char **)malloc(sizeof(char *) * AVX512_methods);
	for (int i = 0; i < AVX512_methods; ++i)
	{
		cacheline_hit_miss_status[i] = (char *)malloc(sizeof(char) * 9);
	}

}

void store_as_csv_v2(char *csv_name, int len)
{
	if (access("./data", 0) == -1)
	{
		mkdir("data", 00700);
	}

	char dir[50] = "./data/";
	strcat(dir, csv_name);
	csv_name = dir;
	FILE *fp = fopen(csv_name, "w");

	fprintf(fp, "num");
	if (len == AVX2_methods)
	{
		for (int i = 0; i < len; ++i)
		{

			fprintf(fp, ",%s", cacheline_hit_miss_status[i]);
			// printf("%s\n",avx2_miss_hit[i]);
		}
	}
	else
	{
		for (int i = 0; i < len; ++i)
		{

			fprintf(fp, ",%s", cacheline_hit_miss_status[i]);
		}
	}
	fprintf(fp, "\n");

	for (int i = 0; i < kase; ++i)
	{
		fprintf(fp, "%d", get_num[i]);
		// printf("%d,", get_num[i]);

		fprintf(fp, ",%.5lf", GBS[i][0]);
		for (int j = 1; j < len; ++j)
		{
			fprintf(fp, ",%.5lf", GBS[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void store_as_csv_v1(char *csv_name)
{
	if (access("./data", 0) == -1)
	{
		mkdir("data", 00700);
		// char *argv[] = {"mkdir", "data", NULL};
		// execvp("mkdir", argv);
	}

	// printf("%d\n", access("./data", 0));
	// int flag = 0;
	// while (access("./data", 0) == -1)
	// {
	// if (flag == 0)
	// 	flag = 1;
	// else
	// 	continue;
	// 	char *argv[] = {"mkdir", "data", NULL};
	// 	if (0 == execvp("mkdir", argv))
	// 		;
	// }
	// printf("%d\n", access("./data", 0));

	char dir[50] = "./data/";
	strcat(dir, csv_name);
	csv_name = dir;
	FILE *fp = fopen(csv_name, "w");

	fprintf(fp, "num,gather_sequentially,general_sequentially,gather_randomly,general_randomly\n");
	for (int i = 0; i < kase; ++i)
	{
		fprintf(fp, "%d,", get_num[i]);
		// printf("%d,", get_num[i]);
		for (int j = 0; j < methods; ++j)
		{
			// if (j == 0)
			// 	printf("%.5lf,", GBS[i][j]);
			fprintf(fp, "%.5lf,", GBS[i][j]);
		}
		fprintf(fp, "\n");
		// printf("\n");
	}
	fclose(fp);
}

void gen_ind(int AVX, char *type, int len)
{
	int time = 0;
	if (AVX == AVX2)
		time = 4;
	else if (AVX == AVX512)
		time = 8;

	idex[0] = 0;
	for (int i = 1; i < len; ++i)
	{

		if (type[i % time] == '0')
			idex[i] = ((idex[i - 1] >> 3) << 3) + 8;
		else
			idex[i] = idex[i - 1] + 1;
		// printf("%d\t", idex[i]);
	}
}


// int main()
// {
// 	// gen_ind(AVX2, "1000", 20);
// 	gen_miss_hit();
// 	return 0;
// }