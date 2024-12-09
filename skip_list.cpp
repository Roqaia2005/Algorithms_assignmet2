#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cstring>
using namespace std;
class Node{
    public:
    int value;
    Node **next;
    Node(int value,int level){
        this->value=value;
        next=new Node*[level+1];
        memset(next, 0, sizeof(Node*)*(level+1));
    }

};
class SkipList{
    int max_level;
    int p;
    int level;//current level
    Node* header;
    public:
    SkipList(int max_level , double p){
        this->max_level=max_level;
        this->p=p;
        level=0;
        header = new Node(-1,max_level);

    }
    int createRandomLevel(){
        float random =(float)rand()/RAND_MAX;//rand value from 1 to 0
        int lvl=0;
        while (random<p&&level<max_level) {
            level++;
            random=(float)rand()/RAND_MAX;
        
        }
        return lvl;
        
    }
    Node * createNewNode(int key,int level){
        Node *newNode=new Node (key,level);
        return newNode;

    }
    void addNode(int value){
        Node*curNode=header; //current node is header
        Node* update[max_level+1];
        memset(update, 0, sizeof(Node*)*(max_level+1));

        for(int i=level;i>=0;i--){
            while (curNode->next[i]!=NULL&&curNode->next[i]->value<value) {
                curNode=curNode->next[i];
            }
            update[i]=curNode;
            
        }
        curNode=curNode->next[0];

        if(curNode==NULL||curNode->value!=value){
            int r=createRandomLevel();
            if(r>level){
                for (int i=level+1;i<r+1;i++)
                update[i] = header;

        
            level = r;
            }
        
        Node* n = createNewNode(value, r);

        
        for (int i=0;i<=r;i++)
        {
            n->next[i] = update[i]->next[i];
            update[i]->next[i] = n;
        }
        cout << "Successfully Inserted key " << value << "\n";
        }


}
void deleteNode(int value){
    Node*curNode=header; //current node is header
        Node* update[max_level+1];
        memset(update, 0, sizeof(Node*)*(max_level+1));
        
        for(int i=level;i>=0;i--){
            while (curNode->next[i]!=NULL&&curNode->next[i]->value<value) {
                curNode=curNode->next[i];
            }
            update[i]=curNode;
            
        }
        curNode=curNode->next[0];
        if(curNode!=NULL&&curNode->value==value){
            for(int i=0;i<=level;i++){
                if(update[i]->next[i]!=curNode){
                    break;
                }
                update[i]->next[i] = curNode->next[i];
            }
            while(level>0 &&header->next[level] == 0){
                level--;
            }
            cout<<"Successfully deleted value "<<value<<"\n";

        }
        cout<<"failed to delete node";
}

void search(int value){
    Node*curNode;
    for(int i=level;i>=0;i--){
        while(curNode->next[i] &&curNode->next[i]->value < value){
            curNode = curNode->next[i];
            }

    }
    curNode=curNode->next[0];
    if(curNode->value==value){
        cout<<"value"<<value<<"found";
}
    else{
    cout<<"value not found";
}

    

}

void display()
{
    cout<<"\n*****Skip List*****"<<"\n";
    for (int i=0;i<=level;i++)
    {
        Node *node = header->next[i];
        cout << "Level " << i << ": ";
        while (node != NULL)
        {
            cout << node->value<<" ";
            node = node->next[i];
        }
        cout << "\n";
    }
}
};
int main()
{
    // Seed random number generator
    srand((unsigned)time(0));

    // create SkipList object with MAXLVL and P
    SkipList lst(3, 0.5);

    lst.addNode(3);
    lst.addNode(6);
    lst.addNode(7);
    lst.addNode(9);
    lst.addNode(12);
    lst.addNode(19);
    lst.addNode(17);
    lst.addNode(26);
    lst.addNode(21);
    lst.addNode(25);
    // lst.search(7);
    lst.deleteNode(0);
    lst.display();
}