/*
 * functionstocomplete.cpp
 *
 *  Created on: Feb 14, 2020
 *      Author: Cody Baker
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "constants.h"
#include "utilities.h"
#include "array_functions.h"

struct Entry {
	std::string word;
	int numberOccurences;
};

Entry wordArr[constants::MAX_WORDS];

const int BARREN_ARR = 0;
int arrSize = BARREN_ARR;

//zero out array that tracks words and their occurrences
void clearArray() {
	arrSize = BARREN_ARR;
}
;

//how many unique words are in array
int getArraySize() {
	return arrSize;
}
;

//get data at a particular location
std::string getArrayWordAt(int i) {

	std::string get = "";
	if (i < arrSize) {
		get = wordArr[i].word;
	}

	return get;

}
;

//get data at a particular location
int getArrayWord_NumbOccur_At(int i) {

	int n = constants::FAIL_NO_ARRAY_DATA;
	if (i < arrSize) {
		n = wordArr[i].numberOccurences;
	}
	return n;
}
;

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(std::fstream &myfstream) {
	if (myfstream.is_open()) {

		std::string fileLine = "";
		while (getline(myfstream, fileLine)) {
			processLine(fileLine);
		}

		return true;
	} else {
		return false;
	}
}
;

/*take 1 line and extract all the tokens from it
 feed each token to processToken for recording*/
void processLine(std::string &myString) {

	std::stringstream ss(myString);
	std::string tempToken;

	while (getline(ss, tempToken, constants::CHAR_TO_SEARCH_FOR)) {
		processToken(tempToken);
	}

}
;

/*string comparer to get around syntax*/
int strComp(std::string s1, std::string s2) {
	toUpper(s1);
	toUpper(s2);
	return (s1.compare(s2));

}

/*Keep track of how many times each token seen*/
void processToken(std::string &token) {

	if (strip_unwanted_chars(token)) {
		for (int i = 0; i < arrSize; i++) {
			if (strComp(token, wordArr[i].word) == 0) {
				wordArr[i].numberOccurences += 1;
				return;
			}
		}
		Entry newcomer;
		newcomer.word = token;
		newcomer.numberOccurences = 1;
		wordArr[arrSize] = newcomer;
		arrSize++;
	}

}
;

/*if you are debugging the file must be in the project parent directory
 in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream &myfile, const std::string &myFileName,
		std::ios_base::openmode mode) {
	myfile.open(myFileName.c_str(), mode);
	return myfile.is_open();
}
;

/*iff myfile is open then close it*/
void closeFile(std::fstream &myfile) {
	if (myfile.is_open()) {
		myfile.close();
	}
}
;

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const std::string &outputfilename) {

	if (arrSize == BARREN_ARR) {
		return constants::FAIL_NO_ARRAY_DATA;
	}
	std::ofstream outFile;
	outFile.open(outputfilename);

	if (outFile.is_open()) {
		for (int i = 0; i < arrSize; i++) {
			outFile << wordArr[i].word << ' ' << wordArr[i].numberOccurences
					<< std::endl;
		}
		return constants::SUCCESS;
	} else {
		return constants::FAIL_FILE_DID_NOT_OPEN;
	}

}
;

/*
 * Sort myEntryArray based on so enum value.
 * The presence of the enum implies a switch statement
 */
void sortArray(constants::sortOrder so) {
	int sortCnt = 0;
	Entry tmpEnt;

	switch (so) {
	case constants::sortOrder::ASCENDING:
		while (sortCnt < arrSize) {
			for (int i = 1; i < arrSize; i++) {
				if (strComp(wordArr[i - 1].word, wordArr[i].word) > 0) {
					tmpEnt = wordArr[i];
					wordArr[i] = wordArr[i - 1];
					wordArr[i - 1] = tmpEnt;
					sortCnt = 0;
				} else {
					sortCnt++;
				}
			}
		}
		return;

	case constants::sortOrder::DESCENDING:
		while (sortCnt < arrSize) {
			for (int i = 1; i < arrSize; i++) {
				if (strComp(wordArr[i - 1].word, wordArr[i].word) < 0) {
					tmpEnt = wordArr[i];
					wordArr[i] = wordArr[i - 1];
					wordArr[i - 1] = tmpEnt;
					sortCnt = 0;
				} else {
					sortCnt++;
				}
			}
		}
		return;

	case constants::sortOrder::NUMBER_OCCURRENCES:
		while (sortCnt < arrSize) {
			for (int i = 1; i < arrSize; i++) {
				if (wordArr[i - 1].numberOccurences
						> wordArr[i].numberOccurences) {
					tmpEnt = wordArr[i];
					wordArr[i] = wordArr[i - 1];
					wordArr[i - 1] = tmpEnt;
					sortCnt = 0;
				} else {
					sortCnt++;
				}
			}
		}
		return;
	case constants::sortOrder::NONE:
		return;
	}

}
;
