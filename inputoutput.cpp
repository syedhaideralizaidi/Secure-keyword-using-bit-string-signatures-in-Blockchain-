#include "stdafx.h"

input_output::input_output() : m_blockchain_path("C:\\blockchaincpp\\blockchain.dat"s)
{
	/*
	std::cout << m_blockchain_path.root_name() << '\n';
	std::cout << m_blockchain_path.root_directory() << '\n';
	std::cout << m_blockchain_path.root_path() << '\n';
	std::cout << m_blockchain_path.relative_path() << '\n';
	std::cout << m_blockchain_path.parent_path() << '\n';
	std::cout << m_blockchain_path.filename() << '\n';
	*/
}

void input_output::importBlockFile(blockchain &chain) 
{
	std::string importedString;
	m_blockchain_fstream.open(m_blockchain_path);
	while (std::getline(m_blockchain_fstream, importedString))
	{
		auto JsonData = nlohmann::json::parse(importedString);
		chain.importBlock(JsonData);
	}

	m_blockchain_fstream.close();
}

bool input_output::blockDirectoryExists() const
{
	bool bc_dir_exists = std::filesystem::exists(m_blockchain_path.parent_path());
	if (bc_dir_exists) {
		std::cout << "Blockhain directory found\n";
		return true;
	}
	else {
		std::cout << "Blockhain directory not found\n";
		return false;
	}	 
}

bool input_output::blockFileExists() const
{
	bool bc_file_exists = std::filesystem::exists(m_blockchain_path);
	if (bc_file_exists) {
		std::cout << "Blockhain file found\n";
		return true;
	}
	else {
		std::cout << "Blockhain file not found\n";
		return false;
	}
}

bool input_output::initialize() const
{
	//check if blockchain directory exists
	if (!blockDirectoryExists()) {		
		bool bc_dir = std::filesystem::create_directories(m_blockchain_path.parent_path());
		if (bc_dir) {
			std::cout << "Created Blockchain directory\n";
		}
		else {
			std::cerr << "Error: Unable to create Blockchain directory\n";
			return false;
		}
	}
		
	return true;
}

bool input_output::deleteBlockFile() const
{
	bool remove_result = std::filesystem::remove(m_blockchain_path);
	if (remove_result) {
		std::cout << "Deleted Blockhain file\n";
		return true;
	}
	else {
		std::cerr << "Error: Unable to delete Blockhain file\n";
		return false;
	}
}

void input_output::saveBlock(nlohmann::json JsonBlockData) 
{
	m_blockchain_fstream.open(m_blockchain_path, std::ios::app);
	m_blockchain_fstream << JsonBlockData;
	m_blockchain_fstream << '\n';
	m_blockchain_fstream.close();
}

