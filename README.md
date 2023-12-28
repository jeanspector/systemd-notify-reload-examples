## Overview

This project demonstrates several ways of creating a C service that
communicates with systemd, enabling the use of `notify-reload` service type.
The same examples can be used to convert an existing service to the new
standard.

See
[systemd.service](https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html)
man page for more details.

## Requirements

`systemd` version >= 253

## Build

### Install dependencies

#### Debian

```
sudo apt-get install libsystemd-dev
```

#### CentOS

```
sudo yum install systemd-devel
```

### Build the binary

```
gcc notify_demo_<VARIANT>.c -o /tmp/notify_demo -lsystemd
```

## systemd service

### Create new service

```
sudo cp notify_demo.service /lib/systemd/system/notify_demo.service
sudo systemctl daemon-reload
```

### Manipulate the service

```
sudo systemctl start notify_demo
sudo systemctl reload notify_demo
sudo systemctl stop notify_demo
```

## Examples

### Foreground process

[notify_demo_foreground.c](notify_demo_foreground.c)

Since `systemd` and, specifically, `notify-reload` service type expects the
foreground process, this is the simplest and also the recommended solution.

### fork-based background process

[notify_demo_detach_fork.c](notify_demo_detach_fork.c)

While perfectly usable, this solution is not recommended. You should only
consider it if you have a legacy service that uses
[fork](https://man7.org/linux/man-pages/man2/fork.2.html),
e.g. of type "forking".

### daemon-based background process

[notify_demo_detach_daemon.c](notify_demo_detach_daemon.c)

WARNING: This solution doesn't work reliably - since
[daemon](https://man7.org/linux/man-pages/man3/daemon.3.html) exits the parent
implicitly, this example fails sporadically due to a race between the service
updating the child PID and systemd registering the parent's exit.

This solution is not recommended and is only included for completeness' sake -
prefer the foreground example, since `systems` handles detaching the process on
its own.

## Debugging

Get/set systemd log level

```
systemd-analyze get-log-level
sudo systemd-analyze set-log-level debug
```

Examine systemd output

```
systemctl status --full --lines=50 notify_demo
journalctl --catalog --pager-end --unit=notify_demo
```

