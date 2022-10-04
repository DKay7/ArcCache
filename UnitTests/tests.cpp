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

TEST(PerfectCachePushTest, HandleKeyTNotEqPageT)
{       

    SomePage page1{11, "str1"};
    SomePage page2{12, "str2"};
    SomePage page3{13, "str3"};
    SomePage page4{14, "str4"};
    SomePage page5{15, "str5"};

    std::vector<std::string> pages{page1.data, page2.data, page3.data, page4.data, page5.data, page1.data, page2.data, page3.data};
    cache::PerfectCache<int, std::string> perfect_cache{3, pages};
    


    EXPECT_EQ(perfect_cache.push(page1.id, page1.data), 0);
    EXPECT_EQ(perfect_cache.push(page2.id, page2.data), 0);
    EXPECT_EQ(perfect_cache.push(page3.id, page3.data), 0);
    EXPECT_EQ(perfect_cache.push(page4.id, page4.data), 0);
    EXPECT_EQ(perfect_cache.push(page5.id, page5.data), 0);
    EXPECT_EQ(perfect_cache.push(page1.id, page1.data), 1);
    EXPECT_EQ(perfect_cache.push(page2.id, page2.data), 1);
    EXPECT_EQ(perfect_cache.push(page3.id, page3.data), 0);


    EXPECT_EQ(perfect_cache.lookup(page1.id), 1);
    EXPECT_EQ(perfect_cache.lookup(page2.id), 1);
    EXPECT_EQ(perfect_cache.lookup(page3.id), 1);
    EXPECT_EQ(perfect_cache.lookup(page4.id), 0);
    EXPECT_EQ(perfect_cache.lookup(page5.id), 0);


}