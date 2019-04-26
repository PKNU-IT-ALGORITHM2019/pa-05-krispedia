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

Node<string>* findNode(Node<string>* node, string target){
    if(node == NULL || node->data == target)
        return node;
    if(target < node->data)
        return findNode(node->left, target);
    else
        return findNode(node->right, target);
}
Node<string>* treeMinimum(Node<string>* node){
    if(node->left != NULL)
        node = node->left;
    return node;
}
Node<string>* successor(Node<string>* node){
    if(node->right != NULL)
        return treeMinimum(node->right);
    Node<string>* parent = node->parent;
    while(parent != NULL && parent->right == node){
        node = parent;
        parent = parent->parent;
    }
    return parent;
}
Node<string>* deleteData(Node<string>* node){
    Node<string>* real;
    Node<string>* temp;
    if(node->left == NULL || node->right == NULL)
        real = node;
    else
        real = successor(node);
    if(real->left != NULL)
        temp = real->left;
    else
        temp = real->right;
    if(temp != NULL)
        temp->parent = real->parent;
    if(real->parent == NULL)
        root = temp;
    else{
        if(real->parent->left == real)
            real->parent->left = temp;
        else
            real->parent->right = temp;
    }
    if(real != node)
        node->data = real->data;
    return real;
}
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
        meaning = line.substr(line.find(")")+2, line.size());

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
    string newWord, newClass, newMeaning, inMeaning;
    cout<<"word: ";
    cin>>newWord;
    cout<<"class: ";
    cin>>newClass;
    cout<<"meaning: ";
    cin>>inMeaning;
    getline(cin, newMeaning);
    cout<<"newMeaning: "<<newMeaning<<endl;

    for(int i=0; i<newWord.size(); i++)
        newWord[i] = tolower(newWord[i]);
    words.push_back(newWord);
    meanings.push_back(newMeaning);
    dict[newWord] = newMeaning;

    Node<string>* newNode = new Node<string>(newWord);
    insertNode(newNode, NULL, root, -1);
}
void find(string argument){
    if(dict[argument] != "")
        cout<<dict[argument]<<endl;
    else
        cout<<"Not found in Dictionary"<<endl;

}
void deleteSingle(string argument){
    for(int i=0; i<argument.size(); i++)
        argument[i] = tolower(argument[i]);
    Node<string>* targetNode = findNode(root, argument);
    if(targetNode != NULL){
        deleteData(targetNode);
        dict.erase(argument);
        cout<<"Deleted successfully."<<endl;
    }
    else
        cout<<"Not found."<<endl;
}
void deleteAll(string filename) {
    cout<<"deleteAll"<<endl;
    vector<string> delWords;
    string input, target;
    int delCount=0;
    ifstream fin;
    fin.open(filename);
    if(fin.is_open())
        cout<<"file open ok!"<<endl;
    else
        cout<<"File not found"<<endl;

    while(!fin.eof()){
        getline(fin,input);
        cout<<"input: "<<input<<endl;
        for(int i=0; i<input.size(); i++)
            input[i] = tolower(input[i]);
        delWords.push_back(input);
    }
    
    for(int i=0; i<delWords.size(); i++){
        target = delWords[i];
        Node<string>* delNode = findNode(root, target);
        if(delNode != NULL){
            deleteData(delNode);
            delCount++;
        }
        else
            cout<<"Not found "<<target<<endl;
    }
    cout<<delCount<<" words were deleted successfully."<<endl;
}
void inorder(Node<string>* node){
    if(node != NULL) {
        inorder(node->left);
        cout<<node->data<<" ";
        inorder(node->right);
    }
}
void commandLine() {
    string line, command, argument;

    while(1) {
        cout<<">>";
        getline(cin, line);
        cout<<line<<endl;

        command = line.substr(0, line.find(" "));

        if(command == "size")
            cout<<words.size()<<endl;

        else if(command == "find"){
            argument = line.substr(line.find(" ")+1, line.size()-1);
            find(argument);
        }

        else if(command == "add"){
            addNewData();
        }

        else if(command == "delete"){
            argument = line.substr(line.find(" ")+1, line.size()-1);
            deleteSingle(argument);
        }

        else if(command == "deleteall"){
            argument = line.substr(line.find(" ")+1, line.size()-1);
            cout<<"del file: "<<argument<<"|"<<endl;
            deleteAll(argument);
        }

        else if(command == "inorder"){
            inorder(root);
            cout<<endl;
        }
        else if(command == "exit"){
            return;
        }
        else
            cout<<"Invalid Command!!"<<endl;
    }

}
int main(void) {

    readInput("test.txt");
    buildTree();

    commandLine();

    return 0;
}
