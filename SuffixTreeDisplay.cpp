/*
 * SuffixTreeDisplay.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: mierzejm
 */

#include <fstream>

#include "SuffixTreeDisplay.h"

using namespace std;

SuffixTreeDisplay::SuffixTreeDisplay() {
	// TODO Auto-generated constructor stub
 }

SuffixTreeDisplay::~SuffixTreeDisplay() {
	// TODO Auto-generated destructor stub
}

void SuffixTreeDisplay::setTree(SuffixTree* pTree) {
	tree = pTree;
}

// display tree
void SuffixTreeDisplay::display() {
	displayNode(tree->getRoot(), "");
}

/*
 * display list of words assigned to the node
 * pNode - terminator node with list of words
 */
void SuffixTreeDisplay::displayNodeWordList(SuffixTreeNode* pNode) {
	set<int> *vWords = pNode->getWordIndxs();
	set<int>::iterator i;

	cout << ":";
	for (i = vWords->begin(); i != vWords->end(); i++) {
		cout << *i << ", ";
	}
}

// display node
void SuffixTreeDisplay::displayNode(SuffixTreeNode* pNode, string pIndent) {
	map<string, SuffixTreeNode*>* vChildren = pNode->getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;
	string vNextIndent = "   ";

	for (i = vChildren->begin(); i != vChildren->end(); i++) { // check all the children nodes
	    if (i->first == i->second->TERMINATOR) { // this is terminator node
	    	cout << i->first;
	    	displayNodeWordList(i->second);
	    } else {
	    	cout << endl << pIndent;
	    	if (pIndent != "") {
	    		cout << "|";
	    		vNextIndent = "    ";
	    	}
	    	cout << "---" << i->first << "-->";
   		}
	    displayNode(i->second, pIndent + vNextIndent);
	}
}

/*
 * Extracts words indexes from subtree
 * pResult - destination for indexes of word
 * pNode - terminator node
 * pWord - word
 */
void SuffixTreeDisplay::restoreWordsIdxs(map<int, string>* pResult,
                                         SuffixTreeNode* pNode,
                                         string pWord) {
	set<int> *vWords = pNode->getWordIndxs();
	set<int>::iterator i;
	map<int, string>::iterator vElement;

	for (i = vWords->begin(); i != vWords->end(); i++) {
		vElement = pResult->find(*i); // find element with the word index
		if (vElement == pResult->end()) { // the key has not been defined yet
			pResult->insert(pair<int, string>(*i, pWord));
		} else {
			if (vElement->second.size() < pWord.size()) { // new word is longer than the one stored in the map
				vElement->second = pWord;
			}
		}
	}
}

/*
 * Extracts words from subtree
 * pResult - destination for words
 * pNode - root of the subtree
 * pCurrWord - prefix to be added
 */
void SuffixTreeDisplay::restoreWords(map<int, string>* pResult,
		                             SuffixTreeNode* pNode,
		                             string pCurrWord) {

	map<string, SuffixTreeNode*>* vChildren = pNode->getChildrenPointer();
	map<string, SuffixTreeNode*>::iterator i;
	string vCurrWord;

	for (i = vChildren->begin(); i != vChildren->end(); i++) { // check all the children nodes
	    if (i->first == i->second->TERMINATOR) { // this is terminator node
	    	restoreWordsIdxs(pResult, i->second, pCurrWord);
	    } else {
	    	vCurrWord = pCurrWord + i->first;
	    	restoreWords(pResult, i->second, vCurrWord);
   		}
	}
}

// returns vector of words stored in the tree
map<int, string> SuffixTreeDisplay::restoreWords() {
	map<int, string> vResult;
	restoreWords(&vResult,
				 tree->getRoot(),
				 "");
	return vResult;
}



// end of SuffixTreeDisplay



