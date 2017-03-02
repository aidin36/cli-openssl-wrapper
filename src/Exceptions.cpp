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
#include "errno.h"
#include <string>

#include "Exceptions.h"

using namespace CLIOpenSSLWrapper;

System::String^ CLIOpenSSLWrapper::GetErrnoDescription()
{
    std::string errMessage(strerror(errno));
    return gcnew System::String(errMessage.c_str());
}

System::String^ GetErrorDescription(int errorCode)
{
    System::String^ message;

    if (errorCode == SSL_ERROR_ZERO_RETURN)
    {
        message = "SSL Connection closed.";
    }
    else if (errorCode == SSL_ERROR_SSL)
    {
        message = "SSL Protocol Error.";
    }
    else if (errorCode == SSL_ERROR_WANT_READ)
    {
        message = "SSLStream have some data to read, when we tried to write to it.";
    }
    else if (errorCode == SSL_ERROR_WANT_WRITE)
    {
        message = "SSLStream have some data to write, when we tried to read from it.";
    }
    else
    {
        message = System::String::Format("Error while connecting. Code: {0}", errorCode);
    }

    message = System::String::Format("{0}, errno: {1}", message, CLIOpenSSLWrapper::GetErrnoDescription());

    return message;
}

SSLConnectionError::SSLConnectionError(int errorCode)
{
    message = GetErrorDescription(errorCode);
}

System::String^ SSLConnectionError::Message::get()
{
    return message;
}

SSLWriteError::SSLWriteError(System::String^ message)
{
    this->message = message;
}

SSLWriteError::SSLWriteError(int errorCode)
{
    this->message = GetErrorDescription(errorCode);
}

System::String^ SSLWriteError::Message::get()
{
    return message;
}

SSLReadError::SSLReadError(System::String^ message)
{
    this->message = message;
}

SSLReadError::SSLReadError(int errorCode)
{
    this->message = GetErrorDescription(errorCode);
}

System::String^ SSLReadError::Message::get()
{
    return message;
}

