#include <iostream>
#include <fstream>

using namespace std;

void main(){
	int* arr = new int[];
	ifstream f;
	f.open("data.txt");

	int i = 0;
	while (f >> arr[i]){

		i++;
	}

	const int SIZE = sizeof(arr) / sizeof(int);
	char charr[SIZE];
	for (int i = 0; i < SIZE; i++){
		char c = (char)arr[i];
		charr[i] = c;
		cout << charr[i] << endl;
	}
	
	cin.get();
}