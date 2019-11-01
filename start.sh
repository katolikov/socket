#!/bin/sh
gcc server.c -o server -std=c89
gcc client.c -o client -std=c89
./server&
osascript -e 'tell app "Terminal"
    do script "cd Desktop
    cd lab5
    ./client"
end tell'
exit 0
