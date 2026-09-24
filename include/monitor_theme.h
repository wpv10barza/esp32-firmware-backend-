#pragma once

#include <cstdint>

namespace monitor_theme {

// Visual design tokens adapted to the source Guition monitor model:
// dark dashboard surfaces, misty-blue secondary text, green primary action,
// and compact rounded status cards suited to a 480x480 panel.
struct Rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

constexpr Rgb background{8, 14, 24};
constexpr Rgb surface{18, 30, 46};
constexpr Rgb surfaceRaised{25, 43, 62};
constexpr Rgb textPrimary{245, 250, 255};
constexpr Rgb textSecondary{157, 180, 192};
constexpr Rgb accentBlue{58, 126, 188};
constexpr Rgb accentGreen{44, 165, 112};
constexpr Rgb accentAmber{210, 150, 50};
constexpr Rgb accentRed{195, 72, 78};
constexpr Rgb outline{61, 90, 108};
constexpr Rgb disabled{61, 90, 108};
constexpr Rgb warningSurface{64, 49, 20};
constexpr Rgb errorSurface{66, 24, 29};

}  // namespace monitor_theme
