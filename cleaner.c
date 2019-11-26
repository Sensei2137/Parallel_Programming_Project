#include <stdio.h>

#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

int main()
{

 msgctl(9001,IPC_RMID,0);
 msgctl(9002,IPC_RMID,0);
 msgctl(9003,IPC_RMID,0);
 msgctl(9004,IPC_RMID,0);
 
 

 
    return 0;
}