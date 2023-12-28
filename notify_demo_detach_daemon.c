#include <systemd/sd-daemon.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

static void signal_handler(int);

struct sigaction sigact;

void init_signals(void)
{
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGHUP, &sigact, (struct sigaction *)NULL);
}

static void signal_handler(int sig)
{
    if (sig == SIGHUP) {
        printf("Reloading (daemon service)...\n");
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        sd_notifyf(0, "RELOADING=1\nMONOTONIC_USEC=%" PRIdMAX "%06d", (intmax_t)ts.tv_sec, (int)(ts.tv_nsec / 1000));
        sleep(6);
        printf("Reload complete\n");
        sd_notify(0, "READY=1");
    }
}

int main(int argc, char **argv)
{
    if (daemon(0, 0) != 0) {
        printf("Failed to daemonize the process\n");
        return 1;
    }
    // Past this point we're in the child process
    pid_t pid = getpid();
    init_signals();
    sd_notifyf(0, "MAINPID=%d\nREADY=1", pid);  // Finished startup
    printf("Child is running with PID=%d\n", pid);
    for (int i = 0; i < 1000000; i++) {
        printf("Doing my thing...\n");
        sleep(3);
    }
    return 0;
}
