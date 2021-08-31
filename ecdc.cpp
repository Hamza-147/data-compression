#include<iostream>
#include <bits/stdc++.h>
#include<fstream>
#include <chrono>
#include<string>
using namespace std; 
using namespace std::chrono;



long long int fileSize(const char *filename) {
			FILE *p_file = fopen(filename, "rb");
			fseek(p_file, 0, SEEK_END);
			long long int size = ftello64(p_file);
			fclose(p_file);
			return size;
}

vector<int> encoding(string s1) 
{ 
	unordered_map<string, int> map_table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		map_table[ch] = i; 
	} 

	string p = "", c = ""; 
	p += s1[0]; 
	int code = 256; 
	vector<int> output_vector; 
	for (int i = 0; i < s1.length(); i++) { 
		if (i != s1.length() - 1) 
			c += s1[i + 1]; 
		if (map_table.find(p + c) != map_table.end()) { 
			p = p + c; 
		} 
		else { 
			output_vector.push_back(map_table[p]); 
			map_table[p + c] = code; 
			code++; 
			p = c; 
		} 
		c = ""; 
	} 
	output_vector.push_back(map_table[p]); 
	cout<<"Encoding done\n";
	return output_vector; 
} 

void decoding(vector<int> op) 
{ 
	ofstream os2("challange_dc.txt");

	unordered_map<int, string> map_table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		map_table[i] = ch; 
	} 
	int old = op[0], n; 
	string s = map_table[old]; 
	string c = ""; 
	c += s[0]; 
	//cout << s; 
	os2 << s;
	int count = 256; 
	for (int i = 0; i < op.size() - 1; i++) { 
		n = op[i + 1]; 
		if (map_table.find(n) == map_table.end()) { 
			s = map_table[old]; 
			s = s + c; 
		} 
		else { 
			s = map_table[n]; 
		} 
		//cout << s; 
		os2 << s;
		c = ""; 
		c += s[0]; 
		map_table[count] = map_table[old] + c; 
		count++; 
		old = n; 
	} 
	cout<<"Decoding done\n";
	os2.close();
} 
int main() 
{ 

	time_point <system_clock> start, end; 
	duration <double> time;
	


	ifstream is1;
	string filename = "Challange.txt";
    is1.open(filename);
    stringstream strStream;
    strStream << is1.rdbuf();
    string str = strStream.str();
	strStream.clear();
	is1.close();

	
	cout << "Encoding\n"; 
	start = system_clock::now();
	vector<int> output_vector = encoding(str); 
	end = system_clock::now();
	time = (end-start);
	cout << "\nCompression Time: " << time.count() << "s" << endl;

	
	
	ofstream os1("challange_ec.txt");
	for (int i = 0; i < output_vector.size(); i++) { 
		//cout << output_code[i] << " "; 
		os1<< output_vector[i] << " ";
	} 
	os1.close();

	cout << "\nDecoding\n"; 
	start = system_clock::now();
	decoding(output_vector); 
	end = system_clock::now();
	time = (end-start);
	cout << "\nDecompression Time: " << time.count() << "s" << endl;

	cout<<"Original file size : " <<fileSize("Challange.txt") <<"bytes\n";
	cout<<"Encoded file size : " << fileSize("challange_ec.txt") <<"bytes\n";
	cout<<"Decoded file size : " << fileSize("challange_dc.txt") <<"bytes\n";
	

	return 0;
}