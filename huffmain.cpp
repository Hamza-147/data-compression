#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <sstream> 
#include <unordered_map>

using namespace std;

#define EMPTY_STRING ""

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};

bool isLeaf(Node* root) {
	return root->left == nullptr && root->right == nullptr;
}

void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr) {
		return;
	}
	if (isLeaf(root)) {
		huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	if (isLeaf(root)) {
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] == '0') {
		decode(root->left, index, str);
	} else {
		decode(root->right, index, str);
	}
}

string buildHuffmanTree(string text)
{
	if (text == EMPTY_STRING) {
		return NULL;
	}

	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	priority_queue<Node*, vector<Node*>, comp> pq;

	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

	
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	Node* root = pq.top();

	
	unordered_map<char, string> huffmanCode;
	encode(root, EMPTY_STRING, huffmanCode);

	cout << "Huffman Codes are:\n" << '\n';
	for (auto pair: huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
	}

	//cout << "\nOriginal string is:\n" << text << '\n';

	string str;
	for (char ch: text) {
		str += huffmanCode[ch];
	}

	return str;

	//cout << "\nEncoded string is:\n" << str << '\n';
	//cout << "\nDecoded string is:\n";

	/*if (isLeaf(root))
	{
		while (root->freq--) {
			cout << root->ch;
		}
	}
	else
	{
		int index = -1;
		while (index < (int)str.size() - 1) {
			decode(root, index, str);
		}
	}*/
}

int main()
{
	ifstream is1;
    is1.open("file1.txt");
    stringstream strStream;
    strStream << is1.rdbuf();
    string str = strStream.str();
	strStream.clear();
	is1.close();
	//cout<<str;

    string file1encode = buildHuffmanTree(str);
	 
	ofstream os1("file1encode.txt");
	os1 << file1encode;
	os1.close();





	ifstream is2;
    is2.open("Challange.txt");
    stringstream strStream1;
    strStream1 << is2.rdbuf();
    string str1 = strStream1.str();
	strStream1.clear();
	is2.close();
	//cout<<str;

    string Challangeencode = buildHuffmanTree(str);
	 
	ofstream os2("Challangeencode.txt");
	os2 << Challangeencode;
	os2.close();



	return 0;

}