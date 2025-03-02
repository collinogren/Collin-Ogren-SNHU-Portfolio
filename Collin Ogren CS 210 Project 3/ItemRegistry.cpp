/*
	Developer: Collin Ogren
	Date: 02/22/2025
	Purpose: Provide functions over the top of std::map to 
	facilitate the required functionality of the program.
*/

#include "ItemRegistry.h"
#include <stdexcept>
#include <fstream>
#include <iostream>

// Constructor that reads the file from the path into a map and writes the backup file.
ItemRegistry::ItemRegistry(std::string path) {
	readItemsFromFile(path);
	writeBackup();
}

// Function that returns the number of items that have been sold. Returns -1 if the item is not in the map.
int ItemRegistry::getNumberOfItems(std::string item) {
	// If the item is in the map, then return the number of items.
	if (itemMap.count(item)) {
		return itemMap[item];
	} else {
		// If the item is not in the map, then return -1.
		return -1;
	}
}

// Function to add an item to the map. If the key (item) is already there, then the count (value) is incremented by 1.
void ItemRegistry::addItem(std::string item) {
	if (itemMap.count(item)) {
		// item is already a key in the unordered map so increment by 1.
		itemMap[item] += 1;
	}
	else {
		// item is not a key in the unordered map so insert a new key value pair with value 1.
		itemMap.insert({ item, 1 });
	}
}

// Function to clear the map.
void ItemRegistry::clear() {
	itemMap.clear();
}

// Read the items from the file and turn it into a map with the keys being the names of the items and the values being the number of occurrences in the file.
void ItemRegistry::readItemsFromFile(std::string path) {
	clear(); // Just in case this was already called for one reason or another.

	// Create a file input stream and open it using the provided path.
	std::ifstream inputFileStream;
	inputFileStream.open(path);

	// Make sure the file could be opened. If not, then print an error message and return.
	if (!inputFileStream.is_open()) {
		std::cout << "Could not open file \"" << path << "\"" << std::endl;
		return;
	}

	// Read every line from the file.
	std::string line;
	while (std::getline(inputFileStream, line)) {
		// Add each item.
		addItem(line);
	}

	// Close the input stream.
	inputFileStream.close();
}

// Function that returns a vector of all key value pairs from the map.
std::vector<std::pair<const std::string, int>> ItemRegistry::getAllKeyValuePairs() {
	// Define a key value pair vector.
	std::vector<std::pair<const std::string, int>> keyValuePairs;
	// Reserve enough space to store every key value pair in the map.
	keyValuePairs.reserve(itemMap.size());

	// Push every pair into the vector.
	for (auto keyValue : itemMap) {
		keyValuePairs.push_back(keyValue);
	}

	// Return the pairs.
	return keyValuePairs;
}

// Function to write the backup file.
void ItemRegistry::writeBackup() {
	// Open a file output stream for "frequency.dat."
	std::ofstream outputFileStream("frequency.dat");

	// Check if it was not able to be opened.
	if (!outputFileStream.is_open()) {
		// If not then print an error message and return.
		std::cout << "Failed to open \"frequency.dat\" for writing." << std::endl;
		return;
	}

	// Get all key value pairs.
	std::vector<std::pair<const std::string, int>> keyValuePairs = getAllKeyValuePairs();

	// Write them all to a file.
	for (std::pair<const std::string, int> keyValuePair : keyValuePairs) {
		outputFileStream << keyValuePair.first << " " << keyValuePair.second << std::endl;
	}

	// Close the stream.
	outputFileStream.close();
}