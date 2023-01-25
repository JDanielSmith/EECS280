// Project UID af1f95f547e44c8ea88730dfb185559d

#include <vector>

#include "Matrix.h"
#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"

using namespace std;


// This is the public Matrix test for which the autograder gives feedback.
// It only tests VERY FEW of the expected behaviors of the Matrix module.
// It will only really tell you if your code compiles and you remembered to
// write the functions. It is not to be trusted. It tells the truth, but not
// the whole truth. It might put you in a blender. You get the point.
// You must write your own comprehensive unit tests in Matrix_tests.cpp!

TEST(test_matrix_basic) {
    std::vector<Matrix> v(1);
    auto mat = &(v.front());
    Matrix_init(mat, 5, 5);

    ASSERT_EQUAL(Matrix_width(mat), 5);
    ASSERT_EQUAL(Matrix_height(mat), 5);

    Matrix_fill(mat, 0);

    int* ptr = Matrix_at(mat, 2, 3);
    ASSERT_EQUAL(Matrix_row(mat, ptr), 2);
    ASSERT_EQUAL(Matrix_column(mat, ptr), 3);
    ASSERT_EQUAL(*ptr, 0);
    *ptr = 42;

    const int* cptr = Matrix_at(mat, 2, 3);
    ASSERT_EQUAL(*cptr, 42);

    Matrix_fill_border(mat, 2);
    ASSERT_EQUAL(*Matrix_at(mat, 0, 0), 2);

    //ASSERT_EQUAL(Matrix_max(mat), 42);
}

TEST(test_matrix_print) {
    std::vector<Matrix> v(1);
    auto mat = &(v.front());
    Matrix_init(mat, 1, 1);

    *Matrix_at(mat, 0, 0) = 42;
    ostringstream expected;
    expected << "1 1\n"
        << "42 \n";
    ostringstream actual;
    Matrix_print(mat, actual);
    ASSERT_EQUAL(expected.str(), actual.str());
}

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
    std::vector<Matrix> v(1);
    auto mat = &(v.front());

    const int width = 3;
    const int height = 5;
    const int value = 42;
    Matrix_init(mat, 3, 5);
    Matrix_fill(mat, value);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
        }
    }
}

// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.h as needed.
TEST(test_Matrix_row_column) {
    std::vector<Matrix> v(1);
    auto mat_ = &(v.front());
    Matrix_init(mat_, 3, 4);
    const auto mat = mat_;

    for (int r = 0; r < Matrix_height(mat); r++)
    {
        for (int c = 0; c < Matrix_width(mat); c++)
        {
            const int* const ptr = Matrix_at(mat, r, c);
            ASSERT_EQUAL(Matrix_row(mat, ptr), r);
            ASSERT_EQUAL(Matrix_column(mat, ptr), c);
        }
    }
}

TEST(test_Matrix_fill_border) {
    std::vector<Matrix> v(1);
    auto mat = &(v.front());
    Matrix_init(mat, 8, 12);

    Matrix_fill(mat, 1);

    Matrix_fill_border(mat, 0);

    constexpr auto first_row = 0;
    constexpr auto first_col = 0;
    const auto last_row = Matrix_height(mat) - 1;
    const auto last_col = Matrix_width(mat) - 1;

    // first and last row
    ASSERT_EQUAL(*Matrix_at(mat, first_row, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, first_row, 1), 0);
    ASSERT_EQUAL(*Matrix_at(mat, first_row, 3), 0);
    ASSERT_EQUAL(*Matrix_at(mat, first_row, last_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, 1), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, 3), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, last_col), 0);

    // first and last column
    ASSERT_EQUAL(*Matrix_at(mat, first_row, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, 1, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, 3, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, first_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, first_row, last_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, 1, last_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, 3, last_col), 0);
    ASSERT_EQUAL(*Matrix_at(mat, last_row, last_col), 0);

    // diagonals
    ASSERT_EQUAL(*Matrix_at(mat, 1, 1), 1);
    ASSERT_EQUAL(*Matrix_at(mat, 2, 2), 1);
    ASSERT_EQUAL(*Matrix_at(mat, 3, 3), 1);
    ASSERT_EQUAL(*Matrix_at(mat, 1, 3), 1);
    ASSERT_EQUAL(*Matrix_at(mat, 2, 2), 1);
    ASSERT_EQUAL(*Matrix_at(mat, 3, 1), 1);
}


// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
