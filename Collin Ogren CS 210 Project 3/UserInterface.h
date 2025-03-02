/*
	Developer: Collin Ogren
	Date: 02/22/2025
	Purpose: Header for UserInterface.cpp
*/

#pragma once

#include "ItemRegistry.h"

class UserInterface {
	// Public constructor and function.
public:
	UserInterface();
	void mainMenu();

	// Private functions and field.
private:
	int menuSelection();
	int userInputIntegerInRange(const char* message, int low, int high);
	void menuSelectionWrongValue();
	void setColorBasedOnValue(int value);
	void printMenuOptions();

	void itemLookup();

	void enumerateItemsNumeric();

	void enumerateItemsHistogram();

	void clear();
	void setColorModeGreen();
	void setColorModeYellow();
	void setColorModeRed();
	void setColorModeNormal();
	void setColorMode(const char* colorMode);
	void clearAndIgnoreCin();
	void waitForEnter();

	ItemRegistry itemRegistry = ItemRegistry("CS210_Project_Three_Input_File.txt");
};
