//
// Created by Aidan on 5/1/2020.
//

#ifndef FINALPROJECT_SOUND_MANAGER_H
#define FINALPROJECT_SOUND_MANAGER_H

#include <cinder/audio/audio.h>
#include <cinder/audio/Voice.h>
#include <cinder/app/App.h>

namespace mylibrary {

const std::string kGameMusicPath = "pulse.mp3";
const std::string kGameOverMusicPath = "mimiga_town.mp3";
const std::string kMenuMusicPath = "menu.mp3";
const std::string kDebrisSoundPath = "debris.wav";
const std::string kThrowSoundPath = "throw.mp3";
const std::string kNearMissSoundPath = "ding.wav";

class SoundManager {
public:

  // Default sound manager constructor.
  SoundManager();

  // Sound manager constructor that defines whether or not assets are to be
  // loaded.
  SoundManager(bool load_assets);

  // Sets the master volume. Is a value between 0.0f and 1.0f.
  void SetVolume(float volume);

  // Stops all the sounds, including the music.
  void StopAll();

  // Starts the game over music.
  void StartGameOverMusic(bool loop);

  // Starts the main menu music.
  void StartMainMenuMusic(bool loop);

  // Starts the game music.
  void StartGameMusic(bool loop);

  // Starts the debris destroy sound.
  void StartDebrisDestroyed();

  // Starts the bullet/sword shot sound.
  void StartBulletShot();

  // Starts the near miss sound.
  void StartNearMiss();

  // Checks whether or not any sounds need to be looped.
  void CheckLoops();

private:

  // Loads in all of the sounds from the files.
  void LoadSounds();

  // Sets all the music parameters to false.
  void StopAllLoops();

  // Stops all the music.
  void StopAllMusic();

  // Stops all the sound effects.
  void StopAllSoundEffects();

  float volume_ = 0.1f;

  cinder::audio::VoiceRef game_over_;
  bool loop_game_over_;

  cinder::audio::VoiceRef main_menu_;
  bool loop_main_menu_;

  cinder::audio::VoiceRef game_;
  bool loop_game_;

  cinder::audio::VoiceRef debris_destroy_;
  cinder::audio::VoiceRef bullet_shot_;
  cinder::audio::VoiceRef near_miss_;

};

}

#endif //FINALPROJECT_SOUND_MANAGER_H
