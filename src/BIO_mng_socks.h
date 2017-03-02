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

/*
 * Defines a custom BIO for Openssl, which wraps a .Net Managed Socket.
 */


#pragma once

#include <openssl/bio.h>


#ifdef  __cplusplus
extern "C" {
#endif

# define BIO_TYPE_MNG_SOCKS ( 98|BIO_TYPE_SOURCE_SINK|BIO_TYPE_DESCRIPTOR)

const BIO_METHOD* BIO_mng_socks(void);


#ifdef  __cplusplus
};
#endif
