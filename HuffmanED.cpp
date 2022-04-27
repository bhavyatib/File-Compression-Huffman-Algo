#include <bits/stdc++.h>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

class Node{
    public:
    char character=' ';
    int isleaf=0,freq,bit;
    Node *left=NULL,*right=NULL;
    Node(){
        this->left=NULL;
        this->right=NULL;
        this->isleaf=0;
    }
};


struct comp{
    bool operator()(Node* a,Node* b)
    {
        return a->freq>b->freq;
    }
};


//For inorder traversal of Huffman tree by using recursion
//O(n) total for recursive inorder traversal of tree
void inorderassign(Node* root) 
{
    if(root->isleaf==1 || (root->left==NULL && root->right==NULL))
     return;

    if(root->left!=NULL) root->left->bit=0;
    if(root->right!=NULL) root->right->bit=1;

    inorderassign(root->left);
    inorderassign(root->right); 
}


//Stores the bit code of each character into a new unordered map
//O(nlogn) total
void inorderstore(Node* root,unordered_map<char,string>& mp,string& temp)
{   if(root==NULL)
      return;
    if(root->isleaf==1 || (root->left==NULL && root->right==NULL)) //O(logn) to traverse till leaf node
    {   temp=temp+to_string(root->bit);
        mp[root->character]= temp; //O(1) insertion in unordered map
        temp.pop_back(); //O(1)
        return;
    }

    temp=temp+to_string(root->bit);

    inorderstore(root->left,mp,temp);
    inorderstore(root->right,mp,temp);
    temp.pop_back(); 
}

//decoding (uses the Huffman tree and the encoded bit string )
//O(nlogn) total
string decoder (Node* root, string encodedString){
    string ds;
    Node* temp= root;
    //iterate through the encoded bit string one character at a time
    for(int i=0;i<encodedString.length();i++){ //O(n) for iteration
        //start from root and traverse until we reach a leaf node
        if (encodedString[i]== '0') 
            temp= temp->left; //O(logn) for traversal till leaf node
        else
            temp= temp->right;

        if (temp->left== NULL && temp->right== NULL)
        {
            //once we reach a leaf node, append the character present in that node to the decoded string
            ds+= temp->character;
            //move back the pointer to the root of the Huffman tree
            temp= root;
        }   
    }
    return ds;
}


int main()
{  

//encoding 

//take input from file using file handling and make an input string

    ifstream fin;
    string inputString;
    fin.open("input1.txt"); 

    while (fin){
        getline (fin, inputString, static_cast<char>(EOF));
    }

    cout <<endl;
    cout << "Your input: " << endl << inputString <<endl;
    cout <<endl;


//find and store frequencies of individual characters using unordered map

    //O(n) total
    unordered_map<char,int> freqmap;
    for(int i=0;i<inputString.length();i++) //O(n) to iterate over the string
      freqmap[inputString[i]]++; //O(1) to insert into unordered map

    //print out the frequency map
    //for(auto &m: freqmap)
      //cout<<m.first<<" "<<m.second<<endl;
      //cout << endl;
     

//create a priority queue using min heap having all nodes  
    priority_queue<Node*,vector<Node*>, comp> pq;
    
    //O(nlogn) total
    for(auto &m : freqmap) //O(n) for the loop - 
    {
        Node* node= new Node();
        node->isleaf=1;
        node->character=m.first;
        node->freq=m.second;
        
        pq.push(node); //O(logn) for heap insertion
    }

    //print out the priority queue
    //while (! pq.empty() ) {
    //cout << pq.top()->character << "\n";
    //pq.pop();
    //}
    //cout <<endl;


//create the Huffman tree using the priority queue
//O(nlogn) total
    while(pq.size()>1) //O(n) for the loop
    {
        Node* minimum=pq.top(); //O(1) for getting minimum element in min-heap
        pq.pop(); //O(logn) for removing minimum element in min-heap (need to heapify)
        Node* second_minimum=pq.top();
        pq.pop();

        Node* node= new Node();
        node->left=minimum;
        node->right=second_minimum;
        node->freq= minimum->freq+second_minimum->freq;

        pq.push(node); //O(logn) for heap insertion
    }

    //while (! pq.empty() ) {
    //cout << pq.top()->freq << "\n";
    //pq.pop();
    //}

    Node* root=pq.top(); //O(1) for getting minimum element in min-heap
    
    inorderassign(root);


//store the leaf node path values in unordered map as codes((unordered map<char,string>))
    unordered_map<char,string> codemap;
    string temp;
    inorderstore(root->left,codemap,temp);
    inorderstore(root->right,codemap,temp);


//Create and write out the codes and the encoded bit string on the command line
    string encodedString;
    //print out the encoded maps
    //for(auto &m: codemap)
      //cout<<m.first<<" "<<m.second<<endl;
      //cout << endl;

    //O(n^2) total
    for(int i=0;i<inputString.length();i++){ //O(n) for iterating through input string
        encodedString += codemap[inputString[i]]; //O(n) for searching in an unordered map
    }

    cout<< "Encoded String: " << encodedString<<endl;
    cout <<endl;

    ofstream fout("output1.txt"); 
    fout << encodedString;
    fout.close();


//decoding
    string decodedString = decoder (root, encodedString);
    //Write out the decoded string on the command line as well as in a file
    cout<< "Decoded String: " << endl << decodedString<<endl;
    cout <<endl;

    ofstream fnewout("decoded1.txt"); 
    fnewout << decodedString;
    fnewout.close();
}