// simpleblockchain.cpp : Defines the entry point for the console application.
#include<functional>
#include "stdafx.h"
#include <iostream>
using namespace std;
#include <string>
#include "bloom_filter.hpp"

block searchFunction(blockchain& bc, blockchain& bc2, bloom_parameters& bp, bloom_filter& bf, string x) {


    if (bf.contains(x)) {
        int i = 0;
        for (; i < bc2.getNumOfBlocks(); i++) {
            const block* bc_ptr = bc2.getBlockByIndex(i);
            string s = bc_ptr->getBlockData();
            int j = 0;
            for (; j < s.length(); j++) {
                if (s[j] == '=') {
                    break;
                }
            }
            string s1 = "";
            for (; j < s.length();j ++) {
                s1 += s[j];
            }
            if (s1 == x) {
                cout << "Record exists\n";
                return *bc2.getBlockByIndex(i);
            }
        }
    }
    else {
        cout << "Key doesnot exist\n";
    }

}


void insertionFunction(blockchain& bc, blockchain& bc2, bloom_parameters& bp ,bloom_filter& bf, string x, string emr) {

   
    std::size_t bit_index = 0;
    std::size_t bit = 0;
 
    //Passing Our key Value into k hash functions
    for (std::size_t i = 0; i < bf.salt_.size(); ++i)
    {
        bf.compute_indices(bf.hash_ap(reinterpret_cast<const unsigned char*>(x.data()), x.size(), bf.salt_[i]), bit_index, bit);
        cout << "Bit Index" << bit_index << endl;
    }
    
    bf.insert(x);
    
    //We have used the number of block as transaction id
    int tx_id= bc.getNumOfBlocks();

    //Insertion of "Data" concatenated with transaction id and Emr 
    string D = "Data:";
    D += to_string(tx_id);
    D += "=";
    D += emr;
    bc2.createBlock(D);
}
int main()
{
	bloom_parameters parameters;
    // How many elements roughly do we expect to insert?
    parameters.projected_element_count = 1000;

    // Maximum tolerable false positive probability? (0,1)
    parameters.false_positive_probability = 0.0001; // 1 in 10000

    // Simple randomizer (optional)
    parameters.random_seed = 0xA5A5A5A5;

    if (!parameters)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters.compute_optimal_parameters();

    //Instantiate Bloom Filter
    bloom_filter filter(parameters);





    std::string str_list[] = { "AbC", "iJk", "XYZ" };

    // Insert into Bloom Filter
    {
        // Insert some strings
        for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string)); ++i)
        {
            filter.insert(str_list[i]);
        }

        // Insert some numbers
        for (std::size_t i = 0; i < 100; ++i)
        {
            filter.insert(i);
        }
    }

    // Query Bloom Filter
    {
        // Query the existence of strings
        for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string)); ++i)
        {
            if (filter.contains(str_list[i]))
            {
                std::cout << "BF contains: " << str_list[i] << std::endl;
            }
        }

        // Query the existence of numbers
        for (std::size_t i = 0; i < 100; ++i)
        {
            if (filter.contains(i))
            {
                std::cout << "BF contains: " << i << std::endl;
            }
        }

        std::string invalid_str_list[] = { "AbCX", "iJkX", "XYZX" };

        // Query the existence of invalid strings
        for (std::size_t i = 0; i < (sizeof(invalid_str_list) / sizeof(std::string)); ++i)
        {
            if (filter.contains(invalid_str_list[i]))
            {
                std::cout << "BF falsely contains: " << invalid_str_list[i] << std::endl;
            }
        }

        // Query the existence of invalid numbers
        for (int i = -1; i > -100; --i)
        {
            if (filter.contains(i))
            {
                std::cout << "BF falsely contains: " << i << std::endl;
            }
        }
    }
    blockchain bh1, bh2;
    std::string data;
    std::cout << "Input number of blocks: ";
    std::getline(std::cin, data);
    const int numBlocksToInput = std::stoi(data);

    for (int i = 0; i < numBlocksToInput; ++i) {
        string key, value;
        std::cout << "Input key: ";
        std::getline(std::cin, key);
        std::cout << "Input Value: ";
        std::getline(std::cin, value);
        insertionFunction(bh1, bh2, parameters, filter, key, value);
    }

    string skey = "";
    cout << "Enter the key word you want to search:\n";
    std::getline(std::cin, data);
    block bk = searchFunction(bh1, bh2, parameters, filter, data); 
    cout << "data = " << data<<endl;


    //std::string data;
    //std::cout << "Input number of blocks: ";
    //std::getline(std::cin, data);
    //const int numBlocksToInput = std::stoi(data);

    //insertionFunction()
	return 0;
}