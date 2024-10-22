#include <iostream>
#include <list>



void merge(const std::list<int>& m1, const std::list<int>& m2, std::list<int>& out) {
    auto iter1 = m1.begin();
    auto iter2 = m2.begin();

    while (iter1 != m1.end() && iter2 != m2.end()) {
        if (*iter1 < *iter2) {
            out.push_back(*iter1++);
        } else {
            out.push_back(*iter2++);
        }
    }

    out.insert(out.end(), iter1, m1.end());
    out.insert(out.end(), iter2, m2.end());
}


void readN(const int N, std::list<int>& in,   std::list<int>& out,std::list<int>::iterator& iter){
    int i =0;
    while (iter != out.end() && i<N){
        in.push_back(*(iter++));
        ++i;
    }
};


std::list<int> mergeSort(std::list<int>& in){
    int n= 1;
    int size = in.size();

    std::list<int> vec;
    std::list<int> m1;
    std::list<int> m2;
    auto inIter = in.begin();
    auto vecIter = vec.begin();

    while (n<size){
        
        while(inIter!= in.end()){
            readN(n,m1,in, inIter);
            readN(n,m2,in, inIter);

            merge(m1,m2, vec);
            m1.clear();
            m2.clear();

        }

        n*=2;
        in.clear();

        if(n>=size){
            return vec;
        }

        vecIter = vec.begin();
        while(vecIter!= vec.end()){
            readN(n,m1,vec, vecIter);
            readN(n,m2,vec, vecIter);

            merge(m1,m2, in);
            m1.clear();
            m2.clear();

        }
      
        n*=2;

        inIter = in.begin();
        vec.clear();
        
    }
  

    return in;

}
    
    
int main(){
    

    std::list<int> vec = {1,-3};
    std::list<int> out = mergeSort(vec);

    for( auto vc : out){

        std::cout<<vc<<" ";

    }

    return 0; 
}