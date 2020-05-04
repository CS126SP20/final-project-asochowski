//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_MYLIBRARY_DEBRIS_H_
#define FINALPROJECT_MYLIBRARY_DEBRIS_H_

#include <Box2D/Box2D.h>
#include "mylibrary/texture_sheet.h"
#include "mylibrary/animation.h"

namespace mylibrary {

const int kDebrisTextureSize = 16;
const int kNumTextures = 5;
const float kDebrisRenderHeight = 130.0 / 1080.0;
const float kDebrisRenderWidth = 130.0 / 1920.0;
const float kDebrisDensity = 10.0f;
const int kDebrisAnimationMs = 50;

const std::string kDebrisTexturePath = "images/debris.png";

const std::vector<Coordinate> kDebrisCoordinates = {{0, 3},
                                                    {1, 3},
                                                    {2, 3},
                                                    {3, 3},
                                                    {4, 3}};

static TextureSheet debris_texture_sheet_;

class Debris {
public:

  // The default constructor.
  Debris();

  // The main constructor of the debris. Spawns a Debris object at the position
  // (x,y) of radius r in the b2World.
  Debris(b2World* world, int x, int y, float r, bool load_assets);

  // The destructor of the debris.
  ~Debris();

  // Returns a pointer to the b2Body representing the debris.
  b2Body* GetBody();

  // Gets the distance between this debris and another b2Body.
  float GetDistanceFrom(b2Body* other_body);

  // Sets that this debris has already has a "near miss" with the player.
  void SetNearMissed();

  // Returns whether or not the debris has had a "near miss" with the player.
  bool HasBeenNearMissed();

  // Gets the current texture of the debris, given its animation.
  cinder::gl::TextureRef GetTexture();

  // Loads the TextureSheet of all debris. MUST BE CALLED BEFORE ANY DEBRIS ARE
  // RENDERED.
  static void LoadTexture();

private:

  // The pointer to the b2Body representing the debris.
  b2Body* body_;

  // Whether or not this debris has has a "near miss" before.
  bool near_missed_ = false;

  // The animation of this debris object.
  Animation animation_;

  // Loads the animation of the debris, with the preloaded TextureSheet.
  void LoadAnimation();

};

}

#endif //FINALPROJECT_MYLIBRARY_DEBRIS_H_
