#!/bin/bash

# Connect to remote server and run the job (open the camera connected to server computer).
# Be sure the IP address matches inside the XML.
../../MultiRecorder --run "sample-remote.xml" --log log_sample_recorder.txt
