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

void encode_Fixed(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr) {
        return;
    }
    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    encode_Fixed(root->left, str + "0", huffmanCode);
    encode_Fixed(root->right, str + "1", huffmanCode);
}
fstream file2("Decode.txt",ios::out);
void decode(Node* root, int &index, string str)
{
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root)) {
//        cout << root->ch;
        file2<<root->ch;
        return;
    }

    index++;

    if (str[index] == '0') {
        decode(root->left, index, str);
    } else {
        decode(root->right, index, str);
    }
}

string build_Fixed_Tree(string text)
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
    encode_Fixed(root, EMPTY_STRING, huffmanCode);

    cout << "Huffman Fixed length Codes are:\n" << '\n';
    int length=0;
    for (auto pair: huffmanCode) {

        if(pair.second.length()>length)
            length=pair.second.length();
    }
    string str;
    for (char ch: text) {
        str += huffmanCode[ch];
    }
    for (auto pair: huffmanCode) {
        while(pair.second.length()!=length)
        {
            pair.second+="0";
            huffmanCode[pair.first]=pair.second;
        }
    }
    for (auto pair: huffmanCode) {
                cout << pair.first << " "  <<pair.second <<'\n';

    }
    string encode_b;
    for (char ch: text) {
        encode_b += huffmanCode[ch];
    }
    int eb=0;
    while(encode_b.length()%8!=0)
    {
        eb++;
        encode_b+="0";
    }
    fstream  file("Encoded.txt",ios::out);

    string str2;
    for(int i=0;  encode_b[i]!='\0';i++)
    {
        str2+=encode_b[i];
        if(str2.length()==8)
        {
            file<<char(stoi(str2));
            str2=" ";
        }
    }

    file.close();
    cout<<"Encoded file built"<<endl;
    int temp=-1;
     root = pq.top();
    while (temp < (int) str.size() - 2) {
        decode(root, temp, str);
    }
    file2.close();
    return str;
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

    string file1encode = build_Fixed_Tree(str);

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

    string Challangeencode = build_Fixed_Tree(str);

    ofstream os2("Challangeencode.txt");
    os2 << Challangeencode;
    os2.close();



    return 0;

}