#include <unordered_map>
#include <iostream>
#include <list>

namespace cache {

template <typename I, typename P>
class LruCache
{   
    private:
        int cache_size;
        std::list<P> page_list;
        using it = typename std::list<P>::iterator;
        std::unordered_map<I, it> hashtable;
        bool delete_last_element_if_needed();

    public:
        LruCache(const int size): cache_size(size), page_list(), hashtable() {}
        bool lookup_insert(I page_id, P page);
        void print_cache_data() const;
};

template <typename I, typename P>
bool LruCache<I, P>::delete_last_element_if_needed() {
    if (page_list.size() > cache_size) {
        auto least_used = hashtable.find(page_list.back());
        hashtable.erase(least_used->first);
        page_list.pop_back();

        return true;
    }

    return false;
}

template <typename I, typename P>
bool LruCache<I, P>::lookup_insert(I page_id, P page) {
    
    auto cached_page = hashtable.find(page_id);
    
    if (cached_page == hashtable.end()) {
        // slow_get_page()
        
        // insert page to list and to hashtable
        page_list.push_front (page);
        hashtable.insert ({page_id, page_list.begin()});

        // delete page from cache if list size is more than cache size    
        delete_last_element_if_needed();

        return false;
    }

    //move page to list head
    page_list.splice(page_list.begin(), page_list, cached_page->second);
    

    return true;
}

template <typename I, typename P>
void LruCache<I, P>::print_cache_data() const{

    for (auto &iter : page_list) {
        std::cout << iter << std::endl;
    }
}

} // namespace cache