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

  // Creates an animation object that will iterate through all of the textures
  // (in order) inside the TextureSheet.
  Animation(TextureSheet& textures);

  // The empty constructor of an animation.
  Animation();

  // Allows you to set the interval between texture switches, in milliseconds.
  void SetInterval(int milliseconds);

  // Starts an animation at a specific interval, in milliseconds.
  void Start(int milliseconds);

  // Gets the current texture. If the time since the last texture change is
  // greater than the current interval, it switches the current texture to the
  // next texture.
  cinder::gl::TextureRef& GetTexture();

private:

  // A TextureSheet object that stores all of the textures in the animation.
  TextureSheet textures_;

  // Stores the index of the current texture in the animation.
  int texture_index_;

  // Stores the interval set by Start() or SetInterval();
  int interval_;

  // The last time that the texture in the animation had changed.
  std::chrono::time_point<std::chrono::system_clock> last_change_time_;

};

} // namespace mylibrary

#endif //FINALPROJECT_ANIMATION_H
