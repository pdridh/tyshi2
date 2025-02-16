#include "Core.h"

void *gameMemoryGet(size_t len)
{
  void *mem;
#if WIN32
  mem = VirtualAlloc(NULL, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
  mem = mmap(0, len,
             PROT_READ | PROT_WRITE,
             MAP_ANON | MAP_PRIVATE,
             -1, 0);
#endif
  return mem;
}

void gameMemoryFree(void *ptr, size_t len)
{
#if WIN32
  VirtualFree(NULL, 0, MEM_RELEASE);
#else
  munmap(ptr, len);
#endif
}