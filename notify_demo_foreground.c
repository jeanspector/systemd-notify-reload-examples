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
        printf("Reloading (simple service)...\n");
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        sd_notifyf(0, "RELOADING=1\nMONOTONIC_USEC=%" PRIdMAX "%06d", (intmax_t)ts.tv_sec, (int)(ts.tv_nsec / 1000));
        sleep(4);
        printf("Reload complete\n");
        sd_notify(0, "READY=1");
    }
}

int main(int argc, char **argv)
{
    init_signals();
    printf("Started...\n");
    sd_notify(0, "READY=1");

    for (int i = 0; i < 1000000; i++) {
        printf("Doing my thing...\n");
        sleep(3);
    }
    return 0;
}
