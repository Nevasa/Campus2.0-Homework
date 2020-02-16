#pragma once

#include <cstdint>

constexpr uint16_t PORT = 27015;
constexpr int BUFFER_SIZE = 256;

bool Initialize();
bool Deinitialize();