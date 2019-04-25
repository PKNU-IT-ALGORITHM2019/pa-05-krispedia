#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

template <typename T>
class Node{
public:
    T data;
    Node* left;
    Node* right;
    Node* parent;

    Node(T data = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL){
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};

vector<string> words;
vector<string> meanings;
map<string, string> dict;

Node<string>* root;

void readInput(string filename) {
    ifstream fin;
    fin.open(filename);

    while(!fin.eof()){
        string word, meaning;
        string line;
        getline(fin, line);
        
        word = line.substr(0, line.find("(")-1);
        for(int i=0; i<word.size(); i++)
            word[i] = tolower(word[i]);
        meaning = line.substr(line.find("(")+1, line.size());

        words.push_back(word);
        meanings.push_back(meaning);
        dict[word] = meaning;
    }
    fin.close();
}
void insertNode(Node<string>* newNode, Node<string>* checkParent, Node<string>* checkNode, int LorR){
    cout<<"insert start"<<endl;
    if(root == NULL){
        root = newNode;
        cout<<"inserted root"<<endl;
    }
    else if(checkNode == NULL){
        newNode->parent = checkParent;
        if(LorR == 0)
            checkParent->left = newNode;
        else
            checkParent->right = newNode;
        
        cout<<"inserted"<<endl;
    }
    else{
        if(newNode->data < checkNode->data)
            insertNode(newNode, checkNode, checkNode->left, 0);
        else
            insertNode(newNode, checkNode, checkNode->right, 1);
    }
}
void buildTree(){
    for(int i=0; i<words.size(); i++){
        cout<<"build start"<<endl;
        Node<string>* newNode = new Node<string>(words[i]);
        insertNode(newNode, NULL, root, -1);
    }

    cout<<"root data: "<<root->data<<endl;
    cout<<"root left: "<<root->left->data<<endl;
    cout<<"root right: "<<root->right->data<<endl;
}
void addNewData() {
    string newWord, newClass, newMeaning;
    cout<<"word: ";
    cin>>newWord;
    cout<<"class: ";
    cin>>newClass;
    cout<<"meaning: ";
    cin>>newMeaning;

    for(int i=0; i<newWord.size(); i++)
        newWord[i] = tolower(newWord[i]);
    words.push_back(newWord);
    meanings.push_back(newMeaning);
    dict[newWord] = newMeaning;

    Node<string>* newNode = new Node<string>(newWord);
    insertNode(newNode, NULL, root, -1);
}
Node<string>* findNode(Node<string>* node, string target){
    if(node == NULL || node->data == target)
        return node;
    if(target < node->data)
        return findNode(node->left, target);
    else
        return findNode(node->right, target);
}
int main(void) {

    readInput("test.txt");
    buildTree();
    addNewData();
    cout<<findNode(root,"kit")->data<<endl;
    

    return 0;
}
