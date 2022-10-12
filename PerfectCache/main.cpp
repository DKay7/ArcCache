#include <iostream>
#include <vector>
#include "perfect_cache.h"

int main() {
    int num_data = 0;
    int cache_size = 0;
    std::cin >> cache_size >> num_data;  
    perfect_cache::data_t<int, int> data;

    while (--num_data >= 0) {
        int page = 0;
        std::cin >> page;
        data.push_back({page, page});
    }

    int cache_hit_cnt = perfect_cache::count_hits(cache_size, data);

    std::cout << cache_hit_cnt << std::endl;

    return 0;

}