#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

float leave_one_out_cross_validation(const vector<vector<double>>& data, unordered_set<int> current_set, int feature_to_add){
    float accuracy = rand() % 100;
    return accuracy;
}


void feature_search_demo(const vector<vector<double>>& data){
    float accuracy;
    unordered_set<int> curr_features;
    for(int i = 1; i < data[0].size(); i++){
        cout << "On the " << i << "th level of the search tree" << endl;
        int feature_to_add = 0;
        float best_acc = 0;
        for(int j = 1; j < data[0].size(); j++){
            if(curr_features.find(feature_to_add) == curr_features.end()){
                accuracy = leave_one_out_cross_validation(data,curr_features,j+1);
                cout << "-- Consider adding the " << j << "th feature" << endl;

                if (accuracy > best_acc){
                    best_acc = accuracy;
                    feature_to_add = j;
                }
            }
        }
        printf("On level %d I added the feature %d to the current set\n", i, feature_to_add);
        curr_features.insert(feature_to_add);
    }
}

float cross_validation(const vector<vector<double>>& data){
    for(int i = 0; i < data.size(); i++){
        int label = data[i][0];
        printf("Looping over i at the %d index\n", i);
        printf("The %dth object is in the class %d\n", i, label);
    }
    return 1.0;
}

int main(){
    ifstream fin;
    fin.open("test.txt");

    if(!fin.is_open()){
        cout << "Error opening file. Exiting..." << endl;
        exit(1);
    }

    vector<vector<double>> table;
    string line;
    while(getline(fin,line)){
        //cout << line << endl;
        istringstream ss(line);
        double element;
        vector<double> temp;
        while(ss >> element)
            temp.push_back(element);
        table.push_back(temp);
    }
    
    // cout << endl << endl;

    // for(auto& i: table){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }   

    feature_search_demo(table);
    // cross_validation(table);
    
    return 0;
}