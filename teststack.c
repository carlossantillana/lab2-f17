/*#include "types.h"
#include "stat.h"
#include "user.h"
int main(){
      int test[2000] ;
      for (int i=0; i < 20; i++){
        test[i] = 0;
      }
      printf(1,"hello %x",test[0]);
      int test2[2000] ;
            for (int i=0; i < 20; i++){
                      test2[i] = 1;
                            }
                  printf(1,"hello %x",test2[0]);

      exit();
}*/

#include "types.h"
#include "stat.h"
#include "user.h"

int test(int n)
{
   test(n+1);
   return n;
}
int main(int argc, char *argv[])
{
  int pid=0;
  pid=fork();
  if(pid==0){
       int x=1;
       printf(1, "address %x\n", &x);
       test(1);
       exit();
  }
  wait();
  exit();
}
