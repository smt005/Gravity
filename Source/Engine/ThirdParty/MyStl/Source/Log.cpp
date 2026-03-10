// ◦ Xyz ◦

#include "Log.h"
#include <chrono>
#include <fstream>
#include <format>
#include <windows.h>

void mystd::WriteLog(const std::string& text) {
    bool needWriteLog = true;

#ifdef _DEBUG
        _CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
        needWriteLog = !IsDebuggerPresent();
#endif

    if (needWriteLog) {
        static const std::string_view logFileName = "Log.txt";
        static bool firstOpen = true;
        std::ofstream str;

        if (firstOpen) {
            firstOpen = false;
            str.open(logFileName.data(), std::ios::trunc);
        }
        else {
            str.open(logFileName.data(), std::ios::app);
        }

        if (!str.is_open()) {
            throw std::runtime_error("[mystd::WriteLog] Cannot open log file.");
        }

        using namespace std::chrono;
        auto now = system_clock::now();
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        str << std::format("[{:%d.%m.%Y %H:%M:%S}] ", now) << text << '\n';
    }
}
