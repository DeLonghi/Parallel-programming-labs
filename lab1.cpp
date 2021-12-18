#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <chrono>
#include <omp.h>

int max(int num_threads, std::vector<int> &vec)
{
    auto start = std::chrono::high_resolution_clock::now();

    int max_number = 0;

    // omp_set_num_threads(num_threads);
    #pragma omp parallel for  num_threads(num_threads) reduction (max:max_number)
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > max_number)
            max_number = vec[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto res = (end - start).count() / (1000 * 1000);
    std::cout << "number of threads: " << num_threads;
    std::cout << " time taken: " << res << "ms";
    std::cout << "  max number: " << max_number << std::endl;
}

int main(int argc, char** argv) {
    size_t size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    std::vector<int> vec;
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i)
        vec.push_back(i);

    for (int i = 1; i <= num_threads; ++i) 
        max(i, vec);
    
}
