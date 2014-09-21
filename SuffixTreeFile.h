#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "SuffixTree.h"

class SuffixTreeFile {
public:
    SuffixTreeFile();
    SuffixTreeFile(SuffixTree* pTree);
	virtual ~SuffixTreeFile();
	void setTree(SuffixTree* pTree);
	SuffixTree * getTree();
    //void save();
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & tree;
    }
    SuffixTree* tree;
//    void saveNode(SuffixTreeNode* pNode);
//    void saveEdge();
};
