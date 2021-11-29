#!/bin/bash

sudo rm -f /bin/Durex 2>/dev/null
sudo kill -9 $(pidof Durex) 2>/dev/null
sudo sed -i '/Durex/d' /etc/crontab 2>/dev/null
