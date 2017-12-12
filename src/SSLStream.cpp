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

#include "SSLStream.h"
#include "Exceptions.h"
#include "ManagedSocketToBIO.h"
#include "ErrorObject.h"


using namespace CLIOpenSSLWrapper;


SSLStream::SSLStream(SSLContext^ context, System::Net::Sockets::Socket^ sock)
{
    if (context->GetRawContext() == NULL)
    {
        throw gcnew System::Exception("Context pointer is NULL!");
    }

    sslPtr = SSL_new(context->GetRawContext());

    if (sslPtr == NULL)
    {
        throw gcnew SSLInitializationError(System::String::Format("Could not create SSL Object! errno: {0}", GetErrnoDescription()));
    }

    this->managedSock = gcnew ManagedSocketToBIO(sock);
    BIO* wrappedBio = managedSock->createBio();

    SSL_set_bio(sslPtr, wrappedBio, wrappedBio);
}

SSLStream::~SSLStream()
{
	SSL_free(sslPtr);
    BIO_free(this->wrappedBio);
}

void SSLStream::Connect()
{
    int connectResult = SSL_connect(sslPtr);

    if (connectResult != 1)
    {
        if (ErrorObject::Exception != nullptr)
        {
            // This is one of our errors, not SSL.
            System::Exception^ error = ErrorObject::Exception;
            ErrorObject::Exception = nullptr;
            throw error;
        }

        // SSL error.
        int errorCode = SSL_get_error(sslPtr, connectResult);

        throw gcnew SSLConnectionError(errorCode);
    }
}

int SSLStream::Write(array<System::Byte>^ buf, int offset, int limit)
{
    char* nativeBuffer = new char[limit];

    try
    {
        System::IntPtr nativeBufPtr(nativeBuffer);
        System::Runtime::InteropServices::Marshal::Copy(buf, offset, nativeBufPtr, limit);

        int writeResult = SSL_write(this->sslPtr, nativeBuffer, limit);

        if (writeResult <= 0)
        {
            if (ErrorObject::Exception != nullptr)
            {
                // This is one of our errors, not SSL.
                System::Exception^ error = ErrorObject::Exception;
                ErrorObject::Exception = nullptr;
                throw error;
            }

            // SSL error.
            int errorCode = SSL_get_error(sslPtr, writeResult);

            throw gcnew SSLWriteError(errorCode);
        }

        if (writeResult != limit)
        {
            throw gcnew SSLCommunicationError(System::String::Format("Tried to write {0}, but {1} wrote.", limit, writeResult));
        }

        return writeResult;
    }
    finally
    {
        delete nativeBuffer;
    }
}

array<System::Byte>^ SSLStream::Read(int num)
{
    //char* nativeBuffer = (char*)calloc(num, sizeof(char*));
    char* nativeBuffer = new char[num];

    try
    {
        int readResult = SSL_read(this->sslPtr, nativeBuffer, num);

        if (readResult <= 0)
        {
            if (ErrorObject::Exception != nullptr)
            {
                // This is one of our errors, not SSL.
                System::Exception^ error = ErrorObject::Exception;
                ErrorObject::Exception = nullptr;
                throw error;
            }

            // SSL error.
            int errorCode = SSL_get_error(sslPtr, readResult);

            throw gcnew SSLReadError(errorCode);
        }

        array<System::Byte>^ byteArray = gcnew array<System::Byte>(readResult);
        System::IntPtr nativeBufPtr((void*)nativeBuffer);
        System::Runtime::InteropServices::Marshal::Copy(nativeBufPtr, byteArray, 0, readResult);

        return byteArray;
    }
    finally
    {
        delete nativeBuffer;
    }
}
