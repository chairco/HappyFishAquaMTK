#include <iostream>

using namespace std;

typedef union Data {
	float f_data;
	unsigned char b_data[4];
}Data;


void main()
{
	Data temp_data;
	temp_data.b_data[0] = 0;
	temp_data.b_data[1] = 0;
	temp_data.b_data[2] = 234;
	temp_data.b_data[3] = 65;

	cout << temp_data.f_data << endl;

}