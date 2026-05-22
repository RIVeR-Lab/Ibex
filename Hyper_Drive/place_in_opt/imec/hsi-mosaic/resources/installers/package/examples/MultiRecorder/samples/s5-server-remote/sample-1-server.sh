#!/bin/bash

# Start the server on remote computer, change localhost to the real IP address (or run it on localhost just to test it).
# Change the IP address in the sample-remote.xml too!
../../MultiRecorder --start-server "localhost:8090" --log log_sample_server.txt
