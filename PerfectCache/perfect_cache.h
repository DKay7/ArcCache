#include <unordered_map>
#include <list>
#include <vector>


namespace cache {

template <typename KeyT, typename PageT>
class PerfectCache {

    private:
        int size_ = 0;
        std::list<PageT> cache_list_;
        using list_const_iter = typename std::list<PageT>::const_iterator;
        std::unordered_map<KeyT, list_const_iter> cache_hashtable_;
        std::unordered_map<PageT, std::vector<int>> future_page_indexes_;
        void fill_future_indexes(const std::vector<PageT>& pages);
        bool delete_last_element_if_needed();

    public:
        PerfectCache(const int size, const std::vector<PageT>& pages): 
            size_(size), cache_list_(), cache_hashtable_(), future_page_indexes_() {
                fill_future_indexes(pages);
        }

        bool push (const KeyT& key, const PageT& page);
};

template <typename KeyT, typename PageT>
void PerfectCache<KeyT, PageT>::fill_future_indexes(const std::vector<PageT>& pages) {
    int idx = 0;
    for (auto& page : pages) {
        auto hashtable_element = future_page_indexes_.find(page);

        if (hashtable_element != future_page_indexes_.cend())
            hashtable_element->second.push_back(idx);
        else
            future_page_indexes_.insert({page, {idx}});

        idx++;
    }
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::delete_last_element_if_needed() {
    if (cache_list_.size() > size_) {
        PageT page_to_drop;
        int min_occurence = 0;
        
        for (auto& lru_list_element : cache_list_) {
            auto future_page = future_page_indexes_.find(lru_list_element);
            
            if (future_page != future_page_indexes_.cend()) { // we'll see page in future once more
                auto first_occurence = *min_element(std::begin(future_page->second), std::end(future_page->second));

                if (first_occurence < min_occurence) {
                    first_occurence = min_occurence;
                    page_to_drop = future_page->first;
                }
            }
        }

          
        std::erase_if(cache_hashtable_, [&](const auto& item) { return *item.second == page_to_drop; });
        cache_list_.remove(page_to_drop);

        return true;
    }

    return false;
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::push (const KeyT& key, const PageT& page) {
    

    delete_last_element_if_needed();
    auto cached_page = cache_hashtable_.find(key);

    if (cached_page != cache_hashtable_.cend())
        return true;

   
    cache_list_.push_back(page);
    cache_hashtable_.insert({key, std::prev(cache_list_.cend())});
    
    return false;
}

} // namespace cache