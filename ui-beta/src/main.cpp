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
  return Renderer([t] { return text(t); });
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

Component ModifiedCollapsible(ConstStringRef label, Component child, Ref<bool> show = false) {
  class Impl : public ComponentBase {
   public:
    Impl(ConstStringRef label, Component child, Ref<bool> show) : show_(show) {
      CheckboxOption opt;
      opt.transform = [](EntryState s) {            // NOLINT
        auto prefix = text(s.state ? "- " : "+ ");  // NOLINT
        auto t = text(s.label);
        if (s.active) {
          t |= bold;
        }
        if (s.focused) {
          t |= inverted;
        }
        return hbox({prefix, t});
      };

      Add(Container::Vertical({
          Checkbox(label, show_.operator->(), opt),
          Maybe(std::move(child), show_.operator->()),
      }));
    }
    Ref<bool> show_;
  };

  return Make<Impl>(std::move(label), std::move(child), show);
}

Component make_collapse(json::jobject energy_profile) {

  std::vector<Component> child_collapsibles;
  std::vector<json::jobject> children = (std::vector<json::jobject>)energy_profile["children"];
  size_t num_children = children.size();

  for (size_t i = 0; i < num_children; i++) {
    child_collapsibles.push_back(make_collapse(children[i]));
  }

  std::string func_id = (std::string) energy_profile["ident"] + " (" + (std::string) energy_profile["energy"] + " mJ)";

  Component main_collapsible = ModifiedCollapsible(func_id,
                  Inner(child_collapsibles));

  if (num_children == 0) {
    return Collapsible(func_id,
                  Empty());
  }
  
  return main_collapsible;
}

int main() {
  std::ifstream file("../src/test.json");

  // Check if the file is opened successfully
  if (!file.is_open()) {
      std::cerr << "Error opening JSON file!" << std::endl;
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
  auto eval_trace = Container::Vertical({});
  auto eval_trace_below = Container::Vertical({});

  int func_selected = 0;
  auto info_container = Text("Main text!");

  Component title_text = Text("Firefly™ - Environmental Profiler");
  auto thread_1_component = make_collapse(result);
  auto thread_2_component = make_collapse(result);
  
  std::vector<std::string> tab_values{
      "thread_1",
      "thread_2",
  };

  int tab_selected = 0;
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  auto tab_container = Container::Tab(
      {
          thread_1_component,
          thread_2_component
      },
      &tab_selected);
  
  {
    eval_trace_below->Add(tab_toggle);
    bottom_panel->Add(eval_trace | flex | size(WIDTH, EQUAL, 100));
    eval_trace->Add(Text("Evaluation Trace") | center);
    eval_trace->Add(tab_container | frame | vscroll_indicator | flex);
    eval_trace->Add(eval_trace_below);
  }

  {
    bottom_panel->Add(Separator());
    bottom_panel->Add(info_container | center | flex);
  }

  auto left = Renderer([] { return text("Left") | center; });
  auto top = Container::Vertical({});
  

  {
    top->Add(title_text | center);
  }
 
  int left_size = 50;
  int right_size = 50;
  int top_size = 10;

  auto container = bottom_panel;
  container = ResizableSplitTop(top, container, &top_size);
 
  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
 
  screen.Loop(renderer);
}