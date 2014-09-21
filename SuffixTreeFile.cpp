#include <fstream>

#include "SuffixTreeFile.h"

using namespace std;

SuffixTreeFile::SuffixTreeFile() {
	// TODO Auto-generated constructor stub
}

SuffixTreeFile::SuffixTreeFile(SuffixTree* pTree) {
    tree = pTree;
    //buffer = new char[1024];
}

SuffixTreeFile::~SuffixTreeFile() {
//	delete[] buffer;
}

void SuffixTreeFile::setTree(SuffixTree* pTree) {
	tree = pTree;
}

SuffixTree * SuffixTreeFile::getTree() {
    return tree;
}

//void SuffixTreeFile::save() {
//    saveNode(tree->getRoot());
//}
