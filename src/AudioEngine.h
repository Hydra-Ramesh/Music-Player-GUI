#pragma once

#include "miniaudio.h"
#include <iostream>
#include <string>
#include <vector>

class AudioEngine {
public:
  AudioEngine();
  ~AudioEngine();

  bool Init();
  void Uninit();
  void PlayFile(const std::string &filepath);
  void Play();
  void Pause();
  void Stop();
  void SetVolume(float volume);
  float GetVolume() const;

  bool IsPlaying() const;
  unsigned long long GetCurrentFrame();
  unsigned long long GetTotalFrames();
  float GetCurrentTimeInSeconds();
  float GetTotalTimeInSeconds();
  void SeekToTime(float seconds);

private:
  ma_engine engine;
  ma_sound sound;
  bool isInitialized = false;
  bool isSoundLoaded = false;
  float currentVolume = 0.8f;
};
