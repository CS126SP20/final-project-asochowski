//
// Created by Aidan on 4/26/2020.
//

#ifndef FINALPROJECT_TEXTURE_SHEET_H
#define FINALPROJECT_TEXTURE_SHEET_H

#include <vector>
#include <cinder/gl/Texture.h>

namespace mylibrary {

struct Coordinate {
  int x;
  int y;
};

class TextureSheet {

public:
  TextureSheet(int width, int height,
      std::vector<Coordinate> texture_coordinates,
      std::string path);
  TextureSheet();
  cinder::gl::TextureRef& Get(int i);
  int Size();

private:
  std::vector<cinder::gl::TextureRef> textures_;

};

}

#endif //FINALPROJECT_TEXTURE_SHEET_H
