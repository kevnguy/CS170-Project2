#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

double leave_one_out_cross_validation(const vector<double>& data, unordered_set<int> current_set, int feature_to_add){
    double accuracy = rand() % 100;
    return accuracy;
}

void feature_search_demo(const vector<double>& data){
    for(int i = 1; i < data.size(); i++){
        cout << "On the " << i << "'th level of the search tree" << endl;
    }
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
        cout << line << endl;
        istringstream ss(line);
        double element;
        vector<double> temp;
        while(ss >> element)
            temp.push_back(element);
        table.push_back(temp);
    }
    
    cout << endl << endl;

    for(auto& i: table){
        for(auto j: i)
            cout << j << ' ';
        cout << endl;
    }   
    
    return 0;
}