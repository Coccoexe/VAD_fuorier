#include <iostream>
#include "Vad.hpp"

using namespace std;

const string INPUT_FILE_NAME = "inputaudio";
const string OUTPUT_FILE_NAME = "outputVAD";

int main()
{

    int N;

    cout<<"Indicare il numero del file da processare: ";
    cin>>N;

    cout<<"Processing file: "<<N<<endl;
    string input = INPUT_FILE_NAME + to_string(N);
    string output = OUTPUT_FILE_NAME + to_string(N); 

    Vad v = Vad(input,output);
    cout<<"Done!\n";
}