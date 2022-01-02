#ifndef UTILITY_H
#define UTILITY_H

#include "stdafx.h"

namespace utility
{
	enum class SHA2_MODE {
		SHA224,
		SHA256,
		SHA384,
		SHA512
	};

	constexpr auto sha_mode = SHA2_MODE::SHA256;

	std::string shaHash(const std::string & data);
}

#endif