#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <wchar.h>
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

enum EF_STATUS {
  EF_STATUS_READ_FILE,
  EF_STATUS_WRITE_STATS,
  EF_STATUS_OUTPUT_STATS,
};

class FileInfo {
private:
  std::map<EF_STATUS, EVENTFUNC *> states;
  std::wstring fileData;
  std::wstring path;
  bool isFileRead;
  FILESTATS fileStats;

public:
  FileInfo();
  ~FileInfo();
  const wchar_t *getPath(void);
  wchar_t getFileCharacter(const size_t &pos);
  void startGetFileStats(std::wstring path, HWND *hWindow);
  void appendData(std::wstring line);
  FILESTATS *getFileStats(void);
  bool getFileRead(void);
  void setFileRead(bool status);
  void releaseSemaphore(HANDLE sem);
  EVENTFUNC *getEventFile(EF_STATUS status);
};
