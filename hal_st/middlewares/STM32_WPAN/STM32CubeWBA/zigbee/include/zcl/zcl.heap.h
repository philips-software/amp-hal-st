/* Copyright [2020 - 2022] Exegin Technologies Limited. All rights reserved. */

#ifndef ZIGBEE_ZCL_HEAP_H
# define ZIGBEE_ZCL_HEAP_H

/* These functions are provided by the stack to allocate memory from
 * the stack's heap. */
void * zb_heap_alloc(struct ZigBeeT *zb, unsigned int sz, const char *funcname, unsigned int linenum);
void zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *funcname, unsigned int linenum);

#if defined(CONFIG_ZB_MEMORY_DEBUG) || defined(CONFIG_ZB_MEMPOOL_HISTORY)
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, __func__, __LINE__)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, __func__, __LINE__)
#else
/* To reduce binary size */
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, "", 0)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, "", 0)
#endif

#endif
