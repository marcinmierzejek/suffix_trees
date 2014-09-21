/*
 * SuffixTree.cpp
 *
 *  Created on: Jun 29, 2012
 *      Author: mierzejm
 */

//#include <fstream>

#include "helpers.h"
#include "SuffixTree.h"

using namespace std;


/*
 * SuffixTreeNode body
 */
const string SuffixTreeNode::TERMINATOR = "$";

SuffixTreeNode::SuffixTreeNode() {
	// TODO Auto-generated constructor stub

}

SuffixTreeNode::~SuffixTreeNode() {
	// TODO Auto-generated destructor stub
}

// get children nodes
map<string, SuffixTreeNode*> SuffixTreeNode::getChildren() {
	return children;
}

// get pointer to children nodes
map<string, SuffixTreeNode*>* SuffixTreeNode::getChildrenPointer() {
	return &children;
}

/*
 * get numbers of words
 */
set<int>* SuffixTreeNode::getWordIndxs() {
	return &wordIndxs;
}

/*
 * get node containing terminator
 *
 */
SuffixTreeNode* SuffixTreeNode::getTerminatorNode() {
	map<string, SuffixTreeNode*>* vChildren = getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;

	for (i = vChildren->begin(); i != vChildren->end(); i++) {
		if (i->first.compare(TERMINATOR) == 0) { // edge label indicates terminating node
			return i->second;
		}
	}

	SuffixTreeNode* vTermNode = new SuffixTreeNode();
	addChild(TERMINATOR, vTermNode, false);
	return vTermNode;
}

/*
 * Add word identifier to the term node
 * pIdx - index of the word
 */
void SuffixTreeNode::addWordIdx(int pIdx) {
	wordIndxs.insert(pIdx);
}

/*
 * If needed creates terminator node and adds pIdx to list of word indexes
 * pIdx - index of the word
 */
void SuffixTreeNode::makeSuffixOf(int pIdx) {
	SuffixTreeNode* vTermNode = getTerminatorNode();
	vTermNode->addWordIdx(pIdx);
}

/*
 * add new child node
 * pIsSuffix - true if pKey is suffix of the word
 * pIdx - index of the word
 */
void SuffixTreeNode::addChild(string pKey,
			                  SuffixTreeNode* pNode,
			                  bool pIsSuffix,
	                          int pIdx) {
	children[pKey] = pNode;
	if (pIsSuffix) {
		pNode->makeSuffixOf(pIdx);
	}
}

// end of SuffixTreeNode

/*
 * SuffixTree body
 */

SuffixTree::SuffixTree() {
	// TODO Auto-generated constructor stub
    root = new SuffixTreeNode();
}

SuffixTree::~SuffixTree() {
	// TODO Auto-generated destructor stub
}

SuffixTreeNode* SuffixTree::getRoot() {
	return root;
}

/* get the longest common prefix out of the first table elements
 *		very naive method
 * pWords - pointer to array of strings
 * pFrom - analyze array from
 * pTo - analyze table to
 * pPosFrom - analyze strings from this position
 * pIsFirst - indicates, if this is the first character of the prefix
 */
string SuffixTree::longestPrefix(string pWords[],
		                         int pFrom,
		                         int* pTo,
		                         size_t pPosFrom,
		                         bool pIsFirst) {
	if (pWords[pFrom].size() <= pPosFrom) {
		return "";
	}
	string vPrefix = pWords[pFrom].substr(pPosFrom, 1);
	int i = pFrom + 1;

	while (i <= *pTo && pWords[i].find(vPrefix, pPosFrom) == pPosFrom) {
		i++;
	}
	i--;

	if (pFrom == i) { // prefix is not shared
		if (pIsFirst) { // this is the first character
			*pTo = i;
			return pWords[pFrom].substr(pPosFrom); // whole word
		} else {
			return ""; // empty string
		}
	} else {
		*pTo = i;
		vPrefix += longestPrefix(pWords, pFrom, pTo, (pPosFrom + 1), false);
	}

	return vPrefix;
}

/*
 * build subtree
 * pParent - pointer to parent node
 * pWords - pointer to array of strings (suffixes)
 * pFrom - analyze array from
 * pTo - analyze table to
 * pPosFrom - analyze strings from this position
 * pIdx - index of the word in text
 */
void SuffixTree::buildSSubTreeNaive(SuffixTreeNode* pParent,
		                            string pWords[],
		                            int pFrom,
		                            int pTo,
		                            size_t pPosFrom,
		                            int pIdx) {
	int vFrom = pFrom;
	int vTo = pTo;
	SuffixTreeNode* vNewNode;
	bool vExists;
	int vNoOfMatchingChars = 0;
	Compare vCompare;

	while (vFrom <= pTo) { // there are still suffixes to be analyzed
		if (pWords[vFrom].size() <= pPosFrom) { // current suffix longer than starting position
			vFrom++; // go to the next suffix
		} else {
			string vPrefix = longestPrefix(pWords, vFrom, &vTo, pPosFrom, true);
			bool vIsSuffix = ((pPosFrom + vPrefix.size() == pWords[vFrom].size()) ? true : false);

			// check, if vPrefix exists in children of the parent node
			map<string, SuffixTreeNode*>* vChildren = pParent->getChildrenPointer();
			map<string, SuffixTreeNode*>::iterator i;
			vExists =  false;

			for (i = vChildren->begin(); i != vChildren->end() && !vExists; i++) { // iterate through children of the node
				if (i->first.compare(vPrefix) == 0) { // such prefix exists
					vNewNode = i->second;
					if (vIsSuffix) {
						vNewNode->makeSuffixOf(pIdx);
					}
					vExists = true;
					break;
				}

				if (i->first.find(vPrefix) == 0) { // edge label starts with vPrefix
					// split edge to
					vNewNode = new SuffixTreeNode(); // create new node
					vNewNode->addChild(i->first.substr(vPrefix.size()), // make new node parent of the old one
							           i->second,
							           false);
					//pParent->addChild(vPrefix, vNewNode, false); // add new node to list of parent's children
					pParent->addChild(vPrefix, vNewNode, vIsSuffix, pIdx); // add new node to list of parent's children
					vChildren->erase(i->first); // erase old entry
					vExists = true;
					break;
				}

				if (vPrefix.find(i->first) == 0) { // prefix starts with edge label
					// move down the tree
					vNewNode = i->second;
					vPrefix = i->first;
					vExists = true;
					break;
				}

				// check if, there is shared prefix
				vNoOfMatchingChars = vCompare.compare(vPrefix, i->first);
				//cout << "vPrefix=" << vPrefix << "\ti->first=" << i->first << "\tvNoOfMatchingChars=" << vNoOfMatchingChars << endl;
				if (vNoOfMatchingChars > 0) { // there are matching chars in current prefix and edge
					// split edge to matching part and the rest
					vNewNode = new SuffixTreeNode(); // create new node for matching chars
					//cout << vPrefix.substr(0, vNoOfMatchingChars) << endl;
					pParent->addChild(vPrefix.substr(0, vNoOfMatchingChars), vNewNode, false); // add new node to list of parent's children
                        //cout << i->first.substr(vNoOfMatchingChars - 1) << endl;
					vNewNode->addChild(i->first.substr(vNoOfMatchingChars), i->second, false);
					vChildren->erase(i->first); // erase old entry

					vNewNode->addChild(vPrefix.substr(vNoOfMatchingChars), new SuffixTreeNode(), true, pIdx);

					vExists = true;
					break;
				}
			}

			if (!vExists) { // new node needs to be created
				vNewNode = new SuffixTreeNode();
				pParent->addChild(vPrefix, vNewNode, vIsSuffix, pIdx);
			}
			buildSSubTreeNaive(vNewNode, pWords, vFrom, vTo, pPosFrom + vPrefix.length(), pIdx);

			vFrom = vTo + 1;
			vTo = pTo;
		}
	}
}

/*
 * Builds suffix tree for pWord
 */
void SuffixTree::buildSTreeNaive(string pWord) {
	// create table of suffixes
	int vNoOfSuffixes = pWord.length();
	string vSuffixes[vNoOfSuffixes];

	int i = 0;

	// array of suffixes
	for (i = 0; i < vNoOfSuffixes; i++) {
		vSuffixes[i] = pWord.substr(i);
	}

	// sort array of suffixes
	sorter.sort(vSuffixes, 0, vNoOfSuffixes - 1);

	buildSSubTreeNaive(root, vSuffixes, 0, (vNoOfSuffixes - 1), 0);
}

void SuffixTree::buildSTree(string pWord) {
	buildSTreeNaive(pWord);
}

/*
 * Identify node representing suffix
 * pNode - pointer to start node
 * pWord - text to be found
 * pPosFrom - start comparing from
 */
/*
SuffixTreeNode* SuffixTree::findSuffixNode(SuffixTreeNode* pNode,
		                                    string* pWord,
		                                    size_t pPosFrom) {

}
*/

// add word to the tree
void SuffixTree::addWord(string* pWord, int pIdx) {
	// create table of suffixes
	int vNoOfSuffixes = (*pWord).length();
	string vSuffixes[vNoOfSuffixes];

	int i = 0;

	// array of suffixes
	for (i = 0; i < vNoOfSuffixes; i++) {
		vSuffixes[i] = (*pWord).substr(i);
	}

	// sort array of suffixes
	sorter.sort(vSuffixes, 0, vNoOfSuffixes - 1);

	// add suffixes to the tree
	buildSSubTreeNaive(root, vSuffixes, 0, (vNoOfSuffixes - 1), 0, pIdx);
}


/*
 * pWords - list of words to be added to the tree
 */
void SuffixTree::buildSTree(vector<string>* pWords) {
	unsigned int i;
    vector<string>::size_type vWordsNo = pWords->size();

    for (i = 0; i < vWordsNo; i++) {
    	//cout << (i + 1) << ":" << (*pWords)[i] << endl;
    	addWord(&(*pWords)[i], (i + 1));
    }

}

/*
 * pNode - start node
 * pWord - text to be found
 * pPosFrom - start comparing from
 */
bool SuffixTree::search(SuffixTreeNode* pNode, string pWord, size_t pPosFrom) {
	if (pWord.size() < pPosFrom) {
		return false;
	}

	map<string, SuffixTreeNode*>* vChildren = pNode->getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;

	bool vResult = false;

	for (i = vChildren->begin(); i != vChildren->end() && !vResult; i++) {
		if (i->first.find(pWord.substr(pPosFrom)) == 0) { // edge label starts with pWord
			vResult = true;
		} else {
			if (pWord.find(i->first) == pPosFrom) { // pWord starts with edge label
				vResult = search(i->second,
						         pWord,
						         (pPosFrom + i->first.size()));
			}
		}
	}

	return vResult;
}

/*
 * Search for the pWord in the tree.
 * Returns:
 * 	true, if pWord is in the tree
 * 	false otherwise
 */
bool SuffixTree::search(string pWord) {
	return search(root, pWord, 0);
}

/*
 * returns true, if a word has been found
 * pNode - start node
 * pWord - text to be found
 * pPosFrom - start comparing from
 * pCurrDist - current distance from the pattern
 * pMaxDist - max allowed distance (defaults to 1)
 */
bool SuffixTree::searchApprox(SuffixTreeNode* pNode,
		                      string pWord,
		                      size_t pPosFrom,
		                      int pCurrDist,
		                      int pMaxDist) {
	if (pCurrDist > pMaxDist) return false;
	if (pWord.size() < pPosFrom) return false;

	map<string, SuffixTreeNode*>* vChildren = pNode->getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;

	bool vResult = false;
	size_t vMatchingLength = 0;
	int vDifferences = 0;

	//cout << "pWord=" << pWord << "\tpPosFrom=" << pPosFrom << "\t->\t" << pWord.substr(pPosFrom) << endl;

	for (i = vChildren->begin(); i != vChildren->end() && !vResult; i++) {
		//cout << "edge label=" << i->first << endl;
		vMatchingLength = 0;
		vDifferences = 0;

		// comparison
		while (   vMatchingLength < (pWord.size() - pPosFrom)
			   && vMatchingLength < i->first.size()
			   && vDifferences <= (pMaxDist - pCurrDist)) {

			if (pWord[vMatchingLength + pPosFrom] != i->first[vMatchingLength]) vDifferences++;

			if (vDifferences <= (pMaxDist - pCurrDist)) {
				vMatchingLength++;
			}
		}

		//cout << "edge label=" << i->first << "\tvMatchingLength=" << vMatchingLength << endl;

		if (vDifferences > (pMaxDist - pCurrDist)) {
			vResult = false;
			continue;
		}

		if (vMatchingLength == (pWord.size() - pPosFrom)) { // whole word is matched
			vResult = true;
		} else {
			if (vMatchingLength == i->first.size()) { // whole edge is matching, but it the word is longer
				// we need to go down the tree
				vResult = searchApprox(i->second,
									   pWord,
									   (pPosFrom + i->first.size()),
									   pCurrDist + vDifferences,
							           pMaxDist);
			}
		}
	}

	return vResult;
}

bool SuffixTree::searchApprox(string pWord) {
	return searchApprox(root, pWord, 0, 0, 1);
}

/*
 * returns matching string
 * pNode - start node
 * pWord - text to be found
 * pPosFrom - start comparing from
 * pCurrDist - current distance from the pattern
 * pMaxDist - max allowed distance (defaults to 1)
 */
string SuffixTree::getMatching(SuffixTreeNode* pNode,
		                       string pWord,
		                       size_t pPosFrom,
		                       int pCurrDist,
		                       int pMaxDist) {
	if (pCurrDist > pMaxDist) return "";
	if (pWord.size() < pPosFrom) return "";

	map<string, SuffixTreeNode*>* vChildren = pNode->getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;

	string vResult = "";
	size_t vMatchingLength = 0;
	int vDifferences = 0;

	cout << "pWord=" << pWord << "\tpPosFrom=" << pPosFrom << "\t->\t" << pWord.substr(pPosFrom) << endl;

    //
	for (i = vChildren->begin(); i != vChildren->end(); i++) {
		cout << "edge label=" << i->first << endl;
		vMatchingLength = 0;
		vDifferences = 0;

		// comparison
		while (   vMatchingLength < (pWord.size() - pPosFrom)
			   && vMatchingLength < i->first.size()
			   && vDifferences <= (pMaxDist - pCurrDist)) {

			if (pWord[vMatchingLength + pPosFrom] != i->first[vMatchingLength]) vDifferences++;

			if (vDifferences <= (pMaxDist - pCurrDist)) {
				vMatchingLength++;
			}
		}

		cout << "edge label=" << i->first << "\tvMatchingLength=" << vMatchingLength << endl;

		if (vDifferences > (pMaxDist - pCurrDist)) {
			vResult = "";
			continue; // move to the next child node
		}

		if (vMatchingLength > 0) {

            vResult = pWord.substr(pPosFrom, vMatchingLength);

            if (vMatchingLength == (pWord.size() - pPosFrom)) { // whole word is matched
                break;
            }

            if (vMatchingLength == i->first.size()) { // whole edge is matching, but the word is longer
                // we need to go down the tree
                vResult += getMatching(i->second,
                                       pWord,
                                       (pPosFrom + i->first.size()),
                                       pCurrDist + vDifferences,
                                       pMaxDist);

//                int vDist = comparer.distHamming(pWord, vResult);
//                cout << "pWord=" << pWord << "\tvResult=" << vResult << "\tvDist=" << vDist << endl;
                if (comparer.distHamming(pWord, vResult) <= pMaxDist) break;
            }
		}
	}

	return vResult;
}

string SuffixTree::getMatching(string pWord) {
    return getMatching(root, pWord, 0, 0, 1);
}

// end of SuffixTree


