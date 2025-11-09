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
                 CreateThread(NULL, 0, ReadFile, this, 0, NULL)),
      efWriteStats(CreateSemaphore(NULL, 1, 1, NULL),
                   CreateThread(NULL, 0, WriteStats, this, 0, NULL)),
      efOutputStats(CreateSemaphore(NULL, 1, 1, NULL),
                    CreateThread(NULL, 0, OutputStats, this, 0, NULL))
{
}

void FileInfo::startGetInfo(std::wstring path, HWND *hWindow)
{
  windowHandler = hWindow;
  this->path    = path;

  if (WaitForSingleObject(efReadFile.sEvent, 0L)) {
    if (!ReleaseSemaphore(&efReadFile, 1, NULL)) {
      MessageBox(
          *hWindow,
          L"Can't release start semaphore",
          L"Error",
          MB_ICONERROR);
    }
  } 
}

// TODO: добавить сюда гетер для статуса семафора
DWORD WINAPI ReadFile(LPVOID fileInfo)
{
  for (;;) {
    if (WaitForSingleObject(efReadFile.sEvent, 0L)) {

    }
  }

  return 0;
}

DWORD WINAPI WriteStats(LPVOID fileInfo)
{
  return 0;
}

DWORD WINAPI OutputStats(LPVOID fileInfo)
{
  return 0;
}