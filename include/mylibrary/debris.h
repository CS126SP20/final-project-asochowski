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

const std::string kDebrisTexturePath = "C:/Users/Aidan/CLionProjects/Cinder/"
                                 "my-projects/final-project-asochowski/"
                                 "assets/npcsym.png";

const std::vector<Coordinate> kDebrisCoordinates = {{0, 3},
                                                    {1, 3},
                                                    {2, 3},
                                                    {3, 3},
                                                    {4, 3}};

static TextureSheet debris_texture_sheet_;

class Debris {
public:
  Debris();
  Debris(b2World* world, int x, int y, float r);
  ~Debris();
  b2Body* GetBody();
  float GetDistanceFrom(b2Body* other_body);
  void SetNearMissed();
  bool HasBeenNearMissed();
  cinder::gl::TextureRef GetTexture();
  static void LoadTexture();

private:
  b2Body* body_;
  bool near_missed_ = false;
  Animation animation_;

  void LoadAnimation();

};

}

#endif //FINALPROJECT_MYLIBRARY_DEBRIS_H_
