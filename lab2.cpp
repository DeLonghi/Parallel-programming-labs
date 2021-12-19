#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <chrono>
#include <omp.h>

int alg_matmul2D_ijk(int N, float **a, float **b, float **c, int num_threads)
{
   auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(num_threads)
   for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
         for (int k = 0; k < N; k++)
            a[i][j] += b[i][k] * c[k][j];

   auto end = std::chrono::high_resolution_clock::now();
   auto res = (end - start).count() / (1000 * 1000);

   std::cout << "number of threads: " << num_threads;
   std::cout << "    time taken: " << res << "ms";
   return (int)res;
}

int alg_matmul2D_ikj(int N, float **a, float **b, float **c, int num_threads)
{
   auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(num_threads)
   for (int i = 0; i < N; i++)
      for (int k = 0; k < N; k++)
         for (int j = 0; j < N; j++)
            a[i][j] += b[i][k] * c[k][j];

   auto end = std::chrono::high_resolution_clock::now();
   auto res = (end - start).count() / (1000 * 1000);

   std::cout << "number of threads: " << num_threads;
   std::cout << "    time taken: " << res << "ms" << std::endl;
   return res;
}

int alg_matmul2D_jki(int N, float **a, float **b, float **c, int num_threads)
{
   auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(num_threads)
   for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++)
         for (int i = 0; i < N; i++)
            a[i][j] += b[i][k] * c[k][j];

   auto end = std::chrono::high_resolution_clock::now();
   auto res = (end - start).count() / (1000 * 1000);

   std::cout << "number of threads: " << num_threads;
   std::cout << "    time taken: " << res << "ms" << std::endl;
   return res;
}

int main(int argc, char **argv)
{
   int N = atoi(argv[1]);
   //  std::cout << "number of threads: ";

   //  size_t size = atoi(argv[1]);
   //  int num_threads = atoi(argv[2]);

   float **mat1 = new float *[N];

   float **mat2 = new float *[N];

   float **product = new float *[N];
   long long t1, t;

   for (int i = 0; i < N; i++)
   {
      mat1[i] = new float[N];
      mat2[i] = new float[N];
      product[i] = new float[N];
      for (int j = 0; j < N; j++)
      {
         mat1[i][j] = (float)i;
         mat2[i][j] = (float)i;
         product[i][j] = 0;
      }
   }

   std::cout << "ijk" << std::endl;
   for (int num_threads = 1; num_threads <= 10; ++num_threads)
   {
      t = alg_matmul2D_ijk(N, product, mat1, mat2, num_threads);
      if (num_threads == 1)
         t1 = t;
      std::cout << "    t1/t: " << (float)t1 / t << std::endl;
      for (int i = 0; i < N; i++)
         for (int j = 0; j < N; j++)
         {
            product[i][j] = 0;
         }
   }
   std::cout << "ikj" << std::endl;

   for (int num_threads = 1; num_threads <= 10; ++num_threads)
   {
      t = alg_matmul2D_ijk(N, product, mat1, mat2, num_threads);
      if (num_threads == 1)
         t1 = t;
      std::cout << "    t1/t: " << (float)t1 / t << std::endl;
      for (int i = 0; i < N; i++)
         for (int j = 0; j < N; j++)
         {
            product[i][j] = 0;
         }
   }
   std::cout << "jki" << std::endl;

   for (int num_threads = 1; num_threads <= 10; ++num_threads)
   {
      t = alg_matmul2D_ijk(N, product, mat1, mat2, num_threads);
      if (num_threads == 1)
         t1 = t;
      std::cout << "    t1/t: " << (float)t1 / t << std::endl;
      for (int i = 0; i < N; i++)
         for (int j = 0; j < N; j++)
         {
            std::cout << product[i][j] << " \n"[j == N - 1];
            product[i][j] = 0;
         }
   }
}