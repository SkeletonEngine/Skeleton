#include "docs.h"

#include <fstream>
#include <sstream>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace Skeleton {
namespace Ui {

bool gDocsWindowOpen = false;

static std::string buf;

void LoadDocs() {
  std::ifstream in_file("res/docs/todo.txt");
  if (!in_file.is_open()) {
    fprintf(stderr, "Failed to open todo file\n");
  }
  std::stringstream buffer;
  buffer << in_file.rdbuf();
  buf = buffer.str();
}

static void SaveDocs() {
  std::ofstream out_file("res/docs/todo.txt");
  out_file << buf;
}

void DrawDocs() {
  if (!gDocsWindowOpen) return;
  
  ImGui::Begin("Docs", &gDocsWindowOpen);
  if (ImGui::InputTextMultiline("##Notes", &buf, ImGui::GetContentRegionAvail())) {
    SaveDocs();
  }
  ImGui::End();
}

}
}
