#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <filesystem>
#include <string>
#include <vector>

#include "PlaylistDLL.h"
#include "SongTrie.h"

class AudioEngine;

class GUI {
public:
  GUI(AudioEngine *engine);
  ~GUI();

  void Render();
  void LoadPlaylist(const std::string &directoryPath);
  void AddFileToPlaylist(const std::string &filepath);

private:
  AudioEngine *audioEngine;

  // Custom Data Structures
  PlaylistDLL playlist;
  SongTrie searchTrie;

  // Search State
  char searchBuffer[128] = "";
  std::vector<std::string> searchResults;
  bool isSearching = false;

  std::string currentTrackName = "No track playing";

  void RenderPlayerControls();
  void RenderPlaylist();
  std::string GetFileName(const std::string &path);
};
