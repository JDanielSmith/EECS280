#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cstring>
#include <filesystem>

#include "Image.h"
#include "processing.h"

static std::string usage(std::string me = "resize.exe") // TODO: `basename $0`
{
	std::string retval = "Usage: " + me + " IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
		"WIDTH and HEIGHT must be less than or equal to original";
	return retval;
}

static int validate(const Image& img, int width, int &height)
{
	// "The desired width is ... less than or equal to the original width of the input image."
	if (!(width <= Image_width(&img)))
	{
		std::cerr << usage() << "\n";
		return EXIT_FAILURE;
	}

	if (height < 0) // optional command-line argument
	{
		height = Image_height(&img);
	}
	// "The desired height is ... less than or equal to the original height of the input image."
	if (!(height <= Image_height(&img)))
	{
		std::cerr << usage() << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

static int resize(const std::filesystem::path& in, const std::filesystem::path& out, int width, int height)
{
	std::ifstream fin(in);
	if (!fin.is_open()) {
		std::cerr << "Error opening file: " << in << "\n";
		return EXIT_FAILURE;
	}

	Image* img = new Image;
	Image_init(img, fin);

	const auto retval = validate(*img, width, height);
	if (retval != EXIT_SUCCESS)
	{
		return retval;
	}


	seam_carve(img, width, height); // TODO


	std::ofstream off(out);
	Image_print(img, off);

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	// "There are 4 or 5 arguments, including the executable name itself (i.e. argv[0])."
	if (!((argc == 4) || (argc == 5)))
	{
		std::cerr << usage() << "\n";
		return EXIT_FAILURE;
	}

	// see usage, above
	const std::string width_(argv[3]);
	const auto width = atoi(width_.c_str());
	// "The desired width is greater than 0 ..."
	if (width < 0)
	{
		std::cerr << usage() << "\n";
		return EXIT_FAILURE;
	}

	int height = -1; // HEIGHT is optional
	if (argc == 5)
	{
		const std::string height_(argv[4]);
		height = atoi(height_.c_str());
		// "The desired width is greater than 0 ..."
		if (height < 0)
		{
			std::cerr << usage() << "\n";
			return EXIT_FAILURE;
		}
	}
	
	const std::string in_filename(argv[1]);
	const std::string out_filename(argv[2]);
	return resize(in_filename, out_filename, width, height);
}
