// BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <assert.h>
#include <iostream>
#include <memory>
#include <stdexcept>

#define ASSERT_TRUE(f) assert(f)
#define ASSERT_FALSE(f) ASSERT_TRUE(!f)

template<typename T>
class BinarySearchTree final
{
    struct Node final
    {
        T value;
        std::unique_ptr<Node> pLeft = nullptr;
        std::unique_ptr<Node> pRight = nullptr;
    };
    std::unique_ptr<Node> pRoot;

    static bool check_sorting_invariant(const Node* pNode)
    {
        if (pNode == nullptr)
        {
            return true;
        }
        const auto& node = *pNode;

        // leaf node is (trivially) OK.
        if ((!node.pLeft) && (!node.pRight))
        {
            return true;
        }

        auto&& value = node.value;
        if (node.pLeft)
        {
            if (!(node.pLeft->value < value))
            {
                return false;
            }

            return check_sorting_invariant(node.pLeft.get());
        }

        if (node.pRight)
        {
            if (!(value < node.pRight->value))
            {
                return false;
            }

            return check_sorting_invariant(node.pLeft.get());
        }

        throw std::logic_error("check_sorting_invariant()");
    }

public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    BinarySearchTree(BinarySearchTree&&) = default;
    BinarySearchTree& operator=(BinarySearchTree&&) = default;

    bool empty() const
    {
        return pRoot.get() == nullptr;
    }

    void insert(const T& v)
    {
        auto pNode = std::make_unique<Node>(Node{ v });
        if (empty())
        {
            pRoot = std::move(pNode);
            return;
        }
    }
    
    bool check_sorting_invariant() const
    {
        return check_sorting_invariant(pRoot.get());
    }

};


int main()
{
    BinarySearchTree<int> b;
    ASSERT_TRUE(b.check_sorting_invariant());

    b.insert(1);
    ASSERT_TRUE(b.check_sorting_invariant());

    b.insert(0);
    ASSERT_TRUE(b.check_sorting_invariant());

    // change first datum to 2, resulting in the first broken tree above
    //*b.begin() = 2;
    ASSERT_FALSE(b.check_sorting_invariant());
}
