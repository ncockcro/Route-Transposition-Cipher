/*
Written by: Nicholas Cockcroft
Date: June 12, 2018

Description: Write a program to take in a message to encrypt, dimensions for the encryption,
and whether the encryption should happen clockwise or counterclockwise. For me details, see the
original reddit post: 
https://www.reddit.com/r/dailyprogrammer/comments/8n8tog/20180530_challenge_362_intermediate_route/
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // Parsing a string to break on ','
#include <algorithm> // Erasing white space from strings
using namespace std;

// struct for holding the row and height length for the encryption
struct Dimensions {
	int row;
	int height;
};

Dimensions getDimensions(string dimensions); // Break the dimensions into seperate numbers
string RemoveSpaces(string dimension); // Removes whitespace from a string

void FillEncryptedMessage(vector< vector<char> > &encryptedMessage, string message, int rows); // Put string into 2-d vector
bool ValidLetter(char letter); // Checks if a character is a letter to be put into the 2-d vector

string EncryptClockwise(vector< vector<char> > &encryptedMessage, int rows); // Clockwise encryption
string ClockwiseMoveDown(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles down through the elements in vector matrix clockwise
string ClockwiseMoveLeft(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles left through the elements in vector matrix clockwise
string ClockwiseMoveUp(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles up through the elements in vector matrix clockwise
string ClockwiseMoveRight(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles right through the elements in vector matrix clockwise

string EncryptCounterClockwise(vector< vector<char> > &encryptedMessage, int rows); // Counter-clockwise encryption
string CounterClockwiseMoveLeft(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles left through the elements in vector matrix counter clockwise
string CounterClockwiseMoveDown(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles down through the elements in vector matrix counter clockwise
string CounterClockwiseMoveRight(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles right through the elements in vector matrix counter clockwise
string CounterClockwiseMoveUp(vector< vector<char> > &encryptedMessage, int rows, int traversalTimes); // Cycles up through the elements in vector matrix counter clockwise

bool notDone(vector< vector<char> > &encrypedMessage, int rows); // Checks to see if the whole vector matrix has been traversed

int main() {

	// Initial variables
	string userString;
	string userDimensions;
	string userDirection;
	Dimensions actualDimensions;

	// Get user input
	cout << "Input string to be encrypted: ";
	getline(cin, userString);
	cout << "Enter dimensions (#,#): ";
	getline(cin, userDimensions);
	cout << "Enter a direction ('c' for clockwise or 'cc' for counter-clockwise): ";
	getline(cin, userDirection);

	// Convert the dimensions the user typed in to actual numbers that are seperated
	actualDimensions = getDimensions(userDimensions);

	// Initialize the vector with the dimensions the user inputed
	vector< vector<char> > encryptedMessage(actualDimensions.height, vector<char>(actualDimensions.row));
	string message;

	// Puts the message the user typed in, into the vector and fills the rest with 'X'
	FillEncryptedMessage(encryptedMessage, userString, actualDimensions.row);

	// Encrypt the vector of the message either clockwise or counter-clockwise
	if (userDirection == "c" || userDirection == "C") {
		message = EncryptClockwise(encryptedMessage, actualDimensions.row);
	}
	else if (userDirection == "cc" || userDirection == "CC") {
		message = EncryptCounterClockwise(encryptedMessage, actualDimensions.row);
	}
	else {
		cout << "Unknown direction, try (c) or (cc) for clockwise or counter-clockwise" << endl;
		exit(1);
	}

	// Output the encrypted message
	cout << endl << message << endl;

	return 0;
}

// Takes the string the user typed in and converts it into two seperate numbers which can be used as 
// the dimensions for the 2-d vector
Dimensions getDimensions(string dimensions) {
	stringstream ss(dimensions);
	string token;
	Dimensions tempDimensions;
	int intToken;

	vector<int> dim;

	// Parsing each character and splitting on the ','
	while (getline(ss, token, ',')) {
		token = RemoveSpaces(token);
		intToken = stoi(token);
		if (stoi(token) > -1 && stoi(token) < 255) {
			if (isdigit(stoi(token)) == 0) {
				dim.push_back(stoi(token));
			}
		}
		else {
			cout << "Error: Dimensions must be between -1 and 255" << endl;
			exit(1);
		}
	}

	tempDimensions.row = dim[0];
	tempDimensions.height = dim[1];

	return tempDimensions;
}

// Removes the white space from a string that is passed in
string RemoveSpaces(string dimension) {
	dimension.erase(remove(dimension.begin(), dimension.end(), ' '), dimension.end());
	return dimension;
}

// Fills the 2-d vector with the string the user typed in and any unfilled spaces with the dimensions
// the user specified will be filled in with 'X'
void FillEncryptedMessage(vector< vector<char> > &encryptedMessage, string message, int rows) {

	// Keeps track of when the user's string has been exhausted of letters and switches over to
	// using 'X's for the rest of the characters in the dimensions
	size_t letterCount = 0;
	bool nextLetter = true;

	message = RemoveSpaces(message);

	// The two for loops cycle through the 2-d vector and fills each index with a letter
	// from the user's string or 'X's
	for (size_t i = 0; i < encryptedMessage.size(); i++) {
		for (int j = 0; j < rows; j++) {
			
			// If the letterCound is less than the user's string length, then there are still letters
			// to be put into the 2-d vector
			if (letterCount < message.length()) {
				// Checking to make sure only letters are being put into the vector
				if (ValidLetter(message[letterCount])) {
					encryptedMessage[i][j] = toupper(message[letterCount]);
				}
				else {
					// If there is a character in the string that is not a letter, we must cycle through until we find one or are
					// at the end of the string
					while (nextLetter) {
						letterCount++;
						if (letterCount < message.length()) {
							if (ValidLetter(message[letterCount])) {
								encryptedMessage[i][j] = toupper(message[letterCount]);
								nextLetter = false;
							}
						}
						else {
							encryptedMessage[i][j] = 'X';
							nextLetter = false;
						}
					}
				}
			}
			// Otherwise, then an 'X' will be placed in the 2-d vector
			else {
				encryptedMessage[i][j] = 'X';
			}
			letterCount++;
			nextLetter = true;
		}
		
	}
}

// Checks a character that is passed in to see whether it is a letter or not
bool ValidLetter(char letter) {

	bool isValid = false;

	// Uppercase letters
	if ((int)letter > 64 && (int)letter < 91) {
		isValid = true;
	}
	// Lowercase letters
	else if ((int)letter > 96 && (int)letter < 123) {
		isValid = true;
	}
	
	return isValid;
}

// Encrypts the 2-d vector clockwise 
string EncryptClockwise(vector< vector<char> > &encryptedMessage, int rows) {
	string encryptedString;
	int i = 0;

	while (notDone(encryptedMessage, rows)) {
		// Down
		if (notDone(encryptedMessage, rows)) {
			encryptedString += ClockwiseMoveDown(encryptedMessage, rows, i);
		}
		// Left
		if (notDone(encryptedMessage, rows)) {
			encryptedString += ClockwiseMoveLeft(encryptedMessage, rows, i);
		}
		// Up
		if (notDone(encryptedMessage, rows)) {
			encryptedString += ClockwiseMoveUp(encryptedMessage, rows, i);
		}
		// Right
		if (notDone(encryptedMessage, rows)) {
			encryptedString += ClockwiseMoveRight(encryptedMessage, rows, i);
		}

		i++;
	}

	return encryptedString;
}

// Cycles through a line of elements in the encrypted message (vector matrix) and only traverses in a downward motion
string ClockwiseMoveDown(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (size_t j = 0 + traversalTime; j < encryptedMessage.size() - traversalTime; j++) {
		if (j < encryptedMessage.size() - traversalTime && rows - 1 - traversalTime > 0) {
			tempString += encryptedMessage[j][rows - 1 - traversalTime];
			encryptedMessage[j][rows - 1 - traversalTime] = '/';
		}
	}

	return tempString;
}

// Cycles through a row of elements in the encrypted message (vector matrix) and only traverses on a single line going left
string ClockwiseMoveLeft(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {
	
	string tempString;

	for (int j = rows - 2 - traversalTime; j > 0 + traversalTime; j--) {
		if (j > 0 && encryptedMessage.size() - 1 - traversalTime > 0) {
			tempString += encryptedMessage[encryptedMessage.size() - 1 - traversalTime][j];
			encryptedMessage[encryptedMessage.size() - 1 - traversalTime][j] = '/';
		}
	}

	return tempString;
}

// Cycles throug a line of elements in the encrypted message (vector matrix) and only traverses in an up motion
string ClockwiseMoveUp(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (int j = encryptedMessage.size() - 1 - traversalTime; j > -1 + traversalTime; j--) {
		if (j > -1 && (unsigned int)traversalTime < encryptedMessage.size() - 1) {
			tempString += encryptedMessage[j][0 + traversalTime];
			encryptedMessage[j][0 + traversalTime] = '/';
		}
	}

	return tempString;
}

// Cycles through a row of elements in the encrypted message (vector matrix) and only traverses to the right
string ClockwiseMoveRight(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (int j = 1 + traversalTime; j < rows - 1 - traversalTime; j++) {
		if (j < rows - 1 && traversalTime < rows - 1) {
			tempString += encryptedMessage[0 + traversalTime][j];
			encryptedMessage[0 + traversalTime][j] = '/';
		}
	}

	return tempString;
}

// Encrypts the 2-d vector counter-clockwise
string EncryptCounterClockwise(vector< vector<char> > &encryptedMessage, int rows) {
	string encryptedString;
	int i = 0;

	while (notDone(encryptedMessage, rows)) {

		// Move left
		if (notDone(encryptedMessage, rows)) {
			encryptedString += CounterClockwiseMoveLeft(encryptedMessage, rows, i);
			cout << "--------------" << endl;
		}
		// Move down
		if (notDone(encryptedMessage, rows)) {
			encryptedString += CounterClockwiseMoveDown(encryptedMessage, rows, i);
			cout << "--------------" << endl;
		}
		// Move right
		if (notDone(encryptedMessage, rows)) {
			encryptedString += CounterClockwiseMoveRight(encryptedMessage, rows, i);
			cout << "--------------" << endl;
		}
		// Move up
		if (notDone(encryptedMessage, rows)) {
			encryptedString += CounterClockwiseMoveUp(encryptedMessage, rows, i);
			cout << "--------------" << endl;
		}
		i++;
	}
	encryptedString = RemoveSpaces(encryptedString);
	return encryptedString;
}

// Cycles through a row of elements in the encrypted message (vector matrix) and only traverses left
string CounterClockwiseMoveLeft(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (int j = rows - 1 - traversalTime; j > -1 + traversalTime; j--) {
		if (j > -1 && 0 + (unsigned int)traversalTime < encryptedMessage.size()) {
			tempString += encryptedMessage[0 + traversalTime][j];
			encryptedMessage[0 + traversalTime][j] = '/';
		}
		cout << j << endl;
	}

	return tempString;
}

// Cycles through a column of elements in the encrypted message (vector matrix) and only traverses on a single line going down
string CounterClockwiseMoveDown(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (size_t j = 1 + traversalTime; j < encryptedMessage.size() - 1 - traversalTime; j++) {
		if (j < encryptedMessage.size() - 1 - (unsigned int)traversalTime && 0 + traversalTime < rows) {
			tempString += encryptedMessage[j][0 + traversalTime];
			encryptedMessage[j][0 + traversalTime] = '/';
		}
		cout << j << endl;
	}

	return tempString;
}

// Cycles throug a row of elements in the encrypted message (vector matrix) and only traverses right
string CounterClockwiseMoveRight(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (int j = 0 + traversalTime; j < rows - traversalTime; j++) {
		if (j < rows - traversalTime && encryptedMessage.size() - 1 - traversalTime > 0) {
			tempString += encryptedMessage[encryptedMessage.size() - 1 - traversalTime][j];
			encryptedMessage[encryptedMessage.size() - 1 - traversalTime][j] = '/';
		}
		cout << j << endl;
	}

	return tempString;
}

// Cycles through a column of elements in the encrypted message (vector matrix) and only traverses up
string CounterClockwiseMoveUp(vector< vector<char> > &encryptedMessage, int rows, int traversalTime) {

	string tempString;

	for (int j = encryptedMessage.size() - 2 - traversalTime; j > 0 + traversalTime; j--) {
		if (j > -1 - traversalTime && rows - 1 - traversalTime > 0 + traversalTime) {
			tempString += encryptedMessage[j][rows - 1 - traversalTime];
			encryptedMessage[j][rows - 1 - traversalTime] = '/';
		}
		cout << j << endl;
	}

	return tempString;
}

// Checks to see whether or not the whole encrypted message has been traversed and encrypted
bool notDone(vector< vector<char> > &encryptedMessage, int rows) {

	int dashCount = 0;

	// Goes through the entire vector matrix looking for '/'
	for (size_t i = 0; i < encryptedMessage.size(); i++) {
		for (int j = 0; j < rows; j++) {
			if (encryptedMessage[i][j] == '/') {
				dashCount++;
			}
		}
	}
	// If the dash counter is equal to the number of letters in the vector matrix, then we have gone through everything
	if (dashCount == encryptedMessage.size() * rows) {
		return false;
	}

	return true;
}