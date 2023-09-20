# pragma once

constexpr int MAX_MEM = 4;  // in GBs

constexpr float FAR_CLIP = 1E8F;  // Far clip
constexpr float EPSILON  = 1E-8F;    // Near clip

const float FOV = 32.f;
constexpr int W = 860;
constexpr int H = 480;
constexpr float AR = (float)W/H;  // Aspect Ratio of image

constexpr int SAMPLES = 2;        // Min: 1    Max: as req
constexpr int RAY_BOUNCES = 2;    // Min: 1    Max: as req(Stack Limit)
constexpr int AA_SAMPLES = 2;     // Antialiasing samples
constexpr int UPDATE_FREQ = 20;  // in % of H

// Flags
constexpr bool GI = true;         // Global illumination
constexpr bool SHADOWS = true;     // Shadows
