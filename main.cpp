#include <iostream>
#include <unordered_map>
using namespace std;

struct Node{
    Node* next;
    Node* prev;
    Node* parent;
    Node* child;
    int val;
    int grad;
    bool isRoot;
    Node(){
        this->next = nullptr;
        this->prev = nullptr;
        this->parent = nullptr;
        this->child = nullptr;
        this->grad = -1;
        this->isRoot = false;
    }
};
Node *root = nullptr;
Node *minim = nullptr;

void insert(int x)
{
    Node *new_nod = new Node;
    new_nod->val = x;
    if (minim == nullptr || x < minim->val)
        minim = new_nod;
    if(root == nullptr){
        new_nod->grad = 1;
        root = new_nod;
        root->isRoot = true;
    }
    else{
        new_nod->next = root;
        root->isRoot = false;
        root->prev = new_nod;
        new_nod->grad = 1;
        root = new_nod;
        root->isRoot = true;
    }
}

void mergeHeaps(Node* parent, Node* child)
{
    if(child->isRoot){
        parent->isRoot = true;
        root= parent;
    }
    if( parent-> child  == nullptr){
        parent->child = child;
        child->parent = parent;
        parent->grad += child->grad;
    }
    else{
        Node* aux = parent->child;
        while(aux->next != nullptr){
            aux = aux->next;
        }
        aux->next = child;
        child->parent = parent;
        parent->grad += child->grad;
        child->prev = aux;
        child->next = nullptr;
    }
}

void consolidate()
{
    unordered_map<int, Node*> map;
    Node *curr = root;
    while( curr != nullptr){
        if(map.find(curr->grad) != map.end()){
            while(map.find(curr->grad) != map.end() && map[curr->grad] != curr) {

                Node *aux = map[curr->grad];
                if (curr->val < aux->val) {
                    map.erase(curr->grad);
                    Node *temp = aux->prev;
                    aux->next = nullptr;
                    if (temp != nullptr)
                        temp->next = curr;
                    curr->prev = temp;
                    mergeHeaps(curr, aux);
                    if(map.find(curr->grad) == map.end())
                        map.insert({curr->grad, curr});
                } else {
                    map.erase(curr->grad);
                    Node *temp = curr->next;
                    curr->next = nullptr;
                    curr->prev = nullptr;
                    aux->next = temp;
                    mergeHeaps(aux, curr);
                    curr = aux;
                    if(map.find(curr->grad) == map.end())
                        map.insert({curr->grad, curr});
                }
            }
        }
        else
            map.insert({curr->grad, curr});
        curr = curr->next;
    }
    delete curr;
}

int extractMin()
{
    int min_val = minim->val;
    if(minim == root){
        Node* temp = minim->child;
        if(temp != nullptr){
            Node* temp2 = temp;
            temp = temp->next;
            temp2->next = minim->next;
            minim->prev = temp2;
            root = temp2;
            while(temp != nullptr){
                Node *aux = temp;
                temp = temp->next;
                aux->parent = nullptr;
                aux->next = root;
                aux->prev = nullptr;
                aux->isRoot = true;
                root->isRoot = false;
                root = aux;
            }
        }
        else {
            root->isRoot = false;
            root = minim->next;
            root->isRoot = true;
        }
    }
    else{
        Node* temp = minim->child;
        while (temp != nullptr) {
            Node *aux = temp;
            temp = temp->next;
            aux->parent = nullptr;
            aux->next = root;
            aux->prev = nullptr;
            aux->isRoot = true;
            root->isRoot = false;
            root = aux;
        }
        Node* prev = minim->prev;
        Node* next = minim->next;
        if(prev != nullptr)
            prev->next = next;
        if(next != nullptr)
            next->prev = prev;
    }
    consolidate();
    Node *root_copy = root;
    int new_min = 320000;
    while(root_copy != nullptr){
        if( root_copy->val < new_min){
            new_min = root_copy->val;
            minim = root_copy;
        }
        root_copy = root_copy->next;
    }
    delete root_copy;
    return min_val;
}
void afisare_roots()
{
    Node *temp;
    temp = root;
    while(temp != nullptr){
        cout << temp->val << " " ;
        temp = temp->next;
    }
    cout << endl;
    delete temp;
}

int main() {

    insert(2);
    insert(1);
    insert(3);
    insert(4);
    insert(7);
    afisare_roots();
    cout << extractMin()<< endl;
    afisare_roots();
    return 0;
}
