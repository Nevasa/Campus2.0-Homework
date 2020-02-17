#pragma once

#include <cstdint>
#include <string>

constexpr uint16_t PORT = 12121;
constexpr int BUFFER_SIZE = 256;

const std::string EXIT_MESSAGE = "q";

bool Initialize();
bool Deinitialize();