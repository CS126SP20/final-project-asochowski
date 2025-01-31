//
// Created by Aidan on 4/18/2020.
//

#include <set>
#include <mylibrary/engine.h>
#include <mylibrary/texture_sheet.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/msw/AppMsw.h>
#include <cinder/Path2d.h>
#include <vector>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/ImageIo.h>
#include <experimental/filesystem>
#include <cinder/app/Window.h>

namespace mylibrary {

//==============================================================================
// Public Functions
//==============================================================================

mylibrary::Engine::Engine(int screen_width, int screen_height,
    bool load_assets) {
  world_ = new b2World(b2Vec2(0, kGravity));
  screen_width_ = screen_width;
  screen_height_ = screen_height;
  m_to_px_ = (float) screen_width / kPixelToMeterRatio;
  px_to_m_ = 1.0f / m_to_px_;
  Player player(world_, load_assets);
  player_ = player;
  CreateBoundaries();
  start_time_ = std::chrono::system_clock::now();

  load_assets_ = load_assets;
  SoundManager sound_manager(load_assets);
  sound_manager_ = sound_manager;

  // Only load textures if it is specified that assets are to be loaded.
  if (load_assets) {
    LoadTextures();
  }
}

Engine::Engine() {
  screen_height_ = 0;
  screen_width_ = 0;
}

Engine::~Engine() {
  delete world_;
}

void mylibrary::Engine::Step() {
  if (running_) {
    CheckReset();
    CheckDebrisSpawn();
    CheckBullets();
    CheckDebrisCollisions();

    // If the game is not over:
    if (!over_) {
      CheckNearMisses();
      UpdatePlayer();
      UpdateScore();
    }

    // Always update the physics if the engine is running, even if the game is
    // over.
    world_->Step(kTimeStep, kVelIterations, kPosIterations);
  }

  // Only check sounds if assets have been loaded.
  if (load_assets_) {
    sound_manager_.CheckLoops();
  }
}

void mylibrary::Engine::KeyPress(int key_code) {
  held_keys_.insert(key_code);
}

void mylibrary::Engine::KeyRelease(int key_code) {
  held_keys_.erase(key_code);
}

void Engine::Draw() {
  if (load_assets_) {

    // Should always be drawing the background, unless the assets aren't loaded.
    DrawBackground();

    // The game is either in-game or at the game over screen.
    if (running_) {
      DrawPlayer();
      DrawDebris();
      DrawBullets();
      DrawPlatforms();

      if (over_) {
        DrawGameOver();
      } else {
        DrawGui();
      }

    // The game is at the title screen.
    } else {
      DrawTitleScreen();
    }
  }
}

void Engine::Start() {
  running_ = true;
  over_ = false;
  start_time_ = std::chrono::system_clock::now();
  score_ = 0;
  num_near_missed_ = 0;
  num_debris_shot_ = 0;
  last_debris_time_ = std::chrono::system_clock::now();

  if (load_assets_) {
    sound_manager_.StartGameMusic(true);
  }
}

void Engine::Stop() {
  Reset();

  if (load_assets_) {
    sound_manager_.StopAll();
    sound_manager_.StartMainMenuMusic(true);
  }

  running_ = false;
}

void Engine::End() {
  if (load_assets_ && !over_) {
    sound_manager_.StartGameOverMusic(true);
  }

  player_.Die();
  over_ = true;
}

bool mylibrary::Engine::IsRunning() {
  return running_;
}

Player& Engine::GetPlayer() {
  return player_;
}

void Engine::UpdateMousePos(const cinder::ivec2& px_pos) {
  mouse_pos_ = px_pos;
}

void Engine::Reset() {
  ClearEntities();
  player_.Reset();

  // Needs this because it will reset constantly until the R key is released if
  // not called.
  held_keys_.erase(cinder::app::KeyEvent::KEY_r);

  Start();
}

int Engine::CalculateScore(int num_shot, int num_near_miss, double seconds) {

  // Returns ((# near miss) + (# debris shot) * 2) * ln(seconds + 1.1)
  return (int) ((double) (num_near_miss + kDebrisMultiplier * num_shot) *
         log(seconds + kLogOffset));
}

//==============================================================================
// Private Functions
//==============================================================================

Debris* Engine::SpawnDebris(int x_px, int y_px) {
  b2Vec2 coords = PxCoordsToMeterCoords(b2Vec2(x_px, y_px));

  // Creating a new debris object and adding it to the list.
  auto* debris = new Debris(world_, coords.x, coords.y, kDebrisSize,
      load_assets_);
  all_debris_.push_back(debris);

  return debris;
}

Bullet* Engine::SpawnBullet(int x_px, int y_px) {
  b2Vec2 target = PxCoordsToMeterCoords(b2Vec2(x_px, y_px));

  // Creating a new bullet object and adding it to the list.
  auto* bullet = new Bullet(world_, player_.GetBody()->GetPosition(),
      target, load_assets_);
  all_bullets_.push_back(bullet);

  // Only play a sound if assets have been loaded.
  if (load_assets_) {
    sound_manager_.StartBulletShot();
  }

  return bullet;
}

void mylibrary::Engine::CreateBoundaries() {

  // Creating the ground
  b2BodyDef ground_body_def;
  ground_body_def.position = b2Vec2(0.0f, 0.0f);
  ground_body_def.angle = 0;
  ground_body_def.type = b2_staticBody;
  b2Body* ground_body = world_->CreateBody(&ground_body_def);

  // Creating the corners of the screen
  b2Vec2 corners[4];
  corners[0].Set((float) -screen_width_ * px_to_m_ / 2,
      (float) screen_height_* px_to_m_ / 2);
  corners[1].Set((float) -screen_width_ * px_to_m_ / 2,
      (float) - screen_height_* px_to_m_);
  corners[2].Set((float) screen_width_ * px_to_m_ / 2,
      (float) - screen_height_* px_to_m_);
  corners[3].Set((float) px_to_m_ * screen_width_ / 2,
      (float) screen_height_* px_to_m_ / 2);

  // Adding the border to the world
  b2ChainShape boundary_chain;
  boundary_chain.CreateLoop(corners, 4);
  ground_body->CreateFixture(&boundary_chain, 0.0f);

  // Creating the two platforms
  b2PolygonShape platform_box;
  platform_box.SetAsBox(kPlatformWidth, kPlatformHeight);

  for (int i = -1; i < 2; i += 2) {
    b2BodyDef platform_body_def;
    platform_body_def.position = b2Vec2(kPlatformDx * i, kPlatformDy);
    platform_body_def.angle = 0;
    platform_body_def.type = b2_staticBody;
    b2Body* platform_body = world_->CreateBody(&platform_body_def);

    platform_body->CreateFixture(&platform_box, 0.0f);
    all_platforms_.push_back(platform_body);
  }
}

void Engine::UpdatePlayer() {
  bool press_w = held_keys_.find(cinder::app::KeyEvent::KEY_w)
      != held_keys_.end();
  bool press_a = held_keys_.find(cinder::app::KeyEvent::KEY_a)
                 != held_keys_.end();
  bool press_s = held_keys_.find(cinder::app::KeyEvent::KEY_s)
                 != held_keys_.end();
  bool press_d = held_keys_.find(cinder::app::KeyEvent::KEY_d)
                 != held_keys_.end();
  bool press_space = held_keys_.find(cinder::app::KeyEvent::KEY_SPACE)
                     != held_keys_.end();

  b2Vec2 current_velocity = player_.GetVelocity();

  // Setting player's left and right movements according to A and D.
  if (press_a && press_d) {
    player_.ApplyImpulse(b2Vec2(-current_velocity.x, 0));
  } else if (press_a) {
    player_.ApplyImpulse(b2Vec2(-kMoveSpeed, 0));
  } else if (press_d) {
    player_.ApplyImpulse(b2Vec2(kMoveSpeed, 0));
  }

  if (press_space) {
    player_.Jump();
  }
}

void Engine::CheckDebrisCollisions() {
  for (int i = all_debris_.size() - 1; i >= 0; i--) {
    Debris* debris = all_debris_.at(i);
    b2ContactEdge* collisions_list = debris->GetBody()->GetContactList();

    // Delete debris that collide with a static body or a bullet:
    if (collisions_list && (collisions_list->other->GetType() == b2_staticBody
    || collisions_list->other->IsBullet())) {

      // If the collision is with a bullet:
      if (collisions_list->other->IsBullet()) {
        num_debris_shot_ += 1;

        // Plays a sound if assets are loaded.
        if (load_assets_) {
          sound_manager_.StartDebrisDestroyed();
        }
      }

      // Removes the debris.
      all_debris_.erase(all_debris_.begin()+i);
      delete debris;

    // If the collision is with the player:
    } else if (collisions_list && player_.IsBody(collisions_list->other)) {

      // Removes the debris and ends the game.
      all_debris_.erase(all_debris_.begin() + i);
      delete debris;
      End();
    }
  }
}

void Engine::CheckBullets() {
  for (int i = all_bullets_.size() - 1; i >= 0; i--) {
    Bullet* bullet = all_bullets_.at(i);
    auto current_element = all_bullets_.begin() + i;

    // Deleting bullets that are 2.5 seconds old
    if (bullet->GetMilliseconds() > kBulletLifetime) {
      all_bullets_.erase(current_element);
      delete bullet;

    // Deleting bullets that collide with a static body
    } else if (bullet->GetBody()->GetContactList()) {
      b2Body* other_body = bullet->GetBody()->GetContactList()->other;

      if (other_body->GetType() == b2_staticBody) {
        all_bullets_.erase(current_element);
        delete bullet;
      }
    }


  }
}

void Engine::CheckDebrisSpawn() {
  std::chrono::time_point<std::chrono::system_clock> current_time =
      std::chrono::system_clock::now();
  auto time_since_last_spawn = current_time - last_debris_time_;
  auto spawn_interval = GetDebrisSpawnInterval();

  // If the time since the last spawn is greater than the current spawn interval
  // time, spawn debris at a random location.
  if (time_since_last_spawn >= spawn_interval) {

    // Spawns a debris at a random spot at the top of the screen.
    SpawnDebris(rand() % screen_width_,
        -kDebrisSpawnHeightMultiplier * screen_height_);

    // Resets the last spawn time.
    last_debris_time_ = std::chrono::system_clock::now();
  }
}

std::chrono::milliseconds Engine::GetDebrisSpawnInterval() {
  std::chrono::time_point<std::chrono::system_clock> current_time =
      std::chrono::system_clock::now();
  auto time_since_start = current_time - start_time_;
  int time_in_seconds = (std::chrono::duration_cast<std::chrono::seconds>
      (time_since_start)).count();

  // Determining delay between spawns from current spawn rate
  double current_spawn_speed = kIntercept + (double) time_in_seconds / kSlope;
  int milliseconds_between_spawns = 1000.0 / current_spawn_speed;

  std::chrono::milliseconds milliseconds(milliseconds_between_spawns);

  return milliseconds;
}


b2Vec2 Engine::PxCoordsToMeterCoords(b2Vec2 px_vec) {
  return b2Vec2((px_vec.x - (float) screen_width_ / 2) * px_to_m_,
                (px_vec.y - (float) screen_height_ / 2) * px_to_m_);
}

b2Vec2 Engine::MeterCoordsToPxCoords(b2Vec2 m_vec) {
  return b2Vec2( (m_to_px_ * m_vec.x) + (float) screen_width_ / 2,
                 (m_to_px_ * m_vec.y) + (float) screen_height_ / 2);
}

void Engine::Click(int x_px, int y_px) {
  if (!running_) {
    Start();
  } else if (player_.CanShoot() && !over_) {

    // Spawns a bullet with a trajectory towards the mouse
    Bullet* bullet = SpawnBullet(x_px, y_px);
    b2Vec2 bullet_trajectory = bullet->GetTrajectory();

    // Knocks player back, reset player shoot timer.
    b2Vec2 player_knockback_impulse =
        b2Vec2(-kPlayerKnockbackForce * bullet_trajectory.x,
            -kPlayerKnockbackForce * bullet_trajectory.y);
    player_.GetBody()->ApplyForceToCenter(player_knockback_impulse);
    player_.Shoot();
  }
}

int Engine::GetSecondsSinceStart() {
  return (std::chrono::duration_cast<std::chrono::seconds>
      (std::chrono::system_clock::now() - start_time_)).count();
}

void Engine::DrawGui() {

  // Calculating font size and loading font.
  int font_size = kFontSizeMultiplier * screen_height_;
  cinder::DataSourceRef font = cinder::app::loadAsset(kFontPath);

  // Drawing a white score display at half the font size.
  std::string score_string = "Score: " + std::to_string(score_);
  cinder::gl::drawStringRight(score_string, cinder::vec2(
      screen_width_, screen_height_ / kTextOffset),
          cinder::Color(1,1,1),
          cinder::Font(font, font_size / 2));

  // Drawing a cooldown bar that can take up a maximum of 1/5 of the screen
  // width.
  cinder::Rectf cooldown_bar(screen_width_ * kCooldownBarOffset,
                             screen_width_ * kCooldownBarOffset,
                             screen_width_ * kCooldownBarOffset +
                             (1 - player_.GetCooldownPercent()) *
                             (screen_width_) / 5,
                             screen_width_ * kCooldownBarOffset * 3);
  cinder::gl::drawSolidRect(cooldown_bar);
}

void Engine::DrawGameOver() {

  // Calculating font size and loading font.
  int font_size = kFontSizeMultiplier * screen_height_;
  cinder::DataSourceRef font = cinder::app::loadAsset(kFontPath);

  b2Vec2 center = MeterCoordsToPxCoords(b2Vec2(0,0));

  // Drawing the text box background at the center of the screen, which takes
  // up 2/5 of the screen.
  cinder::Area text_box(center.x - (float) screen_width_ / 5.0f,
                        center.y - (float) screen_height_ / 5.0f,
                        center.x + (float) screen_width_ / 5.0f,
                        center.y + (float) screen_height_ / 5.0f);
  cinder::gl::draw(text_box_texture_, text_box);

  // Drawing the game over text at the top of the text box.
  cinder::gl::drawStringCentered("GAME OVER!", cinder::vec2(
      center.x,center.y - screen_height_ / 8.0f),
                                 cinder::Color(1,1,1),
                                 cinder::Font(font, font_size));

  // Drawing the score display at the middle of the text box.
  cinder::gl::drawStringCentered("Your score is: "
                                 + std::to_string(score_), cinder::vec2(
      center.x,center.y),cinder::Color(1,1,1),
                                 cinder::Font(font, font_size / 2));

  // Drawing instructions on how to restart the game at the bottom of the text
  // box.
  cinder::gl::drawStringCentered("Press R to restart", cinder::vec2(
      center.x,center.y + screen_height_ / 10.0f),
                                 cinder::Color(1,1,1),
                                 cinder::Font(font, font_size / 2));
}

void Engine::CheckNearMisses() {

  // Looping through all debris objects.
  for (Debris* debris: all_debris_) {

    // If the debris hasn't been near missed yet and the debris is close enough
    // to the player, set is at near missed
    if (debris->GetDistanceFrom(player_.GetBody()) <=
    kNearMissDistance && !debris->HasBeenNearMissed()) {
      debris->SetNearMissed();
      num_near_missed_ += 1;

      // Only play a sound if the assets are loaded.
      if (load_assets_) {
        sound_manager_.StartNearMiss();
      }
    }
  }
}

void Engine::UpdateScore() {
  score_ = CalculateScore(num_debris_shot_, num_near_missed_,
      GetSecondsSinceStart());
}

bool Engine::IsOver() {
  return over_;
}

void Engine::DrawBackground() {
  int m_w = screen_width_ / background_texture_->getWidth();

  // Needs to draw a little bit off the bottom because the 16:9 aspect ratio
  // will round down to 16:8 when rendering, so it adds a black line at the
  // bottom without this multiplier.
  int m_h = screen_height_ * 1.5 / background_texture_->getHeight();

  // Area to iterate over.
  cinder::Area background_area(0, 0, screen_width_ / 8,
      screen_height_ / 8);

  // Rect to iterate.
  cinder::Rectf screen_rect(0, 0, background_texture_
  ->getWidth() * m_w,background_texture_->getHeight() * m_h);

  // Draws this texture repeatedly over the entirety of the screen.
  cinder::gl::draw(background_texture_, background_area, screen_rect);
}

void Engine::DrawDebris() {

  // Calculating the actual debris render width and height in pixels.
  float debris_width = kDebrisRenderWidth * (float) screen_width_;
  float debris_height = kDebrisRenderHeight * (float) screen_height_;

  // Loops through all debris:
  for (Debris* debris: all_debris_) {
    b2Vec2 px_pos = MeterCoordsToPxCoords(((*debris).GetBody()->GetPosition()));

    // Creating the area to draw on.
    cinder::Area debris_rect(px_pos.x - debris_width / 2,
        px_pos.y - debris_height / 2,
        px_pos.x + debris_width / 2,
        px_pos.y + debris_height / 2);

    cinder::gl::draw((*debris).GetTexture(), debris_rect);
  }
}

void Engine::DrawPlayer() {

  // Calculating the actual player width and height in pixels.
  float player_width = kPlayerRenderWidth * (float) screen_width_;
  float player_height = kPlayerRenderHeight * (float) screen_height_;

  b2Vec2 px_pos = MeterCoordsToPxCoords(
      player_.GetBody()->GetPosition());

  // Creating the render rectangle.
  cinder::Area player_rect(px_pos.x - player_width / 2,
      px_pos.y - player_height / 2,
      px_pos.x + player_width / 2,
      px_pos.y + player_height / 2);

  // Determining which way the player should be facing.
  bool left = true;
  if (mouse_pos_.x > px_pos.x) {
    left = false;
  }

  cinder::gl::draw(player_.GetTexture(left), player_rect);
}

void Engine::DrawBullets() {

  // Calculating the actual bullet width and height in pixels.
  float bullet_width = kBulletRenderWidth * (float) screen_width_;
  float bullet_height = kBulletRenderHeight * (float) screen_height_;

  // Loop through all bullets
  for (Bullet* bullet: all_bullets_) {
    b2Vec2 px_pos = MeterCoordsToPxCoords((bullet->GetBody()->GetPosition()));

    // Creating the render rectangle.
    cinder::Area debris_rect(px_pos.x - bullet_width / 2,
        px_pos.y - bullet_height / 2,
        px_pos.x + bullet_width / 2,
        px_pos.y + bullet_height / 2);

    cinder::gl::draw(bullet->GetTexture(), debris_rect);
  }
}

void Engine::DrawPlatforms() {

  // Calculating the actual platform width and height in pixels.
  float width = m_to_px_ * kPlatformWidth;
  float height = m_to_px_ * kPlatformHeight;

  // Looping though all of the platforms.
  for (int i = 0; i < all_platforms_.size(); i++) {
    b2Body* current_body = all_platforms_.at(i);
    b2Vec2 pos = current_body->GetPosition();
    b2Vec2 px_pos = MeterCoordsToPxCoords(pos);

    // Creating the platform render area
    cinder::Area platform_area(px_pos.x - width,
        px_pos.y - 3 * height,
        px_pos.x + width,
        px_pos.y + height);

    cinder::gl::draw(platform_texture_, platform_area);
  }
}


void Engine::DrawTitleScreen() {
  int font_size = kFontSizeMultiplier * screen_height_;
  b2Vec2 center = MeterCoordsToPxCoords(b2Vec2(0,0));

  // Drawing the title at the top of the screen.
  cinder::Area text_box(center.x - (float) screen_width_ / 4.0f,
                        center.y - (float) screen_width_ / 4.0f,
                        center.x + (float) screen_width_ / 4.0f,
                        center.y - (float) screen_width_ / 8.0f);
  cinder::gl::draw(title_texture_, text_box);

  // Drawing bottom instructions.
  cinder::gl::drawStringCentered("LMB to begin, ESC to quit",
      cinder::vec2(center.x,center.y),
      cinder::Color(1,1,1),
      cinder::Font(cinder::app::loadAsset(kFontPath),
          font_size));
}

void Engine::LoadTextures() {

  // Creating backround texture without bilinear filtering and with repeating.
  cinder::gl::Texture::Format fmt;
  fmt.setWrap(GL_REPEAT, GL_REPEAT);
  fmt.enableMipmapping();
  fmt.setMinFilter( GL_NEAREST_MIPMAP_NEAREST );
  fmt.setMagFilter( GL_NEAREST );
  background_texture_ = cinder::gl::Texture::create(cinder::
      loadImage(cinder::app::loadAsset(kBackgroundImagePath)), fmt);

  // Getting platform texture from a texture sheet.
  TextureSheet platform_texture_sheet(80, 32,
      std::vector<Coordinate>{{0,2}},
      kPlatformImagePath);
  platform_texture_ = platform_texture_sheet.Get(0);

  // Getting text box texture from a texture sheet.
  TextureSheet text_box_texture_sheet(22, 18,
      std::vector<Coordinate>{{0,0}},
      kTextBoxImagePath);
  text_box_texture_ = text_box_texture_sheet.Get(0);

  // Getting the title texture from a texture sheet.
  TextureSheet title_texture_sheet(128, 32,
      std::vector<Coordinate>{{0,0}},
      kTitleImagePath);
  title_texture_ = title_texture_sheet.Get(0);

  // Pre-loading the debris and bullet textures to save resources during
  // run-time.
  Debris::LoadTexture();
  Bullet::LoadTexture();
}

void Engine::ClearEntities() {

  // Clearing all bullets
  for (int i = all_bullets_.size() - 1; i >= 0; i--) {
    Bullet *bullet = all_bullets_.at(i);
    all_bullets_.erase(all_bullets_.begin() + i);
    delete bullet;
  }

  // Clearing all the debris
  for (int i = all_debris_.size() - 1; i >= 0; i--) {
    Debris* debris = all_debris_.at(i);
    all_debris_.erase(all_debris_.begin() + i);
    delete debris;
  }
}

void Engine::CheckReset() {
  if (held_keys_.find(cinder::app::KeyEvent::KEY_r)
  != held_keys_.end()) {
    Reset();
  } else if (held_keys_.find(cinder::app::KeyEvent::KEY_ESCAPE)
  != held_keys_.end() && running_) {
    Stop();
  }
}


}