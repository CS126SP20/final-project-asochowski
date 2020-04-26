//
// Created by Aidan on 4/26/2020.
//

#include "mylibrary/texture_sheet.h"

namespace mylibrary {

TextureSheet::TextureSheet(int width, int height,
                           std::vector<Coordinate> texture_coordinates,
                           std::string path) {

  // Loading the whole image source
  cinder::ImageSourceRef full_image = cinder::loadImage(path);

  // Goes through each specified coordinate,
  for (Coordinate& coordinate: texture_coordinates) {

    // Checking out of bounds errors
    if ((coordinate.x + 1) * width - 1 > full_image->getWidth() ||
        (coordinate.y + 1) * height - 1 > full_image->getHeight()) {
      throw -1;
    }

    // Gets only the texture at the coordinate
    cinder::gl::TextureRef texture = cinder::gl::Texture::create(full_image);
    cinder::Area texture_area(coordinate.x * width,
        coordinate.y * height,
        (coordinate.x + 1) * width - 1,
        (coordinate.y + 1) * height - 1);

    // Sets textures bounds to only include one texture
    texture->setCleanBounds(texture_area);
    textures_.push_back(texture);
  }
}

TextureSheet::TextureSheet() {
  // Contains nothing
}

cinder::gl::TextureRef& TextureSheet::Get(int i) {
  return textures_.at(i);
}

int TextureSheet::Size() {
  return textures_.size();
}

}