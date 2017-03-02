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

#include "Stdafx.h"

#include <algorithm>

#include "ManagedSocketToBIO.h"
#include "BIO_mng_socks.h"
#include "Exceptions.h"
#include "ErrorObject.h"


ManagedSocketToBIO::ManagedSocketToBIO(System::Net::Sockets::Socket^ sock)
{
    this->internalSocket = sock;
}

ManagedSocketToBIO::~ManagedSocketToBIO()
{
}

BIO* ManagedSocketToBIO::createBio()
{
    //ManagedSocketToBIOHolder* holder = (ManagedSocketToBIOHolder*)calloc(1, sizeof(ManagedSocketToBIOHolder));
    ManagedSocketToBIOHolder* holder = new ManagedSocketToBIOHolder();

    holder->ptr = this;

    BIO* bio = BIO_new(BIO_mng_socks());

    if (bio == NULL)
    {
        throw gcnew CLIOpenSSLWrapper::SSLInitializationError(
            System::String::Format("Could not create BIO! errno: {0}", CLIOpenSSLWrapper::GetErrnoDescription()));
    }

    BIO_set_data(bio, holder);

    return bio;
}

int ManagedSocketToBIO::write(const char* buf, int num)
{
    // TODO: Maybe we could use __refvalue to avoid copying the buffer.

    array<System::Byte>^ byteArray = gcnew array<System::Byte>(num);
    System::IntPtr bufPtr((void*)buf);
    System::Runtime::InteropServices::Marshal::Copy(bufPtr, byteArray, 0, num);

    int wroteBytes = -1;

    try
    {
        wroteBytes = this->internalSocket->Send(byteArray, num, System::Net::Sockets::SocketFlags::None);
    }
    catch (System::Exception^ error)
    {
        ErrorObject::Exception = error;
        return -1;
    }

    if (wroteBytes != num)
    {
        ErrorObject::Exception = 
            gcnew CLIOpenSSLWrapper::SSLCommunicationError(System::String::Format("Tried to sending {0} bytes using managed socket, but {0} sent.", num, wroteBytes));
        return -1;
    }

    return wroteBytes;
}

int ManagedSocketToBIO::read(char* buf, int size)
{
    array<System::Byte>^ byteArray = gcnew array<System::Byte>(size);

    int readBytes = -1;

    try
    {
        readBytes = this->internalSocket->Receive(byteArray, size, System::Net::Sockets::SocketFlags::None);
    }
    catch (System::Exception^ error)
    {
        ErrorObject::Exception = error;
        return -1;
    }

    System::IntPtr bufPtr(buf);
    System::Runtime::InteropServices::Marshal::Copy(byteArray, 0, bufPtr, readBytes);

    return readBytes;
}
