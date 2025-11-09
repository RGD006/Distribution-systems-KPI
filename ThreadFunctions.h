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

enum EF_STATUS
{
  EF_STATUS_READ_FILE,
  EF_STATUS_WRITE_STATS,
  EF_STATUS_OUTPUT_STATS,
}

class FileInfo {
private:
  HWND *windowHandler;
  std::wstring path;
  FILESTATS fileStats;
  EVENTFUNC efReadFile;
  EVENTFUNC efWriteStats;
  EVENTFUNC efOutputStats;

public:
  FileInfo();
  void startGetInfo(std::wstring path, HWND *hWindow);
  FILESTATS getInfo(void);
  EVENTFUNC getEventFile(EF_STATUS status);
};
