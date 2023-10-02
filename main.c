#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/fcntl.h>


void *try_lock(void *msg )
{
    int fd = open("/var/shared/file", O_CREAT | O_RDWR, 0644);
    int res = flock(fd, LOCK_EX | LOCK_NB);
    printf("%s: FD=%i, flock() res=%i \n", (char*)msg, fd, res);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, try_lock, "Thread 1");
    pthread_create(&thread2, NULL, try_lock, "Thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
