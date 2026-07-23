#include <cerrno>
#include <sys/stat.h>
#include <sys/types.h>

extern "C"
{
    __attribute__((weak)) int _close([[maybe_unused]] int fildes)
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _fstat([[maybe_unused]] int fildes, [[maybe_unused]] struct stat* st)
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _getpid()
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _isatty([[maybe_unused]] int file)
    {
        errno = ENOSYS;
        return 0;
    }

    __attribute__((weak)) int _kill([[maybe_unused]] int pid, [[maybe_unused]] int sig)
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _lseek([[maybe_unused]] int file, [[maybe_unused]] int ptr, [[maybe_unused]] int dir)
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _read([[maybe_unused]] int file, [[maybe_unused]] char* ptr, [[maybe_unused]] int len)
    {
        errno = ENOSYS;
        return -1;
    }

    __attribute__((weak)) int _write([[maybe_unused]] int file, [[maybe_unused]] char* ptr, [[maybe_unused]] int len)
    {
        errno = ENOSYS;
        return -1;
    }
}
