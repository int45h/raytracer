#pragma once

// Shift widths, to get the exact RGBA8888 channel
#define R 24
#define G 16
#define B 8
#define A 0

#define CHANNEL(rgba, c) ((rgba>>c)&255)