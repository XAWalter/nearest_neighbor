#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <chrono>

using namespace std;

// Calculate the accuracy given the current list of features and the possible feature that will be added
// input: data vector with all data, vector of indices of the added features, the feature being tested,
// and whether it is a forwards or backwards search
// output: accuracy of the classification if current feature is added
float accuracy(vector<vector<float>> *data, vector<int> *added_features, int curr_feature, int type){
	// var for storing total number of correctly_classified subjects
	int correctly_classified = 0;

	// loop over the data
	for ( unsigned int i = 0; i < data->size(); i++ ){
		// store class of subject
		int curr_class = (*data)[i][0];
		// store distance, class, and index of nearest neighbor
		float nn_distance = numeric_limits<float>::max();
		int nn_location = -1;
		int nn_class = -1;

		// loop over all data
		for ( unsigned int j = 0; j < data->size(); j++){
			// find nearest neighbor given data
			if ( i != j){
				float distance = 0;
				for ( vector<int>::iterator k = added_features->begin(); k != added_features->end(); k++ ){
					// if backwards elim
					if (type){
						if ( curr_feature != (*k) ){
							distance += pow((*data)[i][*k] - (*data)[j][*k], 2.00);
						}
					}
					// if forwards select
					else{
						distance += pow((*data)[i][*k] - (*data)[j][*k], 2.00);
					}

				}
				// if forwards elect add curr_feature
				if (!type){
					distance += pow((*data)[i][curr_feature] - (*data)[j][curr_feature], 2.0);
				}
				distance = sqrt(distance);

				if ( distance < nn_distance ) {
					nn_distance = distance;
					nn_location = j;
					nn_class = (*data)[j][0];
				}

			}
		}

		if ( nn_class == curr_class ){
			correctly_classified++;
		}

	}

	return ((float)correctly_classified / ((float)data->size()));
}


// if backwards elimination
// input vector of all data
void backward_elim(vector<vector<float>> *data){
	// var to stor added features
	vector<int> added_features;
	float overall_best_accuracy = -1;
	vector<int> best_features;

	// add all features
	for ( unsigned int i = 1; i < (*data)[0].size(); i++){
		added_features.push_back(i);
	}

	// loop over feature size
	for ( unsigned int i = 1; i < (*data)[0].size(); i++ ){
		float best_accuracy = -1;
		int feature_to_del = -1;

		// and find the best feature to remove
		for ( unsigned int j = 1; j < (*data)[0].size(); j++ ){
			if ( find(added_features.begin(), added_features.end(), j) != added_features.end()){
				//cout << "Consider removing feature " << j << endl;
				float curr_accuracy = accuracy(data, &added_features, j, 1);

				if ( curr_accuracy > best_accuracy ){
					best_accuracy = curr_accuracy;
					feature_to_del = j;
				}
			}
		}

		// delete that feature
		added_features.erase(remove(added_features.begin(), added_features.end(), feature_to_del), added_features.end());

		cout << "\n\nUsing Feature Set: { ";
		for ( unsigned int i = 0; i < added_features.size(); i++){
			 cout << added_features[i] << " ";
		}
		cout << "}"<< endl;
		cout << "With accuracy of " << best_accuracy << endl;


		// update overall_best_accuracy if best_accuracy is greater than
		if ( best_accuracy > overall_best_accuracy ){
			overall_best_accuracy = best_accuracy;
			best_features.clear();
			best_features = added_features;
		}
	}
	cout << "\n\nBest Feature Set: { ";
	for ( unsigned int i = 0; i < best_features.size(); i++){
		 cout << best_features[i] << " ";
	}
	cout << "}"<< endl;
	cout << "With accuracy of " << overall_best_accuracy << endl;

}

// if forward select
// input vector of all data
void forward_search(vector<vector<float>> *data){
	// var for all added features
	vector<int> added_features;
	float overall_best_accuracy = -1;
	vector<int> best_features;

	// loop over all features
	for ( unsigned int i = 1; i < (*data)[0].size(); i++ ){
		float best_accuracy = -1;
		int feature_to_add = -1;


		// find feature that would be best to add
		for ( unsigned int j = 1; j < (*data)[0].size(); j++ ){
			if ( find(added_features.begin(), added_features.end(), j) == added_features.end()){
				//cout << "Consider adding feature " << j << endl;
				float curr_accuracy = accuracy(data, &added_features, j, 0);

				if ( curr_accuracy > best_accuracy ){
					best_accuracy = curr_accuracy;
					feature_to_add = j;
				}
			}
		}

		cout << "\n\nUsing Feature Set: { ";
		for ( unsigned int i = 0; i < added_features.size(); i++){
			 cout << added_features[i] << " ";
		}
		cout << feature_to_add << " ";
		cout << "}"<< endl;
		cout << "With accuracy of " << best_accuracy << endl;


		// add the features to the added_features var
		added_features.push_back(feature_to_add);

		// update overall_best_accuracy
		if ( best_accuracy > overall_best_accuracy ){
			overall_best_accuracy = best_accuracy;
			best_features.clear();
			best_features = added_features;
		}
	}

	cout << "\n\nBest Feature Set: { ";
	for ( unsigned int i = 0; i < best_features.size(); i++){
		 cout << best_features[i] << " ";
	}
	cout << "}"<< endl;
	cout << "With accuracy of " << overall_best_accuracy << endl;

}

// load full data into vector
// input pointer to vector that will hold all data
void load_data(vector<vector<float>> *data){
	int input = -1;
	string file_name;

	// User input
	cout << "1. Small File | 2. Large File | 3. Custom File" << endl;
	cin >> input;
	switch ( input ){
		case 1:
			file_name = "CS170_SMALLtestdata__41.txt";
			break;
		case 2:
			file_name = "CS170_largetestdata__11.txt";
			break;

		case 3:
			cout << "Input Name of Data File" << endl;
			cin >> file_name;
			break;

		default:
			file_name = "CS170_SMALLtestdata__41.txt";
			break;
	}

	// Get data from file
	ifstream data_stream;


	data_stream.open(file_name);
	string temp_string;
	int count = 0;
	while ( getline(data_stream, temp_string) ){
		istringstream input_stream(temp_string);
		float temp_data;
		vector<float> temp_vec;
		while ( input_stream >> temp_data ){
			temp_vec.push_back(temp_data);
		}
		// push data into vector
		data->push_back(temp_vec);
		count++;
	}

	data_stream.close();
}

int main(){
	vector<vector<float>> data;
	load_data(&data);

	int input = -1;
	
	cout << "1. Forward Search | 2. Backwards Elimination" << endl;
	cin >> input;

	// start timer
	// https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
	auto start = std::chrono::high_resolution_clock::now();
	switch(input){
		case 1:
			forward_search(&data);
			break;
		case 2:
			backward_elim(&data);
			break;
		default:
			forward_search(&data);
			break;
	}
	// stop timer
	auto stop = std::chrono::high_resolution_clock::now(); 
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Total time in microseconds " << duration.count() << endl;
	return 0;
}
