#include <fstream>

#include "SuffixTreeDisplay.h"
#include "SuffixTreeFile.h"
#include "BibleSuffixTree.h"

using namespace std;


void convertTextToBinary(char * pFileIn,
                         char * pFileOut) {
    cout << "save" << endl;
    string vLine;
    vector<string> vWords;
    ifstream vFile (pFileIn);
    if (vFile.is_open()) {
        // load file
        while ( getline (vFile, vLine) ) {
            //cout << line << endl;
            vWords.push_back(vLine);
        }
        vFile.close();

        // create new instance of SuffixTree
        SuffixTree * vTree = new SuffixTree();
        vTree->buildSTree(&vWords);

        // display tree
        SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
        vTreeDisp->setTree(vTree);
        vTreeDisp->display();

        // save tree to file
        ofstream ofs(pFileOut, fstream::binary | fstream::out);
        boost::archive::binary_oarchive oa(ofs);
        // write class instance to archive
        SuffixTreeFile * vTreeFile = new SuffixTreeFile();
        vTreeFile->setTree(vTree);
        oa << vTreeFile;
    } else cout << "Unable to open file";
}

void loadFromBinary(char * pFileIn) {
    cout << "load" << endl;
    try {
        SuffixTreeFile * vTreeFile = new SuffixTreeFile();
        // create and open an archive for input
        std::ifstream ifs(pFileIn, fstream::binary | fstream::in);
        boost::archive::binary_iarchive ia(ifs);
        // read class state from archive
        ia >> vTreeFile;

        SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
        vTreeDisp->setTree(vTreeFile->getTree());
        vTreeDisp->display();
    } catch (exception & e) {
        cout << "Exception: " << e.what() << endl;
    }
}

void convertBibleTextToBinary(char * pFileIn,
                              char * pFileOut) {
    BibleSuffixTree * vBible = new BibleSuffixTree();
    string vVerse;
    vector<string> vVerses;
    ifstream vFile (pFileIn);
    if (vFile.is_open()) {
        // load file
        while ( getline (vFile, vVerse) ) {
            //cout << line << endl;
            vVerses.push_back(vVerse);
        }
        vFile.close();

        vBible->loadVerses(&vVerses);

        // display tree
        SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
        vTreeDisp->setTree(vBible);
        vTreeDisp->display();

        // save bible tree to file
        ofstream ofs(pFileOut, fstream::binary | fstream::out);
        boost::archive::binary_oarchive oa(ofs);
        // write class instance to archive
        //SuffixTreeFile * vTreeFile = new SuffixTreeFile();
        //vTreeFile->setTree(vTree);
        oa << vBible;
    } else cout << "Unable to open file";
}

void loadAndDisplayBible(char * pFileName) {
    BibleSuffixTree * vBible = new BibleSuffixTree();
    string vVerse;
    vector<string> vVerses;
    ifstream vFile (pFileName);
    if (vFile.is_open()) {
        // load file
        while ( getline (vFile, vVerse) ) {
            //cout << line << endl;
            vVerses.push_back(vVerse);
        }
        vFile.close();

        vBible->loadVerses(&vVerses);

        // display tree
        SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
        vTreeDisp->setTree(vBible);
        vTreeDisp->display();
    } else cout << "Unable to open file";
}

BibleSuffixTree * loadBibleFromBinary(char * pFileIn) {
    try {
        BibleSuffixTree * vBible = new BibleSuffixTree();
        // create and open an archive for input
        std::ifstream ifs(pFileIn, fstream::binary | fstream::in);
        boost::archive::binary_iarchive ia(ifs);
        // read class state from archive
        ia >> vBible;

        //SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
        //vTreeDisp->setTree(vBible);
        //vTreeDisp->display();

        return vBible;

    } catch (exception & e) {
        cout << "Exception: " << e.what() << endl;
    }
}

int main(int argc, char* argv[]) {
	cout << "start" << endl;
	cout << argc << endl;
	cout << argv[0] << endl;
	cout << argv[1] << endl;

    if (argc > 1) {
        cout << "test na argc " << endl;
        // bible
        if (strcmp(argv[1], "bible") == 0) {
            loadAndDisplayBible(argv[2]);
        }
        // load bible and save to binary file
        if (strcmp(argv[1], "save_bible") == 0) {
            convertBibleTextToBinary(argv[2], argv[3]);
        }
        // save tree
        if (strcmp(argv[1], "save") == 0) {
            convertTextToBinary(argv[2], argv[3]);
        }

        // load tree
        if (strcmp(argv[1], "load") == 0) {
            loadFromBinary(argv[2]);
        }

        // load bible tree from binary file
        if (strcmp(argv[1], "load_bible") == 0) {
            BibleSuffixTree * vBible = loadBibleFromBinary(argv[2]);
        }

         // load bible tree from binary file and then test it
        if (strcmp(argv[1], "test_bible") == 0) {
            try {
                cout << "loading...";

                BibleSuffixTree * vBible = loadBibleFromBinary(argv[2]);

                SuffixTreeDisplay * vTreeDisp = new SuffixTreeDisplay();
                vTreeDisp->setTree(vBible);
                vTreeDisp->display();

                cout << endl;

                string vWordToFind = "";
                string vWordFound = "";
                cin >> vWordToFind;
                while (vWordToFind.compare("exit") != 0) {

                    cout << "vWordToFind:\t" << vWordToFind << endl;
                    vWordFound = vBible->getMatching(vWordToFind);
                    cout << "vWordFound:\t" << vWordFound << endl;
                    if (vWordFound.size() > 0) {
                        cout << "found: " << vWordFound;
                    } else {
                        cout << "not found";
                    }
                    cout << endl;

                    cin >> vWordToFind;
                }

            } catch (exception & e) {
                cout << "Exception: " << e.what() << endl;
            }
        }
    }


	return 0;
}
