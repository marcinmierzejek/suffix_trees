#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED

class Simulator {
public:
    void run();
    void generateTestData();
    void loadTestData();
private:
    SuffixTree tree;
};


#endif // SIMULATOR_H_INCLUDED
