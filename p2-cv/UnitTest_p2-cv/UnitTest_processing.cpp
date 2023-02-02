#include "pch.h"
#include "CppUnitTest.h"
#include "p2-cv.h"

#include <assert.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "processing.h"
#include "Image_test_helpers.h"
#include "Matrix_test_helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTestp2cv
{
	TEST_CLASS(UnitTest_processing)
	{
	public:
        const string OUT_PPM_EXT = ".out.ppm";
        const string OUT_TXT_EXT = ".out.txt";

        void test_rotate(const Image* img, string prefix) {
            Image* rotated_img = new Image;
            Image* rotated_img_correct = new Image;

            // Test left rotation
            cout << "Testing " << prefix << " rotate left..." << flush;
            *rotated_img = *img;
            rotate_left(rotated_img);
            write_image(rotated_img, prefix + "_left" + OUT_PPM_EXT);

            load_image(rotated_img_correct, prefix + "_left.correct.ppm");
            assert(Image_equal(rotated_img, rotated_img_correct));
            cout << "PASS" << endl;

            // Test right rotation
            cout << "Testing " << prefix << " rotate right...";
            *rotated_img = *img;
            rotate_right(rotated_img);
            write_image(rotated_img, prefix + "_right" + OUT_PPM_EXT);

            load_image(rotated_img_correct, prefix + "_right.correct.ppm");
            assert(Image_equal(rotated_img, rotated_img_correct));
            cout << "PASS" << endl;

            delete rotated_img_correct;
            delete rotated_img;
        }

        void test_energy(const Matrix* energy_mat, string prefix) {
            cout << "Testing " << prefix << " energy..." << flush;

            write_matrix(energy_mat, prefix + "_energy" + OUT_TXT_EXT);

            Matrix* energy_mat_correct = new Matrix;
            load_matrix(energy_mat_correct, prefix + "_energy_correct.txt");

            assert(Matrix_equal(energy_mat, energy_mat_correct));
            cout << "PASS" << endl;

            delete energy_mat_correct;
        }


        void test_cost(const Matrix* cost_mat, string prefix) {
            cout << "Testing " << prefix << " cost..." << flush;

            write_matrix(cost_mat, prefix + "_cost" + OUT_TXT_EXT);

            Matrix* cost_mat_correct = new Matrix;
            load_matrix(cost_mat_correct, prefix + "_cost_correct.txt");

            assert(Matrix_equal(cost_mat, cost_mat_correct));
            cout << "PASS" << endl;

            delete cost_mat_correct;
        }

        void test_find_seam(const int seam[], int n, string prefix) {
            cout << "Testing " << prefix << " find seam..." << flush;
            write_seam(seam, n, prefix + "_seam" + OUT_TXT_EXT);

            int seam_correct[MAX_MATRIX_HEIGHT];
            load_seam(seam_correct, prefix + "_seam_correct.txt");

            assert(array_equal(seam, seam_correct, n));
            cout << "PASS" << endl;
        }

        void test_remove_seam(const Image* img, const int seam[],
            string prefix) {
            cout << "Testing " << prefix << " remove seam..." << flush;

            Image* removed_img = new Image(*img);
            remove_vertical_seam(removed_img, seam);
            write_image(removed_img, prefix + "_removed" + OUT_PPM_EXT);

            Image* removed_img_correct = new Image;
            load_image(removed_img_correct, prefix + "_removed.correct.ppm");

            assert(Image_equal(removed_img, removed_img_correct));
            cout << "PASS" << endl;

            delete removed_img_correct;
            delete removed_img;
        }

        void test_seam_carve(const Image* img, string prefix,
            int new_width, int new_height) {
            cout << "Testing " << prefix << " seam carve ";
            cout << new_width << "x" << new_height << "..." << flush;
            Image* carved_img = new Image(*img);
            seam_carve(carved_img, new_width, new_height);
            write_image(carved_img,
                prefix + "_" + to_string(new_width) +
                "x" + to_string(new_height) + OUT_PPM_EXT);

            Image* carved_img_correct = new Image;
            load_image(carved_img_correct,
                prefix + "_" + to_string(new_width) +
                "x" + to_string(new_height) + ".correct.ppm");

            assert(Image_equal(carved_img, carved_img_correct));
            cout << "PASS" << endl;

            delete carved_img_correct;
            delete carved_img;
        }

        static void load_matrix(Matrix* mat, string filename) {
            ifstream fin;
            fin.open(filename.c_str());

            if (!fin.is_open()) {
                cout << "Unable to open " << filename << endl;
                exit(EXIT_FAILURE);
            }

            int width, height;
            fin >> width >> height;
            Matrix_init(mat, width, height);

            for (int r = 0; r < height; ++r) {
                for (int c = 0; c < width; ++c) {
                    fin >> *Matrix_at(mat, r, c);
                }
            }
        }

        static void write_matrix(const Matrix* mat, string filename) {
            ofstream fout(filename.c_str());
            Matrix_print(mat, fout);
        }

        static void load_image(Image* img, string filename) {
            ifstream fin;
            fin.open(filename.c_str());

            if (!fin.is_open()) {
                cout << "Unable to open " << filename << endl;
                exit(EXIT_FAILURE);
            }

            Image_init(img, fin);
        }

        static void write_image(const Image* img, string filename) {
            ofstream fout(filename.c_str());
            Image_print(img, fout);
        }

        static void load_seam(int seam[], string filename) {
            ifstream fin;
            fin.open(filename.c_str());

            if (!fin.is_open()) {
                cout << "Unable to open " << filename << endl;
                exit(EXIT_FAILURE);
            }

            for (int i = 0, column; fin >> column; ++i) {
                seam[i] = column;
            }
        }

        static void write_seam(const int seam[], int len, string filename) {
            ofstream fout(filename.c_str());
            for (int i = 0; i < len; ++i) {
                fout << seam[i] << endl;
            }
        }

	};
}
