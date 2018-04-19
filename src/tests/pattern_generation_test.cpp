/*
 *  Copyright (C) 2018 João Borrego and Rui Figueiredo
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*!

    \brief Simple script to generate textures

    \author Rui Figueiredo : ruipimentelfigueiredo
    \author João Borrego   : jsbruglie

*/

#include "pattern_generation/PatternGeneration.h"

// C libraries
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/filesystem.hpp>

// C++ libraries

// Streams and strings
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// File system
#include <boost/filesystem.hpp>


/// Default image file extension
#define IMG_EXT         ".png"
/// Material name prefix
#define MATERIAL_PREFIX "Plugin/"
/// Material name extension
#define MATERIAL_EXT    ".material"

/// Show image GUI
#define SHOW_IMGS       false
/// Generate .material script
#define GENERATE_SCRIPT true

/// Default number of textures
#define ARG_TEXTURES_DEFAULT        10
/// Default index of the first texture
#define ARG_START_DEFAULT           0
/// Default image directory
#define ARG_IMG_RESOLUTION_DEFAULT  500
/// Default output directory
#define ARG_OUTPUT_DIR_DEFAULT     "output/"
/// Default image file extension
#define ARG_TYPE_DEFAULT            "all"

void genScript(const std::string material_name, const std::string & SCRIPTS_DIR){

    std::stringstream script_str;
    std::stringstream file_path;

    if (GENERATE_SCRIPT){

        script_str
        << "material " << MATERIAL_PREFIX << material_name
        << std::endl << "{"
        << std::endl << "  technique"
        << std::endl << "  {"
        << std::endl << "    pass"
        << std::endl << "    {"
        << std::endl << "      texture_unit"
        << std::endl << "      {"
        << std::endl << "        texture " << material_name << IMG_EXT
        << std::endl << "        filtering anistropic"
        << std::endl << "        max_anisotropy 16"
        << std::endl << "      }"
        << std::endl << "    }"
        << std::endl << "  }"
        << std::endl << "}" << std::endl;

        file_path << SCRIPTS_DIR << material_name << MATERIAL_EXT;

        /* Write to file */
        std::ofstream ofs(file_path.str());
        ofs << script_str.str();
        ofs.close();
    }
}

//////////////////////////////////////////////////
const std::string getUsage(const char* argv_0)
{
    return \
        "usage:   " + std::string(argv_0) + " [options]\n" +
        "options: -n <number of textures generate>\n"  +
        "         -i <index of the first texure>\n" +
        "         -d <output directory>\n" +
        "         -t <texture type>\n" +
        "         -r <image resolution>\n";
}

//////////////////////////////////////////////////
void generateFlatTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
    cv::Scalar flat_color = pattern_generation.getRandomColor();
    cv::Mat flat_texture = pattern_generation.getFlatTexture(flat_color,resolution);

    std::stringstream material_name;
    std::stringstream img_filename;

    material_name.str(std::string());
    img_filename.str(std::string());

    material_name << "flat_" << std::to_string(i);
    img_filename << TEXTURES_DIR << material_name.str() << IMG_EXT;

    if (!cv::imwrite(img_filename.str(), flat_texture)){
        std::cout << "[ERROR] Could not save " << img_filename.str() <<
        ". Please ensure the destination folder exists!" << std::endl;
        exit(EXIT_FAILURE);
    }
    genScript(material_name.str(), SCRIPTS_DIR);

    if (SHOW_IMGS)
        cv::imshow("Flat texture", flat_texture);
};

//////////////////////////////////////////////////
void generateChessTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
    /* Initialize random device */
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;
    int squares;
    int block_size;
    /* Generate square texture */
    squares = dist(mt) % 20 + 8;     // v2 in the range 1 to 100
    block_size = resolution / squares;
    if (block_size % 2 == 0)
        block_size++;

    cv::Scalar chess_color_1    = pattern_generation.getRandomColor();
    cv::Scalar chess_color_2    = pattern_generation.getRandomColor();

    cv::Mat chess_board = pattern_generation.getChessTexture(chess_color_1, chess_color_2, block_size, squares);

    // Convert to HSV
    cv::applyColorMap(chess_board, chess_board, cv::COLORMAP_HSV);

    std::stringstream material_name;
    std::stringstream img_filename;

    /* Reset string stream values */
    material_name.str(std::string());
    img_filename.str(std::string());

    material_name << "chess_" << std::to_string(i);
    img_filename << TEXTURES_DIR << material_name.str() << IMG_EXT;

    if (!cv::imwrite(img_filename.str(), chess_board)){
        std::cout << "[ERROR] Could not save " << img_filename.str() <<
        ". Please ensure the destination folder exists!" << std::endl;
        exit(EXIT_FAILURE);
    }
    genScript(material_name.str(), SCRIPTS_DIR);

    if (SHOW_IMGS)
        cv::imshow("Chess board", chess_board);
};

//////////////////////////////////////////////////
void generateGradientTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
    std::stringstream material_name;
    std::stringstream img_filename;

    cv::Scalar gradient_color_1 = pattern_generation.getRandomColor();
    cv::Scalar gradient_color_2 = pattern_generation.getRandomColor();
    cv::Mat gradient_texture = pattern_generation.getGradientTexture(gradient_color_1, gradient_color_2, resolution, false);

    material_name.str(std::string());
    img_filename.str(std::string());

    material_name << "gradient_" << std::to_string(i);
    img_filename << TEXTURES_DIR << material_name.str() << IMG_EXT;

    if (!cv::imwrite(img_filename.str(), gradient_texture)){
    std::cout << "[ERROR] Could not save " << img_filename.str() <<
        ". Please ensure the destination folder exists!" << std::endl;
        exit(EXIT_FAILURE);
    }
    genScript(material_name.str(), SCRIPTS_DIR);

    if (SHOW_IMGS)
        cv::imshow("Gradient texture", gradient_texture);
};

//////////////////////////////////////////////////
void generatePerlinTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
    std::stringstream material_name;
    std::stringstream img_filename;
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int> dist;
    /* Generate perlin noise texture */
    double z1=((double) dist(mt) / (RAND_MAX));
    double z2=((double) dist(mt) / (RAND_MAX));
    double z3=((double) dist(mt) / (RAND_MAX));
    srand(time(0));
    int randomval = dist(mt) % 2;

    cv::Mat perlin_texture = pattern_generation.getPerlinNoiseTexture(resolution,randomval,z1,z2,z3);

    material_name.str(std::string());
    img_filename.str(std::string());

    material_name << "perlin_" << std::to_string(i);
    img_filename << TEXTURES_DIR << material_name.str() << IMG_EXT;

    if (!cv::imwrite(img_filename.str(), perlin_texture)){
    std::cout << "[ERROR] Could not save " << img_filename.str() <<
        ". Please ensure the destination folder exists!" << std::endl;
        exit(EXIT_FAILURE);
    }
    genScript(material_name.str(), SCRIPTS_DIR);

    if (SHOW_IMGS)
        cv::imshow("Perlin noise texture", perlin_texture);
};

//////////////////////////////////////////////////
void parseArgs(
    int argc,
    char** argv,
    unsigned int & textures,
    unsigned int & start,
    std::string & output,
    unsigned int & resolution,
    std::string & type)
{
    int opt;
    bool d, t, i, s, r;

    while ( (opt = getopt(argc,argv,"d: t: n: s: i: r:")) != EOF)
    {
        switch (opt)
        {
            case 'd':
                d=true; output = optarg; break;
            case 't':
                t=true; type = optarg; break;
            case 'n':
                s=true; textures = atoi(optarg); break;
            case 'i':
                i=true; start = atoi(optarg); break;
            case 'r':
                r=true; resolution = atoi(optarg); break;
            default:
                std::cout << getUsage(argv[0]) << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    // If arg was not set then assign default values
    if (!s) textures    = ARG_TEXTURES_DEFAULT;
    if (!d) output      = ARG_OUTPUT_DIR_DEFAULT;
    if (!r) resolution  = ARG_IMG_RESOLUTION_DEFAULT;
    if (!i) start       = ARG_START_DEFAULT;
    if (!t) type        = ARG_TYPE_DEFAULT;
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Command-line arguments
    unsigned int textures {0};
    unsigned int start {0};
    unsigned int resolution {0};
    std::string type;
    std::string media_dir;
    std::string output_dir;

    /* root directory */
    parseArgs(argc, argv, textures, start, media_dir, resolution, type);
    std::string TEXTURES_DIR=media_dir+"textures/";
    std::string SCRIPTS_DIR=media_dir+"scripts/";

    boost::filesystem::path textures_dir(TEXTURES_DIR);
    if (boost::filesystem::create_directories(textures_dir)) {
        std::cout << "Created " << TEXTURES_DIR << " folder"<< "\n";
    }

    boost::filesystem::path scripts_dir(SCRIPTS_DIR);
    if (boost::filesystem::create_directories(scripts_dir)) {
        std::cout << "Created " << SCRIPTS_DIR << " folder"<< "\n";
    }

    /* Initialize random device */
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;

    /* Pattern generator object instance */
    PatternGeneration pattern_generation;

    for (unsigned int i = start; i < textures; ++i)
    {
        std::cout << "\rGenerating " << i + 1 << " of " << textures << std::flush;

        if (type=="all")
        {
            /* Generate flat texture */
            generateFlatTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
            /* Generate chess texture */
            generateChessTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
            /* Generate gradient texture */
            generateGradientTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
            /* Generate perlin noise texture */
            generatePerlinTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
        }
        else if (type=="perlin")
        {
            /* Generate perlin noise texture */
            generatePerlinTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
        }
        else if (type=="gradient")
        {
            /* Generate gradient texture */
            generateGradientTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
        }
        else if (type=="flat")
        {
            /* Generate flat texture */
            generateFlatTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
        }
        else if (type=="chess")
        {
            /* Generate chess texture */
            generateChessTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
        }
        else
        {
            std::cerr << "No valid option selected! Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
