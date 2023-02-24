// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"

#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <algorithm>

// REQUIRES: mat points to a Matrix
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height.
// NOTE:     Do NOT use new or delete here.

int* Matrix_data(Matrix* mat)
{
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    return mat->data;
}
const int* Matrix_data(const Matrix* mat)
{
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    return mat->data;
}

void Matrix_init(Matrix* mat, int width, int height) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    if (!((0 < width) && (width <= MAX_MATRIX_WIDTH)))
    {
        throw std::invalid_argument("width");
    }
    if (!((0 < height) && (height <= MAX_MATRIX_HEIGHT)))
    {
        throw std::invalid_argument("height");
    }

    mat->width = width;
    mat->height = height;

    constexpr auto size = std::extent<decltype(mat->data)>::value;
    assert(size == (MAX_MATRIX_WIDTH * MAX_MATRIX_HEIGHT));
    //int* const data = Matrix_data(mat);
    //for (size_t i = 0; i < size; i++)
    //{
    //   data[i] = -99999;
    //}
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }

    os << Matrix_width(mat) << " " << Matrix_height(mat) << "\n";
    for (int r = 0; r < Matrix_height(mat); r++)
    {
        for (int c = 0; c < Matrix_width(mat); c++)
        {
            os << *(Matrix_at(mat, r, c)) << " ";
        }
        os << "\n";
    }
}

// REQUIRES: mat points to an valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           ptr points to an element in the Matrix
// EFFECTS:  Returns the row of the element pointed to by ptr.
static int size(const Matrix* mat)
{
    return Matrix_width(mat) * Matrix_height(mat);
}

static void validate_ptr(const Matrix* mat, const int* ptr)
{
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    if (ptr == nullptr)
    {
        throw std::invalid_argument("ptr");
    }

    const int* const p = Matrix_data(mat);
    if (ptr < p)
    {
        throw std::invalid_argument("ptr");
    }
    const auto sz = size(mat);
    if (ptr >= p + sz)
    {
        throw std::invalid_argument("ptr");
    }
}

int Matrix_row(const Matrix* mat, const int* ptr) {
    validate_ptr(mat, ptr);

    const auto diff = ptr - Matrix_data(mat);
    const auto retval = diff / Matrix_width(mat);
    return static_cast<int>(retval);
}

// REQUIRES: mat points to a valid Matrix
//           ptr point to an element in the Matrix
// EFFECTS:  Returns the column of the element pointed to by ptr.
int Matrix_column(const Matrix* mat, const int* ptr) {
    validate_ptr(mat, ptr);

    const auto diff = ptr - Matrix_data(mat);
    const auto retval = diff % Matrix_width(mat);
    return static_cast<int>(retval);
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
static void validate_row(const Matrix* mat, int row)
{
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    if (!((0 <= row) && (row < Matrix_height(mat))))
    {
        throw std::invalid_argument("row");
    }
}
static void validate_column(const Matrix* mat, int column)
{
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }
    if (!((0 <= column) && (column < Matrix_width(mat))))
    {
        throw std::invalid_argument("column");
    }
}

static void validate_row_column(const Matrix* mat, int row, int column)
{
    validate_row(mat, row);
    validate_column(mat, column);
}
int* Matrix_at(Matrix* mat, int row, int column) {
    validate_row_column(mat, row, column);
    return Matrix_data(mat) + (row * Matrix_width(mat) + column);
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
    validate_row_column(mat, row, column);
    return Matrix_data(mat) + (row * Matrix_width(mat) + column);
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
static void fill_row(Matrix* mat, int row, int value)
{
    validate_row(mat, row);
    for (int c = 0; c < Matrix_width(mat); c++)
    {
        *(Matrix_at(mat, row, c)) = value;
    }
}

void Matrix_fill(Matrix* mat, int value) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }

    for (int r = 0; r < Matrix_height(mat); r++)
    {
        fill_row(mat, r, value);
    }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
static void fill_column(Matrix* mat, int column, int value)
{
    validate_column(mat, column);
    for (int r = 0; r < Matrix_height(mat); r++)
    {
        *(Matrix_at(mat, r, column)) = value;
    }
}

void Matrix_fill_border(Matrix* mat, int value) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }

    fill_row(mat, 0, value);
    fill_row(mat, Matrix_height(mat)-1, value);

    fill_column(mat, 0, value);
    fill_column(mat, Matrix_width(mat)-1, value);
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
    if (mat == nullptr)
    {
        throw std::invalid_argument("mat");
    }

    int retval = INT_MIN;
    for (int r = 0; r < Matrix_height(mat); r++)
    {
        for (int c = 0; c < Matrix_width(mat); c++)
        {
            retval = std::max(retval, *Matrix_at(mat, r, c));
        }
    }
    return retval;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
static void validate_row_column_startend(const Matrix* mat, int row,
    int column_start, int column_end)
{
    validate_row(mat, row);
    if (!(0 <= column_start))
    {
        throw std::invalid_argument("column_start");
    }
    if (!(column_end <= Matrix_width(mat)))
    {
        throw std::invalid_argument("column_end");
    }
    if (!(column_start < column_end))
    {
        throw std::invalid_argument("column_start");
    }
}

int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
    validate_row_column_startend(mat, row, column_start, column_end);

    int retval = -1;
    int min = INT_MAX;
    for (int c = column_start; c < column_end; c++)
    {
        const auto value = *Matrix_at(mat, row, c);
        if (value < min)
        {
            min = value;
            retval = c;
        }
    }
    return retval;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
    validate_row_column_startend(mat, row, column_start, column_end);

    int retval = INT_MAX;
    for (int c = column_start; c < column_end; c++)
    {
        retval = std::min(retval, *Matrix_at(mat, row, c));
    }
    return retval;
}
