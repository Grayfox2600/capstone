using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.IO;

namespace Listener
{
    class Listener
    {
        public static void StartListening()
        {
            Byte[] ip = { 127, 0, 0, 1 };
            int port = 8858;
            IPAddress ipAddr = new IPAddress(ip);
            TcpListener listener = new TcpListener(ipAddr, port);

            listener.Start();
            if (!listener.Pending())
            {
                int i = 1;
                Console.WriteLine("/server> No incoming connection" );
                while (!listener.Pending())
                {
                    Thread.Sleep(500);
                    Console.Write(".");
                    if (i % 30 == 0) {
                        Console.WriteLine();                        
                    }
                    i++;
                }
                Console.WriteLine();
            }
            
            Socket sock = listener.AcceptSocket(); //blocks
            Stream stream = new NetworkStream(sock);
            StreamWriter sw = new StreamWriter(stream);
            StreamReader sr = new StreamReader(stream);

            Console.WriteLine("/server> Connected to {0} on remote port {1} and local port {2}", 
                IPAddress.Parse(((IPEndPoint) sock.RemoteEndPoint).Address.ToString()), 
                ((IPEndPoint) sock.RemoteEndPoint).Port.ToString(),
                port);

            if (sock.Poll(2000, SelectMode.SelectRead))
            {
                Console.WriteLine("/server> Socket is readable");
                sw.WriteLine("Socket is readable");
            }
            else
                Console.WriteLine("/server> Socket is not readable");

            if (sock.Poll(2000, SelectMode.SelectWrite))
            {
                Console.WriteLine("/server> Socket is writeable");
                sw.WriteLine("Socket is writeable");
            }
            else
                Console.WriteLine("/server> Console is not writeable");
            
            sw.AutoFlush = true;
            sw.WriteLine("Welcome from server side!");
            sw.Flush();

            while (sock.Connected)
            {
                try
                {                   
                    Console.WriteLine("/server> " + sr.ReadLine()); //should block
                    sw.Write("");
                    Thread.Sleep(100);
                }
                catch (System.IO.IOException)
                {
                    Console.WriteLine("/server> Connection terminated");
                }
               
            }
            
            stream.Close();
            sock.Close();
            Thread.Sleep(10000);

        }

        /*static void Main(string[] args)
        {
            StartListening();
        }*/

    }
}
