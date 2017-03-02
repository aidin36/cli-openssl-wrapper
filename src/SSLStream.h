/*
 *  CLIOpenSSLWrapper <https://github.com/aidin36/cli-openssl-wrapper>
 *  Copyright (C) 2016,2017 Aidin Gharibnavaz <aidin@aidinhut.com>
 *
 *  CLIOpenSSLWrapper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CLIOpenSSLWrapper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with CLIOpenSSLWrapper. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <openssl/ssl.h>

#include "SSLContext.h"
#include "ManagedSocketToBIO.h"


namespace CLIOpenSSLWrapper
{
    public ref class SSLStream
    {
    public:
        SSLStream(SSLContext^ context, System::Net::Sockets::Socket^ sock);

        ~SSLStream();

        void Connect();

        /*
         * Writes the specified buffer to the stream.
         *
         * Offset and limit will apply to the specified buffer. For example,
         * offset=5, limit=12 means pick bytes from 5 to 17 from buf and write
         * them.
         */
        int Write(array<System::Byte>^ buf, int offset, int limit);

        /*
         * Reads data from the stream.
         * @param num: Number of bytes to read.
         */
        array<System::Byte>^ Read(int num);

    private:
        SSL* sslPtr;
        ManagedSocketToBIO^ managedSock;
        BIO* wrappedBio;
    };
}
