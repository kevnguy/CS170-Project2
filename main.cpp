#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <math.h>

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

double euclidean_distance(vector<double> a, vector<double> b){
    double sum = 0;
    for(auto i : a)
        sum += (a[i]-b[i])*(a[i]-b[i]);
    return sqrt(sum);
}

float cross_validation(const vector<vector<double>>& data){
    float accuracy = 0;
    int num_correct = 0;
    for(int i = 0; i < data.size(); i++){
        vector<double> obj_to_classify = data[i];
        int label = data[i][0];

        double nearest_neighbor_dist = numeric_limits<double>::max();
        int nearest_neighbor_loc = numeric_limits<int>::max();
        int nearest_neighbor_label = numeric_limits<int>::max();

        for(int j = 0; j < data.size(); j++){
            if(i != j){
                double distance = euclidean_distance(obj_to_classify, data[j]);
                if(distance < nearest_neighbor_dist){
                    nearest_neighbor_dist = distance;
                    nearest_neighbor_loc = j;
                    nearest_neighbor_label = data[nearest_neighbor_loc][0];
                }
                //printf("Ask if %d is nearest neighbor with %d\n", i, j);
            }
        }
        if(label == nearest_neighbor_label) num_correct++;
        printf("Object %d is class %d\n", i, label);
        printf("Its nearest neighbor is %d which is in class %d\n", nearest_neighbor_loc, nearest_neighbor_label);
    }
    accuracy = (float) num_correct / data.size();
    return accuracy;
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
        if(!line.empty()){
            istringstream ss(line);
            double element;
            vector<double> temp;
            while(ss >> element)
                temp.push_back(element);
            table.push_back(temp);
        }
        //cout << line << endl;
    }
    
    // cout << endl << endl;

    // for(auto& i: table){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }   

    // feature_search_demo(table);
    float res = cross_validation(table);
    cout << res;
    
    return 0;
}