#include "GUI.h"
#include "AudioEngine.h"
#include <iostream>

namespace fs = std::filesystem;

GUI::GUI(AudioEngine *engine) : audioEngine(engine) {}

GUI::~GUI() {}

void GUI::LoadPlaylist(const std::string &directoryPath) {
  // playlist.clear(); // Don't clear, append instead
  if (!fs::exists(directoryPath))
    return;
  try {
    for (const auto &entry : fs::directory_iterator(directoryPath)) {
      AddFileToPlaylist(entry.path().string());
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading playlist: " << e.what() << std::endl;
  }
}

void GUI::AddFileToPlaylist(const std::string &filepath) {
  if (filepath.substr(filepath.find_last_of(".") + 1) == "mp3" ||
      filepath.substr(filepath.find_last_of(".") + 1) == "wav" ||
      filepath.substr(filepath.find_last_of(".") + 1) == "MP3" ||
      filepath.substr(filepath.find_last_of(".") + 1) == "WAV") {

    // Check for duplicates
    for (const auto &track : playlist) {
      if (track == filepath)
        return;
    }
    playlist.push_back(filepath);
  }
}

std::string GUI::GetFileName(const std::string &path) {
  return fs::path(path).filename().string();
}

void GUI::Render() {
  // Main Window
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Music Player", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);

  RenderPlayerControls();
  ImGui::Separator();
  RenderPlaylist();

  ImGui::End();
}

void GUI::RenderPlayerControls() {
  ImGui::Text("Now Playing: %s", currentTrackName.c_str());

  // Progress Bar
  float current = audioEngine->GetCurrentTimeInSeconds();
  float total = audioEngine->GetTotalTimeInSeconds();
  float progress = (total > 0.0f) ? (current / total) : 0.0f;

  char overlay[32];
  sprintf(overlay, "%.0f:%.0f / %.0f:%.0f", current / 60, fmod(current, 60),
          total / 60, fmod(total, 60));

  ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), overlay);

  // Controls
  if (ImGui::Button("<<")) {
    // Previous track logic
    if (currentTrackIndex > 0) {
      currentTrackIndex--;
      currentTrackName = GetFileName(playlist[currentTrackIndex]);
      audioEngine->PlayFile(playlist[currentTrackIndex]);
    }
  }
  ImGui::SameLine();

  if (audioEngine->IsPlaying()) {
    if (ImGui::Button("Pause")) {
      audioEngine->Pause();
    }
  } else {
    if (ImGui::Button("Play")) {
      if (currentTrackIndex >= 0) {
        audioEngine->Play();
      } else if (!playlist.empty()) {
        currentTrackIndex = 0;
        currentTrackName = GetFileName(playlist[currentTrackIndex]);
        audioEngine->PlayFile(playlist[currentTrackIndex]);
      }
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Stop")) {
    audioEngine->Stop();
  }

  ImGui::SameLine();
  if (ImGui::Button(">>")) {
    // Next track logic
    if (currentTrackIndex < (int)playlist.size() - 1) {
      currentTrackIndex++;
      currentTrackName = GetFileName(playlist[currentTrackIndex]);
      audioEngine->PlayFile(playlist[currentTrackIndex]);
    }
  }

  // Volume
  float vol = audioEngine->GetVolume();
  if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f)) {
    audioEngine->SetVolume(vol);
  }
}

void GUI::RenderPlaylist() {
  ImGui::Text("Playlist");
  ImGui::BeginChild("PlaylistRegion", ImVec2(0, -1.0f), true);

  for (int i = 0; i < playlist.size(); i++) {
    std::string name = GetFileName(playlist[i]);
    bool isSelected = (currentTrackIndex == i);

    if (ImGui::Selectable(name.c_str(), isSelected)) {
      currentTrackIndex = i;
      currentTrackName = name;
      audioEngine->PlayFile(playlist[i]);
    }

    if (isSelected) {
      ImGui::SetItemDefaultFocus();
    }
  }

  ImGui::EndChild();
}
