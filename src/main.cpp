#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

ftxui::MenuEntryOption Colored(ftxui::Color c);

int main() {
  ftxui::Color aqua = ftxui::Color::RGB(69, 133, 136);
  ftxui::Color red = ftxui::Color::RGB(204, 36, 29);
  ftxui::Color yellow = ftxui::Color::RGB(215, 153, 33);
  ftxui::Color green = ftxui::Color::RGB(152, 151, 26);
  ftxui::Color pink = ftxui::Color::RGB(177, 98, 134);

  auto screen = ftxui::ScreenInteractive::Fullscreen();

  int selected = 0;
  auto menu = ftxui::Container::Vertical(
      {
          ftxui::MenuEntry("Poweroff", Colored(red)),
          ftxui::MenuEntry("Reboot", Colored(aqua)),
          ftxui::MenuEntry("Suspend", Colored(green)),
          ftxui::MenuEntry("Logout", Colored(pink)),
      },
      &selected);

  auto render = ftxui::Renderer(menu, [&] {
    return ftxui::vbox({
        ftxui::hbox(ftxui::text("POWER MENU")) | ftxui::center | ftxui::bold |
            ftxui::color(yellow),
        ftxui::separator(),
        menu->Render() | ftxui::frame,
    });
  });

  render = ftxui::CatchEvent(render, [&](ftxui::Event event) {
    if (event == ftxui::Event::Escape) {
      screen.ExitLoopClosure()();
      return true;
    }

    if (event == ftxui::Event::Return) {
      screen.ExitLoopClosure()();
      switch (selected) {
      case 0:
        system("systemctl poweroff");
        break;
      case 1:
        system("systemctl reboot");
        break;
      case 2:
        system("systemctl suspend");
        break;
      case 3:
        system("killall niri");
        break;
      }
      return true;
    }
    return false;
  });

  screen.Loop(render);
  return 0;
}

ftxui::MenuEntryOption Colored(ftxui::Color c) {
  ftxui::MenuEntryOption option;
  option.animated_colors.foreground.enabled = true;
  option.animated_colors.background.enabled = true;
  option.animated_colors.background.active = c;
  option.animated_colors.foreground.active = ftxui::Color::RGB(235, 219, 178);
  option.animated_colors.foreground.inactive = c;
  return option;
}
