# Project 1
## Compiling
- create the binaries 
```
make all
```

## Execution
### Start the Server
- start the server in one window
```
./chatserve
```
- provide a username to the server
<br>*e.g. 'Bob'* (without quotes)
- provide a port to open on the server for chat
<br>*e.g. '30077'* (without quotes)

### Start Client & Connect
- start the client in another window
```
./chatclient -i 127.0.0.1 -p 30077
```
- provide a username to the client
<br>*e.g. 'Alice'* (without the quotes)

## Control
At anytime during chat on either side you may type the command '/quit' and it will end the process. sending a SIGINT will do the same. 

## Clean up
To clean up the binaries in the directory
```
make clean
```


*This is written using github's flavored markdown*
