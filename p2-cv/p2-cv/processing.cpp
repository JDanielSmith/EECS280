// Project UID af1f95f547e44c8ea88730dfb185559d

#include <cassert>
#include "processing.h"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!

#include <vector>
#include <stdexcept>

// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (energy == nullptr)
    {
        throw std::invalid_argument("energy");
    }

    // "Initialize the energy Matrix with the same size as the Image and fill it with zeros."
    Matrix_init(energy, Image_width(img), Image_height(img));
    Matrix_fill(energy, 0);

    // "Compute the energy for each non-border pixel ..."
    for (int row = 1; row < Image_height(img) - 1; row++)
    {
        for (int column = 1; column < Image_width(img) - 1; column++)
        {
            const auto N = Image_get_pixel(img, row - 1, column);
            const auto S = Image_get_pixel(img, row + 1, column);
            const auto W = Image_get_pixel(img, row, column - 1);
            const auto E = Image_get_pixel(img, row, column + 1);

            const auto energy_X = squared_difference(N, S) + squared_difference(W, E);
            *Matrix_at(energy, row, column) = energy_X;
        }
    }

    // "Find the maximum energy so far, and use it to fill in the border pixels."
    const auto maximum_energy = Matrix_max(energy);
    Matrix_fill_border(energy, maximum_energy);
}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
inline int min(int i, int j, int k) noexcept
{
    const auto min_ij = std::min(i, j);
    return std::min(min_ij, k);
}
static void compute_cost(const Matrix* energy, Matrix* cost, int r, int c)
{
    const auto energy_rc = *Matrix_at(energy, r, c);
    const auto cost_c_1 = c <= 0 ? INT_MAX : *Matrix_at(cost, r - 1, c - 1); // ignore first column
    const auto cost_c = *Matrix_at(cost, r - 1, c);
    const auto cost_c1 = c >= Matrix_width(cost) - 1 ? INT_MAX : *Matrix_at(cost, r - 1, c + 1); // ignore last column

    const auto cost_rc = energy_rc + min(cost_c_1, cost_c, cost_c1);
    *Matrix_at(cost, r, c) = cost_rc;
}
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
    if (energy == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (cost == nullptr)
    {
        throw std::invalid_argument("energy");
    }
    if (energy == cost)
    {
        throw std::logic_error("energy == cost");
    }

    // "Initialize the cost Matrix with the same size as the energy Matrix"
    Matrix_init(cost, Matrix_width(energy), Matrix_height(energy));

    // "Fill in costs for the first row (index 0). The cost for these pixels is just the energy."
    int row = 0;
    for (int column = 0; column < Matrix_width(energy); column++)
    {
        *Matrix_at(cost, row, column) = *Matrix_at(energy, row, column);
    }

    // "Loop through the rest of the pixels in the Matrix, row by row, starting with the second row ..."
    for (row = 1; row < Matrix_height(energy); row++)
    {
        for (int column = 0; column < Matrix_width(energy); column++)
        {
            compute_cost(energy, cost, row, column);
        }
    }
}


// REQUIRES: cost points to a valid Matrix
//           seam points to an array
//           the size of seam is >= Matrix_height(cost)
// MODIFIES: seam[0]...seam[Matrix_height(cost)-1]
// EFFECTS:  seam serves as an "output parameter".
//           The vertical seam with the minimal cost according to the given
//           cost matrix is found and the seam array is filled with the column
//           numbers for each pixel along the seam, with the pixel for each
//           row r placed at seam[r]. While determining the seam, if any pixels
//           tie for lowest cost, the leftmost one (i.e. with the lowest
//           column number) is used.
//           See the project spec for details on computing the minimal seam.
// NOTE:     You should compute the seam in reverse order, starting
//           with the bottom of the image and proceeding to the top,
//           as described in the project spec.
void find_minimal_vertical_seam(const Matrix* cost, int seam[]) {
    if (cost == nullptr)
    {
        throw std::invalid_argument("cost");
    }

    int column_start = 0;
    int column_end = Matrix_width(cost);

    int row = Matrix_height(cost) - 1;
    // "First, find the minimum cost pixel in the bottom row."
    seam[row] = Matrix_column_of_min_value_in_row(cost, row, column_start, column_end);
    row--;

    // "Now, we work our way up, ..."
    for (; row >= 0; row--)
    {
        const auto previous_column = seam[row + 1];
        // "Don't look outside the bounds!"
        column_start = previous_column - 1;
        column_start = column_start < 0 ? 0 : column_start;
        column_end = column_start + 3; // three pixels above, end is exclusive
        column_end = column_end > Matrix_width(cost) ? Matrix_width(cost) : column_end;

        seam[row] = Matrix_column_of_min_value_in_row(cost, row, column_start, column_end);
    }
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam points to an array
//           the size of seam is == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Use the new operator here to create the smaller Image,
//           and then use delete when you are done with it.
static void remove_column(const Image* img, Image* copy, int row, int column_to_remove)
{
    int column = 0;
    int copy_column = 0;
    while (column < Image_width(img))
    {
        if (column != column_to_remove)
        {
            const auto color = Image_get_pixel(img, row, column);
            Image_set_pixel(copy, row, copy_column, color);
            copy_column++;
        }
        column++;
    }
}

void remove_vertical_seam(Image *img, const int seam[]) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }

    // "You should copy into a smaller auxiliary Image ..."
    std::vector<Image> copy_(1);
    auto copy = &(copy_[0]);
    Image_init(copy, Image_width(img) - 1, Image_height(img));

    for (int row = 0; row < Image_height(img); row++)
    {
        const auto column_to_remove = seam[row];
        remove_column(img, copy, row, column_to_remove);
    }

    // "... and then back into the original"
    *img = *copy;
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use the new operator here to create Matrix objects, and
//           then use delete when you are done with them.
void seam_carve_width(Image *img, int newWidth) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 < newWidth) && (newWidth <= Image_width(img))))
    {
        throw std::invalid_argument("newWidth");
    }

    // "...  remove the minimal cost seam until the image has reached the appropriate width."
    std::vector<Matrix> maxtrix_(2);
    auto energy = &(maxtrix_[0]);
    auto cost = &(maxtrix_[1]);
    int seam[MAX_MATRIX_HEIGHT];
    while (Image_width(img) > newWidth)
    {
        compute_energy_matrix(img, energy);
        compute_vertical_cost_matrix(energy, cost);
        find_minimal_vertical_seam(cost, seam);
        remove_vertical_seam(img, seam);
    }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 < newHeight) && (newHeight <= Image_height(img))))
    {
        throw std::invalid_argument("newWidth");
    }

    rotate_left(img);
    seam_carve_width(img, newHeight);
    rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 < newWidth) && (newWidth <= Image_width(img))))
    {
        throw std::invalid_argument("newWidth");
    }
    if (!((0 < newHeight) && (newHeight <= Image_height(img))))
    {
        throw std::invalid_argument("newWidth");
    }

    seam_carve_width(img, newWidth);
    seam_carve_height(img, newHeight);
}
