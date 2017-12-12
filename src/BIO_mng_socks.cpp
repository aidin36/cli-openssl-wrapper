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
#include "BIO_mng_socks.h"

#include <internal/bio.h>

#include "ManagedSocketToBIO.h"


/*
 * Froward declaring all required methods.
 */
static int mns_write(BIO* bio, const char* buf, int num);
static int mns_read(BIO* bio, char* buf, int size);
static int mns_puts(BIO* bio, const char* str);
static int mns_gets(BIO* bio, char* buf, int size);
static long mns_ctrl(BIO* bio, int cmd, long arg1, void* arg2);
static int mns_new(BIO* bio);
static int mns_free(BIO* bio);

/*
 * Method structure defines the interface of the BIO.
 */
static const BIO_METHOD methods_mng_socks = {
    BIO_TYPE_MNG_SOCKS,
    ".Net Managed Sockets",
    mns_write,
    mns_read,
    NULL, //mns_puts,
    NULL, //mns_gets,
    mns_ctrl,
    mns_new,
    mns_free,
    NULL,
};


const BIO_METHOD* BIO_mng_socks(void)
{
    return (&methods_mng_socks);
}

static int mns_new(BIO* bio)
{
    BIO_set_data(bio, NULL);
    BIO_set_init(bio, 1);

    return 1;
}

static int mns_free(BIO* bio)
{
    ManagedSocketToBIOHolder* holder = (ManagedSocketToBIOHolder*)BIO_get_data(bio);

    delete holder->ptr;
	holder->ptr = nullptr;
	delete holder;

    return 1;
}

static int mns_write(BIO* bio, const char* buf, int num)
{
    ManagedSocketToBIOHolder* holder = (ManagedSocketToBIOHolder*)BIO_get_data(bio);

    return holder->ptr->write(buf, num);
}

static int mns_read(BIO* bio, char* buf, int size)
{
    ManagedSocketToBIOHolder* holder = (ManagedSocketToBIOHolder*)BIO_get_data(bio);

    return holder->ptr->read(buf, size);
}

//static int mns_puts(BIO* bio, const char* str)
//{
//    return 0;
//}
//
//static int mns_gets(BIO* bio, char* buf, int size)
//{
//    return 0;
//}

static long mns_ctrl(BIO* bio, int cmd, long arg1, void* arg2)
{
    // I don't know exactly what `cmd' and return values mean!
    // I just found out that OpenSSL expect these values (by debugging through it).
    // ~ Aidin

    if (cmd == BIO_CTRL_FLUSH)
    {
        return 1;
    }
    if (cmd == BIO_CTRL_PUSH)
    {
        return 0;
    }
    else if (cmd == BIO_CTRL_POP)
    {
        return 0;
    }

    return 1;
}
