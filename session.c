#include "myUtil.h"
#include <unistd.h>
#include <sys/types.h>
void session(CLIENT *)
{
    pid_t id;
    int ptoc[2];/*parent to child*/
    int ctop[2];/* child to parent*/
    
}