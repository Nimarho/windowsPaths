#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <cstdio>

#define BUFSIZE 1024

LPCTSTR ErrorMessage(DWORD error)
{
    LPVOID lpMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                  | FORMAT_MESSAGE_FROM_SYSTEM
                  | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  error,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);

    return((LPCTSTR)lpMsgBuf);
}

void PrintError(LPCTSTR errDesc)
{
    LPCTSTR errMsg = ErrorMessage(GetLastError());
    _tprintf(TEXT("\n** ERROR ** %s: %s\n"), errDesc, errMsg);
    LocalFree((LPVOID)errMsg);
}

int main() {
    PWSTR path;
    if (!SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &path))
    {
        std::wstring wstrPath(path);
        std::string userHomePath(wstrPath.begin(), wstrPath.end());
        std::cout << "User's Home Folder Path: " << userHomePath << std::endl;
        CoTaskMemFree(path);
    }
    else
    {
        std::cerr << "Failed to get user's home folder path." << std::endl;
    }

    if (!SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path))
    {
        std::wstring wstrPath(path);
        std::string userHomePath(wstrPath.begin(), wstrPath.end());
        std::cout << "local app data Path: " << userHomePath << std::endl;
        CoTaskMemFree(path);
    }
    else
    {
        std::cerr << "Failed to get user's home folder path." << std::endl;
    }

    if (!SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path))
    {
        std::wstring wstrPath(path);
        std::string userHomePath(wstrPath.begin(), wstrPath.end());
        std::cout << "Roaming app data Path: " << userHomePath << std::endl;
        CoTaskMemFree(path);
    }
    else
    {
        std::cerr << "Failed to get user's home folder path." << std::endl;
    }

    std::string s = R"(C:\Users\mango\CLionProjects\windowsPaths\test.txt)";
    const int length = s.length();
    auto nameOfFile = new char[length + 1];
    strcpy(nameOfFile, s.c_str());
    auto hFile = CreateFile(nameOfFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        PrintError(TEXT("First CreateFile failed"));
        return (1);
    }

    TCHAR buffer[MAX_PATH];
    DWORD result = GetTempPath(MAX_PATH, buffer);

    if (result > 0 && result < MAX_PATH)
    {
        std::string tempFolderPath = buffer;

        std::cout << "Temp Folder Path: " << tempFolderPath << std::endl;
    }
    else
    {
        std::cerr << "Failed to get Temp folder path." << std::endl;
    }

    UINT uRetVal = 0;
    TCHAR szTempFileName[MAX_PATH];
    uRetVal = GetTempFileName(buffer, TEXT("testforlength"), 0,szTempFileName);

    if (!uRetVal)
    {
        std::cerr << "Failed to get Temp file name." << std::endl;
        return -1;
    }

    std::string tempFileName = szTempFileName;

    std::cout << "Temp file name : " << tempFileName << std::endl;

    auto hTempFile = CreateFile(szTempFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hTempFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Unable to open temp file." << std::endl;
        CloseHandle(hFile);
        return (1);
    }

    char  chBuffer[BUFSIZE];
    DWORD dwBytesRead = 0;
    DWORD dwBytesWritten = 0;
    BOOL fSuccess  = FALSE;
    do
    {
        if(ReadFile(hFile, chBuffer, BUFSIZE, &dwBytesRead, NULL))
        {
            CharUpperBuffA(chBuffer, dwBytesRead);

            fSuccess = WriteFile(hTempFile, chBuffer, dwBytesRead, &dwBytesWritten, NULL);
            if(!fSuccess)
            {
                std::cerr << "WriteFile failed." << std::endl;
                return 5;
            }
        }
        else
        {
            std::cerr << "Readfile failed." << std::endl;
            return 6;
        }
    } while (dwBytesRead == BUFSIZE);

    if(!CloseHandle(hFile))
    {
        std::cerr << "CloseHandle(hFile) failed." << std::endl;
        return 7;
    }

    if(!CloseHandle(hTempFile))
    {
        std::cerr << "CloseHandle(hTempFile) failed." << std::endl;
        return 8;
    }

    return 0;
}
