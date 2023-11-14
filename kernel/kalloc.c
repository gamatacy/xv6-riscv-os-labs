// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"


extern char end[];  // first address after kernel.
// defined by kernel.ld.

uint64 refs[REF_INDEX(PHYSTOP) + 1];
struct spinlock refs_lock;

void kinit() {
    char *p = (char *) PGROUNDUP((uint64) end);
    bd_init(p, (void *) PHYSTOP);
    memset(refs, 0, PGROUNDDOWN(PHYSTOP) / PGSIZE * sizeof(uint32));
    initlock(&refs_lock, "refs_lock");
}

void incref(void *pa) {
    ++refs[REF_INDEX((uint64) pa)];
}

void decref(void *pa) {
    if (refs[REF_INDEX((uint64) pa)] <= 0)
        panic("no refs\n");
    --refs[REF_INDEX((uint64) pa)];
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) {
    acquire(&refs_lock);
    decref(pa);
    if (refs[REF_INDEX((uint64) pa)] == 0) bd_free(pa);
    release(&refs_lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void) {
    void *pa = bd_malloc(PGSIZE);
    acquire(&refs_lock);
    incref(pa);
    release(&refs_lock);
    return pa;
}
