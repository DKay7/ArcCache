#include <iostream>
#include <list>
#include <unordered_map>

namespace cache {

enum SourceList {
    invalid = -1,
    T1 = 1,
    T2 = 2,
    B1 = 3,
    B2 = 4,
};

template <typename KeyT, typename PageT>
class PageIter  {
    private:
        using list_iter = typename std::list<std::pair<KeyT, PageT>>::const_iterator;     
    public:
        list_iter page_ptr_;
        SourceList source_list_;

        PageIter (list_iter page_ptr, SourceList source): 
             page_ptr_(page_ptr), source_list_(source) {}
        
};

template <typename KeyT, typename PageT>
class ArcCache {   
    private:
        std::list<std::pair<KeyT, PageT>> T1; // top of L1
        std::list<std::pair<KeyT, PageT>> T2; // top of L2
        std::list<std::pair<KeyT, PageT>> B1; // bottom of L1
        std::list<std::pair<KeyT, PageT>> B2; // bottom of L2
        std::unordered_map<KeyT, PageIter<KeyT, PageT>> page_hashtable;

        int cache_size = 1; // named "c" in paper
        int p_size  = 0;    // named "p" in paper

        void page_was_found (const PageIter<KeyT, PageT> &page_it); // processing cases 1-3 from paper
        void case_4_1();                                            // proceses case 4-A from paper
        void case_4_2();                                            // proceses case 4-A from paper
        void replace (const SourceList page_source);
        void list_printer(const std::list <std::pair<KeyT, PageT>> &list, const std::string list_name) const;
        void hashtable_printer() const;

    public:
        ArcCache(int size): T1(), T2(), B1(), B2(), cache_size{size >= 1 ? size : 1} {}
        ArcCache () {}
        bool push(const KeyT &key, const PageT &page);      // returns true if hit, false if miss
        bool lookup(const KeyT &key);                       // returns true if hit, false if miss
        void cache_printer () const;

};

template <typename KeyT, typename PageT>
bool ArcCache<KeyT, PageT>::lookup (const KeyT &key) {
    auto cached_page = page_hashtable.find(key);

    if (cached_page != page_hashtable.cend()) {
        std::pair<KeyT, PageT> page = *(cached_page->second.page_ptr_);
        page_was_found (cached_page->second);
        T2.push_front(page);
		page_hashtable.erase(key);
        page_hashtable.insert({key, {T2.cbegin(), SourceList::T2}});

        return true;
    }

    return false;
}

template <typename KeyT, typename PageT>
bool ArcCache<KeyT, PageT>::push(const KeyT &key, const PageT &page) {

    if (lookup(key))
        return true;

    int L1_size = T1.size() + B1.size();
    int L2_size = T2.size() + B2.size();

    if (L1_size == cache_size)
        case_4_1 ();
    
    else if (L1_size < cache_size and L1_size + L2_size >= cache_size)
        case_4_2 ();
    
    T1.push_front({key, page});
    page_hashtable.insert({key, {T1.cbegin(), SourceList::T1}});


    return false;
}

template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::page_was_found (const PageIter<KeyT, PageT> &page_it) {

    switch (page_it.source_list_) {
        case SourceList::T1:
            T1.remove({page_it.page_ptr_->first, page_it.page_ptr_->second});
            break;
        
        case SourceList::T2:
            T2.remove({page_it.page_ptr_->first, page_it.page_ptr_->second});
            break;
        
        case SourceList::B1: {
            p_size = std::min<int> (cache_size, p_size + std::max<int> (B2.size() / B1.size(), 1));
            replace(SourceList::B1);
            B1.remove({page_it.page_ptr_->first, page_it.page_ptr_->second});
            break;
        }
        case SourceList::B2: {
            p_size = std::max<int> (0, p_size - std::max<int> (B1.size() / B2.size(), 1));
            replace (SourceList::B2);
            B2.remove({page_it.page_ptr_->first, page_it.page_ptr_->second});
            break;
        }
        default:
            break;
    }

}


template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::case_4_1 () {

    if (T1.size() < cache_size) {
        std::pair<KeyT, PageT> lru_page = B1.back();
        page_hashtable.erase(lru_page.first);
        B1.pop_back();
        replace(SourceList::invalid); 
    }
    else {
        std::pair<KeyT, PageT> lru_page = T1.back();
        page_hashtable.erase(lru_page.first);
        T1.pop_back();
    }

}

template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::case_4_2 () {
    int L1_size = T1.size() + B1.size();
    int L2_size = T2.size() + B2.size();

    if (L1_size + L2_size == 2 * cache_size) {
        std::pair<KeyT, PageT> lru_page = B2.back();
        page_hashtable.erase(lru_page.first);
        B2.pop_back();
    }
    
    replace(SourceList::invalid);

}

template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::replace (SourceList page_source) {
    if (T1.size() >= 1 and ((page_source == SourceList::B2 and T1.size() == p_size) or 
                            (T1.size() > p_size))) {
        std::pair<KeyT, PageT> lru_page = T1.back();
        page_hashtable.erase(lru_page.first); 

        T1.pop_back();
        B1.push_front(lru_page);
        page_hashtable.insert({lru_page.first, {B1.cbegin(), SourceList::B1}});

        return;
    }

    std::pair<KeyT, PageT> lru_page = T2.back();
    page_hashtable.erase(lru_page.first);

    T2.pop_back();
    B2.push_front(lru_page);
    page_hashtable.insert({lru_page.first, {B2.cbegin(), SourceList::B2}});
} 

template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::list_printer (const std::list<std::pair<KeyT, PageT>> &list, std::string list_name) const
{
    std::cout << "--------------------" << std::endl << list_name << ": " << std::endl;
                                        
    for (auto list_iter: list)       
        std::cout << "[" << list_iter.first << "]: " << list_iter.second  << std::endl;
                                        
    std::cout << std::endl << "--------------------" << std::endl;    
}


template <typename KeyT, typename PageT>
void ArcCache<KeyT, PageT>::cache_printer () const
{
    std::cout << "cache_size = " << cache_size << std::endl;
    std::cout << "p_size = "     << p_size     << std::endl;


    list_printer (T1, "T1");
    list_printer (T2, "T2");
    list_printer (B1, "B1");
    list_printer (B2, "B2");
}

} // namespace cache    