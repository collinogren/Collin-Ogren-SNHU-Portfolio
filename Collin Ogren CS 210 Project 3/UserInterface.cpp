/*
	Developer: Collin Ogren
	Date: 02/22/2025
	Purpose: Provide user interface functionalities for the program.
*/

#include "UserInterface.h"
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <iomanip>

// Define a constant for the platform-specific command to clear the terminal.
#ifdef _WIN32 // If compiled for Windows, use the "CLS" command.
const char* CLEAR_TERMINAL_COMMAND = "CLS";
#elif __linux__ // If compiled for Linux, use the "clear -x" command.
const char* CLEAR_TERMINAL_COMMAND = "clear -x";
#elif __APPLE__ // If compiled for MacOS, use the "printf '\33c\e[3J'" command.
const char* CLEAR_TERMINAL_COMMAND = "printf '\33c\e[3J'";
#else // Just in case, for other platforms, do not clear.
const char* CLEAR_TERMINAL_COMMAND = "";
#endif

// Constants to output via cout to set text to the different colors used.
const char* GREEN = "\033[0;32m";
const char* YELLOW = "\033[0;33m";
const char* RED = "\033[0;31m";
const char* RESET = "\033[0m";

// Store low and medium items sold thresholds.
const int LOW_ITEMS_THRESHOLD = 3;
const int MEDIUM_ITEMS_THRESHOLD = 6;

// Store "Corner Grocer in big ascii art.
const std::string TITLE = R"(   _____                              _____                         
  / ____|                            / ____|                        
 | |     ___  _ __ _ __   ___ _ __  | |  __ _ __ ___   ___ ___ _ __ 
 | |    / _ \| '__| '_ \ / _ \ '__| | | |_ | '__/ _ \ / __/ _ \ '__|
 | |___| (_) | |  | | | |  __/ |    | |__| | | | (_) | (_|  __/ |   
  \_____\___/|_|  |_| |_|\___|_|     \_____|_|  \___/ \___\___|_|   
--------------------------------------------------------------------)";

// Constructor that calls the main menu function.
UserInterface::UserInterface() {
	mainMenu();
}

// Helper function to clear and ignore to fix input failure.
void UserInterface::clearAndIgnoreCin() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Helper function to wait for the user to press enter.
void UserInterface::waitForEnter() {
	// String that gets ignored.
	std::string ignored;
	// Prompt.
	std::cout << "Press 'enter' to continue." << std::endl;
	// Wait for enter.
	std::getline(std::cin, ignored);
}

void UserInterface::mainMenu() {
	// Done is a loop control flag. The loop will terminate when done is true.
	bool done = false;

	// Loop until done is true.
	while (!done) {
		// Clear the terminal window.
		clear();

		// Prompt the user, get user input, and take the appropriate course.
		switch (menuSelection()) {
		case 1: // Item lookup case.
			itemLookup();
			break;
		case 2: // List items with numeric quantities case.
			enumerateItemsNumeric();
			break;
		case 3: // List items with a histogram case.
			enumerateItemsHistogram();
			break;
		case 4: // Terminate program case.
			done = true;
			break;
		}
	}

	// Tell the user the program is terminated.
	std::cout << "Program terminated." << std::endl;
}

// Function to print menu options.
void UserInterface::printMenuOptions() {
	// Display the title.
	std::cout << TITLE << std::endl;

	// Prompt the user for input.
	std::cout << "Use the following prompts to see how many items have been sold." << std::endl;
	std::cout << "1. Search items for quantity sold." << std::endl;
	std::cout << "2. List items sold quantity." << std::endl;
	std::cout << "3. List items sold histogram." << std::endl;
	std::cout << "4. Exit the program." << std::endl;
}

// Print menu options and return the user's input for what menu should be selected.
int UserInterface::menuSelection() {
	printMenuOptions();
	return userInputIntegerInRange("Select a menu option (1-4)", 1, 4);
}

// Get an integer input from the user within a range and display a custom message.
int UserInterface::userInputIntegerInRange(const char* message, int low, int high) {
	// Loop control flag.
	bool valueFound = false;

	// Int to store the user selection.
	int selection;

	// Keep looping while there is input failure or the value is not found.
	while (std::cin.fail() || !valueFound) {
		// Display the message
		std::cout << message << std::endl;

		// Prompt for input and get it.
		std::cout << ">> ";
		std::cin >> selection;

		// Check if input failure occurred.
		if (std::cin.fail()) {
			// Fix input failure.
			clearAndIgnoreCin();

			// Tell the user of their mistake.
			menuSelectionWrongValue();

			// Back to the top of the loop.
			continue;
		}

		// Make sure the value is between the given low and high values.
		if (selection >= low && selection <= high) {
			// Set valueFound to true to end the loop.
			valueFound = true;
		}
		else {
			// Otherwise tell the user of their mistake.
			menuSelectionWrongValue();
		}
	}

	// Return the user's selection.
	return selection;
}

// Function to tell the user of their mistake.
void UserInterface::menuSelectionWrongValue() {
	// Set the text color to red.
	setColorModeRed();

	// Tell the user what to do instead.
	std::cout << "Enter only a number between 1 and 4" << std::endl;

	// Set the color back to normal.
	setColorModeNormal();
}

void UserInterface::setColorBasedOnValue(int value) {
	if (value <= LOW_ITEMS_THRESHOLD) {
		setColorModeRed();
	}
	else if (value <= MEDIUM_ITEMS_THRESHOLD) {
		setColorModeYellow();
	}
	else {
		setColorModeGreen();
	}
}

// Menu option 1. Lookup how many of a particular item was purchased
void UserInterface::itemLookup() {
	// Clear the terminal.
	clear();
	
	// Print the title and prompt the user to enter an item to look up.
	std::cout << TITLE << std::endl;
	std::cout << "Enter the name of the item you wish to lookup.\nLeave blank and press 'enter' to cancel." << std::endl;
	std::cout << ">> ";

	// Fix for getline.
	clearAndIgnoreCin();

	// Get user input.
	std::string item;
	std::getline(std::cin, item);

	// If the value is empty then go back to the main menu.
	if (item == "") {
		return;
	}

	// Get the number of items that have been purchased.
	int count = itemRegistry.getNumberOfItems(item);

	// If the item has not been sold, then tell the user about it.
	if (count == -1) {
		std::cout << "No \"" << item << "\"" << " have been sold." << std::endl;
		waitForEnter();
		return;
	}

	// Make the user input lowercase.
	std::transform(item.begin(), item.end(), item.begin(), ::tolower);

	// Set the output based on the number of items purchased.
	setColorBasedOnValue(count);

	// Output how many items were purchased.
	std::cout << count;

	// Set the color mode back to normal.
	setColorModeNormal();

	// Tell the user what the value relates to.
	std::cout << " " << item << " were purchased." << std::endl;

	// Wait for the user to move on.
	waitForEnter();
}

// Menu option 2. List all the items and how many were purchased.
void UserInterface::enumerateItemsNumeric() {
	// Clear the terminal
	clear();
	// Output the title.
	std::cout << TITLE << std::endl;

	// Get all key value pairs.
	std::vector<std::pair<const std::string, int>> keyValuePairs = itemRegistry.getAllKeyValuePairs();

	// For ever key value pair, output the item (key) and how many were purchased (value)
	for (std::pair<const std::string, int> keyValuePair : keyValuePairs) {
		// Output the item.
		std::cout << std::setw(11) << std::left << keyValuePair.first << " ";

		// Store the quantity as count.
		int count = keyValuePair.second;

		// Set the output color based on the count.
		setColorBasedOnValue(count);

		// Output the count.
		std::cout << count << std::endl;

		// Reset the color mode.
		setColorModeNormal();
	}

	// Wait for the user to press enter.
	clearAndIgnoreCin();
	waitForEnter();
}

// Menu option 3. List items and provide purchase information using a histogram.
void UserInterface::enumerateItemsHistogram() {
	// Clear the terminal.
	clear();

	// Output the title.
	std::cout << TITLE << std::endl;

	// For ever key value pair, output the item (key) and how many were purchased (value) as a histogram.
	for (std::pair<const std::string, int> keyValuePair : itemRegistry.getAllKeyValuePairs()) {
		// Output the item with a width such that each row of the histogram will start at the same x position.
		std::cout << std::setw(11) << std::left << keyValuePair.first << " ";

		// Store the quantity as count.
		int count = keyValuePair.second;

		// Output the 'count' number of asterisks and set the color according the value of i.
		for (int i = 0; i < count; i++) {
			// Set the color of the asterisk.
			setColorBasedOnValue(i);

			// Output the asterisk.
			std::cout << "*";
		}

		// Output a new line and set the color mode back to normal.
		std::cout << std::endl;
		setColorModeNormal();
	}

	// Wait for the user to move on.
	clearAndIgnoreCin();
	waitForEnter();
}

// Clear the terminal
void UserInterface::clear() {
	system(CLEAR_TERMINAL_COMMAND);
}

// Set the text color to a given color.
void UserInterface::setColorMode(const char* color) {
	std::cout << color;
}

// Set the color mode to green.
void UserInterface::setColorModeGreen() {
	setColorMode(GREEN);
}

// Set the color mode to yellow.
void UserInterface::setColorModeYellow() {
	setColorMode(YELLOW);
}

// Set the color mode to red.
void UserInterface::setColorModeRed() {
	setColorMode(RED);
}

// Reset the color mode.
void UserInterface::setColorModeNormal() {
	setColorMode(RESET);
}