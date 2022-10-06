#include <unordered_map>
#include <list>
#include <vector>


namespace cache {

template <typename KeyT, typename PageT>
class PerfectCache {

    private:
        int size_ = 0;
        std::unordered_map<KeyT, PageT> cache_hashtable_;
        std::unordered_map<PageT, std::list<int>> future_page_indexes_;
        void fill_future_indexes(const std::vector<PageT>& pages);
        bool delete_last_element_if_needed();

    public:
        PerfectCache(const int size, const std::vector<PageT>& pages): 
            size_(size), cache_hashtable_(), future_page_indexes_() {
                fill_future_indexes(pages);
        }

        bool push (const KeyT& key, const PageT& page);
        bool lookup (const KeyT& key) const;
        void print () const;
        void print_future_table() const;
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
    if (cache_hashtable_.size() > size_) {
        PageT page_to_drop;
        KeyT page_key_to_drop;
        int max_occurence = 0;
        
        for (auto cache_element : cache_hashtable_) {
            auto future_page = future_page_indexes_.find(cache_element.second);
            
            if (future_page == future_page_indexes_.end()) {
                // we found page wich we will no see in future, let's drop it!
                page_to_drop = cache_element.second;
                page_key_to_drop = cache_element.first;
                break;
            }
            else { // we'll see page in future once more
                int last_occurence = future_page->second.front();
                if (last_occurence > max_occurence) {
                    max_occurence = last_occurence;
                    page_to_drop = cache_element.second;
                    page_key_to_drop = cache_element.first;
                }
            }
        }
        
        #ifdef _DEBUG_PRINT
        print_future_table();

        std::cout << "Gonna drop page: " << page_to_drop << "\n";
        #endif

        cache_hashtable_.erase(page_key_to_drop);

        return true;
    }

    return false;
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::push (const KeyT& key, const PageT& page) {
    
    future_page_indexes_[page].pop_front();

    if (future_page_indexes_[page].empty())
        future_page_indexes_.erase(page);

    #ifdef _DEBUG_PRINT

    print();

    if (future_page_indexes_.find(page) != future_page_indexes_.end())
        std::cout << "GONNA PUSH PAGE: " << page << "\n";
    else
        std::cout << "NOT GONNA PUSH PAGE: " << page << "\n";

    std::cout << "look up [" << key << "]: " << lookup(key) << "\n";
    #endif

    if (lookup(key))
        return true;
    
    delete_last_element_if_needed();

    if (future_page_indexes_.find(page) != future_page_indexes_.end())
        cache_hashtable_.insert({key, page});
    
    return false;
}

template <typename KeyT, typename PageT>
bool PerfectCache<KeyT, PageT>::lookup (const KeyT& key) const {
    auto cached_page = cache_hashtable_.find(key);

    if (cached_page != cache_hashtable_.cend())
        return true;
    
    return false;
}

template <typename KeyT, typename PageT>
void PerfectCache<KeyT, PageT>::print () const {
    std::cout<<"CACHE HASH TABLE\n";
    for(auto it : cache_hashtable_)
    {   
        std::cout << it.first << ":" << it.second;
        std::cout<<"\n";
    }
    std::cout<<"\n";

}

template <typename KeyT, typename PageT>
void PerfectCache<KeyT, PageT>::print_future_table () const {
    std::cout<<"FUTURE TABLE\n";

    for(auto it = future_page_indexes_.cbegin(); it != future_page_indexes_.cend(); ++it)
    {   
        std::cout << it->first << ":";
        for (auto& el_it: it->second){
            std::cout << el_it << ", ";
        } 
        std::cout<<"\n";
    }
    
    std::cout<<"\n";

}

} // namespace cache