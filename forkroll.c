#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#define FORKROLL_MAX 17

char *forkroll[FORKROLL_MAX] = {
    "Never",
    "gonna",
    "give",
    "you",
    "up",
    "Never",
    "gonna",
    "let",
    "you",
    "down",
    "Never",
    "gonna",
    "run",
    "around",
    "and",
    "desert",
    "you"
};

static int *forkroll_index;

int main(int argc, char *argv[])
{
    int i = 0;
    int val = 0;
    size_t argv0size = 0;
    pid_t pid[FORKROLL_MAX];

    // get size of default process name
    (void)argc;
    argv0size = strlen(argv[0]);

    // create memory to share between processes
    forkroll_index = mmap(NULL, sizeof(*forkroll_index), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // set index
    (*forkroll_index) = -1;

    // loop through words
    for (i = 0; i < FORKROLL_MAX; i++)
    {
        // fork process
        pid[i] = fork();

        // child process
        if (pid[i] == 0)
        {
            // kill child when parent dies
            prctl(PR_SET_PDEATHSIG, SIGHUP);

            // get index value + increment
            val = ++(*forkroll_index);

            // sleep thread
            usleep((double)(FORKROLL_MAX - val - 1) * 10000.0);

            // overwrite process name
            strncpy(argv[0], forkroll[val], argv0size);

            // loop forever
            while (1) {}
        }
        else if (pid[i] < 0)
        {
            // fork() failed
            fprintf(stderr, "Error!\n");

            return 0;
        }
    }

    // loop forever
    while (1) {}

    return 0;
}