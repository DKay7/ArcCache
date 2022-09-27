#include <iostream>
#include "lru.h"

int main() {
    int num_data = 0;
    int cache_size = 0;
    std::cin >> cache_size >> num_data;  

    cache::LruCache<int, int> lru{cache_size};
    int cache_hit_cnt = 0;

    while (--num_data >= 0) {
        int page = 0;
        std::cin >> page;
        
        cache_hit_cnt += lru.lookup_insert(page, page);
    }

    #ifdef _DEBUG
    std::cout << "Cache contains: " << std::endl;
    lru.print_cache_data();
    #endif

    std::cout << cache_hit_cnt << std::endl;

    return 0;
}