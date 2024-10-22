#include <iostream>
#include <fstream>
#include <string>
#include <ios>
#include <stdlib.h>
#include <cmath>
#include "Tape.hpp"

namespace algorithms {

class TapeAlgorithms {
public:
    // Function to read from the left side of the tape
    static int readLeft(const int N, Tape<int>& in, Tape<int>& out);

    // Merge sort using tape storage
    static Tape<int>& mergeSort(Tape<int>& in, Tape<int>& out,Tape<int>& m1,Tape<int>& m2,  unsigned int len);
    // Function to find the largest power of two less than or equal to n
    static int largestPowerOfTwo(int n);

private:   

    static void merge(Tape<int>& m1, Tape<int>& m2, Tape<int>& out, const int n1, const int n2);
};

} // namespace algorithms