#!/bin/bash

#ls -l /proc/`ps -x | grep tesh | cut -d " " -f 3 | head -n 1`/fd  | tail -n +2 | awk '{ print $9, $10, $11 }'
ls -l /proc/$PPID/fd  | tail -n +2 | awk '{ print $9, $10, $11 }'
