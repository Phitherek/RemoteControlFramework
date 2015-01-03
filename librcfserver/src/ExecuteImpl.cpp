#include "ExecuteImpl.h"
#include "librcfcommon/librcfcommon.h"
//#include <iostream>
#include <cstring>
using namespace RCF::Server;

int ExecuteImpl::execute(std::string exec, std::string* stdout_target, std::string* stderr_target) {
    #if BOOST_OS_WINDOWS
        return windows_execute(exec, stdout_target, stderr_target);
    #elif BOOST_OS_LINUX || BOOST_OS_MACOS
        return unix_execute(exec, stdout_target, stderr_target);
    #endif
}

#if BOOST_OS_LINUX || BOOST_OS_MACOS
    // Partially used the following sources:
    // https://github.com/sni/mod_gearman/blob/master/common/popenRWE.c for the stdout and stderr reading.
    // http://www.microhowto.info/howto/capture_the_output_of_a_child_process_in_c.html for the overall method,
    int ExecuteImpl::unix_execute(std::string exec, std::string* stdout_target, std::string* stderr_target) {
        int in[2];
        int out[2];
        int err[2];
        int pid;
        if(pipe(in) == -1) {
            std::string err = "Cannot open in pipe: ";
            err += strerror(errno);
            throw RCF::Common::RuntimeException(err);
        }
        if(pipe(out) == -1) {
            std::string err = "Cannot open out pipe: ";
            err += strerror(errno);
            close(in[0]);
            close(in[1]);
            throw RCF::Common::RuntimeException(err);
        }
        if(pipe(err) == -1) {
            std::string err = "Cannot open err pipe: ";
            err += strerror(errno);
            close(in[0]);
            close(in[1]);
            close(out[0]);
            close(out[1]);
            throw RCF::Common::RuntimeException(err);
        }
        pid = fork();
        if(pid > 0) {
            close(in[0]);
            close(out[1]);
            close(err[1]);
            char buffer[4096];
            for(int i = 0; i < 4096; i++) {
                buffer[i] = '\0';
            }
            // Read stdout
            while(1) {
                ssize_t count = read(out[0], buffer, sizeof(buffer));
                if(count == -1) {
                    if(errno == EINTR) {
                        continue;
                    } else {
                        std::string err = "Cannot read from command stdout: ";
                        err += strerror(errno);
                        throw RCF::Common::RuntimeException(err);
                    }
                } else if(count == 0) {
                    break;
                } else {
                    *stdout_target += buffer;
                }
            }
            for(int i = 0; i < 4096; i++) {
                buffer[i] = '\0';
            }
            // Read stderr
            while(1) {
                ssize_t count = read(err[0], buffer, sizeof(buffer));
                if(count == -1) {
                    if(errno == EINTR) {
                        continue;
                    } else {
                        std::string err = "Cannot read from command stderr: ";
                        err += strerror(errno);
                        throw RCF::Common::RuntimeException(err);
                    }
                } else if(count == 0) {
                    break;
                } else {
                    *stderr_target += buffer;
                }
            }
            close(out[0]);
            close(err[0]);
            close(in[1]);
        } else if(pid == 0) {
            close(in[1]);
            close(out[0]);
            close(err[0]);
            close(0);
            if(!dup(in[0])) {
                ;
            }
            close(1);
            if(!dup(out[1])) {
                ;
            }
            close(2);
            if(!dup(err[1])) {
                ;
            }
            execl("/bin/sh", "sh", "-c", exec.c_str(), NULL);
        } else {
            std::string err = "Cannot fork: ";
            err += strerror(errno);
            close(in[0]);
            close(in[1]);
            close(out[0]);
            close(out[1]);
            close(err[0]);
            close(err[1]);
            throw RCF::Common::RuntimeException(err);
        }
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status) != 0) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }

#elif BOOST_OS_WINDOWS
    // Partially used the following sources:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682499%28v=vs.85%29.aspx
    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms683189%28v=vs.85%29.aspx
    // Other MSDN sources
    // Don' t even ask how this code works. It works. WinAPI is strange.
    int ExecuteImpl::windows_execute(std::string exec, std::string* stdout_target, std::string* stderr_target) {
        HANDLE outRead = NULL;
        HANDLE outWrite = NULL;
        HANDLE errRead = NULL;
        HANDLE errWrite = NULL;
        SECURITY_ATTRIBUTES sa;
        DWORD errorCode;
        LPVOID errorBuffer;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = true;
        sa.lpSecurityDescriptor = NULL;
        if(!CreatePipe(&outRead, &outWrite, &sa, 0)) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot create stdout pipe: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        if(!SetHandleInformation(outRead, HANDLE_FLAG_INHERIT, 0)) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot set stdout pipe information: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        if(!CreatePipe(&errRead, &errWrite, &sa, 0)) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot create stderr pipe: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        if(!SetHandleInformation(errRead, HANDLE_FLAG_INHERIT, 0)) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot set stderr pipe information: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        TCHAR* cmd = new TCHAR[exec.length()];
        int execLength = exec.length();
        int cmdLength = strlen(cmd);
        for(int i = 0; i < cmdLength; i++) {
            cmd[i] = '\0';
        }
        for(int i = 0; i < execLength; i++) {
            cmd[i] = exec[i];
        }
        cmdLength = strlen(cmd);
        // Sometimes weird trash is added at the end of this TCHAR. This gets rid of it.
        for(int i = execLength; i < cmdLength; i++) {
            cmd[i] = '\0';
        }
        //std::cout << "RCF::Server::ExecuteImpl::execute_windows DEBUG: exec: " << exec << ", cmd: " << cmd << ", execLength: " << execLength << ", cmdLength: " << cmdLength << std::endl;
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        BOOL success = false;
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = errWrite;
        si.hStdOutput = outWrite;
        si.dwFlags |= STARTF_USESTDHANDLES;

        success = CreateProcess(NULL, cmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi);

        if(!success) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot create child process: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD ec;
        if(!GetExitCodeProcess(pi.hProcess, &ec)) {
            errorCode = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorBuffer, 0, NULL);
            std::string err = "Cannot get exit code of a child process: ";
            int strsize = lstrlen((LPTSTR)errorBuffer)*sizeof(TCHAR);
            for(int i = 0; i < strsize; i++) {
                err += ((LPTSTR)errorBuffer)[i];
            }
            LocalFree(errorBuffer);
            throw RCF::Common::RuntimeException(err);
        }
        delete[] cmd;
        cmd = NULL;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(outWrite);
        CloseHandle(errWrite);
        DWORD bytesRead;
        CHAR buffer[4096];
        for(int i = 0; i < 4096; i++) {
            buffer[i] = '\0';
        }
        success = false;
        for(;;) {
            success = ReadFile(outRead, buffer, 4096, &bytesRead, NULL);
            if(!success || bytesRead == 0) break;
            *stdout_target += buffer;
        }
        for(int i = 0; i < 4096; i++) {
            buffer[i] = '\0';
        }
        success = false;
        for(;;) {
            success = ReadFile(errRead, buffer, 4096, &bytesRead, NULL);
            if(!success || bytesRead == 0) break;
            *stderr_target += buffer;
        }
        CloseHandle(outRead);
        CloseHandle(errRead);
        return ec;
    }
#endif
