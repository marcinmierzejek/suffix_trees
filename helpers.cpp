/*
 * helpers.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: mierzejm
 */

#include "helpers.h"

using namespace std;

/* Sort class body */
Sort::Sort() {
	// TODO Auto-generated constructor stub

}

Sort::~Sort() {
	// TODO Auto-generated destructor stub
}

/*
 *
 */
void Sort::merge(string pArr[],
           	     int pLow,
           	     int pMid,
           	     int pHigh) {
	int s = pHigh - pLow + 1; // size of the output table
	string temp[s]; // temporary table
	int i = 0; // index in temp table
	int j = pLow; // index in lower table
	int k = pMid + 1; // index in higher table
	int n = pMid; // max range to copy remaining elements

	// merge the tables
	while (j <= pMid && k <= pHigh) {
		temp[i++] = (pArr[j] < pArr[k] ? pArr[j++] : pArr[k++]);
	}

	// copy remaining part of the tables
	if (j > pMid) { //lower part has been used
		// move the pointer to higher part
		j = k;
		n = pHigh;
	}
	// now copy it
	while (j <= n) {
		temp[i++] = pArr[j++];
	}

	// copy from temp into dest table
	for (i = 0; i < s; i++) {
		pArr[pLow + i] = temp[i];
	}

}

void Sort::mergeSort(string pArr[], int pLow, int pHigh) {
	if (pLow < pHigh) {
		int mid = (pLow + pHigh) / 2;
		mergeSort(pArr, pLow, mid);
		mergeSort(pArr, mid + 1, pHigh);
		merge(pArr, pLow, mid, pHigh);
	}
}

void Sort::sort(string pArr[], int pLow, int pHigh) {
	mergeSort(pArr, pLow, pHigh);
}

/*
 * Compare - class with functions supporting comparisons
 */

/*
 * Function returns number of matching characters
 *
 */
int Compare::compare(string pText, string pPat) {
	int vSize = (pText.size() > pPat.size() ? pPat.size() : pText.size());
	int vResult = 0;

	for (vResult = 0; vResult < vSize; vResult++) {
		if (pText[vResult] != pPat[vResult]) break;
	}

	return vResult;
}

/*
 * Function returns number of matching characters
 *
 */
int Compare::compare(string pText, string pPat, int pDifferences) {
	int vSize = (pText.size() > pPat.size() ? pPat.size() : pText.size());
	int vResult = 0;
	int vDifferences = 0;

	while (vResult < vSize && vDifferences <= pDifferences) {
		if (pText[vResult] != pPat[vResult]) vDifferences++;
		if (vDifferences <= pDifferences) {
			vResult++;
		}
	}

	return vResult;
}

/*
 * Function returns Hamming distance between pattern; considers n first characters of pText, where n is length of pPat
 *
 */
int Compare::distHamming(string pText, string pPat) {
    // comparing only n characters; n - length of shorter string
    int vSize = (pText.size() > pPat.size() ? pPat.size() : pText.size());
    // initial result is length of longer string
	int vResult = (pText.size() < pPat.size() ? pPat.size() : pText.size());

    for (int i = 0; i < vSize; i++) {
		if (pText[i] == pPat[i]) vResult--;
	}

	return vResult;
}
// end of Compare class

/*
 * WordOperations - class with functions to manipulate strings
 */

// converts to lower-case
void WordOperations::lower(string * pText) {
    for (string::size_type i = 0; i < pText->length(); ++i)
        (*pText)[i] = tolower((*pText)[i], loc);
}

// splits input into tokens
void WordOperations::splitVerseIntoWords(string *pText,
                                         vector<string> *pTokens,
                                         int pCase) {
    size_t vCurPos = pText->find_first_not_of(whiteChars); // skip leading white spaces
    size_t vNextPos;

    string vToken;

    while (vCurPos != string::npos) {
        vNextPos = pText->find_first_of(whiteChars, vCurPos);
        vToken = pText->substr(vCurPos, vNextPos - vCurPos);
        if (pCase == LOWER_CASE) {
            lower(&vToken);
        }
        pTokens->push_back(vToken);
        vCurPos = pText->find_first_not_of(whiteChars, vNextPos);
    }
}

// end of WordOperations class
