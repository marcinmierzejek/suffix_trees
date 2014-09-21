#include "BibleSuffixTree.h"

BibleSuffixTree::BibleSuffixTree() {
    lastVerseId = 1;
}

BibleSuffixTree::~BibleSuffixTree()
{
    //dtor
}

int BibleSuffixTree::getVerseId(string pVerseDesc) {
    int vResult = -1;

    vResult = verses[pVerseDesc];
    if (vResult == 0) {
        verses[pVerseDesc] = lastVerseId;
        vResult = lastVerseId++;
    }

    return vResult;
}

void BibleSuffixTree::addVerse(string *pVerseContent,
                               int pVerseID) {
    vector<string> vWords;

 //   cout << *pVerseContent << endl;

    wordOpsHelper.splitVerseIntoWords(pVerseContent, &vWords, wordOpsHelper.LOWER_CASE);

    int vWordIdx = 0;
    vector<string>::iterator vWordsIt;
    for (vWordsIt = vWords.begin(); vWordsIt != vWords.end(); vWordsIt++) {
        addWord(&(*vWordsIt), ((pVerseID * 100) + (vWordIdx++)));
    }
}

void BibleSuffixTree::addVerse(string pVerse) {
    size_t vTabPos = pVerse.find("\t");
    string vVerseDesc = pVerse.substr(0, vTabPos);
    string vVerse = pVerse.substr(vTabPos + 1, string::npos);

    // determine index of verse
    int vVerseId = getVerseId(vVerseDesc);

    addVerse(&vVerse, vVerseId);
}

void BibleSuffixTree::loadVerses(vector<string> * pVerses) {
    vector<string>::iterator vIt;
    for (vIt = pVerses->begin(); vIt != pVerses->end(); vIt++) {
        addVerse(*vIt);
    }
}
