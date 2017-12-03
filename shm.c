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
}shm_table;

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
  acquire(&(shm_table.lock));
  uint found = 0, empty = 0;
  char * pa, * va;
  struct proc * curproc= myproc();
  va = (char*)PGROUNDUP(curproc->sz);
  cprintf("va: %x \n", va);
  for (uint i = 0; i< 64; i++) {
    if (shm_table.shm_pages[i].id == id){
      found = i+1;// if found set found to position of page plus one (for edge cases in if)
     }
  }
  
  if (found ){//case 1
    cprintf("Inside found pa: %x \n",  shm_table.shm_pages[found-1].frame);
    //acquire(&(shm_table.lock));
    pa = shm_table.shm_pages[found-1].frame;//put inside mappages
    mappages(curproc->pgdir, va, PGSIZE , V2P(pa), PTE_U | PTE_W);
    shm_table.shm_pages[found-1].refcnt++;// increments the refcounti
   // release(&(shm_table.lock));
    //curproc->sz += PGSIZE;
   
  }
  else{//case 2
  cprintf("Case 2 \n");
    for (uint i =0; i < 64; i++){
      if (shm_table.shm_pages[i].refcnt == 0 && empty == 0){
        shm_table.shm_pages[i].id = id;
        cprintf("id: %x \n", id);
        empty = i+1;
      }                 
    }
   // acquire(&(shm_table.lock));   
    shm_table.shm_pages[empty-1].frame = kalloc();
    cprintf("kalloc pa: %x \n",  shm_table.shm_pages[empty-1].frame);
    cprintf("uint pa: %x \n",  shm_table.shm_pages[empty-1].frame);

   // shm_table.shm_pages[empty-1].refcnt = 1;
    pa = shm_table.shm_pages[empty-1].frame;//put inside map pages
    memset(pa, 0, PGSIZE);
    //release(&(shm_table.lock));
    mappages(curproc->pgdir, va+ PGSIZE,PGSIZE,V2P(pa), PTE_U | PTE_W);
    shm_table.shm_pages[empty-1].refcnt = 1;
    va = va + PGSIZE;
  }
  curproc->sz += PGSIZE;
 release(&(shm_table.lock));
 *pointer = (char*)va;
 cprintf("Leaving shmopen \n"); 
return * va; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
  acquire(&(shm_table.lock));
 //  char * va;
 // struct proc * curproc = myproc();
 //  va = (char*)PGROUNDUP(curproc->sz);  
  for (uint i = 0; i < 64;i++)
  {
    if (id == shm_table.shm_pages[i].id)
    {
      shm_table.shm_pages[i].refcnt--;
      if (shm_table.shm_pages[i].refcnt == 0)
      {
        shm_table.shm_pages[i].id =0;
        shm_table.shm_pages[i].frame =0;
        shm_table.shm_pages[i].refcnt = 0;
    } // id conditional
  } // for loop 
  }
  release(&(shm_table.lock));
  return 0; //added to remove compiler warning -- you should decide what to return
}
