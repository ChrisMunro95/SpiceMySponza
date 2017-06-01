/**
 * @file    FileHelper.hpp
 * @author  Tyrone Davison
 * @date    September 2012
 */

#pragma once
#ifndef __TYGRA_FILEHELPER__
#define __TYGRA_FILEHELPER__

#include <string>
#include "Image.hpp"

namespace tygra
{
    /**
     * Construct a new string object with the contents of a text file.
     * @param   A valid, full path to a text file to read.
     * @return  The new new string object
     */
    std::string
    stringFromFile(std::string filepath);

    /**
     * Construct a new image object with the contents of a PNG file.
     * @param   A valid path to the PNG file to read.
     * @return  The new image object.
     */
    Image
    imageFromPNG(std::string filepath);

} // end namespace tygra

#endif
