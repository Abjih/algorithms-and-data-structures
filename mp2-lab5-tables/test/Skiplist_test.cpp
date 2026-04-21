#include <gtest.h>
#include <string>
#include "Skiplist.h"

// §´§Ö§ã§ä 1: §±§â§à§Ó§Ö§â§Ü§Ñ §á§å§ã§ä§à§Ô§à §ã§á§Ú§ã§Ü§Ñ
TEST(SkipListTest, EmptyList) {
    SkipList<int> list;
    EXPECT_FALSE(list.search(10));
    EXPECT_FALSE(list.search(0));
}

// §´§Ö§ã§ä 2: §¢§Ñ§Ù§à§Ó§Ñ§ñ §Ó§ã§ä§Ñ§Ó§Ü§Ñ §Ú §á§à§Ú§ã§Ü
TEST(SkipListTest, InsertAndSearch) {
    SkipList<int> list;
    list.insert(5);
    list.insert(10);
    list.insert(15);

    EXPECT_TRUE(list.search(5));
    EXPECT_TRUE(list.search(10));
    EXPECT_TRUE(list.search(15));

    // §±§à§Ú§ã§Ü §ï§Ý§Ö§Þ§Ö§ß§ä§à§Ó, §Ü§à§ä§à§â§í§ç §ß§Ö§ä §Ó §ã§á§Ú§ã§Ü§Ö
    EXPECT_FALSE(list.search(20));
    EXPECT_FALSE(list.search(0));
}

// §´§Ö§ã§ä 3: §µ§Õ§Ñ§Ý§Ö§ß§Ú§Ö §ï§Ý§Ö§Þ§Ö§ß§ä§à§Ó
TEST(SkipListTest, EraseElement) {
    SkipList<int> list;
    list.insert(10);
    list.insert(20);
    list.insert(30);

    // §µ§Õ§Ñ§Ý§ñ§Ö§Þ §ï§Ý§Ö§Þ§Ö§ß§ä §Ú§Ù §ã§Ö§â§Ö§Õ§Ú§ß§í
    list.erase(20);
    EXPECT_FALSE(list.search(20)); // §´§Ö§á§Ö§â§î §Ö§Ô§à §Ò§í§ä§î §ß§Ö §Õ§à§Ý§Ø§ß§à

    // §°§ã§ä§Ñ§Ý§î§ß§í§Ö §ï§Ý§Ö§Þ§Ö§ß§ä§í §Õ§à§Ý§Ø§ß§í §à§ã§ä§Ñ§ä§î§ã§ñ §ß§Ñ §Þ§Ö§ã§ä§Ö
    EXPECT_TRUE(list.search(10));
    EXPECT_TRUE(list.search(30));

    // §µ§Õ§Ñ§Ý§ñ§Ö§Þ §á§Ö§â§Ó§í§Û §ï§Ý§Ö§Þ§Ö§ß§ä
    list.erase(10);
    EXPECT_FALSE(list.search(10));
    EXPECT_TRUE(list.search(30));
}

// §´§Ö§ã§ä 4: §±§à§á§í§ä§Ü§Ñ §å§Õ§Ñ§Ý§Ú§ä§î §ß§Ö§ã§å§ë§Ö§ã§ä§Ó§å§ð§ë§Ú§Û §ï§Ý§Ö§Þ§Ö§ß§ä (§ß§Ö §Õ§à§Ý§Ø§ß§à §Ó§í§Ù§í§Ó§Ñ§ä§î §Ü§â§Ñ§ê)
TEST(SkipListTest, EraseNonExistent) {
    SkipList<int> list;
    list.insert(5);
    list.insert(15);

    list.erase(10); // §¿§Ý§Ö§Þ§Ö§ß§ä§Ñ 10 §ß§Ö§ä §Ó §ã§á§Ú§ã§Ü§Ö

    EXPECT_TRUE(list.search(5));
    EXPECT_TRUE(list.search(15));
}

// §´§Ö§ã§ä 5: §²§Ñ§Ò§à§ä§Ñ §ã §Õ§â§å§Ô§Ú§Þ §ä§Ú§á§à§Þ §Õ§Ñ§ß§ß§í§ç (std::string)
TEST(SkipListTest, StringType) {
    SkipList<std::string> list;
    list.insert("apple");
    list.insert("banana");
    list.insert("cherry");

    EXPECT_TRUE(list.search("apple"));
    EXPECT_TRUE(list.search("cherry"));
    EXPECT_FALSE(list.search("grape"));

    list.erase("banana");
    EXPECT_FALSE(list.search("banana"));
    EXPECT_TRUE(list.search("apple"));
}

// §´§Ö§ã§ä 6: §¯§Ñ§Ô§â§å§Ù§à§é§ß§à§Ö §ä§Ö§ã§ä§Ú§â§à§Ó§Ñ§ß§Ú§Ö (§ã§ä§â§Ö§ã§ã-§ä§Ö§ã§ä)
// §±§â§à§Ó§Ö§â§ñ§Ö§ä, §é§ä§à §ã§ä§â§å§Ü§ä§å§â§Ñ §ß§Ö §Ý§à§Þ§Ñ§Ö§ä§ã§ñ §á§â§Ú §Ò§à§Ý§î§ê§à§Þ §Ü§à§Ý§Ú§é§Ö§ã§ä§Ó§Ö §Õ§Ñ§ß§ß§í§ç §Ú §â§Ñ§ß§Õ§à§Þ§Ú§Ù§Ñ§è§Ú§ñ §â§Ñ§Ò§à§ä§Ñ§Ö§ä §Ü§à§â§â§Ö§Ü§ä§ß§à
TEST(SkipListTest, LargeDataset) {
    SkipList<int> list;
    const int num_elements = 10000;

    // §£§ã§ä§Ñ§Ó§Ý§ñ§Ö§Þ 10 000 §ï§Ý§Ö§Þ§Ö§ß§ä§à§Ó
    for (int i = 0; i < num_elements; ++i) {
        list.insert(i);
    }

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ, §é§ä§à §Ó§ã§Ö §à§ß§Ú §ß§Ñ §Þ§Ö§ã§ä§Ö
    for (int i = 0; i < num_elements; ++i) {
        EXPECT_TRUE(list.search(i));
    }

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §Ô§â§Ñ§ß§Ú§è§í
    EXPECT_FALSE(list.search(-1));
    EXPECT_FALSE(list.search(num_elements));

    // §µ§Õ§Ñ§Ý§ñ§Ö§Þ §é§Ö§ä§ß§í§Ö §é§Ú§ã§Ý§Ñ
    for (int i = 0; i < num_elements; i += 2) {
        list.erase(i);
    }

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ, §é§ä§à §é§Ö§ä§ß§í§Ö §å§Õ§Ñ§Ý§Ú§Ý§Ú§ã§î, §Ñ §ß§Ö§é§Ö§ä§ß§í§Ö §à§ã§ä§Ñ§Ý§Ú§ã§î
    for (int i = 0; i < num_elements; ++i) {
        if (i % 2 == 0) {
            EXPECT_FALSE(list.search(i));
        }
        else {
            EXPECT_TRUE(list.search(i));
        }
    }
}