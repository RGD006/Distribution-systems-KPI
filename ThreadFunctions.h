#pragma once
#include <windows.h>
#include <string>
#include <cstdint>

struct EVENTFUNC {
  HANDLE sEvent;  // event semaphore
  HANDLE fEvent;  // event function
  EVENTFUNC(HANDLE _sEvent, HANDLE _fEvent);
  EVENTFUNC();
};

struct FILESTATS {
  uint32_t numberLines;
  uint32_t numberWords;
  uint32_t numberCharacters;
};

class FileInfo {
private:
  HWND *windowHandler;
  FILESTATS fileStats;
  EVENTFUNC efReadFile;
  EVENTFUNC efWriteStats;
  EVENTFUNC efOutputStats;

public:
  FileInfo();
  void startGetInfo(std::wstring path, HWND *hWindow);
  FILESTATS getInfo(void);
};
