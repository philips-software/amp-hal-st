int errno;

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>

void __initialize_args(int* p_argc, char*** p_argv);

void __attribute__((weak)) __initialize_args(int* p_argc, char*** p_argv)
{
    static char name[] = "";

    // The string pointed to by argv[0] represents the program name;
    // argv[0][0] shall be the null character if the program name is not
    // available from the host environment. argv[argc] shall be a null pointer.
    // (static, no const)
    static char* argv[2] = { name, NULL };

    *p_argc = 1;
    *p_argv = &argv[0];
    return;
}

int raise(int sig __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int kill(pid_t pid __attribute__((unused)), int sig __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

char* __env[1] = { 0 };
char** environ = __env;

int __attribute__((weak)) _chown(const char* path __attribute__((unused)), uid_t owner __attribute__((unused)), gid_t group __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _close(int fildes __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _execve(char* name __attribute__((unused)), char** argv __attribute__((unused)), char** env __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _fork(void)
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _fstat(int fildes __attribute__((unused)), struct stat* st __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _getpid(void)
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _gettimeofday(struct timeval* ptimeval __attribute__((unused)), void* ptimezone __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _isatty(int file __attribute__((unused)))
{
    errno = ENOSYS;
    return 0;
}

int __attribute__((weak)) _kill(int pid __attribute__((unused)), int sig __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _link(char* existing __attribute__((unused)), char* _new __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _lseek(int file __attribute__((unused)), int ptr __attribute__((unused)), int dir __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _open(char* file __attribute__((unused)), int flags __attribute__((unused)), int mode __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _read(int file __attribute__((unused)), char* ptr __attribute__((unused)), int len __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _readlink(const char* path __attribute__((unused)), char* buf __attribute__((unused)), size_t bufsize __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _stat(const char* file __attribute__((unused)), struct stat* st __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _symlink(const char* path1 __attribute__((unused)), const char* path2 __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

clock_t __attribute__((weak)) _times(struct tms* buf __attribute__((unused)))
{
    errno = ENOSYS;
    return ((clock_t)-1);
}

int __attribute__((weak)) _unlink(char* name __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _wait(int* status __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}

int __attribute__((weak)) _write(int file __attribute__((unused)), char* ptr __attribute__((unused)),
    int len __attribute__((unused)))
{
    errno = ENOSYS;
    return -1;
}
