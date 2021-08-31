#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;



string huffVal[256] = {""};
	
typedef struct Node {
public:
	char chrcter;
	unsigned long long int count;
	Node *left, *right;

	Node(unsigned long long int count) {
		this->chrcter = 0;
		this->count = count;
		this->left = this->right = nullptr;
	}

	Node(char chrcter, unsigned long long int count) {
		this->chrcter = chrcter;
		this->count = count;
		this->left = this->right = nullptr;
	}
} Node;

Node *CombineNodes(Node *a, Node *b) {
			Node *parent = new Node(a->count+b->count);
			parent->left = b;
			parent->right = a;
			return parent;
	}

bool sortt(Node *a, Node *b) { 
	return (a->count > b->count); 
	}	

unsigned long long int fileSize(const char *filename) {
			FILE *p_file = fopen(filename, "rb");
			fseek(p_file, 0, SEEK_END);
			unsigned long long int size = ftello64(p_file);
			fclose(p_file);
			return size;
		}
		
void Inorder(Node *root, string &val) {
	if(root) {
		val.push_back('0');
		Inorder(root->left, val);
		val.pop_back();
		if (!root->left && !root->right) {
			cout<<"Character: "<< root->chrcter << " Count : "<<root->count;
			cout << "Huffman value : " << val << endl;
		}
		val.push_back('1');
		Inorder(root->right, val);
		val.pop_back();
	}
}


	
		

		vector <Node *> sort_char(const map <char, unsigned long long int >&val) {
			vector < Node* > store;
			auto it = begin(val);
			for(; it != end(val); ++it) 
				store.push_back(new Node(it->first, it->second));
			sort(begin(store), end(store), sortt);

			return store;
		}
		
		string gen_header(char padding) {
			string header = "";
			
			unsigned char uniqueChar = 255;
			
			for(int i = 0; i < 256; ++i) {
				if(huffVal[i].size()) {
					header.push_back(i);
					header.push_back(huffVal[i].size());
					header += huffVal[i];
					++uniqueChar;
				}
			}
			char val = uniqueChar;
			
			return val+header+(char)padding;
		}

		unsigned long long int store_huff_val(Node *root, string &val) {
			unsigned long long int temp = 0;  
			if(root) {
				val.push_back('0');
				temp = store_huff_val(root->left, val);
				val.pop_back();
				if (!root->left && !root->right) {
					huffVal[(unsigned char)root->chrcter] = val;
					temp += val.size()*root->count;
				}
				val.push_back('1');
				temp += store_huff_val(root->right, val);
				val.pop_back();
			}
			return temp;
		}


		Node *generateTree(const map <char, unsigned long long int>&val) {
			vector < Node* > store = sort_char(val);
			Node *one, *two, *parent;
			sort(begin(store), end(store), sortt);

			while (store.size() > 2) {
				one = *(end(store)-1); two = *(end(store)-2);
				parent = CombineNodes(one, two);
				store.pop_back(); store.pop_back();
				store.push_back(parent);

				vector <Node *> :: iterator it1 = end(store)-2;
				while((*it1)->count < parent->count && it1 != begin(store)) 
					--it1;
				sort(it1, end(store), sortt);
			}
			one = *(end(store)-1); two = *(end(store)-2);
			parent = CombineNodes(one, two);
			return parent;
		}
void Compression(const char *filename, const unsigned long long int Filesize, const unsigned long long int pred_fs) {
			const char padding = (8 - ((pred_fs)&(7)))&(7);
			string header = gen_header(padding);
			int header_i = 0;
			const int h_length = header.size();
			
			FILE *iptr = fopen(filename, "rb"), *optr = fopen((string(filename)+".abiz").c_str(), "wb");
			
			while(header_i < h_length) {
				fputc(header[header_i], optr);
				++header_i;
			}

			if (!iptr) {
				perror("Error: File not found: ");
				exit(-1);
			}

			unsigned char ch, fch = 0;
			char counter = 7;
			unsigned long long int size = 0, i;
			while(size != Filesize) {
				ch = fgetc(iptr);
				i = 0;
				while(huffVal[ch][i] != '\0') {
					fch = fch | ((huffVal[ch][i] - '0')<<counter);
					--counter;
					if(counter == -1) {
						fputc(fch, optr);
						counter = 7;
						fch = 0;
					}
					++i;
				}
				++size;
				
					
			}
			if(fch) 
				fputc(fch, optr);
			printf("\n");
			fclose(iptr);
			fclose(optr);
		}

map <char, unsigned long long int> fileCoping(const char* filename, unsigned long long int Filesize) {
			register FILE *ptr = fopen(filename, "rb");

			if (ptr == NULL) {
				perror("Error: File not found:");
				exit(-1);
			}
			register unsigned char ch;
			register unsigned long long int size = 0, filesize = Filesize;
			vector<unsigned long long int>Store(256, 0);

			while(size != filesize) {
				ch = fgetc(ptr);
				++Store[ch];
				++size;
			}
			map <char, unsigned long long int> store;
			for(int i = 0; i < 256; ++i)
				if(Store[i])
					store[i] = Store[i];
			fclose(ptr);
			return store;
}

	
void generateTree(Node * const root, const string &codes, const unsigned char ch) {
			Node *trav = root;
			int i = 0;
			while(codes[i] != '\0') {
				if(codes[i] == '0') {
					if(!trav->left)
						trav->left = new Node(0);
					trav = trav->left;
				} else {
					if(!trav->right) 
						trav->right = new Node(0);
					trav = trav->right;
				}
				++i;
			}
			trav->chrcter = ch;
		}
		pair<Node*, pair<unsigned char, int> >dc_header(FILE *iptr) {
			Node *root = new Node(0);
			int charactercount, buffer, total_length = 1;
			register char ch, len;
			charactercount = fgetc(iptr);
			string codes;
			++charactercount;
			while(charactercount) {
				ch = fgetc(iptr);
				codes = ""; 
				buffer = 0;
				len = fgetc(iptr);
				buffer = len;

				while(buffer > codes.size()) 
					codes += fgetc(iptr);
				total_length += codes.size()+2;

				generateTree(root, codes, ch);
				--charactercount;
			}
			unsigned char padding = fgetc(iptr);
			++total_length;
			return {root, {padding, total_length}};
		}

		void Decompression(const char*filename, const unsigned long long int Filesize, const unsigned long long int leftover) {
			string fl = string(filename);
			FILE *iptr = fopen(fl.c_str(), "rb");
			FILE *optr = fopen(string("output"+fl.substr(0, fl.length()-5)).c_str(), "wb");
			
			if (iptr == NULL) {
				perror("Error: File not found");
				exit(-1);
			}

			pair<Node*, pair<unsigned char, int> >headerData = dc_header(iptr);
			Node *const root = headerData.first;
			const auto padding = headerData.second.first;
			const auto head_size = headerData.second.second;

			char ch, counter = 7;
			unsigned long long int size = 0;
			const unsigned long long int filesize = Filesize-head_size;
			Node *trav = root;
			ch = fgetc(iptr);
			while(size != filesize) {
				while(counter >= 0) {
					trav = ch & (1<<counter) ? trav->right : trav->left;
					ch ^= (1<<counter);
					--counter;
					if(!trav->left && !trav->right) {
						fputc(trav->chrcter, optr);
						if(size == filesize-1 && padding == counter+1) 
							break;
						trav = root;
					}
				}
				++size;
				counter = 7;
		
				ch = fgetc(iptr);
			}
			fclose(iptr);
			fclose(optr);
		}



int main(int argc, char *argv[]) {
	
	
	const char *choice = argv[1], *filename = argv[2];

	cout<<filename<<endl;
	
	time_point <system_clock> start, end; 
	duration <double> time;
	unsigned long long int filesize, pred_fs;
	if(string(choice) == "-c") {
	
		filesize = fileSize(filename);
		auto mapper = fileCoping(filename, filesize);
		Node *const root = generateTree(mapper);
		string buf = "";
		pred_fs = store_huff_val(root, buf);

		
		cout<< "Original file size : "<< filesize<< endl;
		printf("Compressed file size : %lld bytes\n", (pred_fs+7)>>3);
		
		start = system_clock::now();
		Compression(filename, filesize, pred_fs);
		end = system_clock::now();

		time = (end-start);
		cout << "Compression Time: " << time.count() << "sec" << endl;
	
	}
	else if(string(choice) == "-dc"){
		filesize = fileSize(filename);
		start = system_clock::now();
		Decompression(filename, filesize, pred_fs);
		end = system_clock::now();

		time = (end-start);
		cout << "Decompression Time: " << time.count() << "sec" << endl;
	}
	return 0;
	
}
