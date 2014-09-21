/*
 * SuffixTree.h
 *
 *  Created on: Feb 24, 2013
 *      Author: mierzejm
 */
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>


#include "helpers.h"

using namespace std;

#ifndef SUFFIXTREE_HPP_
#define SUFFIXTREE_HPP_

// SuffixTreeNode
class SuffixTreeNode {
public:
	static const string TERMINATOR;
	SuffixTreeNode();
	virtual ~SuffixTreeNode();
	map<string, SuffixTreeNode*> getChildren();
	map<string, SuffixTreeNode*>* getChildrenPointer();
	set<int>* getWordIndxs(); // get numbers of words
	void addChild(string pKey,
            	  SuffixTreeNode* pNode,
            	  bool pIsSuffix,
                  int pIdx = 0);
	void makeSuffixOf(int pIdx);
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & wordIndxs;
        ar & children;
    }
	map<string, SuffixTreeNode*> children;
	set<int> wordIndxs; // numbers of words
	SuffixTreeNode* getTerminatorNode();
	void addWordIdx(int pIdx);
};

// SuffixTree
class SuffixTree {
public:
	SuffixTree();
	virtual ~SuffixTree();
	SuffixTreeNode* getRoot();
	void buildSTree(string pWord);
	void buildSTree(vector<string>* pWords);
	bool search(string pWord);
	bool searchApprox(string pWord);
	string getMatching(string pWord);
protected:
    void addWord(string* pWord, int pIdx); // add word to the tree
    SuffixTreeNode* root; // root of the tree
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & root;
    }
	Sort sorter;
	Compare comparer;
	string longestPrefix(string pWords[], int pFrom, int* pTo, size_t pPosFrom, bool pIsFirst);
	void buildSSubTreeNaive(SuffixTreeNode* pParent,
							string pWords[],
							int pFrom,
							int pTo,
							size_t pPosFrom,
							int pIdx = 0);
	void buildSTreeNaive(string pWord);
	bool search(SuffixTreeNode* pParent, string pWord, size_t pPosFrom);
	bool searchApprox(SuffixTreeNode* pParent,
			          string pWord,
			          size_t pPosFrom,
			          int pCurrDist,
			          int pMaxDist = 1);
    string getMatching(SuffixTreeNode* pParent,
			           string pWord,
			           size_t pPosFrom,
			           int pCurrDist,
			           int pMaxDist = 1);
};


#endif /* SUFFIXTREE_HPP_ */
