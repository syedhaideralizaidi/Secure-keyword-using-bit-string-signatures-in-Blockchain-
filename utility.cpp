#include "utility.h"
#include "stdafx.h"

std::string utility::shaHash(const std::string & data)
{
	switch (utility::sha_mode)
	{
	case utility::SHA2_MODE::SHA224:
		return sha224(data);
	case utility::SHA2_MODE::SHA256:
		return sha256(data);
	case utility::SHA2_MODE::SHA384:
		return sha384(data);
	case utility::SHA2_MODE::SHA512:
		return sha512(data);
	default:
		return std::string();
	}
}
