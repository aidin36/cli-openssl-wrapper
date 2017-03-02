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

#include "SSLContext.h"
#include "SampleCertificateVerifier.h"


using namespace CLIOpenSSLWrapper;


SSLContext::SSLContext()
{
    sslCtxPtr = SSL_CTX_new(TLS_client_method());

    if (sslCtxPtr == NULL)
    {
        throw gcnew System::Exception("Could not create SSL Context!");
    }

    // Use no version lesser that TLSv1.
    SSL_CTX_set_min_proto_version(sslCtxPtr, TLS1_VERSION);

    SSL_CTX_set_verify(sslCtxPtr, SSL_VERIFY_PEER, SampleCertificateVerifier);
};

SSLContext::~SSLContext()
{
    // `new' method will increase reference by one. `free' will decrease it.
    SSL_CTX_free(sslCtxPtr);
};


SSL_CTX* SSLContext::GetRawContext()
{
    return sslCtxPtr;
}
