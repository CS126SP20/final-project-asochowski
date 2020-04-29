//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_MYLIBRARY_DEBRIS_H_
#define FINALPROJECT_MYLIBRARY_DEBRIS_H_

#include <Box2D/Box2D.h>
#include "mylibrary/texture_sheet.h"
#include "mylibrary/animation.h"

namespace mylibrary {

const int kDebrisSize = 16;
const int kNumTextures = 5;
const std::string kDebrisTexture = "C:/Users/Aidan/CLionProjects/Cinder/"
                                 "my-projects/final-project-asochowski/"
                                 "assets/npcsym.png";

const std::vector<Coordinate> kBulletCoordinates = {{0,2}};
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
