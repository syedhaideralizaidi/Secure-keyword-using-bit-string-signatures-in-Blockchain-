#include "stdafx.h"

block::block(const std::string &previousHash, std::uint32_t nonce, int difficulty, const std::string & data) :
	m_timestamp(std::time(nullptr)),
	m_previous_hash{ previousHash },
	m_current_hash{},
	m_nonce(nonce),	
	m_difficulty(difficulty),
	m_data{ data }
{	
	generateBlockHash();
}

block::block(const nlohmann::json &JsonData) : 
	m_timestamp(JsonData[0]),
	m_previous_hash(JsonData[1].get<std::string>()),
	m_current_hash(JsonData[2].get<std::string>()),
	m_nonce(JsonData[3]),
	m_difficulty(JsonData[4]),
	m_data(JsonData[5].get<std::string>())
{	
}

std::string block::createStringForHashing() const
{
	//Stringify block so it can be hashed
	std::string buffer;

	buffer += std::to_string(m_timestamp);
	buffer += m_previous_hash;
	buffer += std::to_string(m_difficulty);
	buffer += m_data;

	return buffer;
}

void block::generateBlockHash()
{
	std::string zeros(m_difficulty, '0');
	std::string stringForHashing = createStringForHashing();

	for (;;)
	{
		std::string dataToHash = stringForHashing + std::to_string(m_nonce);
		std::string hash = utility::shaHash(dataToHash);

		if (hash.substr(0, m_difficulty) != zeros) {
			++m_nonce;
		}
		else {
			//std::cout << "Hash found: " << hash << " Nonce: " << m_nonce << '\n'; 
			m_current_hash = hash;				
			return;
		}
	}
}

std::string block::getBlockHash() const
{
	return m_current_hash;
}

std::string block::getPreviousBlockHash() const
{
	return m_previous_hash;
}

std::uint32_t block::getNonce() const
{
	return m_nonce;
}

int block::getDifficulty() const
{
	return m_difficulty;
}

std::string block::getBlockData() const
{
	return m_data;
}

time_t block::getTimestamp() const
{
	return m_timestamp;
}

nlohmann::json block::blockToJson() const
{
	nlohmann::json JsonBlockData;

	JsonBlockData[0] = m_timestamp;
	JsonBlockData[1] = m_previous_hash;
	JsonBlockData[2] = m_current_hash;
	JsonBlockData[3] = m_nonce;
	JsonBlockData[4] = m_difficulty;
	JsonBlockData[5] = m_data;

	return JsonBlockData;
}
