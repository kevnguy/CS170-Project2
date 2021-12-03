#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <math.h>
#include <algorithm>
#include <numeric>

//https://en.cppreference.com/w/cpp/algorithm/iota

using namespace std;

float leave_one_out_cross_validation(const vector<vector<float>>& data, vector<int> current_set, int feature_to_add){
    float accuracy = rand() % 100;
    return accuracy;
}

template <typename T>
float euclidean_distance(vector<T> a, vector<T> b){
    float sum = 0;
    for(int i = 1; i < a.size(); i++)
        sum += (a[i]-b[i])*(a[i]-b[i]);
    return sqrt(sum);
}

void clear_other_features(vector<vector<float>>& data, const vector<int>& feature_set){
	// cout << "| ";
	// for(auto i : feature_set) cout << i << ' '; cout << endl;
	// cout << "Before" << endl;
	// for(auto& i: data){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }

    for(int h = 0; h < data.size(); h++){
        for(int k = 1; k < data[0].size(); k++){
            if(find(feature_set.begin(), feature_set.end(), k) == feature_set.end())
                data[h][k] = 0;
        }
    }
	// cout << endl << "After" << endl;

	// for(auto& i: data){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }

}

float cross_val(vector<vector<float>> data, const vector<int>& current_set, int feature_to_modify, bool forward){
	int nearest_neighbor_loc;
    int nearest_neighbor_label;
    float nearest_neighbor_dist;
	float distance;
	int num_correct = 0;
	vector<int> new_feature_set = current_set;
	
	if(forward){
		if(feature_to_modify > 0)
			new_feature_set.push_back(feature_to_modify); 
	}
	else{
		vector<int>::iterator it; it = find(new_feature_set.begin(), new_feature_set.end(), feature_to_modify);
		int index = it - new_feature_set.begin();
		new_feature_set.erase(new_feature_set.begin()+index);
	}
	
	clear_other_features(data, new_feature_set);

	// for(auto& i: data){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }

	vector<float> obj_to_classify;
	int obj_label;
	for(int i = 0; i < data.size(); i++){
		obj_to_classify = data[i];
		obj_label = data[i][0];

		nearest_neighbor_dist = numeric_limits<float>::max();
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

void fsearch(const vector<vector<float>>& data){
	vector<int> curr_features;
	vector<int> best_features;
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
	float most_occurring_value = (count_c1 > count_c2) ? count_c1 : count_c2;
	float rate = best_res = (float) most_occurring_value / data.size();
	printf("Default rate: %f\n", rate);

	for(int i = 1; i < data[0].size(); i++){
		printf("On the %d level of the search tree\n", i);
		feature_to_add = -1;
		best_acc = 0;

		for(int j = 1; j < data[0].size(); j++){
			if(find(curr_features.begin(), curr_features.end(), j) == curr_features.end()){
				//printf("\tConsider adding the %d feature\n", j);
				accuracy = cross_val(data, curr_features,j,true);

				if(accuracy > best_acc){
					best_acc = accuracy;
					feature_to_add = j;
				}
			}
		}
		curr_features.push_back(feature_to_add);
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

void bsearch(const vector<vector<float>>& data){
	vector<int> curr_features(data[0].size()-1);

	iota(curr_features.begin(), curr_features.end(), 1);

	vector<int> best_features;
	vector<int> temp_features;
	int index;
	vector<int>::iterator it;
	int feature_to_remove;
	float best_acc;
	float best_res = cross_val(data, curr_features, 0,true);
	float accuracy;

	cout << "Using all the features, accuracy is " << best_res << endl;
	cout << "Begining search" << endl;
	for(int i = 1; i < data[0].size(); i++){
		//printf("On the %d level of the search tree\n", i);
		feature_to_remove = -1;
		best_acc = 0;
		temp_features = curr_features;
		
		for(int j = 1; j < data[0].size(); j++){
			if(find(curr_features.begin(), curr_features.end(), j) != curr_features.end()){
				it = find(temp_features.begin(), temp_features.end(), j);
				index = it - temp_features.begin();
				temp_features.erase(temp_features.begin()+index); 

				cout << "\tRemoving feature " << j;
				accuracy = cross_val(data, curr_features,j,false);
				cout << ". Using feature(s) { ";
				for(const auto& k : temp_features) cout << k << ' '; 
				cout << "} accuracy is " << accuracy << endl;
			
				temp_features = curr_features;

				if(accuracy > best_acc){
					best_acc = accuracy;
					feature_to_remove = j;
				}
			}
		}
		if(feature_to_remove != -1){
			it = find(curr_features.begin(), curr_features.end(), feature_to_remove);
			index = it - curr_features.begin();
			curr_features.erase(curr_features.begin()+index); 
			cout << "Removed feature " << feature_to_remove << ". Using feature(s) { ";
			for(const auto& i : curr_features) cout << i << ' '; 
			cout << "} Accuracy is now " << best_acc;
		}

		if(best_acc > best_res){
			best_res = best_acc;
			best_features = curr_features;
			cout << endl;
		}
		else if(best_acc == best_res){
			cout << ". Accuracy REMAINED the same" << endl;
		}
		else{
			cout << ". Accuracy has DECREASED!" << endl;
		}
	}
	cout << endl << "------------------------- Results -------------------------" << endl;
	cout << "Best accuracy acheived: " << best_res << " using feature(s) { ";
	for(const auto& i : best_features) cout << i << ' '; cout << '}' << endl;
}

int main(){
    ifstream fin;
    //fin.open("Data\\Test\\test.txt");
    //fin.open("test2.txt");
	//fin.open("Ver_2_CS170_Fall_2021_LARGE_data__22.txt");
	//fin.open("Ver_2_CS170_Fall_2021_LARGE_data__27.txt");
	//fin.open("Data\\Ver_2_CS170_Fall_2021_LARGE_data__37.txt");
	fin.open("Data\\Ver_2_CS170_Fall_2021_Small_data__24.txt");

    if(!fin.is_open()){
        cout << "Error opening file. Exiting..." << endl;
        exit(1);
    }

    vector<vector<float>> table;
    string line;
    while(getline(fin,line)){
        if(!line.empty()){
            istringstream ss(line);
            float element;
            vector<float> temp;
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

    //fsearch(table);
	bsearch(table);
    // float res = cross_validation(table,set,4);
    // cout << res;
    
    return 0;
}