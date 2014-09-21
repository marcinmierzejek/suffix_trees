/*
 * SuffixTreeDisplay.h
 *
 *  Created on: Feb 24, 2013
 *      Author: mierzejm
 */
#include <iostream>
#include "SuffixTree.h"

#ifndef SUFFIXTREEDISPLAY_H_
#define SUFFIXTREEDISPLAY_H_

// SuffixTreeDisplay
class SuffixTreeDisplay {
public:
	SuffixTreeDisplay();
	virtual ~SuffixTreeDisplay();
	void setTree(SuffixTree* pTree);
	void display(); // display whole tree
	//void displayIntoOStream(ofstream pOutput); // display whole tree - redirects into output stream
	map<int, string> restoreWords();
private:
	SuffixTree* tree;
	void displayNode(SuffixTreeNode* pNode, string pIndent); // display node
	void displayNodeWordList(SuffixTreeNode* pNode); // display list of worrds assigned to node
	void restoreWordsIdxs(map<int, string>* pResult,
	                      SuffixTreeNode* pNode,
	                      string pWord);
	void restoreWords(map<int, string>* pResult,
		              SuffixTreeNode* pNode,
		              string pCurrWord);
};

#endif /* SUFFIXTREEDISPLAY_H_ */
