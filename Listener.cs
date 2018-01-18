using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.IO;
using System.Net.NetworkInformation;

namespace Listener
{
    static class Listener
    {
        public static void Main(string[] args)
        {
            Byte[] ip = { 192, 168, 0, 10 }; //Local IP
            IPAddress ipAddr = new IPAddress(ip);
            int port = 8858; //Local port

            while (true)
            {
                TcpListener listener = new TcpListener(ipAddr, port);
                listener.Start();

                if (!listener.Pending())
                {
                    int i = 1;
                    Console.WriteLine("server> No incoming connection");
                    while (!listener.Pending())
                    {
                       Thread.Sleep(500);
                       Console.Write(".");
                       if (i % 30 == 0)
                       {
                           Console.WriteLine();
                       }
                        i++;
                    }
                    Console.WriteLine();
                }

                Socket sock = listener.AcceptSocket(); //Blocks until incoming connection

                Console.WriteLine("server> Connected to {0} on local port {1}", sock.RemoteEndPoint, port);
                NetworkStream stream = new NetworkStream(sock);
                StreamReader sr = new StreamReader(stream);
                StreamWriter sw = new StreamWriter(stream);
                sw.AutoFlush = true;
                string data_in = "";

                try
                {
                    sw.WriteLine("server> Server says hello");

                    while (GetState(listener) == TcpState.Established)
                    {
                        data_in = sr.ReadLine(); //doesn't need to block since we can check state of tcp connection
                        Console.WriteLine(data_in); //displays contents of data_in buffer
                    }
                    Console.WriteLine("server> Stream closed");
                    stream.Close();
                }
                catch (Exception e)
                {
                    Console.WriteLine("server> Some exception occured:\n" + e.Message);
                }

                Console.WriteLine("server> {0} disconnected", sock.RemoteEndPoint);
                sock.Close();
                listener.Stop();

            }           

        }
        public static TcpState GetState(this TcpListener tcpListener)
        {
            var foo = IPGlobalProperties.GetIPGlobalProperties().GetActiveTcpConnections().FirstOrDefault(x => x.LocalEndPoint.Equals(tcpListener.Server.LocalEndPoint));
            return foo != null ? foo.State : TcpState.Unknown;
        }

    }
}
