# Project 2
Creating a simple file transfer program using TCP.

*This project was developed locally and has been tested on the flip.engr.oregonsate.edu server.*

## Compiling
- create the c binary for the server 
  - ```make all```

## Execution
### Start the Server
- start the server in one window, specifying which port to use for the control socket.
  - ```./ftserver -p 30051```

### Run Client
- start the client in another window; Specifying host info, data port, and command:
  - list server directory:
    - ```./chatclient -H 127.0.0.1 -P 30051 -D 30052 -l```
  - get a file from server directory:
    - ```./chatclient -H 127.0.0.1 -P 30051 -D 30052 -g testfile.txt```

## Cleaning up
- To clean up the binaries in the directory
  - ```make clean```

*This is written using github's flavored markdown*
