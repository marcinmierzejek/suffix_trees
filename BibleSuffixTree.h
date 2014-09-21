#ifndef BIBLESUFFIXTREE_H
#define BIBLESUFFIXTREE_H

#include "SuffixTree.h"
#include "helpers.h"


class BibleSuffixTree : public SuffixTree {
public:
    BibleSuffixTree();
    virtual ~BibleSuffixTree();
    void loadVerses(vector<string> * pVerses);
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & lastVerseId;
        ar & verses;
        ar & root;
    }
    int lastVerseId;
    map<string, int> verses;
    WordOperations wordOpsHelper;
    int getVerseId(string pVerseDesc);
    void addVerse(string pVerse);
    void addVerse(string *pVerseContent,
                 int pVerseID);
};

#endif // BIBLESUFFIXTREE_H
