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

/*
 * This class holds the last occurred error.
 * This object make it possible to pass managed errors through unmanaged layer.
 *
 * The `Exception' field inside this object contains the error. This field
 * is unique for each thread.
 */
ref class ErrorObject
{
public:

    [System::ThreadStatic]
    static System::Exception^ Exception;
};
