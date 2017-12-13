CLIOpenSSLWrapper
-----------------
This is a wrapper around OpenSSL library, written using C++/CLI, so it can be used by any dotNet languages like C#, F#, VB.Net, and like that.

Why another wrapper?
--------------------
At this time, the only OpenSSL wrapper for dotNet is `openssl-net`. We used that in one of our enterprise applications, but we faced some unknown and strange problems. In some specific network configurations, it hungs when it tries to write data from its memory stream to the dotNet Socket. We debugged it to that point, and couldn't understand what's wrong with it. After two weeks of working on that problem, I decided to write my own wrapper. This time, instead of using a memory stream for connecting dotNet Socket and OpenSSL BIO, I write a new BIO for OpenSSL.

How is it work?
---------------
SSLContext is a simple wrapper around the OpenSSL's CTX object. It just initialize the CTX.

SSLStream wrapps a `.Net Socket`, and automatically encrypt messages before writing them to the socket, and decrypt them after reading them from the socket. It gives you a clean interface to work with SSL.

Inside, to make OpenSSL work with .Net Socket, there's a custom BIO. BIO is something that OpenSSL can write to and read from it. So, I wrote a BIO which can write to and read from .Net Socket, and pass it to OpenSSL. `BIO_mng_socks` files define the BIO, and `ManagedSocketToBIO` is the class that actually wrapps the `.Net Socket`.

What is supported?
------------------
Right now, it only gives you a `SSLStream` class that can be used to read and write to a SSL/TLS connection. Other functions of OpenSSL like `X509` related functions are not supported.

In must of cases, this is enougth. You can add customizations by modifiying the source code itself.

How to use it?
--------------
Create a `dotNet Socket` and establish a connection to your server. Then, pass it to the `SSLStream` class. Now, you can use `write` and `read` methods of the `SSLStream` class to send and receive data from/to server.

Note that the `dotNet Socket` should no longer be used.

To validate a server certificate, you can modify `SampleCertificateVerifier` function.

The project is created to compile with `dotNet framework 3.5`, so `Visual Studio 2008` should be installed on your system. You can change the setting if you want to use higher versions of dotNet.

*Important Note*: Unfortunately, right now, you have to call Dispose of `SSLStream' and `SSLContext' manually.

Here is an example code:

``` C#
using System;
using System.Collections.Generic;

namespace CLIOpenSSLWrapperSample
{
    class Program
    {
        static void Main(string[] args)
        {
            // Creating a TCP Stream socket.
            System.Net.Sockets.Socket clientSocket = new System.Net.Sockets.Socket(System.Net.Sockets.AddressFamily.InterNetwork,
                                                                                   System.Net.Sockets.SocketType.Stream,
                                                                                   System.Net.Sockets.ProtocolType.Tcp);
            // SSL Protocol requires the socket to be blocking.
            clientSocket.Blocking = true;

            // Connecting the socket to the server.
            clientSocket.Connect("192.168.22.1", 9081);

            // Wrapping the socket with a SSL Stream.
            CLIOpenSSLWrapper.SSLContext sslContext = new CLIOpenSSLWrapper.SSLContext();
            CLIOpenSSLWrapper.SSLStream sslStream = new CLIOpenSSLWrapper.SSLStream(sslContext, clientSocket);

            try
            {
                // Making a SSL connection (e.g. letting OpenSSL doing its handshake.)
                sslStream.Connect();

                // Sending six bytes of test data.
                byte[] testData = { 64, 12, 123, 222, 32, 41 };
                sslStream.Write(testData, 0, 6);

                // Receiving eight bytes of test data.
                byte[] receivedBuffer = sslStream.Read(8);
			}
            finally
            {
                sslStream.Dispose();
                sslContext.Dispose();
            }
        }
    }
}
```

Copyright
---------
Copyright 2016-2018 Aidin Gharibnavaz

CLIOpenSSLWrapper library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
