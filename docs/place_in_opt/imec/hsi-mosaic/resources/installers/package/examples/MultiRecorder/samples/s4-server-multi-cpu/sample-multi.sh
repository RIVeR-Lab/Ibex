#!/bin/bash

# start remote server - you should kill the process later
../../MultiRecorder --start-server "localhost:8090" --log log_sample_server.txt &

# wait a bit for server init
sleep 1

# connect to remote server and run the job (open the camera connected to server computer)
../../MultiRecorder --run "sample-multi.xml" --log log_sample_recorder.txt

# check running processes, just to be sure server still running
printf "\n\nEND\n\nMultiRecorders processes running: "
pgrep MultiRecorder

# kill the server process
pkill -n MultiRecorder
sleep 1

echo
printf "MultiRecorders processes running after server kill: "
# check running processes, just to be sure server is gone
pgrep MultiRecorder
echo ""
