#ifndef T_COLOR_H
#define T_COLOR_H

/*!
 * Color code:
 * https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
 *
 * How to use this function:
 * ```c++
 *      std::cout << "Esse texto aparece normal, "
 *                << TColor::colorize("texto em vermelho e negrito",
 *                   TColor::RED, TColor::BOLD)
 *                << "\n";
 * ```
 *
 * ```c++
 *  #include <iostream>
 *  #include "tcolor.h"
 *
 *  int main() {
 *      // Imprimie uma frase com todas as cores possíveis
 *      for(int i{0} ; i < TColor::color_len; ++i ){
 *          std::cout << TColor::colorize("Palavra colorida",
 *                       TColor::color_list[i] ) << '\n';
 *      }
 *
 *      return 0;
 *  }
 * ```
 */
#include <sstream>
using std::ostringstream;
using std::wostringstream;
#include <string>
using std::string;
using std::wstring;
#include <array>
using std::array;

namespace TColor {
// Alias
using value_type = short;

/// Color attribute - colors
static constexpr short BLACK{30};
static constexpr short RED{31};
static constexpr short GREEN{32};
static constexpr short YELLOW{33};
static constexpr short BLUE{34};
static constexpr short MAGENTA{35};
static constexpr short CYAN{36};
static constexpr short WHITE{37};
static constexpr short BRIGHT_BLACK{90};
static constexpr short BRIGHT_RED{91};
static constexpr short BRIGHT_GREEN{92};
static constexpr short BRIGHT_YELLOW{93};
static constexpr short BRIGHT_BLUE{94};
static constexpr short BRIGHT_MAGENTA{95};
static constexpr short BRIGHT_CYAN{96};
static constexpr short BRIGHT_WHITE{97};
/// Color attributes - modificators.
static constexpr short REGULAR{0};
static constexpr short BOLD{1};
static constexpr short UNDERLINE{4};
static constexpr short BLINK{5};
static constexpr short REVERSE{7};

static constexpr short colors_length{14};
/// List of colors. You may which to change color ordering.
static constexpr array<value_type, colors_length> color_list{
    31, 32, 33, 34, 35, 36, 37, 91, 92, 93, 94, 95, 96, 97};

/// Returns a string with a colored message.
/*!
 * @param msg Message to display.
 * @param color Color code to apply to the message.
 * @param modifier Modifier code to apply to the message.
 * @return A string with the embedded color/modifier escape codes.
 */
inline string colorize(std::string_view msg, short color = TColor::WHITE,
                       short modifier = TColor::REGULAR) {
  ostringstream oss;
  oss << "\33[" << modifier << ";" << color << "m" << msg << "\33[0m";
  return oss.str();
}

/// Wide char version
inline wstring w_colorize(const wstring &msg, short color = TColor::WHITE,
                          short modifier = TColor::REGULAR) {
  wostringstream oss;
  oss << L"\33[" << modifier << L";" << color << L"m" << msg << L"\33[0m";
  return oss.str();
}
} // namespace TColor
#endif
