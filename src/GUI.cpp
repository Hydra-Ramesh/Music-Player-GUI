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

    // Check for duplicates (O(N) - could be O(1) with a HashSet, but DLL is
    // O(N))
    Node *temp = playlist.GetHead();
    while (temp) {
      if (temp->filepath == filepath)
        return;
      temp = temp->next;
    }

    playlist.Add(filepath);
    searchTrie.Insert(GetFileName(filepath), filepath);
  }
}

std::string GUI::GetFileName(const std::string &path) {
  return fs::path(path).filename().string();
}

void GUI::Render() {
  // Main Window
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Music Player by @Code&Cofee", nullptr,
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
  snprintf(overlay, sizeof(overlay), "%.0f:%.0f / %.0f:%.0f", current / 60,
           fmod(current, 60), total / 60, fmod(total, 60));

  ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), overlay);

  // Controls
  if (ImGui::Button("<<")) {
    Node *prev = playlist.Prev();
    if (prev) {
      currentTrackName = prev->filename;
      audioEngine->PlayFile(prev->filepath);
    }
  }
  ImGui::SameLine();

  if (audioEngine->IsPlaying()) {
    if (ImGui::Button("Pause")) {
      audioEngine->Pause();
    }
  } else {
    if (ImGui::Button("Play")) {
      if (playlist.GetCurrent()) {
        audioEngine->Play();
      } else if (!playlist.IsEmpty()) {
        playlist.SetCurrent(playlist.GetHead());
        currentTrackName = playlist.GetHead()->filename;
        audioEngine->PlayFile(playlist.GetHead()->filepath);
      }
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Stop")) {
    audioEngine->Stop();
  }

  ImGui::SameLine();
  if (ImGui::Button(">>")) {
    Node *next = playlist.Next();
    if (next) {
      currentTrackName = next->filename;
      audioEngine->PlayFile(next->filepath);
    }
  }

  // Volume
  float vol = audioEngine->GetVolume();
  if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f)) {
    audioEngine->SetVolume(vol);
  }
}

void GUI::RenderPlaylist() {
  ImGui::Text("Playlist (DSA Optimized)");

  // Search Bar
  if (ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer))) {
    std::string query = searchBuffer;
    if (query.empty()) {
      isSearching = false;
    } else {
      isSearching = true;
      searchResults = searchTrie.Search(query);
    }
  }

  ImGui::BeginChild("PlaylistRegion", ImVec2(0, -1.0f), true);

  if (isSearching) {
    for (const auto &path : searchResults) {
      std::string name = GetFileName(path);
      if (ImGui::Selectable(name.c_str())) {
        // Find node in DLL to set current (O(N) search, but acceptable for UI
        // click)
        Node *temp = playlist.GetHead();
        while (temp) {
          if (temp->filepath == path) {
            playlist.SetCurrent(temp);
            currentTrackName = temp->filename;
            audioEngine->PlayFile(temp->filepath);
            break;
          }
          temp = temp->next;
        }
      }
    }
  } else {
    Node *temp = playlist.GetHead();
    while (temp) {
      bool isSelected = (playlist.GetCurrent() == temp);
      if (ImGui::Selectable(temp->filename.c_str(), isSelected)) {
        playlist.SetCurrent(temp);
        currentTrackName = temp->filename;
        audioEngine->PlayFile(temp->filepath);
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
      temp = temp->next;
    }
  }

  ImGui::EndChild();
}
