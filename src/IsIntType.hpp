#ifndef ISINTTYPE_HPP
#define ISINTTYPE_HPP

#include "IsAnyOfTypes.hpp"

/**
 * Check if template parameter is of integer type.
 * @tparam T parameter to check.
 * @return if @a T is an integer, structure provides a member constant @a value
 * equal to @p true. Otherwise @a value is @p false.
 */
template<typename T>
struct IsIntType:
	IsAnyOfTypes<T, short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long>
{
};

#endif

//(c)MP: Copyright © 2018, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
