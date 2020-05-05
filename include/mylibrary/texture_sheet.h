//
// Created by Aidan on 4/26/2020.
//

#ifndef FINALPROJECT_TEXTURE_SHEET_H
#define FINALPROJECT_TEXTURE_SHEET_H

#include <vector>
#include <cinder/gl/Texture.h>
#include <cinder/app/App.h>

namespace mylibrary {

// A coordinate representing a specific frame in a texture sheet.
struct Coordinate {
  int x;
  int y;
};

// An object that stores multiple textures from an image. Takes in an image and
// a set of coordinates of a specific size, and parses them into individual
// textures.
class TextureSheet {

public:

  // Given a list of coordinates, the height and width of a coordinate, and
  // a file path name, creates a texture sheet of the textures at each
  // coordinate.
  TextureSheet(int width, int height,
      std::vector<Coordinate> texture_coordinates,
      std::string path);

  // The default constructor.
  TextureSheet();

  // Gets the texture of index i.
  cinder::gl::TextureRef& Get(int i);

  // Gets the number of textures in a texture sheet.
  int Size();

private:

  // A vector storing all of the textures in the texture sheet.
  std::vector<cinder::gl::TextureRef> textures_;

};

}

#endif //FINALPROJECT_TEXTURE_SHEET_H
