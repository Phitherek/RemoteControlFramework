#include "ExecuteImpl.h"
#include "librcfcommon/librcfcommon.h"
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
            execl(exec.c_str(), exec.substr(0, exec.find(' ')-1).c_str(), NULL);
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
    int ExecuteImpl::windows_execute(std::string exec, std::string* stdout_target, std::string* stderr_target) {
        HANDLE outRead = NULL;
        HANDLE outWrite = NULL;
        HANDLE errRead = NULL;
        HANDLE errWrite = NULL;
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = true;
        sa.lpSecurityDescriptor = NULL;
        if(!CreatePipe(&outRead, &outWrite, &sa, 0)) {
            throw RCF::Common::RuntimeException("Cannot create stdout pipe!");
        }
        if(!SetHandleInformation(&outRead, HANDLE_FLAG_INHERIT, 0)) {
            throw RCF::Common::RuntimeException("Cannot set stdout pipe handle information!");
        }
         if(!CreatePipe(&errRead, &errWrite, &sa, 0)) {
            throw RCF::Common::RuntimeException("Cannot create stderr pipe!");
        }
        if(!SetHandleInformation(&errRead, HANDLE_FLAG_INHERIT, 0)) {
            throw RCF::Common::RuntimeException("Cannot set stderr pipe handle information!");
        }
        TCHAR cmd[] = TEXT(exec.c_str());
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        BOOL success = false;
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = errWrite;
        si.hStdOutput = outWrite;
        si.dwFlags |= STARTF_USESTDHANDLES;

        success = CreateProcess(cmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi);

        if(!success) {
            throw RCF::Common::RuntimeException("Cannot create child process!");
        }
        DWORD bytesRead, bytesWritten;
        CHAR buffer[4096];
        success = false;
        for(;;) {
            success = ReadFile(outRead, buffer, 4096, &bytesRead, NULL);
            if(!success || bytesRead == 0) break;
            *stdout_target << buffer;
        }
        success = false;
        for(;;) {
            success = ReadFile(errRead, buffer, 4096, &bytesRead, NULL);
            if(!success || bytesRead == 0) break;
            *stderr_target << buffer;
        }
        DWORD ec;
        if(!GetExitCodeProcess(pi.hProcess, &ec)) {
            throw RCF::Common::RuntimeException("Cannot get exit code of a child process!");
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(outWrite);
        CloseHandle(errWrite);
        return ec;
    }
#endif
