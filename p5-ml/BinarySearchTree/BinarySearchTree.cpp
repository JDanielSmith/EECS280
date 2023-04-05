// BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <assert.h>
#include <iostream>
#include <memory>
#include <stdexcept>

#define ASSERT_TRUE(f) assert(f)
#define ASSERT_FALSE(f) ASSERT_TRUE(!f)

template<typename T>
struct Node final
{
    T value;
    std::unique_ptr<Node> pLeft;
    std::unique_ptr<Node> pRight;
};
template<typename T>
inline Node<T>* left(Node<T>& node)
{
    return node.pLeft.get();
}
template<typename T>
inline const Node<T>* left(const Node<T>& node)
{
    return node.pLeft.get();
}
template<typename T>
inline Node<T>* right(Node<T>& node)
{
    return node.pRight.get();
}
template<typename T>
inline const Node<T>* right(const Node<T>& node)
{
    return node.pRight.get();
}


template<typename T>
static bool check_sorting_invariant(const Node<T>& node)
{
    auto&& value = node.value;
    
    if (auto pLeft = left(node))
    {
        if (!(pLeft->value < value))
        {
            return false;
        }

        if (!check_sorting_invariant(*pLeft))
        {
            return false;
        }
    }

    if (auto pRight = right(node))
    {
        if (!(value < pRight->value))
        {
            return false;
        }

        if (!check_sorting_invariant(*pRight))
        {
            return false;
        }
    }

    return true;
}
template<typename T>
static bool check_sorting_invariant(const Node<T>* pNode)
{
    if (pNode == nullptr)
    {
        return true;
    }
    return check_sorting_invariant(*pNode);
}

template<typename T>
auto make_Node(const T& v)
{
    return std::make_unique<Node<T>>(Node<T>{ v });
}

template<typename T>
auto make_Tree_1_23(const T& v1, const T& v2, const T& v3)
{
    auto root = make_Node(v1);
    root->pLeft = make_Node(v2);
    root->pRight = make_Node(v3);
    return root;
}

template<typename T>
auto make_Tree_1_2x_3x(const T& v1, const T& v2, const T& v3)
{
    auto root = make_Node(v1);
    root->pLeft = make_Node(v2);
    root->pLeft->pLeft = make_Node(v3);
    return root;
}

template<typename T>
auto make_Tree_1_2x_x3(const T& v1, const T& v2, const T& v3)
{
    auto root = make_Node(v1);
    root->pLeft = make_Node(v2);
    root->pLeft->pRight = make_Node(v3);
    return root;
}

template<typename T>
auto make_Tree_1_x2_3x(const T& v1, const T& v2, const T& v3)
{
    auto root = make_Node(v1);
    root->pRight = make_Node(v2);
    root->pRight->pLeft = make_Node(v3);
    return root;
}

template<typename T>
auto make_Tree_1_x2_x3(const T& v1, const T& v2, const T& v3)
{
    auto root = make_Node(v1);
    root->pRight = make_Node(v2);
    root->pRight->pRight = make_Node(v3);
    return root;
}

int main()
{
    std::unique_ptr<Node<int>> empty;
    ASSERT_TRUE(check_sorting_invariant(empty.get()));

    /*************************************************************************/
    
    auto node = make_Node(314);
    ASSERT_TRUE(check_sorting_invariant(*node));

    /*************************************************************************/
    
    auto n_1_2x = make_Node(1);
    n_1_2x->pLeft = make_Node(2);
    ASSERT_FALSE(check_sorting_invariant(*n_1_2x));

    auto n_1_x2 = make_Node(1);
    n_1_x2->pRight = make_Node(2);
    ASSERT_TRUE(check_sorting_invariant(*n_1_x2));

    auto n_2_1x = make_Node(2);
    n_2_1x->pLeft = make_Node(1);
    ASSERT_TRUE(check_sorting_invariant(*n_2_1x));

    auto n_2_x1 = make_Node(2);
    n_2_x1->pRight = make_Node(1);
    ASSERT_FALSE(check_sorting_invariant(*n_2_x1));


    /*************************************************************************/

    auto n_1_2x_3x = make_Tree_1_2x_3x(1, 2, 3); // 1<2>3
    ASSERT_FALSE(check_sorting_invariant(*n_1_2x_3x));
    auto n_1_3x_2x = make_Tree_1_2x_3x(1, 3, 2); // 1->3->2
    ASSERT_FALSE(check_sorting_invariant(*n_1_3x_2x));

    auto n_1_2x_x3 = make_Tree_1_2x_x3(1, 2, 3); // 1->2->[]
    ASSERT_FALSE(check_sorting_invariant(*n_1_2x_x3));
    auto n_1_3x_x2 = make_Tree_1_2x_x3(1, 3, 2);
    ASSERT_FALSE(check_sorting_invariant(*n_1_3x_x2));

    auto n_1_x2_3x = make_Tree_1_x2_3x(1, 2, 3);
    ASSERT_FALSE(check_sorting_invariant(*n_1_x2_3x));
    auto n_1_x3_2x = make_Tree_1_x2_3x(1, 3, 2);
    ASSERT_TRUE(check_sorting_invariant(*n_1_x3_2x));

    auto n_1_x2_x3 = make_Tree_1_x2_x3(1, 2, 3);
    ASSERT_TRUE(check_sorting_invariant(*n_1_x2_x3));
    auto n_1_x3_x2 = make_Tree_1_x2_x3(1, 3, 2);
    ASSERT_TRUE(check_sorting_invariant(*n_1_x2_x3));

    auto n_1_23 = make_Tree_1_23(1, 2, 3); // 1((2)(3)) 1<2>3
    ASSERT_FALSE(check_sorting_invariant(*n_1_23));
    auto n_1_32 = make_Tree_1_23(1, 3, 2); // 1((3)(2)) 1<3>2
    ASSERT_FALSE(check_sorting_invariant(*n_1_32));


    auto n_2_1x_3x = make_Tree_1_2x_3x(2, 1, 3);
    ASSERT_FALSE(check_sorting_invariant(*n_2_1x_3x));
    auto n_2_3x_1x = make_Tree_1_2x_3x(2, 3, 1);
    ASSERT_FALSE(check_sorting_invariant(*n_2_3x_1x));

    auto n_2_1x_x3 = make_Tree_1_2x_x3(2, 1, 3);
    ASSERT_TRUE(check_sorting_invariant(*n_2_1x_x3));
    auto n_2_3x_x1 = make_Tree_1_2x_x3(2, 3, 1);
    ASSERT_FALSE(check_sorting_invariant(*n_2_3x_x1));

    auto n_2_x3_1x = make_Tree_1_x2_3x(2, 3, 1);
    ASSERT_TRUE(check_sorting_invariant(*n_2_x3_1x));
    auto n_2_x1_3x = make_Tree_1_x2_3x(2, 1, 3);
    ASSERT_FALSE(check_sorting_invariant(*n_2_x1_3x));

    auto n_2_x1_x3 = make_Tree_1_x2_x3(2, 1, 3);
    ASSERT_FALSE(check_sorting_invariant(*n_2_x1_x3));
    auto n_2_x3_x1 = make_Tree_1_x2_x3(2, 3, 1);
    ASSERT_FALSE(check_sorting_invariant(*n_2_x3_x1));

    auto n_2_13 = make_Tree_1_23(2, 1, 3);
    ASSERT_TRUE(check_sorting_invariant(*n_2_13));
    auto n_2_31 = make_Tree_1_23(2, 3, 1);
    ASSERT_FALSE(check_sorting_invariant(*n_2_31));





    auto n_3_2x_1x = make_Tree_1_2x_3x(3, 2, 1);
    ASSERT_TRUE(check_sorting_invariant(*n_3_2x_1x));

    auto n_3_1x_x2 = make_Tree_1_2x_x3(3, 1, 2);
    ASSERT_TRUE(check_sorting_invariant(*n_3_1x_x2));
}
