//
// Created by Aidan on 4/26/2020.
//

#include "mylibrary/animation.h"

namespace mylibrary {

Animation::Animation(mylibrary::TextureSheet& textures) {
  textures_ = textures;
  texture_index_ = 0;
  interval_ = -1;
}

Animation::Animation() {
  // Do nothing
}

void Animation::Start(int milliseconds) {
  interval_ = milliseconds;
  texture_index_ = 0;
  last_change_time_ = std::chrono::system_clock::now();
}

void Animation::SetInterval(int milliseconds) {
  interval_ = milliseconds;
}

cinder::gl::TextureRef& Animation::GetTexture() {
  // Updates texture_index_ if time elapsed is greater than the interval
  if (interval_ > 0 && std::chrono::system_clock::now() - last_change_time_
  > std::chrono::milliseconds(interval_)) {
    last_change_time_ = std::chrono::system_clock::now();
    texture_index_ = (texture_index_ + 1) % textures_.Size();
  }

  return textures_.Get(texture_index_);
}


}