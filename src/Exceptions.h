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


namespace CLIOpenSSLWrapper
{

    /*
     * Gets the description of the current `errno'.
     * Sometimes `errno' contains more information about the error that
     * previously occurred.
     */
    System::String^ GetErrnoDescription();

    public ref class SSLInitializationError : System::Exception
    {
    public:
        SSLInitializationError(System::String^ message)
            : System::Exception(message)
        {
        }
    };

    public ref class SSLConnectionError : System::Exception
    {
    public:
        SSLConnectionError(int errorCode);

        property System::String^ Message {
            virtual System::String^ get() override;
        }

    private:
        System::String^ message;
    };

    public ref class SSLWriteError : System::Exception
    {
    public:
        SSLWriteError(int errorCode);

        SSLWriteError(System::String^ message);

        property System::String^ Message {
            virtual System::String^ get() override;
        }

    private:
        System::String^ message;
    };

    public ref class SSLReadError : System::Exception
    {
    public:
        SSLReadError(int errorCode);

        SSLReadError(System::String^ message);

        property System::String^ Message {
            virtual System::String^ get() override;
        }

    private:
        System::String^ message;
    };

    public ref class SSLCommunicationError : System::Exception
    {
    public:
        SSLCommunicationError(System::String^ message)
            : System::Exception(message)
        {
        }
    };

    public ref class SSLCertificateError : System::Exception
    {
    public:
        SSLCertificateError(System::String^ message)
            : System::Exception(message)
        {
        }
    };
}
