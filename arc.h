#include <unordered_map>
#include <iostream>
#include <list>
#include <map>

namespace cache {

enum SourceList {
    invalid = -1,
    T1 = 1,
    T2 = 2,
    B1 = 3,
    B2 = 4,
};

template <typename PageT>
class PageIter  {
    private:
        using list_iter = std::list<PageT>::iterator;     
    public:
        std::list<PageT>::iterator page_ptr;
        SourceList source_list = SourceList::invalid;
        
        // PageIter(list_iter iter, SourceList source):
        //     page_ptr(iter) source_list(source) {}
        
};

template <typename PageT>
class ArcCache {   
    private:
        std::list<PageT> T1 = {}; // top of L1
        std::list<PageT> T2 = {}; // top of L2
        std::list<PageT> B1 = {}; // bottom of L1
        std::list<PageT> B2 = {}; // bottom of L2
        std::map<PageT, PageIter<PageT>> page_map;

        int cache_size = 1; // named "c" in paper
        int p_size  = 0; // named "p" in paper

        void case_1  (PageIter<PageT> page_it, SourceList page_source);
        void case_2  (PageIter<PageT> page_it, SourceList page_source);
        void case_3  (PageIter<PageT> page_it, SourceList page_source);
        void case_4_1(SourceList page_source);
        void case_4_2(SourceList page_source);
        void replace (SourceList page_source);
        void list_printer (const std::list <PageT> &list, std::string list_name) const;

    public:
        void cache_printer () const;
        bool push(PageT page);        // returns true if hit, false if miss
        ArcCache(int size) {
            cache_size = (size / 2) >= 1 ? size / 2 : 1;
        }

        ArcCache () {}
};

template <typename PageT>
bool ArcCache<PageT>::push(PageT page) {

    auto cached_page = page_map.find(page);
    SourceList page_source = invalid;
    bool is_hit = false;

    if (cached_page != page_map.end()) {
        page_source = cached_page->second.source_list;
        is_hit = true;
    }

    if (page_source == SourceList::T1 or page_source == SourceList::T2)
        case_1(cached_page->second, page_source);

    else if (page_source == SourceList::B1)
        case_2(cached_page->second, page_source);

    else if (page_source == SourceList::B2)
        case_3(cached_page->second, page_source);
    
    else {
        int L1_size = T1.size() + B1.size();
        int L2_size = T2.size() + B2.size();

        if (L1_size == cache_size)
            case_4_1 (page_source);
        
        else if (L1_size < cache_size and L1_size + L2_size >= cache_size)
            case_4_2 (page_source);
        
        T1.push_front(page);
        page_map[page] = {T1.begin(), SourceList::T1};
    }

    return is_hit;
}

template <typename PageT>
void ArcCache<PageT>::case_1 (PageIter<PageT> page_it, SourceList page_source) {
    PageT page = *(page_it.page_ptr);

    if (page_source == SourceList::T1)
        T1.erase(page_it.page_ptr);
    else
        T2.erase(page_it.page_ptr);
    
    T2.push_front(page);
    page_map[page] = {T2.begin(), SourceList::T2};
}

template <typename PageT>
void ArcCache<PageT>::case_2 (PageIter<PageT> page_it, SourceList page_source) {
    p_size = std::min<int> (cache_size, p_size + std::max<int> (B2.size() / B1.size(), 1));
    PageT page = *(page_it.page_ptr);
  
    replace (page_source);

    B1.erase(page_it.page_ptr);
    
    T2.push_front(page);
    page_map[page] = {T2.begin(), SourceList::T2};
}

template <typename PageT>
void ArcCache<PageT>::case_3 (PageIter<PageT> page_it, SourceList page_source) {
    p_size = std::max<int> (0, p_size - std::max<int> (B1.size() / B2.size(), 1));
    PageT page = *(page_it.page_ptr);

    replace (page_source);

    B2.erase(page_it.page_ptr);
    
    T2.push_front(page);
    page_map[page] = {T2.begin(), SourceList::T2};
}

template <typename PageT>
void ArcCache<PageT>::case_4_1 (SourceList page_source) {
    PageT page;

    if (T1.size() < cache_size) {
        page = B1.back();
        B1.pop_back();

        replace(page_source);
    }
    else {
        page = T1.back();
        T1.pop_back();
    }

    page_map.erase(page);
}

template <typename PageT>
void ArcCache<PageT>::case_4_2 (SourceList page_source) {
    int L1_size = T1.size() + B1.size();
    int L2_size = T2.size() + B2.size();

    if (L1_size + L2_size == 2 * cache_size) {
        PageT page = B2.back();
        B2.pop_back();
        page_map.erase(page);
    }

    replace(page_source);
}

template <typename PageT>
void ArcCache<PageT>::replace (SourceList page_source) {
    if (T1.size() >= 1 and ((page_source == SourceList::B2 and T1.size() == p_size) or 
                            (T1.size() > p_size))) {
        PageT lru_page = T1.back();
        T1.pop_back();
        B1.push_front(lru_page);
        page_map[lru_page] = {B1.begin(), SourceList::B1};

        return;
    }

    PageT lru_page = T2.back();
    T2.pop_back();
    B2.push_front(lru_page);
    page_map[lru_page] = {B2.begin(), SourceList::B2};
} 

template <typename PageT>
void ArcCache <PageT>::list_printer (const std::list <PageT> &list, std::string list_name) const
{
    std::cout << "--------------------" << std::endl << list_name << ": ";
                                        
    for (auto list_iter: list)       
        std::cout << list_iter << " ";
                                        
    std::cout << std::endl << "--------------------" << std::endl;    
}

template <typename PageT>
void ArcCache <PageT>::cache_printer () const
{
    std::cout << "cache_size = " << cache_size << std::endl;
    std::cout << "p_size = "     << p_size     << std::endl;

    list_printer (T1, "T1");
    list_printer (T2, "T2");
    list_printer (B1, "B1");
    list_printer (B2, "B2");
}

}