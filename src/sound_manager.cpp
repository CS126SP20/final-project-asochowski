//
// Created by Aidan on 5/1/2020.
//

#include "mylibrary/sound_manager.h"


namespace mylibrary {

//==============================================================================
// Public Functions
//==============================================================================

SoundManager::SoundManager() {
  // Do nothing here.
}

SoundManager::SoundManager(bool load_assets) {
  if (load_assets) {
    LoadSounds();
    SetVolume(volume_);
  }
}

void SoundManager::SetVolume(float volume) {
  if (volume >= 0.0f && volume <= 1.0f) {
    volume_ = volume;
  }

  // Sets all sound's volume to the current volume.
  game_->setVolume(volume_);
  game_over_->setVolume(volume_);
  main_menu_->setVolume(volume_);
  bullet_shot_->setVolume(volume_);
  debris_destroy_->setVolume(volume_);
  near_miss_->setVolume(volume_);
}

void SoundManager::StopAll() {
  StopAllLoops();
  StopAllMusic();
  StopAllSoundEffects();
}

void SoundManager::StartGameOverMusic(bool loop) {
  StopAllLoops();
  StopAllMusic();

  game_over_->start();
  loop_game_over_ = loop;
}

void SoundManager::StartMainMenuMusic(bool loop) {
  StopAllLoops();
  StopAllMusic();

  main_menu_->start();
  loop_main_menu_ = loop;
}

void SoundManager::StartGameMusic(bool loop) {
  StopAllLoops();
  StopAllMusic();

  game_->start();
  loop_game_ = loop;
}

void SoundManager::StartDebrisDestroyed() {
  debris_destroy_->stop();
  debris_destroy_->start();
}

void SoundManager::StartBulletShot() {
  bullet_shot_->stop();
  bullet_shot_->start();
}

void SoundManager::StartNearMiss() {
  near_miss_->stop();
  near_miss_->start();
}

void SoundManager::CheckLoops() {

  // Checking main menu music loop.
  if (loop_main_menu_ && !main_menu_->isPlaying()) {
    StartMainMenuMusic(loop_main_menu_);

  // Checking the game over music loop.
  } else if (loop_game_over_ && !game_over_->isPlaying()) {
    StartGameOverMusic(loop_game_over_);

  // Checking the in-game music loop.
  } else if (loop_game_ && !game_->isPlaying()) {
    StartGameMusic(loop_game_);
  }
}

//==============================================================================
// Private Functions
//==============================================================================

void SoundManager::LoadSounds() {

  // In-game music loading.
  cinder::audio::SourceFileRef game_music = cinder::audio::load
      (cinder::app::loadAsset(kGameMusicPath));
  game_ = cinder::audio::Voice::create(game_music);

  // Game over music loading.
  cinder::audio::SourceFileRef game_over_music = cinder::audio::load
      (cinder::app::loadAsset(kGameOverMusicPath));
  game_over_ = cinder::audio::Voice::create(game_over_music);

  // Menu music loading.
  cinder::audio::SourceFileRef menu_music = cinder::audio::load
      (cinder::app::loadAsset(kMenuMusicPath));
  main_menu_ = cinder::audio::Voice::create(menu_music);

  // Debris destroyed sound effect loading.
  cinder::audio::SourceFileRef debris_sound = cinder::audio::load
      (cinder::app::loadAsset(kDebrisSoundPath));
  debris_destroy_ = cinder::audio::Voice::create(debris_sound);

  // Throwing sound loading.
  cinder::audio::SourceFileRef throw_sound = cinder::audio::load
      (cinder::app::loadAsset(kThrowSoundPath));
  bullet_shot_ = cinder::audio::Voice::create(throw_sound);

  // Near miss sound loading.
  cinder::audio::SourceFileRef near_miss_sound = cinder::audio::load
      (cinder::app::loadAsset(kNearMissSoundPath));
  near_miss_ = cinder::audio::Voice::create(near_miss_sound);
}

void SoundManager::StopAllLoops() {
  loop_game_over_ = false;
  loop_game_ = false;
  loop_main_menu_ = false;
}

void SoundManager::StopAllMusic() {
  game_over_->stop();
  game_->stop();
  main_menu_->stop();
}

void SoundManager::StopAllSoundEffects() {
  debris_destroy_->stop();
  bullet_shot_->stop();
  near_miss_->stop();
}

}