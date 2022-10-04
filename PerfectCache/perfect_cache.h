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
        bool lookup (const KeyT& key) const;
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
    if (cache_list_.size() >= size_) {
        PageT page_to_drop;
        int max_occurence = 0;
        
        for (auto& cache_list_element : cache_list_) {
            auto future_page = future_page_indexes_.find(cache_list_element);
            
            if (future_page == future_page_indexes_.cend() or future_page->second.size() == 0) { 
                // we found page wich we will no see in future, let's drop it!
                page_to_drop = cache_list_.back(); 
                break;
            }
            else { // we'll see page in future once more
                // int first_occurence = *min_element(std::begin(future_page->second), std::end(future_page->second));
                int last_occurence = future_page->second.back();
                if (last_occurence > max_occurence) {
                    max_occurence = last_occurence;
                    page_to_drop = future_page->first;
                }
            }
        }
        
        #ifdef _DEBUG
        std::cout<<"FUTURE TABLE\n";
        for(auto it = future_page_indexes_.cbegin(); it != future_page_indexes_.cend(); ++it)
        {   
            std::cout << it->first << ":";
            for (auto& el_it: it->second){
                std::cout << el_it << ", ";
            } 
            std::cout<<"\n";
        }

        std::cout << "LIST SIZE: " << cache_list_.size() << " PAGE TO DROP: " << page_to_drop << std::endl;
        #endif

        std::erase_if(cache_hashtable_, [&](const auto& item) { return *(item.second) == page_to_drop; });
        cache_list_.remove(page_to_drop);

        return true;
    }

    return false;
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::push (const KeyT& key, const PageT& page) {
    
    if (lookup(key))
        return true;

    #ifdef _DEBUG
    std::cout<<"CACHE HASH TABLE\n";
        for(auto it = cache_hashtable_.cbegin(); it != cache_hashtable_.cend(); ++it)
        {   
            std::cout << it->first << ":" << *it->second;
            std::cout<<"\n";
        }

    std::cout << "CUR LIST: \n";
    for (auto& el_it: cache_list_){
        std::cout << el_it << ", ";
    }
    
    std::cout<<"\n";
    std::cout << "GONNA PUSH PAGE: " << page << "\n";
    #endif
    
    delete_last_element_if_needed();

    cache_list_.push_back(page);
    cache_hashtable_.insert({key, std::prev(cache_list_.cend())});
    future_page_indexes_[page].erase(future_page_indexes_[page].begin());
    
    return false;
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::lookup (const KeyT& key) const {
    auto cached_page = cache_hashtable_.find(key);

    if (cached_page != cache_hashtable_.cend())
        return true;
    
    return false;
}

} // namespace cache