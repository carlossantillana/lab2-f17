#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {
  uint found = 0, empty = 0;
  char * pa, * va;
  struct proc * curproc= myproc();
  va = (char*)PGROUNDUP(curproc->sz);
  for (uint i = 0; i< 64; i++) {
    if (shm_table.shm_pages[i].id == id){
      found = i+1;// if found set found to position of page plus one (for edge cases in if)
     }
  }
  if (found ){//case 1
    pa = shm_table.shm_pages[found-1].frame;//put inside mappages
    mappages(curproc->pgdir, va, PGSIZE , V2P(pa), PTE_U | PTE_W);
    shm_table.shm_pages[found-1].refcnt++;// increments the refcounti
    curproc->sz += PGSIZE;
   
  }
  else{//case 2
    for (uint i =0; i < 64; i++){
      if (shm_table.shm_pages[i].refcnt == 0 && empty == 0){
        shm_table.shm_pages[i].id = id;
          empty = i+1;
      }                 
    }
    shm_table.shm_pages[empty-1].frame = kalloc();
    shm_table.shm_pages[empty-1].refcnt = 1;
    pa = shm_table.shm_pages[empty-1].frame;//put inside map pages
    mappages(curproc->pgdir, va ,PGSIZE,V2P(pa), PTE_U | PTE_W);
    shm_table.shm_pages[empty-1].refcnt++;// increments the refcount

  }

  
return (char*) va; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!




return 0; //added to remove compiler warning -- you should decide what to return
}
