#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
class NodePlayer{
    public:
    string name;
    int score;
    NodePlayer **next;
    NodePlayer(string name,int score,int level){
        this->name=name;
        this->score=score;
        next=new NodePlayer*[level+1];
        memset(next, 0, sizeof(NodePlayer*)*(level+1));
    }

};
/////////////////////////////////
class SkipListPlayer{
    int max_level;
    int p;
    int level;//current level
    NodePlayer* header;
    public:

    ///////////////////////////
    SkipListPlayer(int max_level , double p){
        this->max_level=max_level;
        this->p=p;
        level=0;
        header = new NodePlayer("",0,max_level);
    }
    //////////////////////////////
    int createRandomLevel(){
        float random =(float)rand()/RAND_MAX;//rand value from 1 to 0
        int lvl=0;
        while (random<p&&lvl<max_level) {
            lvl++;
            random=(float)rand()/RAND_MAX;
        
        }
        return lvl;
        
    }
    //////////////////////////
    NodePlayer * createNewNode(string key,int score,int level){
        NodePlayer *newNode=new NodePlayer (key,score,level);
        return newNode;
    }
 ////////////////////////////////////////

void addPlayer(string name,int score){
        NodePlayer*curNode=header; //current node is header
        NodePlayer* update[max_level+1];
        memset(update, 0, sizeof(NodePlayer*)*(max_level+1));

        for(int i=level;i>=0;i--){
            while (curNode->next[i]!=NULL&&curNode->next[i]->name<name) {
                curNode=curNode->next[i];
            }
            update[i]=curNode;
            
        }
        curNode=curNode->next[0];
// the player already exist so just update his score if the new score is different


        if(curNode!=NULL&&curNode->name==name&&curNode->score!=score){
            curNode->score=score;
            cout<<"successfully updated score of player "<<name<<" to "<<score<<"\n";}
        else if(curNode!=NULL&&curNode->name==name&&curNode->score==score){
            
            cout<<"player "<<name<<" already exists with the score "<<score<<"\n";}
        else{
            int r=createRandomLevel();
            if(r>level){
                for (int i=level+1;i<r+1;i++){
                update[i] = header;
                }
                level = r;
            }
        
        NodePlayer* n = createNewNode(name,score, r);

        
        for (int i=0;i<=r;i++)
        {
            n->next[i] = update[i]->next[i];
            update[i]->next[i] = n;
        }
        cout << "Successfully added player " << name << "\n";
        }
        }

/////////////////////////////////////////////////
void deleteNode(string name){
    NodePlayer*curNode=header; //current node is header
        NodePlayer* update[max_level+1];
        memset(update, 0, sizeof(NodePlayer*)*(max_level+1));
        
        for(int i=level;i>=0;i--){
            while (curNode->next[i]!=NULL&&curNode->next[i]->name<name) {
                curNode=curNode->next[i];
            }
            update[i]=curNode;
            
        }
        curNode=curNode->next[0];
        if(curNode!=NULL&&curNode->name==name){
            for(int i=0;i<=level;i++){
                if(update[i]->next[i]!=curNode){
                    break;
                }
                update[i]->next[i] = curNode->next[i];
            }
            while(level>0 &&header->next[level] == 0){
                level--;
            }
            delete curNode;
            cout<<"Successfully deleted player "<<name<<"\n";
            return;

        }
        cout<<"failed to delete player";
}
///////////////////////////////////////////////
int search(string name){
    NodePlayer*curNode=header;
    for(int i=level;i>=0;i--){
        while(curNode->next[i] &&curNode->next[i]->name < name){
            curNode = curNode->next[i];
            }

    }
    curNode=curNode->next[0];
    if(curNode!=NULL&& curNode->name==name){
        return curNode->score;
        
}
    return -1;

}

vector<pair<string, int>> getTopN(int n) {
        vector<pair<string, int>> players;
        NodePlayer* current = header->next[0];

        while (current != nullptr) {
            players.push_back({current->name, current->score});
            current = current->next[0];
        }

        sort(players.begin(), players.end(), [](pair<string, int>& a, pair<string, int>& b) {
            return b.second < a.second;
        });

        if (players.size() > n) {
            players.resize(n);
        }
        return players;
    }
/////////////////////////////////////
void display()
{
    cout<<"\n*****Players*****"<<"\n";
    for (int i=0;i<=level;i++)
    {
        NodePlayer *node = header->next[i];
        cout << "Level " << i << ": ";
        while (node != NULL)
        {
            cout << node->name<<" ";
            node = node->next[i];
        }
        cout << "\n";
    }
}
};
////////////////////////////////////////////////////////
int main() {

    SkipListPlayer game(4,0.5);

    // Players join the game
    game.addPlayer("Player1", 100);
    game.addPlayer("Player2", 50);
    game.addPlayer("Player3", 80);
    game.addPlayer("Player3", 80);// if I try to update with the same score , no update allowed

    // Dynamic score updates
    game.addPlayer("Player1", 90);
    // Retrieve leaderboard
    cout << "Leaderboard:\n";
    auto leaderboard = game.getTopN(2);// top 2 players 1,3
    for (auto& player : leaderboard) {
        cout << player.first << ": " << player.second << endl;
    }

    // Player leaves the game
    game.deleteNode("Player3");// delete player 3 ; player 1,2 are top two

    // Retrieve a player's score abitlity to retrieve player score at any time
    cout << "Player1's score: "<< game.search("Player1") << endl;

    // New player joins
    game.addPlayer("Player4", 60); // new top 2 are 1,4

    // Updated leaderboard
    cout << "Updated Leaderboard:\n";
    leaderboard = game.getTopN(2);
    for (auto& player : leaderboard) {
        cout << player.first << ": " << player.second << endl;
    }
    game.display();

    return 0;
}
