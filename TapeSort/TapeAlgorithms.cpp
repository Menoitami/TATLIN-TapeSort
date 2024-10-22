#include "TapeAlgorithms.h"



int algorithms::TapeAlgorithms::readLeft(const int N, Tape<int>& in, Tape<int>& out) {
        int i = 0;
        while (i != N - 1 && in.getDistanceFromStart() != 0) {
            out.write(in.read());
            out.move(-1);  
            in.move(-1); 
            ++i;
        }
        out.write(in.read());
        ++i;
        return i;
    }

// Merge function for merge sort
void algorithms::TapeAlgorithms::merge(Tape<int>& m1, Tape<int>& m2, Tape<int>& out, const int n1, const int n2) {
    if (out.getSize() == 0) ++out;

    int m1Size = m1.getSize();
    int m2Size = m2.getSize();

    if (m1Size == 0 || m2Size == 0) {
        throw std::logic_error("tape is empty");
    }

    int i = 0, j = 0;
    int num1 = m1.read();
    int num2 = m2.read();

    while (i < n1 && j < n2 && i < m1Size && j < m2Size) {
        if (num1 > num2) {
            out.write(num1);
            ++out;
            if (i != n1 - 1 && i != m1Size - 1) ++m1;
            num1 = m1.read();
            ++i;
        } else {
            out.write(num2);
            ++out;
            if (j != n2 - 1 && j != m2Size - 1) ++m2;
            num2 = m2.read();
            ++j;
        }
    }

    // Write remaining elements
    while (i < n1 && i < m1Size) {
        out.write(m1.read());
        ++out;
        if (i != n1 - 1 && i != m1Size - 1) ++m1;
        ++i;
    }

    while (j < n2 && j < m2Size) {
        out.write(m2.read());
        ++out;
        if (j != n2 - 1 && j != m2Size - 1) ++m2;
        ++j;
    }
}

// Merge sort using tape storage
Tape<int>& algorithms::TapeAlgorithms::mergeSort(Tape<int>& in, Tape<int>& out,Tape<int>& m1,Tape<int>& m2,  unsigned int len) {
    if (len == 1) {
        out.write(in.read());
        return out; 
    }

    int n = 1;
    int n1;
    int n2;

    if (m1.getSize() == 0 )++m1;
   if (m2.getSize() == 0 )++m2;

    Tape<int>* readingTape = &in;
    Tape<int>* writingTape = &out;
    Tape<int>* temp = readingTape;

    while (n < len) {
        for (int i = 0; i < len; i += n * 2) {
            n1 = readLeft(n, *readingTape, m1);
            --(*readingTape);
            n2 = readLeft(n, *readingTape, m2);
            if (!(i + n * 2 >= len)) --(*readingTape);
            merge(m1, m2, *writingTape, n1, n2);
        }

        if ((*writingTape).getDistanceFromStart() == (*writingTape).getSize() - 1) {
            (*writingTape).pop_back();
        } else {
            --(*writingTape);
        }

        temp = readingTape;
        readingTape = writingTape;
        writingTape = temp;
        n *= 2;
    }
    return *readingTape;
}

// Function to find the largest power of two less than or equal to n
int algorithms::TapeAlgorithms::largestPowerOfTwo(int n) {
    return std::pow(2, static_cast<int>(std::log2(n)));
}

