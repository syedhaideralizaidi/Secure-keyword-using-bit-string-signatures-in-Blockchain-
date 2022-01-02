#include "blockchain.h"
#include "stdafx.h"

blockchain::blockchain() : m_current_index(-1), m_bchain{}, m_input_output(blockchain::enableInputOutput ? std::make_unique<input_output>() : nullptr)
{
	if (enableInputOutput) {
		if (m_input_output->blockFileExists()) {
			m_input_output->importBlockFile(*this);
			return;
		}
		else {
			m_input_output->initialize();
		}
	}
	createGenesisBlock();
}

int blockchain::getIndex() const
{
	return m_current_index;
}

const block* blockchain::getBlockByIndex(int index) const
{
	if (index >= 0 && index <= m_current_index) {
		return m_bchain[index].get();
	}
	else {
		return nullptr;
	}
}

const block* blockchain::getLastBlock() const
{
	if (m_current_index >= 0) {
		return m_bchain[m_current_index].get();
	}
	else {
		return nullptr;
	}
}

bool blockchain::validateBlockchain() const
{
	if (m_bchain.empty()) {
		return false;
	}

	for (auto i = m_bchain.size() - 1; i > 0; --i) {
		const block * currentBlock = m_bchain[i].get();
		const block * previousBlock = m_bchain[i - 1].get();	
		if (currentBlock->getPreviousBlockHash() !=  previousBlock->getBlockHash()) {
			return false;
		}
	}
	return true;
}

bool blockchain::validateBlockAtIndex(int index) const
{
	if (index == 0) {
		return true;
	}
	if (index < 0 || index > m_current_index) {
		return false;
	}

	return m_bchain[index]->getPreviousBlockHash() == m_bchain[index - 1]->getBlockHash();
}

bool blockchain::validateNewBlock(const std::unique_ptr<block>& new_block)
{
	if (m_bchain.empty()) {
		return true;
	}
	
	return new_block->getPreviousBlockHash() == m_bchain.back()->getBlockHash();
}

void blockchain::createGenesisBlock()
{
	using namespace std::string_literals;

	const std::string genesisPreviousHash = "0"s;
	const int genesisDifficulty = 3;
	const std::uint32_t gensisNonce = 1000;
	const std::string genesisData = "Genesis Block"s;

	auto genesisBlock = std::make_unique<block>(genesisPreviousHash, gensisNonce, genesisDifficulty, genesisData);

	m_bchain.push_back(std::move(genesisBlock));
	++m_current_index;

	if (enableInputOutput) {
		m_input_output->saveBlock(m_bchain.back()->blockToJson());
	}

	std::cout << "Created genesis block #" << m_current_index << '\n';
	std::cout << "Genesis block hash: " << m_bchain.back()->getBlockHash() << '\n';
	std::cout << "Data encapsulated: " << m_bchain.back()->getBlockData() << '\n';
	std::cout << "Number of blocks: " << getNumOfBlocks() << '\n';
	std::cout << "Chain initialized\n\n";
}

bool blockchain::createBlock(const std::string &data)
{
	auto new_block = std::make_unique<block>(m_bchain.back()->getBlockHash(), 0, m_bchain.back()->getDifficulty(), data);

	if (validateNewBlock(new_block)) {
		m_bchain.push_back(std::move(new_block));
		++m_current_index;

		if (enableInputOutput) {
			m_input_output->saveBlock(m_bchain.back()->blockToJson());
		}

		std::cout << "Created and validated block #" << m_current_index << '\n';
		std::cout << "Block hash: " << m_bchain.back()->getBlockHash() << '\n';
		std::cout << "Data encapsulated: " << m_bchain.back()->getBlockData() << '\n';
		std::cout << "Previous block hash: " << m_bchain.back()->getPreviousBlockHash() << "\n";
		std::cout << "Number of blocks: " << getNumOfBlocks() << '\n';
		return true;
	}
	else {
		std::cerr << "Failed to validate block #" << m_current_index << '\n';
		return false;
	}
}

bool blockchain::importBlock(const nlohmann::json & dataJson)
{
	auto new_block = std::make_unique<block>(dataJson);
	
	if (validateNewBlock(new_block)) {
		//import block		
		m_bchain.push_back(std::move(new_block));
		++m_current_index;
		std::cout << "Imported and validated block #" << m_current_index << '\n';
		std::cout << "Block hash: " << m_bchain.back()->getBlockHash() << '\n';
		std::cout << "Data encapsulated: " << m_bchain.back()->getBlockData() << '\n';
		std::cout << "Previous block hash: " << m_bchain.back()->getPreviousBlockHash() << "\n";
		std::cout << "Number of blocks: " << getNumOfBlocks() << '\n';
		return true;
	}
	else {
		std::cerr << "Failed to validate block #" << m_current_index << '\n';
		return false;
	}
}

// returns number of blocks
std::size_t blockchain::getNumOfBlocks() const
{
	return m_bchain.size();
}
