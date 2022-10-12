#include <gtest/gtest.h>
#include <string>
#include "arc.h"
#include "perfect_cache.h"

struct SomePage {
    int id;
    std::string data;
};

TEST(ArcPushTest, HandleKeyTNotEqPageT)
{   
    cache::ArcCache<int, std::string> arc{3};
    
    SomePage page1{4242, "str1"};
    SomePage page2{4243, "str2"};
    SomePage page3{4244, "arc_cache"};
    SomePage page4{4245, "str2"};
    SomePage page5{4246, "str3"};

    EXPECT_EQ(arc.push(page1.id, page1.data), 0);
    EXPECT_EQ(arc.push(page2.id, page2.data), 0);
    EXPECT_EQ(arc.push(page3.id, page3.data), 0);
    EXPECT_EQ(arc.push(page4.id, page4.data), 0);
    EXPECT_EQ(arc.push(page5.id, page5.data), 0);
    


    EXPECT_EQ(arc.lookup(page1.id), 0);
    EXPECT_EQ(arc.lookup(page2.id), 0);
    EXPECT_EQ(arc.lookup(page3.id), 1);
    EXPECT_EQ(arc.lookup(page4.id), 1);
    EXPECT_EQ(arc.lookup(page5.id), 1);


}

TEST(PerfectCachePushTest, test_1) {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};

    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(7, perfect_cache::count_hits(4, data));
}

TEST(PerfectCachePushTest, test_2) {
    std::vector<int> pages = {1, 2, 99, 3, 4, 5, 112, 6, 1, 0, 2, 7, 8};

    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(2, perfect_cache::count_hits(5, data));
}

TEST(PerfectCachePushTest, test_3) {
    std::vector<int> pages = {2, 4, 2, 4, 6, 4, 6, 7, 6, 6, 7, 9, 6, 4, 3, 5, 7, 8, 6, 5, 4, 3, 4, 5, 7, 8, 8, 7, 6};

    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(20, perfect_cache::count_hits(5, data));
}
 
TEST(PerfectCachePushTest, test_4) {
    std::vector<int> pages = {91, 123, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};


    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(5, perfect_cache::count_hits(5, data));
}

TEST(PerfectCachePushTest, test_5) {
    std::vector<int> pages = {7, 2, 7, 4, 7, 8, 7, 16, 7, 32, 7, 64, 7, 128, 7, 256, 7, 1024, 7};

    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(9, perfect_cache::count_hits(2, data));
}

TEST(PerfectCachePushTest, test_6) {
    std::vector<int> pages = {4, 7, 3, 6, 85, 34, 3276, 6, 8, 5, 3, 4242, 4, 7, 8, 6, 4, 4, 5, 6, 7, 8};

    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);
    EXPECT_EQ(11, perfect_cache::count_hits(5, data));
}

TEST(PerfectCachePushTest, test_7) {
    std::vector<int> pages = {2, 2, 1, 3, 3, 73, 16, 4, 1, 5, 6, 2};
    perfect_cache::data_t<int, int> data = perfect_cache::get_vec_of_pairs(pages, pages);

    EXPECT_EQ(4, perfect_cache::count_hits(3, data));
}