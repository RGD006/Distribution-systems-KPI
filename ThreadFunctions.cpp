#include "ThreadFunctions.h"
#include "Resource.h"
#include <fstream>
#include <stdexcept>
#include <codecvt>

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
{
  EVENTFUNC *efReadFile       = new EVENTFUNC;
  states[EF_STATUS_READ_FILE] = efReadFile;
  efReadFile->sEvent          = CreateSemaphore(NULL, 0, 1, NULL);
  efReadFile->fEvent          = CreateThread(NULL, 0, ReadFile, this, 0, NULL);

  EVENTFUNC *efWriteStats       = new EVENTFUNC;
  states[EF_STATUS_WRITE_STATS] = efWriteStats;
  efWriteStats->sEvent          = CreateSemaphore(NULL, 0, 1, NULL);
  efWriteStats->fEvent          = CreateThread(NULL, 0, WriteStats, this, 0, NULL);

  EVENTFUNC *efOutputStats       = new EVENTFUNC;
  states[EF_STATUS_OUTPUT_STATS] = efOutputStats;
  efOutputStats->sEvent          = CreateSemaphore(NULL, 0, 1, NULL);
  efOutputStats->fEvent          = CreateThread(NULL, 0, OutputStats, this, 0, NULL);
}

void FileInfo::releaseSemaphore(HANDLE sem)
{
  if (!ReleaseSemaphore(sem, 1, NULL)) {
    uint32_t error = GetLastError();
    std::wstring errorMessage(L"Can't release semaphore " + std::to_wstring(error));

    MessageBox(nullptr, errorMessage.data(), L"Error", MB_ICONERROR);
  }
}

EVENTFUNC *FileInfo::getEventFile(EF_STATUS status)
{
  return states.at(status);
}

void FileInfo::startGetFileStats(std::wstring path, HWND *hWindow)
{
  this->path       = path;
  HANDLE semHandle = getEventFile(EF_STATUS_READ_FILE)->sEvent;
  releaseSemaphore(semHandle);
}

FILESTATS *FileInfo::getFileStats(void)
{
  return &fileStats;
}

const wchar_t *FileInfo::getPath(void)
{
  return path.data();
}

wchar_t FileInfo::getFileCharacter(const size_t &pos)
{
  return !fileData.empty() && pos < fileData.size() ? fileData.at(pos) : '\0';
}

bool FileInfo::getFileRead(void)
{
  return isFileRead;
}

void FileInfo::setFileRead(bool status)
{
  isFileRead = status;
}

void FileInfo::appendData(std::wstring line)
{
  fileData += line;
}

DWORD WINAPI ReadFile(LPVOID fileInfo)
{
  FileInfo *object = reinterpret_cast<FileInfo *>(fileInfo);
  HANDLE semHandle = object->getEventFile(EF_STATUS_READ_FILE)->sEvent;

  WaitForSingleObject(semHandle, INFINITE);
  std::wstring line;

  std::wifstream file(object->getPath(), std::ios::binary);
  file.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

  while (std::getline(file, line)) {
    object->appendData(line);
    object->appendData(L"\n");
  }

  object->releaseSemaphore(object->getEventFile(EF_STATUS_WRITE_STATS)->sEvent);

  return 0;
}

DWORD WINAPI WriteStats(LPVOID fileInfo)
{
  FileInfo *object                  = reinterpret_cast<FileInfo *>(fileInfo);
  FILESTATS *fileStats              = object->getFileStats();
  HANDLE semHandle                  = object->getEventFile(EF_STATUS_WRITE_STATS)->sEvent;
  constexpr DWORD countStatsTimeout = 5000;

  while (true) {
    if (WaitForSingleObject(semHandle, countStatsTimeout) == WAIT_OBJECT_0) {
      for (;;) {
        wchar_t ch = object->getFileCharacter(fileStats->numberCharacters);

        if (ch == L'\0')
          break;

        fileStats->numberCharacters++;

        if (ch == L'\n')
          fileStats->numberLines++;

        if (ch == L' ' || ch == L'\t')
          fileStats->numberWords++;
      }

      object->releaseSemaphore(object->getEventFile(EF_STATUS_OUTPUT_STATS)->sEvent);
    }
  }
}

DWORD WINAPI OutputStats(LPVOID fileInfo)
{
  FileInfo *object     = reinterpret_cast<FileInfo *>(fileInfo);
  FILESTATS *fileStats = object->getFileStats();
  HANDLE semHandle     = object->getEventFile(EF_STATUS_OUTPUT_STATS)->sEvent;

  WaitForSingleObject(semHandle, INFINITE);

  std::wstring resultInfo =
      L"Number lines: " + std::to_wstring(fileStats->numberLines) +
      L"\nNumber words: " + std::to_wstring(fileStats->numberWords) +
      L"\nNumber characters: " + std::to_wstring(fileStats->numberCharacters);

  std::wstring *stats = new std::wstring(resultInfo);
  MessageBox(NULL, resultInfo.data(), L"Success", MB_OK);

  return 0;
}

FileInfo::~FileInfo()
{
  for (auto &el : states) {
    delete el.second;
  }
}