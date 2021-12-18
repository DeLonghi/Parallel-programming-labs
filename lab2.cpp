#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <chrono>
#include <omp.h>



int alg_matmul2D_ijk(int N, float** a, float** b,
 float** c, int num_threads)
{
       auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
            a[i][j] +=b[i][k] * c[k][j];


    auto end = std::chrono::high_resolution_clock::now();
    auto res = (end - start).count() / (1000 * 1000);

    std::cout << "number of threads: " << num_threads;
    std::cout << " time taken: " << res << "ms" << std::endl;
   return 0;
}


int main(int argc, char** argv) {
   int N=800;
      //  std::cout << "number of threads: ";

   //  size_t size = atoi(argv[1]);
   //  int num_threads = atoi(argv[2]);

    float** mat1  = new float*[N];

   float** mat2 = new float*[N];

    float** product = new float*[N];


   for (int i = 0; i < N; i++) {
      mat1[i] = new float[N];
      mat2[i] = new float[N];
      product[i] = new float[N];
		for (int j = 0; j < N; j++) {
			mat1[i][j] = (float)i;
         mat2[i][j] = (float)i;
         product[i][j] = 0.;
      }
	}


   for (int i = 1; i < 10; ++i) 
    alg_matmul2D_ijk(N, product, mat1, mat2, i);


  
// for (int i = 0; i < N; i++)
//        for (int j = 0; j < N; j++)
  
//           // Prints ' ' if j != n-1 else prints '\n'          
//           std::cout << product[i][j] << " \n"[j == N-1];

   //  std::vector<int> vec;
   //  vec.reserve(size);

   //  for (size_t i = 0; i < size; ++i)
   //      vec.push_back(i);

   //  for (int i = 1; i <= num_threads; ++i) 
   //      max(i, vec);
    
}