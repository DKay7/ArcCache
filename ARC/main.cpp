#include <iostream>
#include "arc.h"

int main() {
    int num_data = 0;
    int cache_size = 0;
    std::cin >> cache_size >> num_data;  


    cache::ArcCache<int> arc(cache_size);

    long int cache_hit_cnt = 0;
    while (--num_data >= 0) {
        int page = 0;
        std::cin >> page;
        
        cache_hit_cnt += arc.push(page);
    }

    #ifdef _DEBUG
    std::cout << "Cache contains: " << std::endl;
    arc.cache_printer();
    #endif

    std::cout << cache_hit_cnt << std::endl;

    return 0;
}