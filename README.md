# capstone
Repository for the ENGR499 Capstone project.
Includes code for ESP8266 module in Arduino and socket listener written in C#.

Start C# listener. It will open a console window and begin waiting for an incoming TCP connection.
Power up Arduino device running SocketClient code, and open serial monitor to observe it's behaviour.

The Listener.cs program listens for an incoming connection on a specified port. When an incoming connection is detected, it accepts
the connection, creates a socket, and opens a data stream to the client. The server ("Listener") sends a handshake message then
waits for incoming data while consistently checking the status of the tcp connection. When data is received, it is printed to the
console (will either write to a file or display a graph in later versions). When the tcp connection is closed by the client, the
server closes the stream and socket and stops the listener, then prompts the user to decide whether to listen for a new incoming
connection or quit the application. 

SocketListener.ino initializes a TCP connection to a specified server, sends a message, receives a message, then sends 15 data
points at 1 point per second to the server, before closing the connection. In practical application, we will continuously transmit
data until the Arduino device is powered off or reset, or perhaps until another condition is true.
