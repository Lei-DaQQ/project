//2021-11-23
#include "common.h"



double gather_sequentially(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0;
	}
	double time = 0.0;

	//store time
	// __m256i ind = _mm256_set_epi64x(0, 8, 16, 24);
	// __m256d tgather = _mm256_i64gather_pd(&array, ind, 8);
	// gettimeofday(&t1, NULL);
	// for (int i = 0; i < num / 4; i++)
	// {
	// 	_mm256_store_pd(res + (4 * i), tgather);
	// }
	// gettimeofday(&t2, NULL);

	// time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	// printf("store time:%.5f\n", time);

	gettimeofday(&t1, NULL);
	for (int i = 0; i < num / 4; i++)
	{

		__m256i ind4 = _mm256_set_epi64x(4 * i, 4 * i + 1, 4 * i + 2, 4 * i + 3);
		__m256d gather = _mm256_i64gather_pd(&array, ind4, 8);
		_mm256_store_pd(res + (4 * i), gather);
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0; //ms
																				// printf("**************** time:%.5f\n", time);

	if (PRINTF)
		printf("\ngather sequentially read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);

	// printf("num:%d\t",num);
	// printf("bandwidth:%.5f\t",num / Unit / time * 1000 * 8);
	return num / Unit / time * 1000 * 8;
}

double gather_randomly0(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	gettimeofday(&t1, NULL);
	for (int i = 0; i < num / 4; i++)
	{
		__m256i ind4 = _mm256_set_epi64x(idex[4 * i], idex[4 * i + 1], idex[4 * i + 2], idex[4 * i + 3]);
		__m256d gather = _mm256_i64gather_pd(&array, ind4, 8);
		// _mm256_store_pd(res + (4 * i), gather);
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\ngather randomly read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);

	/************************stroe time**************************************/
	// struct timeval t3, t4;
	// double store_time = 0.0;
	// for (int i = 0; i < num / 4; i++)
	// {

	// 	__m256i ind4 = _mm256_set_epi64x(idex[4 * i], idex[4 * i + 1], idex[4 * i + 2], idex[4 * i + 3]);
	// 	__m256d gather = _mm256_i64gather_pd(&array, ind4, 8);
	// 	gettimeofday(&t3, NULL);
	// 	_mm256_store_pd(res + (4 * i), gather);
	// 	gettimeofday(&t4, NULL);

	// 	store_time += 1000 * (t4.tv_sec - t3.tv_sec) + (t4.tv_usec - t3.tv_usec) / 1000.0; //ms
	// }
	// // printf("store time: %.5f\n", store_time);

	// time -= store_time;
	// // printf("\ngather sequentially read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);

	return num / Unit / time * 1000 * 8;
}

double gather_randomly1(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	//store time
	// __m256i ind = _mm256_set_epi64x(0, 8, 16, 24);
	// __m256d tgather = _mm256_i64gather_pd(&array, ind, 8);
	// gettimeofday(&t1, NULL);
	// for (int i = 0; i < num / 4; i++)
	// {
	// 	_mm256_store_pd(res + (4 * i), tgather);
	// }
	// gettimeofday(&t2, NULL);

	// time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	//

	gettimeofday(&t1, NULL);
	for (int i = 0, pos = 0; i < num / 4; i++, pos += 32)
	{
		__m256i ind4 = _mm256_set_epi64x(pos, pos + 8, pos + 16, pos + 24);
		__m256d gather = _mm256_i64gather_pd(&array, ind4, 8);
		_mm256_store_pd(res + (4 * i), gather);
	}
	gettimeofday(&t2, NULL);

	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\ngather randomly read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);
	return num / Unit / time * 1000 * 8;
}

void previous_test()
{
	kase = 0;
	// double gbs20 = 0.0;
	for (int i = 100; i <= MAX_GET_NUM; i += 1000)
	{
		get_num[kase] = i; // only store 1 time.
		int j = 0;

		double seq_gbs = 0.0;
		for (int k = 0; k < 20; ++k)
		{
			flush_cache();
			seq_gbs += gather_sequentially(i);
			// printf("seq_gbs:%.5f\t",seq_gbs);
		}
		GBS[kase][j++] = seq_gbs / 20;

		// if (kase == 2)
		// {
		// 	gbs20 = GBS[2][0];
		// 	printf("GBS[2][0]:%.5f\t", GBS[2][0]);
		// }
		// if (kase > 2)
		// {
		// 	if (gbs20 != GBS[2][0])
		// 	{
		// 		printf("kase:%d\t", kase);
		// 		printf("gbs20:%.5f, GBS[2][0]:%.5f\n", gbs20, GBS[2][0]);
		// 	}
		// }

		for (int k = 0; k < 10; ++k)
		{
			flush_cache();
			seq_gbs += general_sequentially(i);
		}
		GBS[kase][j++] = seq_gbs / 10;

		// GBS[kase][j++] = gather_sequentially(i);
		// flush_cache();
		// GBS[kase][j++] = general_sequentially(i);
		// flush_cache();
		flush_cache();
		GBS[kase][j++] = gather_randomly0(i);
		flush_cache();
		GBS[kase][j++] = general_randomly0(i);
		kase++;
	}

	// printf("GBS[2][0]:%.5f\t", GBS[2][0]);
}



double gather_by_index(int num)
{

	double time = 0.0;

	gettimeofday(&t1, NULL);

	int base = 0;
	for (int i = 0; i < num / 4; i++)
	{
		
		__m256i ind4 = _mm256_set_epi64x(idex[base++], idex[base++], idex[base++], idex[base++]);
		__m256d gather = _mm256_i64gather_pd(&array, ind4, 8);
		// _mm256_store_pd(res + (4 * i), gather);
	}
	gettimeofday(&t2, NULL);

	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0;
	if (PRINTF)
		printf("\ngather: read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);

	return num / Unit / time * 1000 * 8;
}



void start_to_test_v1(int avx)
{

	gen_miss_hit(avx);
	kase = 0;
	for (int num = 100; num <= MAX_GET_NUM; num += 1000)
	{
		get_num[kase] = num; // only store 1 time.

		for (int i = 0; i < 16; ++i)
		{

			gen_ind(avx, cacheline_hit_miss_status[i], num);

			double gbs = 0.0;
			int cnt = ten;
			for (int k = 0; k < cnt; ++k)
			{

				flush_cache();
				gbs += gather_by_index(num);
			}
			GBS[kase][i] = gbs / cnt;
		}
		++kase;
	}
}

void start_to_test_v2(int avx)
{

	gen_miss_hit(avx);
	// printf("%s\n", avx2_miss_hit[1]);

	for (int i = 0; i < 16; ++i)
	{
		kase = 0;

		gen_ind(avx, cacheline_hit_miss_status[i], MAX_GET_NUM);

		for (int num = 100; num <= MAX_GET_NUM; num += 1000)
		{
			get_num[kase] = num; // only store 1 time.

			int cnt = ten;
			double gbs = 0.0;
			flush_cache();

			for (int k = 0; k < cnt; ++k)
			{
				gbs += gather_by_index(num);
			}
			GBS[kase][i] = gbs / cnt;

			++kase;
		}
	}
}

int main(int argc, char *argv[])
{
	/* 
argv[] 

argv[0]:	program name,  

argv[1]:	the .csv file's name, don's include .csv 
			default name will be GBS256.csv, or GBS512.csv
			suggesting GBS256o0.csv GBS512o1.csv etc.

argv[2]:	your PC's L3 cache capacity. haven't use it.

 */

	initialize();

	// start_to_test_v1(AVX2);
	start_to_test_v2(AVX2);

	/*********************** store the result as csv format****************/
	char filename[20] = "GBS256";
	if (argc > 1)
		strcpy(filename, argv[1]);
	strcat(filename, ".csv");
	// store_as_csv_v1(filename);
	store_as_csv_v2(filename, AVX2_methods);

	return 0;
}
