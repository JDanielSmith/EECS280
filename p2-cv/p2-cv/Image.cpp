// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Image.h"

#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <algorithm>

// REQUIRES: img points to an Image
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, int width, int height) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 < width) && (width <= MAX_MATRIX_WIDTH)))
    {
        throw std::invalid_argument("width");
    }
    if (!((0 < height) && (height <= MAX_MATRIX_HEIGHT)))
    {
        throw std::invalid_argument("height");
    }

    img->width = width;
    img->height = height;
    Matrix_init(&(img->red_channel), Image_width(img), Image_height(img));
    Matrix_init(&(img->green_channel), Image_width(img), Image_height(img));
    Matrix_init(&(img->blue_channel), Image_width(img), Image_height(img));
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, std::istream& is) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }

    std::string p3;
    is >> p3;
    if (p3 != "P3")
    {
        throw std::invalid_argument("is is not a PPM file!");
    }

    int width, height;
    is >> width >> height;
    Image_init(img, width, height);

    int RGB_intensities;
    is >> RGB_intensities;
    if (RGB_intensities != 255)
    {
        throw std::invalid_argument("is is not a PPM file!");
    }

    for (int row = 0; row < Image_height(img); row++)
    {
        for (int column = 0; column < Image_width(img); column++)
        {
            Pixel color;
            is >> color.r >> color.g >> color.b;
            Image_set_pixel(img, row, column, color);
        }
    }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }

    os << "P3\n";
    os << Image_width(img) << " " << Image_height(img) << "\n";
    os << "255\n";

    for (int row = 0; row < Image_height(img); row++)
    {
        for (int column = 0; column < Image_width(img); column++)
        {
            const auto color = Image_get_pixel(img, row, column);
            os << color.r << " " << color.g << " " << color.b << " ";
        }
        os << "\n";
    }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    return img->width;
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    return img->height;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
static void validate_row(const Image* img, int row)
{
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 <= row) && (row < Image_height(img))))
    {
        throw std::invalid_argument("row");
    }
}
static void validate_column(const Image* img, int column)
{
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }
    if (!((0 <= column) && (column <= Image_width(img))))
    {
        throw std::invalid_argument("column");
    }
}

static void validate_row_column(const Image* img, int row, int column)
{
    validate_row(img, row);
    validate_column(img, column);
}

Pixel Image_get_pixel(const Image* img, int row, int column) {
    validate_row_column(img, row, column);

    Pixel retval{ -1, -1, -1 };
    retval.r = *Matrix_at(&(img->red_channel), row, column);
    retval.g = *Matrix_at(&(img->green_channel), row, column);
    retval.b = *Matrix_at(&(img->blue_channel), row, column);
    return retval;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
    validate_row_column(img, row, column);

    *Matrix_at(&(img->red_channel), row, column) = color.r;
    *Matrix_at(&(img->green_channel), row, column) = color.g;
    *Matrix_at(&(img->blue_channel), row, column) = color.b;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
    if (img == nullptr)
    {
        throw std::invalid_argument("img");
    }

    for (int row = 0; row < Image_height(img); row++)
    {
        for (int column = 0; column < Image_width(img); column++)
        {
            Image_set_pixel(img, row, column, color);
        }
    }
}
