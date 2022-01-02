#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "stdafx.h"
#include <fstream>
#include <filesystem>

using namespace std::string_literals;

class input_output {
private:
	const std::filesystem::path m_blockchain_path;
	std::fstream m_blockchain_fstream;
public:
	input_output();
	~input_output() = default;

	void importBlockFile(blockchain & chain);
	bool blockDirectoryExists() const;
	bool blockFileExists() const;
	bool initialize() const;
	bool deleteBlockFile() const;
	void saveBlock(nlohmann::json JsonBlockData);
};

#endif