#include <windows.h>
#include <filesystem>
#include <string>
#include <vector>

int main()
{
    wchar_t modulePath[MAX_PATH]{};
    const DWORD length = GetModuleFileNameW(nullptr, modulePath, MAX_PATH);
    if (length == 0 || length == MAX_PATH) return 1;
    const std::filesystem::path base = std::filesystem::path(modulePath).parent_path();
    const std::filesystem::path runtime = base / L"runtime";
    const std::filesystem::path game = base / L"ThaiChessApp.exe";
    if (!SetDllDirectoryW(runtime.c_str())) return 2;
    DWORD pathLength = GetEnvironmentVariableW(L"PATH", nullptr, 0);
    std::vector<wchar_t> pathBuffer(pathLength == 0 ? 1 : pathLength);
    if (pathLength > 0) GetEnvironmentVariableW(L"PATH", pathBuffer.data(), pathLength);
    const std::wstring inheritedPath = runtime.wstring() + L";" + (pathLength > 0 ? std::wstring(pathBuffer.data()) : L"");
    if (!SetEnvironmentVariableW(L"PATH", inheritedPath.c_str())) return 3;
    std::wstring command = L"\"" + game.wstring() + L"\"";
    STARTUPINFOW startup{}; startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(game.c_str(), command.data(), nullptr, nullptr, FALSE, 0, nullptr, base.c_str(), &startup, &process)) return 4;
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return 0;
}
