#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
#include <fstream>
#include <sstream>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border

#include "json.h"
 

using namespace ftxui;



Component Text(const std::string& t) {
  return Renderer([t] { return text(t) | borderEmpty; });
}

Component Separator() {
  return Renderer([] { return separator() | borderEmpty; });
}


Component Inner(std::vector<Component> children) {
  Component vlist = Container::Vertical(std::move(children));
  return Renderer(vlist, [vlist] {
    return hbox({
        text(" "),
        vlist->Render(),
    });
  });
}
 
Component Empty() {
  return std::make_shared<ComponentBase>();
}

Component make_collapse(json::jobject energy_profile) {

  std::vector<Component> child_collapsibles;
  std::vector<json::jobject> children = (std::vector<json::jobject>)energy_profile["children"];
  size_t num_children = children.size();

  for (size_t i = 0; i < num_children; i++) {
    child_collapsibles.push_back(make_collapse(children[i]));
  }

  std::string func_id = (std::string) energy_profile["ident"] + " - " + (std::string) energy_profile["energy"] + " mJ";


  if (num_children == 0) {
    return Collapsible(func_id,
                  Empty());
  }
  
  return Collapsible(func_id,
                  Inner(child_collapsibles));
}

int main() {
  std::ifstream file("test.json");

  // Check if the file is opened successfully
  if (!file.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1;
  }

  // Read the entire file content into a string
  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string fileContent = buffer.str();

  // Close the file
  file.close();

  json::jobject result = json::jobject::parse(fileContent);

  auto screen = ScreenInteractive::Fullscreen();

  auto bottom_panel = Container::Horizontal({});

  auto component = make_collapse(result);
 
  {
    bottom_panel->Add(component | flex);
  }

  {
    bottom_panel->Add(Separator());
    bottom_panel->Add(Text("Cool info") | center | flex);
  }

  auto left = Renderer([] { return text("Left") | center; });
  auto top = Renderer([] { return text("Save energy, save the planet! -Ben Song") | center; });
 
  int left_size = 50;
  int right_size = 50;
  int top_size = 10;

  auto container = bottom_panel;
  container = ResizableSplitTop(top, container, &top_size);
 
  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
 
  screen.Loop(renderer);
}