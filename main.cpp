#include<bits/stdc++.h>
using namespace std;

class Coach{
    public:
    string cateogry;
    vector<int> seats;
    unordered_map<char,vector<vector<int>>> seats;
};

class Train{
    public:
    int number;
    string src;
    string dest;
    int dist;
    vector<Coach*> coaches;
};
// class CoachS :Train{
//     public:
//     vector<vector<int>> seats;
// };
// class CoachB : Train{
//     public:
//     vector<vector<int>> seats;
// };
// class CoachA : Train{
//     public:
//     vector<vector<int>> seats;
// };
// class CoachH : Train{
//     public:
//     vector<vector<int>> seats;
// };
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
void printTrain(Train* t){
    cout<<t->number<<","<<t->dest<<","<<t->src<<","<<t->dist<<endl;
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

// void printCoach(Train* t){
//     unordered_map<char,vector<vector<int>>> mp = t->coach;
//     for(auto i:mp){
//         cout<<"Coach : "<<i.first<<endl;
//         for(int j=0;j<i.second.size();j++){
//             cout<<"S-"<<j+1<<endl;
//             for(auto k:i.second[j]){
//                 cout<<k<<" ";
//             }cout<<endl;
//         }
//     }
// }

void printTrains(Train* t){
    
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
            Coach* c = new Coach();
            vector<int> temp(i.second);
            c->cateogry = i.first;
            c->seats = temp;
        }
        printTrains(t);
        // for(auto i:coachArr){
        //     vector<int> temp(i.second);
        //     t->seats[i.first[0]].push_back(temp);
        // }
        //printCoach(t);
        // for(auto i:coachArr){
        //     char currCoach = i.first[0];
        //     if(currCoach == 'S'){
        //         vector<int> temp(i.second);
        //         t->coaches[0].push_back(temp);
        //     }else if(currCoach == 'B'){
        //         vector<int> temp(i.second);
        //         t->coaches[1].push_back(temp);
        //     }
        //     else if(currCoach == 'A'){
        //         vector<int> temp(i.second);
        //         t->coaches[2].push_back(temp);
        //     }
        //     else{
        //         vector<int> temp(i.second);
        //         t->coaches[3].push_back(temp);
        //     }
        // }
        // CoachA* CA;
        // CoachB* CB;
        // CoachH* CH;
        // CoachS* CS;
        
        // for(auto i:coachArr){
        //     char currCoach = i.first[0];
        //     // cout<<i.first<<","<<i.second<<endl;
        //     if(t->coach.find(currCoach) == t->coach.end()){
        //         if(currCoach == 'A'){
        //             CA = new CoachA();
        //             CA->seats.push_back(vector<int>(i.second));
        //         }else if(currCoach == 'B'){
        //             CB = new CoachB();
        //             CB->seats.push_back(vector<int>(i.second));
        //         }
        //         if(currCoach == 'H'){
        //             CH = new CoachH();
        //             CH->seats.push_back(vector<int>(i.second));
        //         }
        //         else{
        //             CS = new CoachS();
        //             CS->seats.push_back(vector<int>(i.second));
        //         }
        //     }else{
        //          if(currCoach == 'A'){
        //             CA->seats.push_back(vector<int>(i.second));
        //         }else if(currCoach == 'B'){
        //             CB->seats.push_back(vector<int>(i.second));
        //         }
        //         if(currCoach == 'H'){
        //             CH->seats.push_back(vector<int>(i.second));
        //         }
        //         else{
        //             CS->seats.push_back(vector<int>(i.second));
        //         }
        //     }
        // }

        // for(auto i:coachArr){
        //     string currCoach = i.first;
        //     int currSeats = i.second;
        //     char key = currCoach[0];
        //     if(t->coach.find(key) != t->coach.end()){
        //         t->coach[key].push_back(vector<int>(currSeats));
        //     }else{
        //         t->coach[key] = vector<vector<int>>(currSeats);
        //     }     
        // }
        //printCoach(t);

        
    }

    //ticket request start
    cout<<"over\n";

    return 0;
}