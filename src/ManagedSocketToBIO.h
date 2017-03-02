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

#include <vcclr.h>

#include <openssl/ssl.h>


/*
 * An adapter between the managed socket and BIO.
 *
 * You should not create an instance directly from it.
 * Use ManagedSocketBIOFactory instead.
 */
ref class ManagedSocketToBIO
{
    ref class PrivateData;

public:
    /*
     * @param sock: Socket to wrap.
     */
    ManagedSocketToBIO(System::Net::Sockets::Socket^ sock);

    ~ManagedSocketToBIO();

    /*
     * Creates a new instance of the BIO, wrapped around the specified socket.
     */
    BIO* createBio();

    int write(const char* buf, int num);

    int read(char* buf, int size);

private:
    System::Net::Sockets::Socket^ internalSocket;
};


/*
 * Holds a pointer to the ManagedSocketToBIO.
 * This is used to create a native pointer to that managed class, in order
 * to pass it to the BIO.
 */
struct ManagedSocketToBIOHolder
{
    gcroot<ManagedSocketToBIO^> ptr;
};
