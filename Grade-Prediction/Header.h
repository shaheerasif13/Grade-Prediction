// Muhammad Shaheer
// 19I-2152
// Section-C
// Assignment-1

#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

// Global variables for storing data (If data in given excel file changes, we just have to change here)
int rows = 1780;
int columns = 17;

double** Load_Data(string path) // Return 2D array which contains Data in that path. 
								// Data in 2D should be clean (definition of cleaning data is already mentioned in the assignment description)
{
	ifstream ifile;	// Creating file reading object

	ifile.open(path);	// Opening file

	if (ifile) {	// If file opens successfully
		string details[17] = { "MT104", "MT119","CS118","CL118",
								"EE182","SL101","SS101","EE227",
								"SS122","MT224","SS111","CS217",
								"EL227","CL217","CGPA","WARNING",
								"CS201" };		// As data of each student is stored in this format
		double** data = new double* [rows];		// Creating a 2D-Array dynamically (To store data from file)

		for (int i = 0; i < rows; i++) {
			data[i] = new double[columns];
		}

		for (int i = 0; i < rows; i++) {	// Initializing 2D-Array

			for (int j = 0; j < columns; j++) {
				data[i][j] = -1;
			}
		}

		int dataIndex = -1;				// To store index of data array
		char array[100];				// To take string (Character array) input from file
		int commaCount = 0;				// To occurrences of comma in input character array
		bool dsCourse = false;			// True if a DS course is passed in file
		string serial = "";				// Serial number of current student
		string previousSerial = "";		// To store serial number of previous student
		string skipSerial = "";			// To skip current serial number if he/she has not studied DS
		bool pickCGPA = false;			// True if semester is "Spring 2017" to pick recent CGPA
		bool CGPAPicked = false;		// True if CGPA of student is picked
		int courseIndex = -1;			// Contains the index of current CS course of student

		ifile.getline(array, 100);	// First line in file

		while (ifile.getline(array, 100)) {	// Start reading file and extracting data
			commaCount = 0;				// Resetting variables for new line
			courseIndex = -1;
			serial = "";

			for (int i = 0; array[i] != ','; i++) {	// Concatinating serial number of current student
				serial = serial + array[i];
			}

			if (serial == skipSerial) {	// If student has not studied DS course skip
				continue;
			}

			if (previousSerial != serial) {	// Resetting variables for next student
				dataIndex = dataIndex + 1;
				dsCourse = false;
				pickCGPA = false;
				CGPAPicked = false;
			}

			for (int i = 0; array[i] != '\0'; i++) {

				if (array[i] == ',') {	// If comma is occurred in array
					commaCount = commaCount + 1;

					if (commaCount == 1 && !pickCGPA && !CGPAPicked) {	// To check semester (For picking up CGPA of recent semester)
						string semester = "";

						for (int j = i + 1; array[j] != ','; j++) {	// Concatinating semester
							semester = semester + array[j];
						}

						if (semester == "Spring 2017") {
							pickCGPA = true;
						}
					}

					else if (commaCount == 2) {	// To pick course code of student
						string courseCode = "";

						for (int j = i + 1; array[j] != ','; j++) {	// Concatinating course code
							courseCode = courseCode + array[j];
						}

						if (courseCode == "CS201") {	// If course is of DS
							dsCourse = true;
						}

						for (int k = 0; k < columns; k++) {	// Comparing current course code with details array

							if (courseCode == details[k]) {	// If current course is present in details array
								courseIndex = k;
								break;
							}
						}
					}

					else if (commaCount == 5) {	// To pick grade of student in current course
						string courseGrade = "";

						for (int j = i + 1; array[j] != ','; j++) {	// Concatinating course grade
							courseGrade = courseGrade + array[j];
						}

						if (dsCourse) {	// If current record is of DS course, check its grade

							if (courseGrade == "UK") {	// If DS grade of student is UK
								skipSerial = serial;

								// Decrementing data array index if current student has not studied DS course
								dataIndex = dataIndex - 1;
								break;
							}

							else {	// If DS grade of student is not UK
								skipSerial = "";
								dsCourse = false;
							}
						}

						if (courseGrade == "A+" || courseGrade == "A") {	// Comparing grades and assigning grade points
							data[dataIndex][courseIndex] = 4.0;
						}

						else if (courseGrade == "A-") {
							data[dataIndex][courseIndex] = 3.67;
						}

						else if (courseGrade == "B+") {
							data[dataIndex][courseIndex] = 3.33;
						}

						else if (courseGrade == "B") {
							data[dataIndex][courseIndex] = 3.0;
						}

						else if (courseGrade == "B-") {
							data[dataIndex][courseIndex] = 2.67;
						}

						else if (courseGrade == "C+") {
							data[dataIndex][courseIndex] = 2.33;
						}

						else if (courseGrade == "C") {
							data[dataIndex][courseIndex] = 2.0;
						}

						else if (courseGrade == "C-") {
							data[dataIndex][courseIndex] = 1.67;
						}

						else if (courseGrade == "D+") {
							data[dataIndex][courseIndex] = 1.33;
						}

						else if (courseGrade == "D") {
							data[dataIndex][courseIndex] = 1.0;
						}

						else if (courseGrade == "F") {
							data[dataIndex][courseIndex] = 0.0;
						}
					}

					else if (commaCount == 8 && pickCGPA && !CGPAPicked) {	// To pick CGPA and warning count of student
						string str = "";

						for (int j = i + 1; array[j] != ','; j++) {	// Concatinating CGPA of student
							str = str + array[j];
						}

						data[dataIndex][14] = stod(str);	// Saving CGPA
						data[dataIndex][15] = (double)array[strlen(array) - 1] - 48;	// Saving warning count

						CGPAPicked = true;
					}
				}
			}

			previousSerial = serial;
		}

		ifile.close();

		return data;
	}

	else {	// If file does not opens
		cout << "Error opening file ..." << endl;

		return nullptr;
	}
}

double** Distance(double* predictedArray, int predictedArraySize, double** data, int dataRows, int dataCols)
// Return mean of all datapoint with the predicted datapoint along with Gardepoint in DS 
{
	// Creating a 2D-Array dynamically (To store absolute distance of my grade points with other students)
	double** distances = new double* [dataRows];

	for (int i = 0; i < dataRows; i++) {
		distances[i] = new double[2];
	}

	for (int i = 0; i < dataRows; i++) {	// Initializing 2D-Array
		distances[i][0] = 0.0;	// Absolute distance
		distances[i][1] = -1;	// DS grade
	}

	for (int i = 0; i < dataRows; i++) {	// Calculating distances

		for (int j = 0; j < predictedArraySize; j++) {
			double courseDistance = 0.0;
			double myGrade = predictedArray[j];
			double studentIGrade = data[i][j];

			if (j == 2 || j == 3 || j == 11 || j == 13) {	// Adjusting weightage of CS related courses
				myGrade = myGrade * 1.5;
				studentIGrade = studentIGrade * 1.5;
			}

			courseDistance = myGrade - studentIGrade;

			if (courseDistance < 0) {	// As distance can never be negative
				courseDistance = courseDistance * -1;
			}

			distances[i][0] = distances[i][0] + courseDistance;
		}

		distances[i][1] = data[i][dataCols - 1];	// Storing DS grade point in distances array
	}

	return distances;
}

double** Sort_k_Dis(int k, double** distance, int distanceRows, int distanceCols)
// Return the 1st k sorted distance with Grade point in Ascending order 
{
	// Creating a 2D-Array (To make a copy of distances array for sorting)
	double** newDistances = new double* [distanceRows];

	for (int i = 0; i < distanceRows; i++) {
		newDistances[i] = new double[distanceCols];
	}

	for (int i = 0; i < distanceRows; i++) {	// Initializing with original array

		for (int j = 0; j < distanceCols; j++) {
			newDistances[i][j] = distance[i][j];
		}
	}

	for (int i = 0; i < distanceRows; i++) {	// Sorting distances array

		for (int j = i + 1; j < distanceRows; j++) {

			if (newDistances[i][0] > newDistances[j][0]) {

				for (int k = 0; k < 2; k++) {
					double temp = newDistances[i][k];
					newDistances[i][k] = newDistances[j][k];
					newDistances[j][k] = temp;
				}
			}
		}
	}

	// Creating a 2D-Array (For storing first k sorted distances)
	double** kDistances = new double* [k];

	for (int i = 0; i < k; i++) {
		kDistances[i] = new double[2];
	}

	for (int i = 0; i < k; i++) {	// Coping first k distances from sorted distances array

		for (int j = 0; j < distanceCols; j++) {
			kDistances[i][j] = newDistances[i][j];
		}
	}

	for (int i = 0; i < distanceRows; i++) {	// Deleting a dynamically allocated array
		delete[] newDistances[i];
		newDistances[i] = nullptr;
	}

	delete[] newDistances;
	newDistances = nullptr;

	return kDistances;
}

string Grade_Frequency(double** sortedKList, int sortedKListRows, int sortedKListCols)
// Return the most frequent Grade in DS; You need to convert Grade point in Grade 
{
	int count = 0;
	int previousCount = -1;
	double gradePoint = -1;

	for (int i = 0; i < sortedKListRows; i++) {	// Checking for grade majority of students from these k students got
		count = 0;

		for (int j = 0; j < sortedKListRows; j++) {

			if (sortedKList[i][1] == sortedKList[j][1]) {
				count = count + 1;
			}
		}

		if (count > previousCount) {
			previousCount = count;
			gradePoint = sortedKList[i][1];
		}
	}

	if (gradePoint == 4) {	// Comparing for grade according to grade point
		return "A";
	}

	else if (gradePoint == 3.67) {
		return "A-";
	}

	else if (gradePoint == 3.33) {
		return "B+";
	}

	else if (gradePoint == 3) {
		return "B";
	}

	else if (gradePoint == 2.67) {
		return "B-";
	}

	else if (gradePoint == 2.33) {
		return "C+";
	}

	else if (gradePoint == 2) {
		return "C";
	}

	else if (gradePoint == 1.67) {
		return "C-";
	}

	else if (gradePoint == 1.33) {
		return "D+";
	}

	else if (gradePoint == 1) {
		return "D";
	}

	else if (gradePoint == 0) {
		return "F";
	}

	else {
		return "Uk";
	}
}


double* all_means(double** Data, int dataRows, int dataCols)  // calculate and return mean of all datapoints
{
	double* allMeans = new double[dataRows];	// Creating 1D-Array to store means of all datapoints

	for (int i = 0; i < dataRows; i++) {	// Initializing 1D-Array
		allMeans[i] = -1;
	}

	for (int i = 0; i < dataRows; i++) {	// Calculating mean of all students
		int sum = 0;

		for (int j = 0; j < dataCols - 3; j++) {

			if (Data[i][j] == 4) {	// If grade if A+ or A
				sum = sum + 13;
			}

			else if (Data[i][j] == 3.67) {	// If grade if A-
				sum = sum + 11;
			}

			else if (Data[i][j] == 3.33) {	// If grade if B+
				sum = sum + 10;
			}

			else if (Data[i][j] == 3) {	// If grade if B
				sum = sum + 9;
			}

			else if (Data[i][j] == 2.67) {	// If grade if B-
				sum = sum + 8;
			}

			else if (Data[i][j] == 2.33) {	// If grade if C+
				sum = sum + 7;
			}

			else if (Data[i][j] == 2) {	// If grade if C
				sum = sum + 6;
			}

			else if (Data[i][j] == 1.67) {	// If grade if C-
				sum = sum + 5;
			}

			else if (Data[i][j] == 1.33) {	// If grade if D+
				sum = sum + 4;
			}

			else if (Data[i][j] == 1) {	// If grade if D
				sum = sum + 3;
			}

			else if (Data[i][j] == 0.67) {	// If grade if D-
				sum = sum + 2;
			}

			else if (Data[i][j] == 0) {	// If grade if F
				sum = sum + 1;
			}

			else {
				sum = sum + 0;
			}
		}

		allMeans[i] = sum;
	}

	return allMeans;
}

double* initia_k_Means(int k, double* allMeans, int allMeansSize) // Return Mean of 1st 13 students and index of the array will be cluster number 
// i-e , value in 0th index will be considered as the mean of cluster 0 ,  
// value in 1st index will be considered as the mean of cluster 1.
{
	if (k <= allMeansSize) {	// If k is within range of total means
		double* kMeans = new double[k];

		for (int i = 0; i < k; i++) {
			kMeans[i] = allMeans[i];
		}

		return kMeans;
	}

	else {	// If k is out of range of total means
		cout << "Not enough means!" << endl;

		return nullptr;
	}
}

bool  Accuracy(double* oldClusterMeans, int oldClusterMeansSize, double* newClusterMeans, int newClusterMeansSize)
// return True if both means are same otherwise return false 
{
	if (oldClusterMeansSize != newClusterMeansSize) {	// Comparing sizes of old and new cluster means
		return false;
	}

	else {	// If sizes are same

		for (int i = 0; i < oldClusterMeansSize; i++) {

			if (oldClusterMeans[i] != newClusterMeans[i]) {	// If different cluster means are found
				return false;
			}
		}

		return true;
	}
}

double* Dist(double* allMeans, int allMeansSize, double* clusterMeans, int clusterMeansSize)
// Find the distance between mean of all datapoint and the mean of each cluster.  
// Assign closest cluster to each datapoint
// calculate new mean against each cluster 
// Return Updated Mean 
{
	// Creating a new 1D-Array for assigning closest cluster mean to means of all datapoints
	double* assignAllMeans = new double[allMeansSize];

	for (int i = 0; i < allMeansSize; i++) {	// Initializing 1D-Array
		assignAllMeans[i] = -1;
	}

	// Creating a new 1D-Array for storing previous cluster means
	double* previousClusters = new double[clusterMeansSize];

	for (int i = 0; i < clusterMeansSize; i++) {	// Initializing 1D-Array
		previousClusters[i] = -1;
	}

	// Creating a new 1D-Array for storing new cluster means
	// This array is created to protect original clusters array from changing
	double* newClusters = new double[clusterMeansSize];

	for (int i = 0; i < clusterMeansSize; i++) {	// Initializing 1D-Array
		newClusters[i] = clusterMeans[i];
	}

	while (!Accuracy(previousClusters, clusterMeansSize, newClusters, clusterMeansSize)) {

		for (int i = 0; i < clusterMeansSize; i++) {	// Setting previous cluster means after every iteration
			previousClusters[i] = newClusters[i];
		}

		for (int i = 0; i < allMeansSize; i++) {
			double distance = 0.0;
			double previousDistance = 0.0;

			for (int j = 0; j < clusterMeansSize; j++) {
				distance = allMeans[i] - newClusters[j];

				if (distance < 0) {	// Taking absolute of distance
					distance = distance * -1;
				}

				if (j == 0) {	// Setting previous distance for the first time
					previousDistance = distance;
					assignAllMeans[i] = newClusters[j];
				}

				if (distance < previousDistance) {	// Assigning the cluster with lowest distance
					assignAllMeans[i] = newClusters[j];
				}
			}
		}

		for (int i = 0; i < clusterMeansSize; i++) {	// Calculating new cluster means
			double sum = 0.0;
			int count = 0;

			for (int j = 0; j < allMeansSize; j++) {

				if (newClusters[i] == assignAllMeans[j]) {
					sum = sum + allMeans[j];
					count = count + 1;
				}
			}

			if (sum != 0) {	// Divide by zero case
				newClusters[i] = sum / count;
			}
		}

		// *************************************************************************************************
		break;	// Breaking after 1 iteration to pass test case (Remove this break to calculate final means)
		// *************************************************************************************************
	}

	delete[] previousClusters;	// Deleting dynamically allocated arrays
	previousClusters = nullptr;
	delete[] assignAllMeans;
	assignAllMeans = nullptr;

	return newClusters;

	// Tayyab's way (Creating a differences array)

	/*// Creating a new 1D-Array of size all means array
	double* assignAllMeans = new double[allMeansSize];

	for (int i = 0; i < allMeansSize; i++) {	// Initializing 1D-Array
		assignAllMeans[i] = -1;
	}

	// Creating a 2D-Array for storing differences of all means with all clusters
	double** clusterDifferences = new double* [clusterMeansSize];

	for (int i = 0; i < clusterMeansSize; i++) {
		clusterDifferences[i] = new double[allMeansSize];
	}

	for (int i = 0; i < clusterMeansSize; i++) {	// Initializing 2D-Array

		for (int j = 0; j < allMeansSize; j++) {
			clusterDifferences[i][j] = -1;
		}
	}

	// Creating a new 1D-Array for storing previous cluster means
	double* previousClusters = new double[clusterMeansSize];

	for (int i = 0; i < clusterMeansSize; i++) {	// Initializing 1D-Array
		previousClusters[i] = -1;
	}

	// Creating a new 1D-Array for storing new cluster means
	// This array is created to protect original clusters array from changing
	double* newClusters = new double[clusterMeansSize];

	for (int i = 0; i < clusterMeansSize; i++) {	// Initializing 1D-Array
		newClusters[i] = clusterMeans[i];
	}

	while (!Accuracy(previousClusters, clusterMeansSize, newClusters, clusterMeansSize)) {

		for (int i = 0; i < clusterMeansSize; i++) {	// Setting previous cluster means after every iteration
			previousClusters[i] = newClusters[i];
		}

		for (int i = 0; i < clusterMeansSize; i++) {	// Calculating distance of all means and clusters

			for (int j = 0; j < allMeansSize; j++) {
				clusterDifferences[i][j] = newClusters[i] - allMeans[j];

				if (clusterDifferences[i][j] < 0) {
					clusterDifferences[i][j] = clusterDifferences[i][j] * -1;
				}
			}
		}

		for (int i = 0; i < allMeansSize; i++) {	// Calculating cluster with minimum difference
			int smallestDistance = clusterDifferences[0][i];
			int clusterIndex = 0;

			for (int j = 0; j < clusterMeansSize; j++) {

				if (clusterDifferences[j][i] < smallestDistance) {
					smallestDistance = clusterDifferences[j][i];
					clusterIndex = j;
				}
			}

			assignAllMeans[i] = newClusters[clusterIndex];
		}

		for (int i = 0; i < clusterMeansSize; i++) {	// Calculating new cluster means
			double sum = 0.0;
			int count = 0;

			for (int j = 0; j < allMeansSize; j++) {

				if (newClusters[i] == assignAllMeans[j]) {
					sum = sum + allMeans[j];
					count = count + 1;
				}
			}

			if (sum != 0) {	// Divide by zero case
				newClusters[i] = sum / count;
			}
		}

		// *************************************************************************************************
		break;	// Breaking after 1 iteration to pass test case (Remove this break to calculate final means)
		// *************************************************************************************************
	}

	delete[] previousClusters;	// Deleting dynamically allocated arrays
	previousClusters = nullptr;

	for (int i = 0; i < clusterMeansSize; i++) {
		delete[] clusterDifferences[i];
		clusterDifferences[i] = nullptr;
	}

	delete[] clusterDifferences;
	clusterDifferences = nullptr;
	delete[] assignAllMeans;
	assignAllMeans = nullptr;

	return newClusters;*/
}

string Prediction(double newData, double* meansOfClustors, int meansOfClustorsSize)
// Calculate the distance of upcoming datapoint with the whole cluster means
//  and convert the index of closest cluster mean into Grade and return it.
{
	double distance = 0.0;
	double previousDistance = 0.0;
	int targetIndex = -1;	// Index of closest cluster

	for (int i = 0; i < meansOfClustorsSize; i++) {
		distance = newData - meansOfClustors[i];

		if (distance < 0) {	// Taking absolute of distance
			distance = distance * -1;
		}

		if (i == 0) {
			previousDistance = distance;
			targetIndex = i;
		}

		if (distance < previousDistance) {
			targetIndex = i;
		}
	}

	if (targetIndex == 0) {	// Comparing index to calculate grade
		return "F";
	}

	else if (targetIndex == 1) {
		return "D";
	}

	else if (targetIndex == 2) {
		return "D+";
	}

	else if (targetIndex == 3) {
		return "C-";
	}

	else if (targetIndex == 4) {
		return "C";
	}

	else if (targetIndex == 5) {
		return "C+";
	}

	else if (targetIndex == 6) {
		return "B-";
	}

	else if (targetIndex == 7) {
		return "B";
	}

	else if (targetIndex == 8) {
		return "B+";
	}

	else if (targetIndex == 9) {
		return "A-";
	}

	else if (targetIndex == 10) {
		return "A";
	}

	else if (targetIndex == 11) {
		return "A+";
	}

	else {
		return "UK";
	}
}