#include <gtest.h>
#include "tables.h"

class TableTest : public ::testing::Test {
protected:

    UnorderedTable<int> unorderedTable;
    OrderedTable<int> orderedTable;

    void SetUp() override {
        unorderedTable.insert(1, 10);
        unorderedTable.insert(2, 20);

        orderedTable.insert(2, 20);
        orderedTable.insert(1, 10);
        orderedTable.insert(3, 30);
    }

};

TEST_F(TableTest, UnorderedTableBasic) {
    EXPECT_EQ(unorderedTable.getSize(), 2);
    EXPECT_FALSE(unorderedTable.isEmpty());

    auto it = unorderedTable.find(1);
    EXPECT_EQ(it->second, 10);
}

TEST_F(TableTest, UnorderedTableInsertErase) {
    EXPECT_FALSE(unorderedTable.insert(1, 100));
    EXPECT_TRUE(unorderedTable.insert(3, 30));

    EXPECT_TRUE(unorderedTable.erase(1));
    EXPECT_EQ(unorderedTable.getSize(), 2);
    EXPECT_FALSE(unorderedTable.erase(1));
}
TEST_F(TableTest, OrderedTableBoundaries) {
    OrderedTable<int> table;
    EXPECT_EQ(table.find(0), nullptr);
    table.insert(10, 100);
    EXPECT_EQ(table.find(5), nullptr);
    EXPECT_EQ(table.find(15), nullptr);
}
TEST_F(TableTest, OrderedTableOrdering) {
    auto it = orderedTable.begin();
    EXPECT_EQ(it->first, 1); it++;
    EXPECT_EQ(it->first, 2); it++;
    EXPECT_EQ(it->first, 3);

    EXPECT_EQ(orderedTable.find(2)->second, 20);
    EXPECT_EQ(orderedTable.find(4), nullptr);
}

TEST_F(TableTest, ClearTest) {
    unorderedTable.clear();
    orderedTable.clear();

    EXPECT_TRUE(unorderedTable.isEmpty());
    EXPECT_TRUE(orderedTable.isEmpty());
}


class HashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table = new HashTable<int>();
    }
    void TearDown() override {
        delete table;
    }
    HashTable<int>* table;
};

TEST_F(HashTableTest, ConstructorAndBasicProperties) {
    EXPECT_TRUE(table->isEmpty());
    EXPECT_EQ(table->getSize(), 0);

    HashTable<int> customSizeTable(256);
    EXPECT_TRUE(customSizeTable.isEmpty());
}

TEST_F(HashTableTest, InsertOperations) {
    EXPECT_TRUE(table->insert(1, 10));
    EXPECT_EQ(table->getSize(), 1);
    EXPECT_FALSE(table->isEmpty());

    EXPECT_TRUE(table->insert(2, 20));
    EXPECT_EQ(table->getSize(), 2);

    EXPECT_FALSE(table->insert(1, 100));
    EXPECT_EQ(table->getSize(), 2);
}

TEST_F(HashTableTest, FindOperations) {
    table->insert(1, 10);
    table->insert(2, 20);
    table->insert(3, 30);

    auto found = table->find(1);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->first, 1);
    EXPECT_EQ(found->second, 10);

    found = table->find(2);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->first, 2);
    EXPECT_EQ(found->second, 20);

    EXPECT_EQ(table->find(100), nullptr);
}

TEST_F(HashTableTest, EraseOperations) {
    table->insert(1, 10);
    table->insert(2, 20);
    table->insert(3, 30);
    EXPECT_TRUE(table->erase(2));
    EXPECT_EQ(table->getSize(), 2);
    EXPECT_EQ(table->find(2), nullptr);

    EXPECT_FALSE(table->erase(100));
    EXPECT_EQ(table->getSize(), 2);
}

TEST_F(HashTableTest, ClearOperation) {
    table->insert(1, 10);
    table->insert(2, 20);
    table->insert(3, 30);

    EXPECT_EQ(table->getSize(), 3);
    table->clear();
    EXPECT_TRUE(table->isEmpty());
    EXPECT_EQ(table->getSize(), 0);
}

TEST_F(HashTableTest, CollisionHandling) {
    HashTable<int> smallTable(4);

    EXPECT_TRUE(smallTable.insert(1, 10));
    EXPECT_TRUE(smallTable.insert(5, 50));
    EXPECT_TRUE(smallTable.insert(9, 90));

    EXPECT_EQ(smallTable.getSize(), 3);

    ASSERT_NE(smallTable.find(1), nullptr);
    EXPECT_EQ(smallTable.find(1)->second, 10);

    ASSERT_NE(smallTable.find(5), nullptr);
    EXPECT_EQ(smallTable.find(5)->second, 50);

    ASSERT_NE(smallTable.find(9), nullptr);
    EXPECT_EQ(smallTable.find(9)->second, 90);
}