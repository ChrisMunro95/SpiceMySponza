/**
 * @file    Image.hpp
 * @author  Tyrone Davison
 * @date    October 2012
 */

#pragma once
#ifndef __TYGRA_IMAGE__
#define __TYGRA_IMAGE__

#include <vector>
#include <cstdint>

namespace tygra
{

class Image
{
public:

    Image() : width_(0),
              height_(0),
              components_per_pixel_(0),
              bytes_per_component_(0)
    {
    }

    Image(Image&& rhs)
    {
        width_ = rhs.width_;
        height_ = rhs.height_;
        components_per_pixel_ = rhs.components_per_pixel_;
        bytes_per_component_ = rhs.bytes_per_component_;
        data_ = std::move(rhs.data_);
    }

    bool containsData() const
    {
        return !data_.empty();
    }

    unsigned int width() const
    {
        return width_;
    }

    unsigned int height() const
    {
        return height_;
    }

    unsigned int componentsPerPixel() const
    {
        return components_per_pixel_;
    }

    unsigned int bytesPerComponent() const
    {
        return bytes_per_component_;
    }
        
    const void* pixels() const
    {
        return !containsData() ? nullptr : &data_[0];
    }

    void* pixels()
    {
        return !containsData() ? nullptr : &data_[0];
    }

    const void* operator()(int x, int y) const
    {
        return !containsData() ? nullptr : data_.data() + (y * width_ + x)
                                           * components_per_pixel_
                                           * bytes_per_component_;
    }

    void* operator()(int x, int y)
    {
        return !containsData() ? nullptr : data_.data() + (y * width_ + x)
                                           * components_per_pixel_
                                           * bytes_per_component_;
    }

    void init(unsigned int width,
              unsigned int height,
              unsigned int components_per_pixel,
              unsigned int bytes_per_component)
    {
        width_ = width;
        height_ = height;
        components_per_pixel_ = components_per_pixel;
        bytes_per_component_ = bytes_per_component;
        const unsigned int number_of_bytes
            = width * height * components_per_pixel * bytes_per_component;
        data_.resize(number_of_bytes);
    }

private:
    unsigned int width_;
    unsigned int height_;
    unsigned int components_per_pixel_;
    unsigned int bytes_per_component_;
    std::vector<int8_t> data_;

};

} // end namespace tygra

#endif
