# client_server_portable
TCP/IP Linux and Windows sockets client / Server example.


## Environment

Tested in Ubuntu 18.04 with mingw and wine
sudo apt-get install mingw-w64 
sudo apt install wine-stable

## Example

The starting example was taken from here:

https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

This example is very basic. Other socket related calls might need Windows-specific calls.

To run server/client:
```
wine ./server.exe
wine ./client.exe
```

Exit with 'exit' on both sides.


