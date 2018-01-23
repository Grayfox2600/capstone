using System;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;
using System.Net.NetworkInformation;

namespace Listener
{
    static class Listener
    {
        public static void Main(string[] args)
        {
            Byte[] ip = { 192, 168, 0, 153 };            //Local IP
            IPAddress ipAddr = new IPAddress(ip);
            int port = 8858;                            //Local port
            bool reconn = true;                         //Boolean switch for looping

            while (reconn)                      //Loop until reconnect evaluates to false
            {
                TcpListener listener = new TcpListener(ipAddr, port);

                try                         //Catch an exception if local IP is incorrect or invalid
                {
                    listener.Start();
                }
                catch (System.Net.Sockets.SocketException)
                {
                    Console.WriteLine("Exception: Invalid local IP address");
                    Thread.Sleep(1000);
                    return;
                }           

                if (!listener.Pending())        //If no connection incoming:
                {
                    int i = 1;
                    Console.WriteLine("server> No incoming connection");        //Inform user we do not have a connection yet
                    while (!listener.Pending())
                    {
                       Thread.Sleep(500);
                       Console.Write(".");      //Print "." every half-second while waiting, to show the program is still running
                       if (i % 30 == 0)         //Only 30 "." per line
                       {
                           Console.WriteLine();
                       }
                        i++;
                        if (i >= 90)
                        {
                            Console.WriteLine("\nConnection timeout");
                            return;
                        }
                    }
                    Console.WriteLine();
                }

                Socket sock = listener.AcceptSocket(); //Normally blocks until incoming connection, but above code runs until connection available

                Console.WriteLine("server> Connected to {0} on local port {1}", sock.RemoteEndPoint, port);
                NetworkStream stream = new NetworkStream(sock);     //Open network stream on top of socket
                StreamReader sr = new StreamReader(stream);         //Create stream reader on top of stream
                StreamWriter sw = new StreamWriter(stream);         //Create stream writer on top of stream
                sw.AutoFlush = true;                                //Turn on autoflush for stream writer to automatically clear the write buffer
                string data_in = "";                //This is our stream input buffer, may not need this if we are ONLY reading directly to console

                try
                {
                    sw.WriteLine("server> Server says hello");

                    while (GetState(listener) == TcpState.Established)  //Run this while the TCP connection state is established
                    {
                        data_in = sr.ReadLine();        //doesn't need to block since we can check state of tcp connection
                        Console.WriteLine(data_in);     //displays contents of data_in buffer
                    }
                    Console.WriteLine("server> Stream closed");     //When client has severed tcp connection, inform user that the stream is closing (mostly unnecessary)
                    stream.Close();     //Close stream
                }
                catch (Exception e)
                {
                    Console.WriteLine("server> Some exception occured:\n" + e.Message);     //Catch any potential exception from reading the stream
                }

                Console.WriteLine("server> {0} disconnected", sock.RemoteEndPoint);     //Inform user of disconnection
                sock.Close();           //Close socket
                listener.Stop();        //Stop listener

                bool  verifyIn = false;     //Boolean for input verification
                while (!verifyIn)
                {
                    Console.WriteLine("Enter y to listen for another connection, or n to quit:");
                    char loop = Console.ReadKey().KeyChar;              //Read single char key input
                    if (loop == ('y'))                                  //Some input verification
                    {
                        Console.WriteLine("\nListening for new connection...");
                        verifyIn = true;
                    }
                    else if (loop == 'n')
                    {
                        Console.WriteLine("\nClosing program...");
                        verifyIn = true;
                        reconn = false;
                    }
                    else
                        Console.WriteLine("\nInvalid input, try again");
                }

            }

            return;       

        }
        public static TcpState GetState(this TcpListener tcpListener)       //This is a method to check the state of the tcp connection
        {
            var connInfo = IPGlobalProperties.GetIPGlobalProperties().GetActiveTcpConnections().FirstOrDefault(x => x.LocalEndPoint.Equals(tcpListener.Server.LocalEndPoint));   //This is wizardry. Returns TcpConnectionInformation object
            return connInfo != null ? connInfo.State : TcpState.Unknown;      //If TcpConnectionInformation object is null return state "unknown", else return the actual state. Will return "established" if connection is live.
        }

    }
}
