
# Durex

## Overview

A trojan is whatever program that seems, or at least pretends to be legit, but that hides a malicious payload in it, which is executed in the background, often under the user's knowledge.

This project is a small trojan. The visible part of the iceberg is a funny Greed game, but will (in background) write the payload in the OS binaries, start it, and add an entry to the crontab, in order to run it after each reboot.

The payload will open a TCP connection on the port 4242 of the infected machine. It will accept 3 simultaneous clients, and will ask for a password when a client connects. When a client logs successfully, a remote root shell will spawn.

Well, Christmas is coming, here's the shell password: `lfalkau`

## Compilation & Usage

**Clone & run:**

```
git clone git@github.com:lfalkau/durex
cd durex
make
./Durex #It has to be executed as root on the victim's machine
```

**Some useful commands:**

- `pstree | grep Durex` See if the malware is running
- `sudo kill -9 $(pidof Durex)` Kill the malware
- `sudo rm /bin/Durex` Delete the malware

## Work in progress 🔨

The project is not finished yet, here's the things that needs to be implemented:

- [ ] Create a pidfile to avoid launching several Durex instances 
- [ ] Implement my own `daemon` function
- [x] Don't write to crontab if it has already been done
- [x] Clean the victim's machine with the make clean rule


## Disclaimer ⚠️

I made this program for educative purpose, and I made it public for educative purpose. (That's why it's pretty easily detectable)

Anyway, as Google would say:
> Don't be evil.
