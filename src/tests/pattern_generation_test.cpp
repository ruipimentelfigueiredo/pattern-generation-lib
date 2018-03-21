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

    \brief Generic for pattern generation

    \author Rui Figueiredo : ruipimentelfigueiredo
    \author João Borrego   : jsbruglie

*/

#include <sstream>
#include <string>

#include "pattern_generation/PatternGeneration.h"

/* File streams */
#include <fstream>

// C libraries
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/filesystem.hpp>

// C++ libraries

// I/O streams
#include <iostream>
// File system
#include <boost/filesystem.hpp>


/* Media output directory */
#define MEDIA_DIR "../../media/materials/"
/* Textures subdir */
//#define TEXTURES_DIR MEDIA_DIR "textures/"
/* Scripts subdir */
//#define SCRIPTS_DIR MEDIA_DIR "scripts/"

/* Material name prefix */
#define MATERIAL_PREFIX "Plugin/"
/* Material name extension */
#define MATERIAL_EXT ".material"

/* Show image GUI */
#define SHOW_IMGS false

/* Generate .material script */
#define GENERATE_SCRIPT true

/// Default number of scenes
#define ARG_SCENES_DEFAULT      10
/// Default index of the first scene
#define ARG_START_DEFAULT       0
/// Default image directory
#define ARG_IMG_RESOLUTION_DEFAULT    500
/// Default dataset directory
#define ARG_DATASET_DIR_DEFAULT "dataset"

#define UNIFORM  "uniform"
#define PERLIN   "perlin"
#define GRADIENT "gradient"
#define ALL      "all"


#define ARG_TYPE_DEFAULT "dataset"


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
        << std::endl << "        texture " << material_name << ".jpg"
        << std::endl << "        filtering anistropic"
        << std::endl << "        max_anisotropy 16" 
        << std::endl << "      }"
        << std::endl << "    }"
        << std::endl << "  }"
        << std::endl << "}";

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
        "options: -n <number of scenes to generate>\n"  +
        "         -i <index of the first scene>\n" +
        "         -d <dataset output directory>\n" +
        "         -t <texture type>\n" +
        "         -r <images resolution>\n";
}

cv::Mat generateFlatTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
	cv::Scalar flat_color       = pattern_generation.getRandomColor();
	/* Generate flat texture */

	cv::Mat flat_texture = pattern_generation.getFlatTexture(flat_color,resolution);
       
        std::stringstream material_name;
        std::stringstream img_filename;

	material_name.str(std::string());
	img_filename.str(std::string());

	material_name << "flat_" << std::to_string(i);
	img_filename << TEXTURES_DIR << material_name.str() << ".jpg";

	if (!cv::imwrite(img_filename.str(), flat_texture)){
	    std::cout << "[ERROR] Could not save " << img_filename.str() <<
	    ". Please ensure the destination folder exists!" << std::endl;
	    exit(EXIT_FAILURE);
	}
	genScript(material_name.str(), SCRIPTS_DIR);

	if (SHOW_IMGS)
	    cv::imshow("Flat texture", flat_texture);
};

cv::Mat generateChessTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
	/* Initialize random device */
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist;
	int squares;
	int block_size;
	/* Generate square texture */
	//generateSquareTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
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
	img_filename << TEXTURES_DIR << material_name.str() << ".jpg";

	if (!cv::imwrite(img_filename.str(), chess_board)){
	    std::cout << "[ERROR] Could not save " << img_filename.str() <<
	    ". Please ensure the destination folder exists!" << std::endl;
	    exit(EXIT_FAILURE);
	}
	genScript(material_name.str(), SCRIPTS_DIR);

	if (SHOW_IMGS)
	    cv::imshow("Chess board", chess_board);
};


cv::Mat generateGradientTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
{
        std::stringstream material_name;
        std::stringstream img_filename;
	cv::Scalar gradient_color_1 = pattern_generation.getRandomColor();
	cv::Scalar gradient_color_2 = pattern_generation.getRandomColor();
	cv::Mat gradient_texture = pattern_generation.getGradientTexture(gradient_color_1, gradient_color_2, resolution, false);

	material_name.str(std::string());
	img_filename.str(std::string());

	material_name << "gradient_" << std::to_string(i);
	img_filename << TEXTURES_DIR << material_name.str() << ".jpg";

	if (!cv::imwrite(img_filename.str(), gradient_texture)){
	std::cout << "[ERROR] Could not save " << img_filename.str() <<
	    ". Please ensure the destination folder exists!" << std::endl;
	    exit(EXIT_FAILURE);
	}
	genScript(material_name.str(), SCRIPTS_DIR);

	if (SHOW_IMGS)
	    cv::imshow("Gradient texture", gradient_texture);
};

cv::Mat generatePerlinTexture(PatternGeneration & pattern_generation, unsigned int & resolution, const unsigned int & i, std::string & TEXTURES_DIR, std::string & SCRIPTS_DIR)
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
	img_filename << TEXTURES_DIR << material_name.str() << ".jpg";

	if (!cv::imwrite(img_filename.str(), perlin_texture)){
	std::cout << "[ERROR] Could not save " << img_filename.str() <<
	    ". Please ensure the destination folder exists!" << std::endl;
	    exit(EXIT_FAILURE);
	}
	genScript(material_name.str(), SCRIPTS_DIR);

	if (SHOW_IMGS)
	    cv::imshow("Perlin noise texture", perlin_texture);

};


void parseArgs(
    int argc,
    char** argv,
    unsigned int & scenes,
    unsigned int & start,
    std::string & dataset,
    unsigned int & resolution,
    std::string & type)
{

    int opt;
    bool d, t, i, s, r;

    while ( (opt = getopt(argc,argv,"n: d: t: s: r: h: i:")) != EOF)
    {
        switch (opt)
        {
            case 'd':
                d=true; dataset = optarg;    break;
            case 't':
                t=true; type = atoi(optarg); break;
            case 'n':
                s=true; scenes = atoi(optarg); break;
            case 'i':
                i=true; start = atoi(optarg); break;
            case 'r':
                r=true; resolution = atoi(optarg);    break;
            default:
                std::cout << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    // If arg was not set then assign default values
    if (!s) scenes      = ARG_SCENES_DEFAULT;
    if (!d) dataset     = ARG_DATASET_DIR_DEFAULT;
    if (!r) resolution  = ARG_IMG_RESOLUTION_DEFAULT;
    if (!i) start       = ARG_START_DEFAULT;
    if (!t) type        = ARG_TYPE_DEFAULT;
}



int main(int argc, char **argv)
{
    // Command-line arguments
    unsigned int scenes {0};
    unsigned int start {0};
    unsigned int resolution {0};
    std::string type;
    std::string media_dir;
    std::string dataset_dir;


    /* Number of images per class of texture */

    /* root directory */

    parseArgs(argc, argv, scenes, start, media_dir, resolution, type);
    std::string TEXTURES_DIR=media_dir+"textures/";
    std::string SCRIPTS_DIR=media_dir+"scripts/";

    boost::filesystem::path textures_dir(TEXTURES_DIR);
    if(boost::filesystem::create_directory(textures_dir)) {
	std::cout << "created " << TEXTURES_DIR << " folder"<< "\n";
    }

    boost::filesystem::path scripts_dir(SCRIPTS_DIR);
    if(boost::filesystem::create_directory(scripts_dir)) {
	std::cout << "created " << SCRIPTS_DIR << " folder"<< "\n";
    }

    /* Initialize random device */
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;

    /* Pattern generator object instance */   
    PatternGeneration pattern_generation;



    std::stringstream material_name;
    std::stringstream img_filename;

    for (unsigned int i = start; i < scenes; ++i)
    {
	      if(type=="all")
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
	      else if(type=="perlin")
              {
		/* Generate perlin noise texture */
		generatePerlinTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
              }
	      else if(type=="gradient")
              {
		/* Generate gradient texture */
		generateGradientTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
              }
	      else if(type=="flat")
	      {
                /* Generate flat texture */
		generateFlatTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
	      }
	      else if(type=="chess")
	      {
                /* Generate chess texture */
		generateChessTexture(pattern_generation, resolution, i, TEXTURES_DIR, SCRIPTS_DIR);
	      }
	      // you can have any number of case statements.
	      else
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
	    
    } 

    return 0;
}
