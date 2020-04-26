//
// Created by Aidan on 4/26/2020.
//

#ifndef FINALPROJECT_ANIMATION_H
#define FINALPROJECT_ANIMATION_H

#include <vector>
#include <cinder/gl/Texture.h>
#include <mylibrary/texture_sheet.h>
#include <chrono>

using std::vector;

namespace mylibrary {

  class Animation {

  public:
    Animation(TextureSheet& textures);
    void SetInterval(int milliseconds);
    void Start(int milliseconds);
    cinder::gl::TextureRef& GetTexture();

  private:
    TextureSheet textures_;
    int texture_index_;
    int interval_;
    std::chrono::time_point<std::chrono::system_clock> last_change_time_;

  };

} // namespace mylibrary

#endif //FINALPROJECT_ANIMATION_H