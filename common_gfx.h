#pragma once

// Shift widths, to get the exact RGBA8888 channel
#define _R 24
#define _G 16
#define _B 8
#define _A 0

#define CHANNEL(rgba, c) ((rgba>>c)&255)