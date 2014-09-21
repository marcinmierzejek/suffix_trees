/*
 * helpers.h
 *
 *  Created on: Feb 24, 2013
 *      Author: mierzejm
 */

#include <string>
#include <vector>
#include <iostream>
#include <locale>

using namespace std;

#ifndef HELPERS_H_
#define HELPERS_H_

/*
 * Number of elements in an array
 */
template<typename T, int size>
int GetArrLength(T(&)[size]){return size;}

/*
 * Sort - class with functions supporting sorting
 */
class Sort {
public:
	Sort();
	virtual ~Sort();
	void sort(string pArr[],
			  int pLow,
			  int pHigh);
private:
	void merge(string pArr[],
			   int pLow,
			   int pMid,
			   int pHigh);
	void mergeSort(string pArr[],
			       int pLow,
			       int pHigh);
};

/*
 * Compare - class with functions supporting comparisons
 */
class Compare {
public:
    // returns number of matching characters
	int compare(string pText, string pPat);
	// returns number of matching characters, but some can differ
	int compare(string pText, string pPat, int pDifferences);
	// returns Hamming distance between pattern; considers n first characters of pText, where n is length of pPat
	int distHamming(string pText, string pPat);
};

/*
 * WordOperations - class with functions to manipulate strings
 */
class WordOperations {
private:
    const string whiteChars = " .,!?:;'()-\t\n";
    locale loc;
public:
    const int ORIGINAL_CASE = 0;
    const int LOWER_CASE = 1;
    const int UPPER_CASE = 2;
    /*
    pText - pointer to input text
    pTokens - pointer to output vector of strings
    pCase - controls case of the output:
        ORIGINAL_CASE - keep original;
        LOWER_CASE - lowercase;
        UPPER_CASE - uppercase
    */
    void splitVerseIntoWords(string *pText, vector<string> *pTokens, int pCase = 0);
    void lower(string * pText);
};

#endif /* HELPERS_H_ */
