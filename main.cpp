#include<bits/stdc++.h>
using namespace std;

class Coach{
    public:
    string cateogry;
    vector<int> seats;
};

class Train{
    public:
    int number;
    string src;
    string dest;
    int dist;
    vector<Coach*> coaches;
};

vector<string> splitDistance(string s){
    vector<string> ans;
    string temp = "";
    for(int i=0;i<s.size();i++){
        if(s[i] == '-'){
            ans.push_back(temp);
            temp = "";
        }else{
            temp += s[i];
        }
    }
    ans.push_back(temp);
    return ans;
}

Train* getTrainDetails(string trainDetails){
    vector<string> tempStrArr;
    string temp = "";
    for(int i=0;i<trainDetails.size();i++){
        if(trainDetails[i] == ' '){
            tempStrArr.push_back(temp);
            temp = "";
        }else temp += trainDetails[i];
    }
    tempStrArr.push_back(temp);
    // for(auto i:tempStrArr){
    //     cout<<i<<endl;
    // }
    Train * t = new Train();
    t->number = stoi(tempStrArr[0]);
    t->src = splitDistance(tempStrArr[1])[0];
    t->dest = splitDistance(tempStrArr[2])[0];
    t->dist = stoi(splitDistance(tempStrArr[2])[1]);
    return t;
}
void printCoach(Coach* c){
    cout<<c->cateogry<<endl;
    for(auto i:c->seats)cout<<i<<" ";
    cout<<endl;
    return;
}
void printTrain(Train* t){
    cout<<t->number<<","<<t->dest<<","<<t->src<<","<<t->dist<<endl;
    for(auto i:t->coaches){
        printCoach(i);
    }
    return;
}
vector<pair<string,int>> getCoachDetails(string s){
    vector<pair<string,int>> ans;
    vector<string> tempArr;
    string temp = "";
    for(auto i:s){
        if(i == ' '){
            tempArr.push_back(temp);
            temp = "";
        }else temp += i;
    }
    tempArr.push_back(temp);
    string coach;
    int seats;
    for(int i=1;i<tempArr.size();i++){
        string temp = "";
        for(auto j:tempArr[i]){
            if(j == '-'){
                coach = temp;
                temp = "";
            }else temp += j;
        }
        seats = stoi(temp);
        ans.push_back(make_pair(coach,seats));
    }
    return ans;

}





int main(){
    //data of all trains
    vector<Train*> trains;

    //maping trains
    unordered_map<int,pair<string,string>> trainsMap;
    //train input
    int tt;
    cin>>tt;
    for(int i=0;i<tt;i++){
        //train detials
        cin.ignore();
        string trainDetail;
        getline(cin,trainDetail);
        //cout<<trainDetail<<endl;
        Train* t = new Train();
        t = getTrainDetails(trainDetail);
        printTrain(t);
        trains.push_back(t);
        trainsMap[t->number] = make_pair(t->src,t->dest);

        //coach details
        cin.ignore();
        string coachDetail;
        getline(cin,coachDetail);
        vector<pair<string,int>> coachArr = getCoachDetails(coachDetail);
        for(auto i:coachArr){
            cout<<i.first<<","<<i.second<<endl;
        }
        for(auto i:coachArr){
            Coach* c = new Coach();
            vector<int> temp(i.second);
            c->cateogry = i.first;
            c->seats = temp;
            t->coaches.push_back(c);
        }
        printTrain(t);
        
        
    }

    //ticket request start
    cout<<"over\n";

    return 0;
}