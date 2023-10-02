# Reproducer: Docker Desktop for Mac and flock()

## Description
This is a minimal reproducer for a bug in Docker Desktop for Mac with VirtioFS and `flock()`.

## Description
When using VirtioFS, `flock()` does not work as expected. The multiple threads can acquire exclusive locks on the same file.

```c
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
```

## Steps to reproduce
1. Clone this repository
2. Build docker image: `docker build -t reproducer .`
3. Run the reproducer without a shared directory: `docker run reproducer`. This works as expected: One thread managed to acquire the lock, 
    the other thread does not:
    ```
    Thread 2: FD=3, flock() res=0 
    Thread 1: FD=4, flock() res=-1 
    ```
4. Now mount a shared directory: `docker run -v /Users/jara/shared:/var/shared reproducer`. This does not work as expected: Both threads managed to acquire the lock:
    ```
    Thread 2: FD=3, flock() res=0 
    Thread 1: FD=4, flock() res=0 
    ```
    This should never happen!  

Subsequent executions work as expected. It's only a problem when the file we are locking does not exist - it's opened with `O_CREAT | O_RDWR` flags. 

## Technical details
I can reproduce this only when Docker Desktop is configured to use VirtioFS. gRPC FUSE always works as expected.
