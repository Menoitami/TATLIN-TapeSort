#include <iostream>
#include <fstream>
#include <string>
#include <ios>
#include <filesystem>
#include <stdlib.h>
#include <cmath>
#include <list>
#include <memory>
#include <ctime>
#include "Tape.hpp"
#include "TapeAlgorithms.h"


struct FileReader {
    std::ifstream file;  
    int currentNumber;   
    bool isEnd;         

    FileReader(const std::string& fileName) : file(fileName), isEnd(false) {
        if (!(file >> currentNumber)) {  
            isEnd = true;                
        }
    }

    ~FileReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    bool readNext() {
        if (file >> currentNumber) {
            return true;
        } else {
            isEnd = true;  
            return false;
        }
    }

    bool hasEnded() const {
        return isEnd;
    }
};


void clearTempDirectory() {
    std::filesystem::path tempDir = "temp";

    if (std::filesystem::exists(tempDir) && std::filesystem::is_directory(tempDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
            std::filesystem::remove(entry.path());
        }
        
        std::cout << "Temporary files and directory removed.\n";
    } else {
        std::cerr << "Temp directory does not exist.\n";
    }
}





int main(int argc, char** args) {

    auto start = std::chrono::high_resolution_clock::now();

    if (argc < 4) {
        std::cout << "Usage: <data.txt> <output.txt> <n> \n";
        return 1;
    }

    std::string outputFileName = args[2];
    std::ofstream outputMergedFile(outputFileName, std::ios::out);
    if (!outputMergedFile.is_open()) {
        std::cerr << "Can't create merged output file\n";
        return 1;
    }


    std::string fileName = args[1];
    const int N = algorithms::TapeAlgorithms::largestPowerOfTwo(std::stoi(args[3]));

    if (N<8){
        std::cerr<<"n must be greater than 8";
        return 1;

    }

    int K = N/4;

    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Can't open the file\n";
        return 1;
    }


    if (!std::filesystem::exists("temp")) {
        std::filesystem::create_directory("temp");
    }

    int totalNumbers = 0;
    int num;
    while (inputFile >> num) {
        ++totalNumbers;
    }

    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    static int fileCounter = 0;
    Tape<int> tape1, out, m1, m2;
    tape1.move(1);
    out.move(1);
    int processedCount = 0;
    int k =1;


    while (totalNumbers!=processedCount) {
        int count = 0;
        
        

        if (totalNumbers - processedCount < K) {
            K = algorithms::TapeAlgorithms::largestPowerOfTwo(totalNumbers - processedCount);
        }
        

        while (count < K && inputFile >> num) {
            if (count > 0) {
                tape1.move(1);
            }
            tape1.write(num);
            ++count;
            ++processedCount;
        }

        Tape<int>& sortedTape = algorithms::TapeAlgorithms::mergeSort(tape1, out,m1,m2, K);
        std::string outFileName = "./temp/sorted" + std::to_string(++fileCounter) + ".txt";

        std::ofstream outputFile(outFileName, std::ios::out);
        if (!outputFile.is_open()) {
            std::cerr << "Can't create output file\n";
            return 1;
        }

        int writtenCount = 0;
        while (!sortedTape.isStart() && writtenCount < count) {
            outputFile << sortedTape.read() << "\n";
            sortedTape.move(-1);
            ++writtenCount;
        }
        if (writtenCount < count) {
            outputFile << sortedTape.read() << "\n";
        }

        std::cout << "Processed number: " << processedCount << "\n";
        std::cout << "Files generated: " << fileCounter << "\n";
        
        outputFile.close();
        
        
    }
    
    inputFile.close();


    std::list<std::unique_ptr<FileReader>> cluster;
    processedCount=0;

    
     for (int i =1; i<=fileCounter; ++i){
        std::string outFileName = "./temp/sorted" + std::to_string(i) + ".txt";
        cluster.push_back(std::make_unique<FileReader>(outFileName));
     }
    
    
    processedCount = 0; 

    while (!cluster.empty()) {
        
        auto minIt = cluster.begin();
        for (auto it = cluster.begin(); it != cluster.end(); ++it) {
            if ((*it)->currentNumber < (*minIt)->currentNumber) {
                minIt = it;
            }
        }

        
        outputMergedFile << (*minIt)->currentNumber << "\n";
        ++processedCount;

        if (!(*minIt)->readNext()) {
            cluster.erase(minIt);
        }
    }

    // Закрываем выходной файл
    outputMergedFile.close();
    std::cout << "All files merged successfully. Processed " << processedCount << " numbers.\n";
    clearTempDirectory();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<"Program worked for: "<<std::chrono::duration<float>(end-start).count()<<" seconds\n";

    return 0;
}


