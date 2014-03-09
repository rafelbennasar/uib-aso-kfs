#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>


int sem_init(int *mutex);
int sem_get(int *mutex);
int sem_wait(int s);
int sem_signal(int s);
