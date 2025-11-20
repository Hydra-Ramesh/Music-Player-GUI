#define MINIAUDIO_IMPLEMENTATION
#include "AudioEngine.h"

AudioEngine::AudioEngine() {}

AudioEngine::~AudioEngine() { Uninit(); }

bool AudioEngine::Init() {
  // Reverting to default configuration to fix assertion failure
  ma_result result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    std::cerr << "Failed to initialize audio engine." << std::endl;
    return false;
  }
  isInitialized = true;
  SetVolume(currentVolume);
  return true;
}

void AudioEngine::Uninit() {
  if (isSoundLoaded) {
    ma_sound_uninit(&sound);
    isSoundLoaded = false;
  }
  if (isInitialized) {
    ma_engine_uninit(&engine);
    isInitialized = false;
  }
}

void AudioEngine::PlayFile(const std::string &filepath) {
  if (!isInitialized)
    return;

  if (isSoundLoaded) {
    ma_sound_uninit(&sound);
  }

  // Try to decode the whole file to avoid streaming issues with some MP3s
  ma_result result = ma_sound_init_from_file(
      &engine, filepath.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, &sound);
  if (result != MA_SUCCESS) {
    std::cerr << "Failed to load sound file: " << filepath << std::endl;
    return;
  }

  isSoundLoaded = true;
  ma_sound_start(&sound);
}

void AudioEngine::Play() {
  if (isSoundLoaded) {
    ma_sound_start(&sound);
  }
}

void AudioEngine::Pause() {
  if (isSoundLoaded) {
    ma_sound_stop(&sound);
  }
}

void AudioEngine::Stop() {
  if (isSoundLoaded) {
    ma_sound_stop(&sound);
    ma_sound_seek_to_pcm_frame(&sound, 0);
  }
}

void AudioEngine::SetVolume(float volume) {
  currentVolume = volume;
  if (isInitialized) {
    ma_engine_set_volume(&engine, volume);
  }
}

float AudioEngine::GetVolume() const { return currentVolume; }

bool AudioEngine::IsPlaying() const {
  if (!isSoundLoaded)
    return false;
  return ma_sound_is_playing(&sound);
}

unsigned long long AudioEngine::GetCurrentFrame() {
  if (!isSoundLoaded)
    return 0;
  return ma_sound_get_time_in_pcm_frames(&sound);
}

unsigned long long AudioEngine::GetTotalFrames() {
  if (!isSoundLoaded)
    return 0;
  ma_uint64 length;
  ma_sound_get_length_in_pcm_frames(&sound, &length);
  return length;
}

float AudioEngine::GetCurrentTimeInSeconds() {
  if (!isSoundLoaded)
    return 0.0f;
  // Assuming 48000 sample rate for simplicity or query it if needed,
  // but miniaudio handles time conversions usually.
  // Better way:
  float cursor;
  ma_sound_get_cursor_in_seconds(&sound, &cursor);
  return cursor;
}

float AudioEngine::GetTotalTimeInSeconds() {
  if (!isSoundLoaded)
    return 0.0f;
  float length;
  ma_sound_get_length_in_seconds(&sound, &length);
  return length;
}

void AudioEngine::SeekToTime(float seconds) {
  if (isSoundLoaded) {
    ma_uint32 sampleRate = ma_engine_get_sample_rate(&engine);
    if (sampleRate == 0)
      sampleRate = 48000; // Fallback
    ma_sound_seek_to_pcm_frame(&sound, (ma_uint64)(seconds * sampleRate));
  }
}
