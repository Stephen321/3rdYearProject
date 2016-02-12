#include <iostream>

using namespace std;

void main(){
	int arr[] = {
		1,2
	};
	const int SIZE = sizeof(arr) / sizeof(int);
	char charr[SIZE];
	for (int i = 0; i < SIZE; i++){
		char c = (char)arr[i];
		charr[i] = c;
		cout << charr[i] << endl;
	}
	
	cin.get();
}