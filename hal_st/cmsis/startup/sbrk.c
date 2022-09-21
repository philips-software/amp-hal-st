#include <sys/types.h>
#include <errno.h>

extern char _Heap_Begin;
extern char _Heap_Limit;

__attribute__((__used__)) caddr_t _sbrk(int incr)
{
    static char* current_heap_end = &_Heap_Begin;

    char* current_block_address = current_heap_end;

    incr = (incr + 3) & (~3); // align value to 4
    if (current_heap_end + incr > &_Heap_Limit)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    current_heap_end += incr;

    return (caddr_t)current_block_address;
}
