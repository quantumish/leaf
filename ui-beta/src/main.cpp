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

  auto color1 = Color::RGB(250, 250, 110);
  auto color2 = Color::RGB(196, 236, 116);
  auto color3 = Color::RGB(146, 220, 126);
  auto color4 = Color::RGB(100, 201, 135);
  auto color5 = Color::RGB(57, 180, 142);
  auto color6 = Color::RGB(8, 159, 143);
  auto color7 = Color::RGB(0, 137, 138);
  auto color8 = Color::RGB(8, 115, 127);
  auto color9 = Color::RGB(33, 93, 110);
  
  auto linet = Renderer([&color9] { return text("                              ") | color(color9) ; }); 
  auto line1 = Renderer([&color1] { return text("                  ====        ") | color(color1) ; });  
  auto line2 = Renderer([&color2] { return text("          -:::::%=%%%==       ") | color(color2) ; });  
  auto line3 = Renderer([&color3] { return text("     %:::::-%++++++*==#%             _____.__                _____.__         ") | color(color3) ; });  
  auto line4 = Renderer([&color4] { return text("     %:::%******%++%%=:            _/ ____\\__|______   _____/ ____\\  | ___.__.") | color(color4) ; });  
  auto line5 = Renderer([&color5] { return text("       %*********%*+:::            \\   __\\|  \\_  __ \\_/ __ \\   __\\|  |<   |  |") | color(color5) ; });  
  auto line6 = Renderer([&color6] { return text("       %*********%:::::             |  |  |  ||  | \\/\\  ___/|  |  |  |_\\___  |") | color(color6) ; });  
  auto line7 = Renderer([&color7] { return text("      %********%::::::              |__|  |__||__|    \\___  >__|  |____/ ____|") | color(color7) ; });  
  auto line8 = Renderer([&color8] { return text("      *%%%%%%%*  %%%                                      \\/           \\/     ") | color(color8) ; }); 
  auto line9 = Renderer([&color9] { return text("       %%%*%                  ") | color(color9) ; });
  auto lineb = Renderer([&color9] { return text("                              ") | color(color9) ; });  
  
  auto firecolor = Color::RGB(255, 77, 13);
  auto desatfirecolor = Color::RGB(173, 62, 23);

  auto logo = Container::Vertical({
      linet,
      line1,
      line2,
      line3,
      line4,
      line5,
      line6,
      line7,
      line8,
      line9,
      lineb
  });
  
  auto bignumbers = Container::Vertical({
    Renderer([&firecolor]      { return text("                                                                              ") | color(firecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                              ") | color(firecolor) | center ; }),
    Renderer([&desatfirecolor] { return text("                  If your program run on 1 000 000 computers,                 ") | color(desatfirecolor) | center; }),
    Renderer([&firecolor]      { return text("                  it would release 123456 kg of CO2.                          ") | color(firecolor) | bold | center; }),
    Renderer([&firecolor]      { return text("                  It could power the Eiffel tower for 100 days.               ") | color(firecolor) | bold | center; }),
    Renderer([&firecolor]      { return text("                  You would owe $69420 in carbon taxes.                       ") | color(firecolor) | bold | center; }),
    Renderer([&firecolor]      { return text("                                                                              ") | color(firecolor) | center ; }),
    Renderer([&desatfirecolor] { return text("                  Total joules used (per run): 10 J                           ") | color(desatfirecolor) | center ; }),
    Renderer([&desatfirecolor] { return text("                  Total kg CO2 released (per run): 0.1 kg                     ") | color(desatfirecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                              ") | color(firecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                              ") | color(firecolor) | center ; })
  });
  
  auto top = Container::Horizontal({logo, bignumbers});

  int left_size = 50;
  int right_size = 50;
  int top_size = 11;
  
  auto container = bottom_panel;
  container = ResizableSplitTop(top, container, &top_size);
  
  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
 
  screen.Loop(renderer);
}