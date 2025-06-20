//Mark Toni Ramsol L. Tagalogon 
//Maria Lourdes T. Villaruz 
#include <iostream>
#include <iomanip>
#include <queue>
#include <unordered_map>
#define EMPTY_STRING ""
#include <fstream>
#include <windows.h>
using namespace std;
HANDLE color=GetStdHandle(STD_OUTPUT_HANDLE);

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


char fn[1000];
char chr;
string contents;
string file="_compressed.txt";
int frequency[256];
int size =0;
int op;


int menu();
void menubar();
void display();
void initialize();
void fileopen();
void huffmanTree(string text);
void savecodes(string str);
void retrievecode(Node* root,int index,string str);
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode);
void decode(Node* root, int &data, string str);
void loadingBar();



void initialize(){
int i;
for (i=0;i<256;i++)
    frequency[i]=0;
        }

void fileopen(){
FILE *fp;
string out="EXIT";
do {
system("cls");
SetConsoleTextAttribute(color, 15);cout<<"Input EXIT to terminate"<<endl;
cout<<"Input file to open: ";
scanf("%s",fn);
fp = fopen(fn,"r");
if (fp==NULL){
    SetConsoleTextAttribute(color, 4); cout<<"File Error.\n";system("pause");
}
else{
    break;
}

}while(fn!=out);

while(!feof(fp)){
        chr = fgetc(fp);
        contents += chr;
        frequency[chr]++;
        size++;
           }
fclose(fp);
system("cls");
loadingBar();
menubar();
}

void display(){
system("cls");
cout<<"File contains:"<<endl;
cout<<contents<<endl;
cout<<"ASCII\tChar\t\tFrequency"<<endl;
for (int i=0;i<256;i++){
    if (frequency[i]>0){
    switch(i){
        case 10:cout<<endl<<i<<"\t"<<"newline"  <<"\t\t"<<frequency[i];break;
        case 9 :cout<<endl<<i<<"\t"<<"tab"      <<"\t\t"<<frequency[i];break;
        case 32:cout<<endl<<i<<"\t"<<"space"    <<"\t\t"<<frequency[i];break;
        default:cout<<endl<<i<<"\t"<<char(i)    <<"\t\t"<<frequency[i];break;
            }
    }}
 SetConsoleTextAttribute(color, 2);    cout<<"\nFile size: "<<size-1<<" bits."<<endl;
system("pause");
}

int main(){

initialize();
fileopen();
}


int menu(){
system("cls");
SetConsoleTextAttribute(color, 15);
cout<<"\tMENU\n"<<endl;
cout<<"1. Compress"<<endl;
cout<<"2. Decompress"<<endl;
cout<<"3. Display Frequency"<<endl;
cout<<"4. Exit"<<endl;
cout<<"Select(1-4):";
cin>>op;
return op;
}

void menubar(){
while (true){
    op = menu();
    switch(op){
    case 1: system("cls");
    loadingBar();
    huffmanTree(contents);break;
    case 2: system("cls");
    loadingBar();
    huffmanTree(contents);break;
    case 3: system("cls");
    display();break;
    case 4: exit(0);break;
    default: cout<<" 1 to 4 only.\n";system("pause");
    }
  }
}


struct comp
{
    bool operator()(const Node* l, const Node* r) const
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


void decode(Node* root, int &data, string str)
{
    if (root == nullptr) {
        return;
    }


    if (isLeaf(root))
    {
        cout << root->ch;
        return;
    }

    data++;

    if (str[data] == '0') {
        decode(root->left,data, str);
    }
    else {
        decode(root->right, data, str);
    }
}


void huffmanTree(string text)
{


    if (text == EMPTY_STRING) {
        return;
    }

    unordered_map<char, int> freq;
     for(int i=0;i<256;i++){
       if(frequency[i]>0)
            freq[i]= frequency[i];
    }

    priority_queue<Node*, vector<Node*>, comp> pq;


    for (auto pair: freq) {
        if(pair.first>0)
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1)
    {


        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();    pq.pop();

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }


    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);


    string str;
    for (char ch: text) {
        str += huffmanCode[ch];
    }


    if(op==1){

    cout << "Huffman Codes are:\n" << endl;
    for (auto pair: huffmanCode) {
        switch(pair.first){
        case 10:cout << "newline" << "\t\t" << pair.second << endl;break;
        case 9 :cout << "tab" << "\t\t" << pair.second << endl;break;
        case 32:cout << "space" << "\t\t" << pair.second << endl;break;
        default:cout << pair.first << "\t\t" << pair.second << endl;break;}

    }


    savecodes(str);
    cout<<endl<<"Compressed code "<<endl<<"->"<<str<<endl;
     SetConsoleTextAttribute(color, 2);cout<<"\nCompressed successfully!"<<endl;
    system("pause");

    } else if (op==2){
    int data = -1;
    retrievecode(root,data,str);
     SetConsoleTextAttribute(color, 2);cout<<"\n\nDecompressed successfully!"<<endl;
    system("pause");
    }


}


void savecodes(string str){
file = fn+file;
ofstream fp(file);
fp << str << endl;
fp.close();
}

void retrievecode(Node* root,int data,string str){
system("cls");
cout<<endl<<"Compressed code "<<endl<<"->"<<str<<endl;
cout <<endl<<"The decoded file is: "<<endl;
file = fn+file;
ifstream fp(file);
getline(fp,str);

if (isLeaf(root))
    {

        while (root->freq--) {
            cout << root->ch;
        }
    }
    else {

        while (data < (int)str.size() - 1) {
            decode(root, data, str);
        }
    }

fp.close();
}
void loadingBar()
{
    system("color 0F");
    char a = 177, b = 219;

    cout<<"\n\n\n\n";
    cout<<"\n\n\n\n\t\t\t\t\t"
            "Loading...\n\n";
    cout<<"\t\t\t\t\t";


    for (int i = 0; i < 26; i++)
        cout<<""<<a;


    cout<<"\r";
    cout<<"\t\t\t\t\t";


    for (int i = 0; i < 26; i++) {
        cout<<""<< b;


        Sleep(40);
    }
system("cls");
}
