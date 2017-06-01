/**
 * @file    FileHelper.cpp
 * @author  Tyrone Davison
 * @date    September 2012
 */

#define _CRT_SECURE_NO_WARNINGS

#include <tygra/FileHelper.hpp>
#include <png/png.h>
#include <fstream>
#include <sstream>
#include <cassert>

namespace tygra
{

std::string
stringFromFile(std::string filepath)
{
    std::ifstream fp;
    fp.open(filepath, std::ifstream::in);
    if (fp.is_open() == false) {
        return "";
    }
    std::stringstream ss;
    ss << fp.rdbuf();
    return ss.str();
}

Image
imageFromPNG(std::string filepath)
{
    Image result;

    FILE *fp = fopen(filepath.c_str(), "rb");
    if (fp == nullptr) {
        return result;
    }

    const int header_size = 8;
    png_byte header[header_size];
    fread(header, 1, header_size, fp);

    if (png_sig_cmp(header, 0, header_size) != 0) {
        fclose(fp);
        return result;
    }

    png_structp png_ptr = png_create_read_struct(
                              PNG_LIBPNG_VER_STRING,
                              nullptr,
                              nullptr,
                              nullptr);
    if (png_ptr == nullptr) {
        fclose(fp);
        return result;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return result;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return result;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, header_size);
    png_read_png(png_ptr,
                 info_ptr,
                 PNG_TRANSFORM_PACKING
                 | PNG_TRANSFORM_EXPAND
                 | PNG_TRANSFORM_SWAP_ENDIAN,
                 nullptr);

    const int image_width = png_get_image_width(png_ptr, info_ptr);
    const int image_height = png_get_image_height(png_ptr, info_ptr);
    const int bits_per_channel = png_get_bit_depth(png_ptr, info_ptr);
    const int bytes_per_channel = bits_per_channel / 8;
    const int channels_per_pixel = png_get_channels(png_ptr, info_ptr);
    assert(bits_per_channel % 8 == 0);
    result.init(image_width,
                image_height,
                channels_per_pixel,
                bytes_per_channel);
    png_bytep pixels = (png_bytep)result.pixels();
    png_bytepp row_ptrs = png_get_rows(png_ptr, info_ptr);
    for (int y=0; y<image_height; ++y) {
        memcpy(pixels + y * png_get_rowbytes(png_ptr, info_ptr),
               row_ptrs[image_height-y-1],
               png_get_rowbytes(png_ptr, info_ptr));
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    return result;
}

} // end namespace tyga
