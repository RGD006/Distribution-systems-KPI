#pragma once
#include <windows.h>
#include <string.h>
#include <cstdint>

struct DINFO
{
  uint32_t numberLines;
  uint32_t numberWords;
  uint32_t numberCharacters;
};

extern DINFO dataInfo;

DWORD WINAPI ReadFile(LPVOID filePath);
DWORD WINAPI WriteStats(LPVOID dataArray);
DWORD WINAPI OutputStats(LPVOID Label);
