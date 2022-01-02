#ifndef BLOCK_H
#define BLOCK_H

#include "stdafx.h"

class blockchain;
class block
{
private:	
	time_t m_timestamp;
	std::string m_previous_hash;
	std::string m_current_hash;		
	std::uint32_t m_nonce;	
	int m_difficulty;
	std::string m_data;

	std::string createStringForHashing() const;
	void generateBlockHash();
public:
	block() = delete;
	block(const std::string &previousHash, std::uint32_t nonce, int difficulty, const std::string &data);
	block(const nlohmann::json &JsonData);
	~block() = default;
	
	std::uint32_t getNonce() const;
	std::string getBlockHash() const;
	std::string getPreviousBlockHash() const;
	int getDifficulty() const;
	std::string getBlockData() const;
	time_t getTimestamp() const;

	nlohmann::json blockToJson() const;			
};

#endif