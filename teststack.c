#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(){
      int test[2049] ;
      for (int i=0; i < 20; i++){
        test[i] = 0;
      }
      printf(1,"hello %x",test[0]);
      exit();
}

