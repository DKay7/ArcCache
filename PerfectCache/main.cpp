#include <iostream>
#include <vector>
#include "perfect_cache.h"

int main() {
    int num_data = 0;
    int cache_size = 0;
    std::cin >> cache_size >> num_data;  


    std::vector<int> pages;

    while (--num_data >= 0) {
        int page = 0;
        std::cin >> page;
        pages.push_back(page);
    }

    cache::PerfectCache<int, int> perfect_cache(cache_size, pages);

    int cache_hit_cnt = 0;
    for (auto& page_it : pages) {

        int key = page_it;
        cache_hit_cnt += perfect_cache.push(key, page_it);
    }



    std::cout << cache_hit_cnt << std::endl;

    return 0;

}