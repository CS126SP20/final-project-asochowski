//
// Created by Aidan on 4/26/2020.
//

#ifndef FINALPROJECT_ANIMATION_H
#define FINALPROJECT_ANIMATION_H

#include <vector>
#include <cinder/gl/Texture.h>
#include <chrono>

using std::vector;

namespace mylibrary {

  class Animation {

  public:
    Animation(vector<cinder::gl::TextureRef>& textures);
    void SetInterval(int milliseconds);
    void Start(int milliseconds);
    cinder::gl::TextureRef& GetTexture();

  private:
    vector<cinder::gl::TextureRef> textures_;
    int texture_index_;
    int interval_;
    std::chrono::time_point<std::chrono::system_clock> last_change_time_;

  };

} // namespace mylibrary

#endif //FINALPROJECT_ANIMATION_H
