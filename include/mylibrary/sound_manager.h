//
// Created by Aidan on 5/1/2020.
//

#ifndef FINALPROJECT_SOUND_MANAGER_H
#define FINALPROJECT_SOUND_MANAGER_H

#include <cinder/audio/audio.h>
#include <cinder/audio/Voice.h>
#include <cinder/app/App.h>

namespace mylibrary {

// The Paths for all audio assets.
const std::string kGameMusicPath = "audio/pulse.mp3";
const std::string kGameOverMusicPath = "audio/mimiga_town.mp3";
const std::string kMenuMusicPath = "audio/menu.mp3";
const std::string kDebrisSoundPath = "audio/debris.wav";
const std::string kThrowSoundPath = "audio/throw.mp3";
const std::string kNearMissSoundPath = "audio/ding.wav";

// An object that manages multiple different audio sources. Makes sure that only
// one music track is being played once and can play various sound effects.
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

  // The volume.
  float volume_ = 0.1f;

  // The Voice reference for game over music and its loop variable.
  cinder::audio::VoiceRef game_over_;
  bool loop_game_over_;

  // The Voice reference for the main menu music and its loop variable.
  cinder::audio::VoiceRef main_menu_;
  bool loop_main_menu_;

  // The Voice reference for game music and its loop variable.
  cinder::audio::VoiceRef game_;
  bool loop_game_;

  // Voice references for all sound effects.
  cinder::audio::VoiceRef debris_destroy_;
  cinder::audio::VoiceRef bullet_shot_;
  cinder::audio::VoiceRef near_miss_;

};

}

#endif //FINALPROJECT_SOUND_MANAGER_H
