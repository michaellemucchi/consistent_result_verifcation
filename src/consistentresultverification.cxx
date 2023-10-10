#include <algorithm>
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"



int main(int argc, char** argv) {

    std::ifstream file1;  // create file object to read in data
    file1.open(argv[1]);  // open our sample json file
    nlohmann::json Object1; // create object to store data in 
    if (file1.is_open()) {  // if the file is open, do this
        file1 >> Object1;   // transfer data into an object
    }  
    file1.close();  // close the file to prevent memory leaks

    std::ifstream file2;  // create file object to read in data
    file2.open(argv[2]);   // open our sample json file
    nlohmann::json Object2; // create object to store data in 
    if (file2.is_open()) {  // if the file is open, do this
        file2 >> Object2;   // transfer data into an object
    }  
    file2.close();  // close the file to prevent memory leaks

    int arraylength = Object1["metadata"]["arraySize"];  // create integer value for the length of the arrays to traverse
    int num_samples = Object2["metadata"]["numSamples"];   // create integer value for number of samples to check
    nlohmann::json OutputObject;  // store output data in this object
    int counter = 0;  // create an integer counter to store number of mismatches
    std::string samplenum; // create a string for sample number 
    bool mismatch_happened = false; // use boolean to account for extra multiple mismatches in same sample

    for (int i = 1; i < num_samples + 1; i++) {  // iterate through the total number of samples
         if (i >= 10) { // formatting the sample part of the output object
                samplenum = "Sample";  // if the sample number is >= 10 then label it like so
            } else {
                samplenum = "Sample0";  // if the sample number is < 10 then label it like so
            }
            samplenum += std::to_string(i);  // add on the current sample number
        for (int j = 0; j < arraylength; j++) {  // iterate through the entire array and check for mismatches
            if (Object1[samplenum][j] != Object2[samplenum][j]) {  // if the value at the current index of the array doesn't match with the other one, continue 
                mismatch_happened = true;  // set our mismatch boolean to true to prevent multiple counter attempts
                int vals[2] = { Object1[samplenum][j], Object2[samplenum][j]};  // create array for mismatched values
                OutputObject[samplenum][argv[2]] = Object2[samplenum];  // write in one of the arrays from one of the files
                OutputObject[samplenum]["Mismatches"][std::to_string(j)] = vals;   // store these values in the output object
                OutputObject[samplenum][argv[1]] = Object1[samplenum];  // write in the other array that was compared into output object
            }
        }
        if (mismatch_happened) {  // multiple mismatch counter prevention
            counter++;  // iterate our counter
            mismatch_happened = false;  // reset the boolean and go again for another sample
        }
    }

    OutputObject["metadata"]["File1"] = Object1["metadata"];  // add in the metadata portion for file 1 to our output object
    OutputObject["metadata"]["File1"]["name"] = argv[1];  // add in the file1 name into our output object
    OutputObject["metadata"]["File2"] = Object2["metadata"];  // add in the metadata portion for file 2 to our output object
    OutputObject["metadata"]["File2"]["name"] = argv[2];  // add in the file2 name into our output object
    OutputObject["metadata"]["samplesWithConflictingResults"] = counter;  // apply our counter to samples with conflicting results in the output object

    std::cout << OutputObject.dump(2) << std::endl; // print out our output object using dump to make it look nice

    return 0;
}
