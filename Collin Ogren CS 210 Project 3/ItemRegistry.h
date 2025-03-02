/*
	Developer: Collin Ogren
	Date: 02/22/2025
	Purpose: Header for ItemRegistry.cpp
*/

#pragma once

#include <map>
#include <string>
#include <vector>

class ItemRegistry {
	// Private field and function.
private:
	std::map<std::string, int> itemMap;
	
	void readItemsFromFile(std::string path);

	// Public functions and constructor.
public:
	ItemRegistry(std::string path);

	int getNumberOfItems(std::string item);
	void addItem(std::string item);
	std::vector<std::pair<const std::string, int>> getAllKeyValuePairs();
	void writeBackup();
	void clear();
};