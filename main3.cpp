#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <math.h>
#include <algorithm>
#include <numeric>

//https://en.cppreference.com/w/cpp/algorithm/iota
//https://www.mathsisfun.com/data/standard-deviation-formulas.html
//https://www.statology.org/z-score-normalization/

using namespace std;

template <typename T>
double sum(const vector<T>& col){
	double sum = 0;
	for(const auto& i : col) sum += i;
	return sum;
}

template <typename T>
double std_dev(const vector<T>& col){
	vector<T> diff_square;
	double col_mean = sum(col) / col.size();
	double mean_sqr_diff = 0;
	for(const auto& i : col)
		diff_square.push_back((i - col_mean)*(i - col_mean));
	mean_sqr_diff = (sum(diff_square) / (diff_square.size()-1));
	return sqrt(mean_sqr_diff);
}


template <typename T>
void z_normalize(vector<vector<T>>& table){
	cout << "Data set of " << table.size() << " with " << table[0].size()-1 << " features" << endl;
	vector<T> temp;
	double col_mean;
	double standard_dev;
	for(int i = 1; i < table[0].size(); i++){
		temp.clear();
		for(int j = 0; j < table.size(); j++)
			temp.push_back(table[j][i]);
		col_mean = sum(temp) / temp.size();
		standard_dev = std_dev(temp);
		if(standard_dev)
			for(int k = 0; k < table.size(); k++)
				table[k][i] = (table[k][i] - col_mean) / standard_dev;
	}
}

template <typename T>
float euclidean_distance(const vector<T>& a, const vector<T>& b){
    float sum = 0;
    for(int i = 1; i < a.size(); i++)
        sum += (a[i]-b[i])*(a[i]-b[i]);
    return sqrt(sum);
}

void clear_other_features(vector<vector<float>>& data, const vector<int>& feature_set){
    for(int h = 0; h < data.size(); h++){
        for(int k = 1; k < data[0].size(); k++){
            if(find(feature_set.begin(), feature_set.end(), k) == feature_set.end())
                data[h][k] = 0;
        }
    }
}

float cross_val(vector<vector<float>> data, const vector<int>& current_set){
	vector<float> obj_to_classify;
	int obj_label;
	int nearest_neighbor_loc;
    int nearest_neighbor_label;
    float nearest_neighbor_dist;
	float distance;
	int num_correct = 0;
	
	clear_other_features(data, current_set);

	for(int i = 0; i < data.size(); i++){
		//LOOCV
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
	//cout << ' ' << num_correct << "/" << data.size() << endl; 
	return (float) num_correct / data.size();
}

// performs forward selection
void fsearch(const vector<vector<float>>& data){
	cout << "Beginning forward selection" << endl;
	vector<int> curr_features;
	vector<int> best_features;
	vector<int> test_features;
	int feature_to_add;
	float local_acc;
	float global_acc = 0;
	float accuracy;

	// Default rate calculation
	global_acc  = cross_val(data,curr_features);
	cout << "Default rate: " << global_acc << endl;

	for(int i = 1; i < data[0].size(); i++){
		printf("On the %d level of the search tree\n", i);
		feature_to_add = -1;
		local_acc = 0;

		for(int j = 1; j < data[0].size(); j++){
			if(find(curr_features.begin(), curr_features.end(), j) == curr_features.end()){
				//printf("\tConsider adding the %d feature\n", j);
				test_features = curr_features;
				cout << "\tAdding feature " << j;
				test_features.push_back(j);
				accuracy = cross_val(data, test_features);
				cout << ". Using feature(s) { ";
				for(const auto& k : test_features) cout << k << ' '; 
				cout << "} accuracy is " << accuracy << endl;

				if(accuracy > local_acc){
					local_acc = accuracy;
					feature_to_add = j;
				}
			}
		}
		curr_features.push_back(feature_to_add);
		cout << "Added feature " << feature_to_add;
		cout << ". Using feature(s) { ";
		for(const auto& k : curr_features) cout << k << ' '; 
		cout << "} accuracy is " << local_acc << endl;
		
		if(local_acc > global_acc){
			global_acc = local_acc;
			best_features = curr_features;
		}
	}

	cout << endl << "------------------------- Results -------------------------" << endl;
	cout << "Best accuracy acheived: " << global_acc << " using feature(s) { ";
	for(const auto& i : best_features) cout << i << ' '; cout << '}' << endl;
}

// finds and removes feature from feature set
void remove_feature(int feature, vector<int>& feature_set){
	int index;
	vector<int>::iterator it = find(feature_set.begin(), feature_set.end(), feature);
	index = it - feature_set.begin();
	feature_set.erase(feature_set.begin()+index);
}

// performs backwards elimination
void bsearch(const vector<vector<float>>& data){
	cout << "Beginning backwards elimination" << endl;
	vector<int> curr_features(data[0].size()-1);
	iota(curr_features.begin(), curr_features.end(), 1);

	vector<int> best_features = curr_features;
	vector<int> test_features;
	int feature_to_remove;
	float accuracy;
	float local_acc;
	float global_acc = cross_val(data, curr_features);

	cout << "Using all the features, accuracy is " << global_acc << endl;
	cout << "Beginning search" << endl;

	for(int i = 1; i < data[0].size(); i++){
		cout << "On the " << i << " level of the search tree" << endl;
		feature_to_remove = -1;
		local_acc = 0;
		
		// Test removal of feature	
		for(int j = 1; j < data[0].size(); j++){
			if(find(curr_features.begin(), curr_features.end(), j) != curr_features.end()){
				test_features = curr_features;

				cout << "\tRemoving feature " << j;
				remove_feature(j, test_features);
				accuracy = cross_val(data, test_features);
				cout << ". Using feature(s) { ";
				for(const auto& k : test_features) cout << k << ' '; 
				cout << "} accuracy is " << accuracy << endl;

				if(accuracy > local_acc){
					local_acc = accuracy;
					feature_to_remove = j;
				}
			}
		}
		remove_feature(feature_to_remove, curr_features);
		cout << "Removed feature " << feature_to_remove << ". Using feature(s) { ";
		for(const auto& i : curr_features) cout << i << ' '; 
		cout << "} Accuracy is now " << local_acc;

		if(local_acc > global_acc){
			global_acc = local_acc;
			best_features = curr_features;
			cout << endl;
		}
		else if(fabs(global_acc - local_acc) < 0.0001){
			cout << ". Accuracy REMAINED the same" << endl;
			best_features = curr_features;
		}
		else{
			cout << ". Accuracy has DECREASED!" << endl;
		}
	}
	cout << endl << "------------------------- Results -------------------------" << endl;
	cout << "Best accuracy acheived: " << global_acc << " using feature(s) { ";
	for(const auto& i : best_features) cout << i << ' '; cout << '}' << endl;
}

vector<vector<float> > parse_table(ifstream& in){
	vector<vector<float>> table;
	string line;
	float element;
    while(getline(in,line)){
        if(!line.empty()){
            istringstream ss(line);
            vector<float> temp;
            while(ss >> element)
                temp.push_back(element);
            table.push_back(temp);
        }
    }
	return table;
}

int main(int argc, char *argv[]){
    ifstream fin;
	if(argc == 1){
		cout << "Usage: " << argv[0] << " <path to file>" << endl;
		exit(1);
	}
	if(argc > 1){
		fin.open(argv[1]);
	}
	else
    //fin.open("Data\\Test\\test.txt");
    //fin.open("test2.txt");
	//fin.open("Data\\Test\\edit.txt");
	//fin.open("Data\\Test\\mod.txt");
	fin.open("Data/Test/Ver_2_CS170_Fall_2021_LARGE_data__22.txt");
	//fin.open("Data\\Test\\Ver_2_CS170_Fall_2021_LARGE_data__27.txt");
	//fin.open("Data\\Test\\Ver_2_CS170_Fall_2021_Small_data__61.txt");
	//fin.open("Ver_2_CS170_Fall_2021_LARGE_data__27.txt");
	//fin.open("Data\\Ver_2_CS170_Fall_2021_LARGE_data__37.txt");
	//fin.open("Data\\Ver_2_CS170_Fall_2021_Small_data__24.txt");

    if(!fin.is_open()){
        cout << "Error opening file. Exiting..." << endl;
        exit(1);
    }

	vector<vector<float>> table = parse_table(fin);
	int user_input;
	cout << "Welcome to Kevin Nguyen's feature selection program" << endl
	     << "Normalizing data..." << endl;
	z_normalize(table);
	cout << "Please enter the algorithm you want to run: " << endl
		 << "   [1] Forward Selection" << endl
		 << "   [2] Backwards Elimination" << endl;
	cin >> user_input;

	if(user_input == 1)
		fsearch(table);
	else if(user_input == 2)
		bsearch(table);
	else{
		cout << "Error. Invalid selection." << endl;
		return -1;
	}

	// vector<int> curr_features(table[0].size()-1);
	// iota(curr_features.begin(), curr_features.end(), 1);
	// curr_features.pop_back();
	// remove_feature(5,curr_features);
	// clear_other_features(table,curr_features);

	// for(auto& i: table){
    //     for(auto j: i)
    //         cout << j << ' ';
    //     cout << endl;
    // }

    return 0;
}