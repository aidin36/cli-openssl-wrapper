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

#include "SampleCertificateVerifier.h"

#include <string>
#include "ErrorObject.h"
#include "Exceptions.h"


int CLIOpenSSLWrapper::SampleCertificateVerifier(int preverifyOK, X509_STORE_CTX* certificateStore)
{
    // I don't know why this method calls twice. This if is existed to
    // prevent checking the certificate twice.
    if (preverifyOK == 1)
    {
        return 1;
    }

    X509* serverCert = X509_STORE_CTX_get0_cert(certificateStore);

    // Checking certificate expiration date.
    if (X509_cmp_current_time(X509_get_notAfter(serverCert)) < 0)
    {
        ErrorObject::Exception = gcnew CLIOpenSSLWrapper::SSLCertificateError("Received certificate is expired.");
        return 0;
    }

    return 1;
};
