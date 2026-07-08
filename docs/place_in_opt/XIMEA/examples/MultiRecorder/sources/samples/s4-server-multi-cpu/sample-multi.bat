@ECHO OFF
:: start remote server - you should kill the process later
START /B ..\..\MultiRecorder.exe --start-server "localhost:8090" --log log_sample_server.txt

:: wait a bit for server init
TIMEOUT /t 1 /nobreak > NUL

:: connect to remote server and run the job (open the camera connected to server computer)
..\..\MultiRecorder.exe --run "sample-multi.xml" --log log_sample_recorder.txt

:: kill the server process
TASKKILL /F /IM "MultiRecorder.exe"

