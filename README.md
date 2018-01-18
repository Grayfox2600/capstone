# capstone
Repository for the ENGR499 Capstone project.

Includes code for ESP8266 module in Arduino and socket listener written in C#.

Start C# listener. It will open a console window and begin waiting for an incoming TCP connection.
Power up Arduino device running SocketClient code, and open serial monitor to observe it's behaviour.

The Listener program is in early development and simply accepts a connection, sends a message, receives a message, and checks if the TCP
connection is open. If so, continue receiving data. If not, close the stream and socket, stop the listener, then start listening for a new
TCP connection.

SocketListener simply initializes a TCP connection to a specified server, sends a message, receives a message, then closes the connection
after a timeout period before starting again. The purpose of the timeout period is to test the capability of the listener application to
determine the state of the TCP connection. In practical application, we will continuously transmit data until the Arduino device is powered
off or reset, or perhaps until another condition is true.
