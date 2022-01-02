#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "stdafx.h"
#include "inputoutput.h"

class blockchain
{
	static constexpr bool enableInputOutput = true;
private:
	
	int m_current_index; //blocks are 0 indexed	
	std::vector<std::unique_ptr<block>> m_bchain;
	std::unique_ptr<input_output> m_input_output;

public:
	blockchain();	
	~blockchain() = default;

	int getIndex() const;	
	const block* getBlockByIndex(int index) const;
	const block* getLastBlock() const;

	bool validateBlockchain() const;
	bool validateBlockAtIndex(int index) const;
	bool validateNewBlock(const std::unique_ptr<block>& new_block);

	void createGenesisBlock();
	bool createBlock(const std::string &data);
	bool importBlock(const nlohmann::json & dataJson);

	std::size_t getNumOfBlocks() const;
};

#endif