#include "common.h"

double gather_sequentially(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0;
	}
	double time = 0.0;

	gettimeofday(&t1, NULL);
	for (int i = 0; i < num / 8; i++)
	{

		__m512i ind8 = _mm512_set_epi64(8 * i, 8 * i + 1, 8 * i + 2, 8 * i + 3, 8 * i + 4, 8 * i + 5, 8 * i + 6, 8 * i + 7);
		__m512d gather = _mm512_i64gather_pd(ind8, &array, 8);
		// _mm512_storeu_pd(res + (8 * i), gather);
	}
	gettimeofday(&t2, NULL);
	time = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000.0; //ms
	if (PRINTF)
		printf("\ngather sequentially read %d double from %ld elements costs %.5lf ms.\n", num, LEN, time);

	/************************stroe time**************************************/
	// struct timeval t3, t4;
	// /double store_time = 0.0;
	// for (int i = 0; i < num / 4; i++)
	// {

	// 	__m512i ind8 = _mm512_set_epi64(8 * i, 8 * i + 1, 8 * i + 2, 8 * i + 3, 8 * i + 4, 8 * i + 5, 8 * i + 6, 8 * i + 7);
	// 	__m512d gather = _mm512_i64gather_pd(ind8, &array, 8);
	// 	gettimeofday(&t3, NULL);
	// 	_mm512_storeu_pd(res + (8 * i), gather);
	// 	gettimeofday(&t4, NULL);

	// 	store_time += 1000 * (t4.tv_sec - t3.tv_sec) + (t4.tv_usec - t3.tv_usec) / 1000.0; //ms
	// }
	// // printf("store time: %.5f\n", store_time);
	// time -= store_time;
	return num / Unit / time * 1000 * 8;
}

double gather_randomly(int num)
{
	if (num > MAX_GET_NUM)
	{
		printf("too large!\n");
		return 0.0;
	}
	double time = 0.0;

	gettimeofday(&t1, NULL);
	// for (int i = 0; i < num ; i = i + 64)
	for (int i = 0, pos = 0; i < num / 8; i++, pos += 64)
	{

		// __m512i ind8 = _mm512_set_epi64(idex[8 * i], idex[8 * i + 1], idex[8 * i + 2],
		// 								idex[8 * i + 3], idex[8 * i + 4], idex[8 * i + 5],
		// 								idex[8 * i + 6], idex[8 * i + 7]);
		__m512i ind8 = _mm512_set_epi64(pos, pos + 8, pos + 16,
										pos + 24, pos + 32, pos + 40,
										pos + 48, pos + 56);
		__m512d gather = _mm512_i64gather_pd(ind8, &array, 8);
		// _mm512_storeu_pd(res + (8 * i), gather);
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

	// 	__m512i ind8 = _mm512_set_epi64(idex[8 * i], idex[8 * i + 1], idex[8 * i + 2],
	// 									idex[8 * i + 3], idex[8 * i + 4], idex[8 * i + 5],
	// 									idex[8 * i + 6], idex[8 * i + 7]);
	// 	__m512d gather = _mm512_i64gather_pd(ind8, &array, 8);
	// 	gettimeofday(&t3, NULL);
	// 	_mm512_storeu_pd(res + (8 * i), gather);
	// 	gettimeofday(&t4, NULL);

	// 	store_time += 1000 * (t4.tv_sec - t3.tv_sec) + (t4.tv_usec - t3.tv_usec) / 1000.0; //ms
	// }
	// // printf("store time: %.5f\n", store_time);
	// time -= store_time;

	return num / Unit / time * 1000 * 8;
}

void previous_test()
{

	kase = 0;
	for (int i = 100; i <= MAX_GET_NUM; i += 1000)
	{
		get_num[kase] = i; // only store 1 time.
		int j = 0;

		double seq_gbs = 0.0;
		for (int k = 0; k < 20; ++k)
		{
			flush_cache();
			seq_gbs += gather_sequentially(i);
		}
		GBS[kase][j++] = seq_gbs / 20;

		seq_gbs = 0.0;
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
		GBS[kase][j++] = gather_randomly(i);
		flush_cache();
		GBS[kase][j++] = general_randomly0(i);
		kase++;
	}
}

double gather_by_index(int num)
{

	double time = 0.0;

	gettimeofday(&t1, NULL);

	int base = 0;
	for (int i = 0; i < num / 8; i++)
	{

		__m512i ind8 = _mm512_set_epi64(idex[base++], idex[base++], idex[base++], idex[base++],
										idex[base++], idex[base++], idex[base++], idex[base++]);
		__m512d gather = _mm512_i64gather_pd(ind8, &array, 8);
		// _mm512_store_pd(res + (8 * i), gather);
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

	for (int i = 0; i < 256; ++i)
	{
		kase = 0;
		// printf("status:%s\n",cacheline_hit_miss_status[i]);
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

	initialize();
	start_to_test_v2(AVX512);

	/*********************** store the result as csv format****************/
	char filename[20] = "GBS512";
	if (argc > 1)
		strcpy(filename, argv[1]);
	strcat(filename, ".csv");
	store_as_csv_v2(filename,AVX512_methods);

	return 0;
}
