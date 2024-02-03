#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
#include <fstream>
#include <sstream>
#include <format>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border

#include "json.h"

#define A_MILLION 1000000
#define JOULE_TO_CO2 0.0000001158426d
#define JOULE_TO_TREE_COST 0.000000002316852d
#define JOULE_TO_EIFFEL_DAYS 0.000000000015d

using namespace ftxui;

Component Text(const std::string& t) {
  return Renderer([t] { return text(t); });
}
Component Paragraph(const std::string& t) {
  return Renderer([t] { return paragraph(t); });
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
    return Text(func_id);
  }
  
  return main_collapsible;
}

int main() {
  std::ifstream file("./test2.json");

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
  uint64_t total_mj_usage = (long) result["energy"];
  double total_j_usage = total_mj_usage / 1000;
  double kgCO2 = total_j_usage * JOULE_TO_CO2;
  double dollarTrees = total_j_usage * JOULE_TO_TREE_COST;
  double eiffelDays = total_j_usage * JOULE_TO_EIFFEL_DAYS;

  auto screen = ScreenInteractive::Fullscreen();

  auto bottom_panel = Container::Horizontal({});
  auto eval_trace = Container::Vertical({});
  auto eval_trace_below = Container::Vertical({});

  int func_selected = 0;
  
  

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
    bottom_panel->Add(eval_trace | flex | size(WIDTH, EQUAL, 70));
    eval_trace->Add(Text("Evaluation Trace") | center);
    eval_trace->Add(tab_container | frame | vscroll_indicator | flex);
    eval_trace->Add(eval_trace_below);
  }


  auto info_container = Container::Vertical({});
  auto info_container_wrapper = Container::Horizontal({});
  auto info_container1 = Container::Vertical({});
  auto info_container2 = Container::Vertical({});

  std::vector<json::jobject> children = (std::vector<json::jobject>)result["worst"];
  
  {
    bottom_panel->Add(Separator());
    info_container->Add(Text("Hot Spots") | center);
    size_t counter = 0;
    // for (size_t i = 0; i < children.size() / 2; i++) {
    int i = 0;
      auto row = Container::Horizontal({});
      auto cell = Container::Vertical({});
      auto cell1 = Container::Vertical({});

      cell->Add(Text(children[i]["ident"]) | bold | underlined);
        
      
      auto ss = std::stringstream{(std::string) children[i]["disas"]};

      for (std::string line; std::getline(ss, line, '\n');)
        cell->Add(Paragraph(line) | frame);
      
      row->Add(cell  | frame | border | flex | size(WIDTH, EQUAL, 80));
      info_container1->Add(row);
      counter++;      

      for (auto thing : children[i]["instrs"]) {
        auto row = Container::Horizontal({});        
        row->Add(Text(std::to_string(thing[key])) | bold);
        row->Add(Text(std::to_string(val)));
        info_container2->Add(row);
      }

      
      auto ss = std::stringstream{children[i]["disas"]};

      for (std::string line; std::getline(ss, line, '\n');)
        cell->Add(Paragraph(line) | frame);
      
    //   row->Add(cell  | frame | border | flex | size(WIDTH, EQUAL, 80));
    //   info_container2->Add(row);
    // }
    
  info_container_wrapper->Add(info_container1 | flex | frame | vscroll_indicator);
  info_container_wrapper->Add(info_container2 | flex | frame | vscroll_indicator);
  info_container->Add(info_container_wrapper);
  bottom_panel->Add(info_container);
  

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
  auto line3 = Renderer([&color3] { return text("     %:::::-%++++++*==#%           _____.__                _____.__         ") | color(color3) ; });  
  auto line4 = Renderer([&color4] { return text("     %:::%******%++%%=:          _/ ____\\__|______   _____/ ____\\  | ___.__.") | color(color4) ; });  
  auto line5 = Renderer([&color5] { return text("       %*********%*+:::          \\   __\\|  \\_  __ \\_/ __ \\   __\\|  |<   |  |") | color(color5) ; });  
  auto line6 = Renderer([&color6] { return text("       %*********%:::::           |  |  |  ||  | \\/\\  ___/|  |  |  |_\\___  |") | color(color6) ; });  
  auto line7 = Renderer([&color7] { return text("      %********%::::::            |__|  |__||__|    \\___  >__|  |____/ ____|") | color(color7) ; });  
  auto line8 = Renderer([&color8] { return text("      *%%%%%%%*  %%%                                    \\/           \\/     ") | color(color8) ; }); 
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
  
  char wouldrelease[150];
  char powereiffel[150];
  char owemoney[150];
  char totalmjline[150];
  char totalkgco2[150];

  snprintf(wouldrelease, 150, "             it would release %lf kg of CO2.                      ", kgCO2 * A_MILLION);
  snprintf(powereiffel, 150, "             It could power the Eiffel tower for %lf days.           ", eiffelDays * A_MILLION);
  snprintf(owemoney, 150, "             You would owe $%lf in carbon taxes.                   ", dollarTrees * A_MILLION);
  snprintf(totalmjline, 150, "             Total joules used (per run): %ld mJ                       ", (long) total_mj_usage);
  snprintf(totalkgco2, 150, "             Total kg CO2 released (per run): %lf kg                 ", kgCO2);

  auto bignumbers = Container::Vertical({
    Renderer([&firecolor]      { return text("                                                                     ") | color(firecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                     ") | color(firecolor) | center ; }),
    Renderer([&desatfirecolor] { return text("             If your program run on 1 000 000 computers,             ") | color(desatfirecolor) | center; }),
    Renderer([&firecolor, &wouldrelease]      { return text(wouldrelease) | color(firecolor) | bold | center; }),
    Renderer([&firecolor, &powereiffel]      { return text(powereiffel) | color(firecolor) | bold | center; }),
    Renderer([&firecolor, &owemoney]      { return text(owemoney) | color(firecolor) | bold | center; }),
    Renderer([&firecolor]      { return text("                                                                     ") | color(firecolor) | center ; }),
    Renderer([&desatfirecolor, &totalmjline] { return text(totalmjline) | color(desatfirecolor) | center ; }),
    Renderer([&desatfirecolor, &totalkgco2] { return text(totalkgco2) | color(desatfirecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                     ") | color(firecolor) | center ; }),
    Renderer([&firecolor]      { return text("                                                                     ") | color(firecolor) | center ; })
  });

  auto flame1 = Color::RGB(255, 255, 31);
  auto flame2 = Color::RGB(249, 213, 0);
  auto flame3 = Color::RGB(238, 173, 0);
  auto flame4 = Color::RGB(222, 133, 0);
  auto flame5 = Color::RGB(201, 95, 0);
  auto flame6 = Color::RGB(176, 56, 0);
  auto flame7 = Color::RGB(147, 1, 1);
  auto flame8 = Color::RGB(127, 1, 1);

  auto flames = Container::Vertical({
    Renderer([&flame1]      { return text(" ") | color(flame1) | center ; }),
    Renderer([&flame1]      { return text("              (  .      )                     ") | color(flame1) | center ; }),
    Renderer([&flame2]      { return text("          )           (              )        ") | color(flame2) | center ; }),
    Renderer([&flame3] { return text("                .  '   .   '  .  '  .         ") | color(flame3) | center; }),
    Renderer([&flame4]      { return text("       (    , )       (.   )  (   ',    )     ") | color(flame4) | center; }),
    Renderer([&flame5]      { return text("        .' ) ( . )    ,  ( ,     )   ( .      ") | color(flame5) | center; }),
    Renderer([&flame6]      { return text("     ). , ( .   (  ) ( , ')  .' (  ,    )     ") | color(flame6) | center; }),
    Renderer([&flame7]      { return text("    (_,) . ), ) _) _,')  (, ) '. )  ,. (' )   ") | color(flame7) | center ; }),
    Renderer([&flame8] { return text("jgs^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ") | color(flame8) | center ; }),
    Renderer([&flame8] { return text(" ") | color(flame8) | center ; })
  });

  auto top = Container::Horizontal({logo, bignumbers, flames});

  int left_size = 50;
  int right_size = 50;
  int top_size = 11;
  
  auto container = bottom_panel;
  container = ResizableSplitTop(top, container, &top_size);
  
  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
 
  screen.Loop(renderer);
}
