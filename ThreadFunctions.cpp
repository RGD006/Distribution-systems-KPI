#include "ThreadFunctions.h"

static DWORD WINAPI ReadFile(LPVOID filePath);
static DWORD WINAPI WriteStats(LPVOID dataArray);
static DWORD WINAPI OutputStats(LPVOID window);

EVENTFUNC::EVENTFUNC(HANDLE _sEvent, HANDLE _fEvent)
{
  sEvent = _sEvent;
  fEvent = _fEvent;
}

EVENTFUNC::EVENTFUNC()
{
  sEvent = nullptr;
  fEvent = nullptr;
}

FileInfo::FileInfo()
    : efReadFile(CreateSemaphore(NULL, 1, 1, NULL),
                 CreateThread(NULL, 0, ReadFile, NULL, 0, NULL)),
      efWriteStats(CreateSemaphore(NULL, 1, 1, NULL),
                 CreateThread(NULL, 0, WriteStats, NULL, 0, NULL)),
      efOutputStats(CreateSemaphore(NULL, 1, 1, NULL),
                 CreateThread(NULL, 0, OutputStats, NULL, 0, NULL))
{
}

DWORD WINAPI ReadFile(LPVOID filePath)
{

  return 0;
}

DWORD WINAPI WriteStats(LPVOID dataArray)
{

  return 0;
}

DWORD WINAPI OutputStats(LPVOID window)
{

  return 0;
}