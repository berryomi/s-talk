## s-talk (Simple Talk)

A multi-threaded "chat"-like facility that enables communication between two terminals. 

## Testing

Type in command line:
```
./s-talk [my port number] [remote machine name] [remote port number] 
```

For example, one communicator (machine321, port number 5000) types:
```
./s-talk 5000 machine123 6000
```
And the other communicator (machine123, port number 6000) types:
```
./s-talk 6000 machine321 5000 
```


The application terminates when either user enters '!'.