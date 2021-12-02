#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <math.h>
#include <algorithm>

using namespace std;


float leave_one_out_cross_validation(const vector<vector<double>>& data, unordered_set<int> current_set, int feature_to_add){
    float accuracy = rand() % 100;
    return accuracy;
}

double euclidean_distance(vector<double> a, vector<double> b){
    double sum = 0;
    for(int i = 1; i < a.size(); i++)
        sum += (a[i]-b[i])*(a[i]-b[i]);
    return sqrt(sum);
}

void clear_other_features(vector<vector<double>>& data, const unordered_set<int> feature_set){
    for(int h = 0; h < data.size(); h++){
        for(int k = 1; k < data[0].size(); k++){
            if(feature_set.find(k) == feature_set.end())
                data[h][k] = 0;
        }
    }
}

float cross_val(vector<vector<double>> data, const unordered_set<int>& current_set, int feature_to_add){
	int nearest_neighbor_loc;
    int nearest_neighbor_label;
    double nearest_neighbor_dist;
	double distance;
	int num_correct = 0;
	unordered_set<int> new_feature_set = current_set;
	new_feature_set.insert(feature_to_add);

	clear_other_features(data, new_feature_set);

	vector<double> obj_to_classify;
	int obj_label;
	for(int i = 0; i < data.size(); i++){
		obj_to_classify = data[i];
		obj_label = data[i][0];

		nearest_neighbor_dist = numeric_limits<double>::max();
		nearest_neighbor_loc = -1;
		for(int j = 0; j < data.size(); j++){
			if(i != j){
				distance = euclidean_distance(obj_to_classify, data[j]);
				if(distance < nearest_neighbor_dist){
					nearest_neighbor_dist = distance;
					nearest_neighbor_loc = j;
					nearest_neighbor_label = data[nearest_neighbor_loc][0];
				}
				//cout << distance << " ";
				//printf("Ask if %d is nearest neighbor with %d\n",i,j);
			}
		}
		//cout << endl;
		//printf("Object %d is class %d\n", i, obj_label);
        //printf("Its nearest neighbor is %d which is in class %d\n", nearest_neighbor_loc, nearest_neighbor_label);
		if(obj_label == nearest_neighbor_label) num_correct++;
		// printf("Looping over i at the %d location\n", i);
		// printf("The %dth object is in the class %d\n", i, obj_label);
	}
	//cout << num_correct << "/" << data.size() << endl; 
	return (float) num_correct / data.size();
}

void fsearch(const vector<vector<double>>& data){
	unordered_set<int> curr_features;
	unordered_set<int> best_features;
	int feature_to_add;
	float best_acc;
	float best_res = 0;
	float accuracy;

	int count_c1 = 0;
	int count_c2 = 0;
	for(int k = 0; k < data.size(); k++){
		if(data[k][0] == 2) count_c2++; 
		else count_c1++; 
	}
	double most_occurring_value = (count_c1 > count_c2) ? count_c1 : count_c2;
	float rate = (float) most_occurring_value / data.size();
	printf("Default rate: %f\n", rate);

	for(int i = 1; i < data[0].size(); i++){
		printf("On the %d level of the search tree\n", i);
		feature_to_add = -1;
		best_acc = 0;

		for(int j = 1; j < data[0].size(); j++){
			if(curr_features.find(j) == curr_features.end()){
				//printf("\tConsider adding the %d feature\n", j);
				accuracy = cross_val(data, curr_features,j);

				if(accuracy > best_acc){
					best_acc = accuracy;
					feature_to_add = j;
				}
			}
		}
		curr_features.insert(feature_to_add);
		printf("On level %d I added feature %d to current set\n", i, feature_to_add);
		cout << "Features: { ";
		for(const auto& i : curr_features) cout << i << ' '; cout << '}' << endl;
		printf("Accuracy is now %f\n", best_acc);
		
		if(best_acc > best_res){
			best_res = best_acc;
			best_features = curr_features;
		}
	}
	cout << "Best acc of " << best_res << " { ";
	for(const auto& i : best_features) cout << i << ' '; cout << '}' << endl;
}

int main(){
    ifstream fin;
    //fin.open("test.txt");
    //fin.open("test2.txt");
    //fin.open("Ver_2_CS170_Fall_2021_Small_data__61.txt");
	//fin.open("Ver_2_CS170_Fall_2021_LARGE_data__22.txt");
	fin.open("Data\\Ver_2_CS170_Fall_2021_LARGE_data__37.txt");
	//fin.open("Data\\Ver_2_CS170_Fall_2021_Small_data__24.txt");

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

    fsearch(table);
    // float res = cross_validation(table,set,4);
    // cout << res;
    
    return 0;
}