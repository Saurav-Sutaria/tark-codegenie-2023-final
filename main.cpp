#include <bits/stdc++.h>
using namespace std;

class Route{
    public:
    pair<string, int> station;
};
class Seat{
    public:
    int number;
    vector<string> dates;
    string src;
    string dest;

    Seat(int number){
        this->number = number;
    }
};
class Coach{
    public:
    char category;
    int coachNum;
    vector<Seat *> seats;
};
class Train{
    public:
    string number;
    vector<Route *> routes;
    vector<Coach *> coaches;
};

vector<string> split(string s){
    vector<string> ans;
    string temp = "";
    for (auto i : s){
        if (i == ' '){
            ans.push_back(temp);
            temp = "";
        }
        else temp += i;
    }
    ans.push_back(temp);
    return ans;
}
vector<string> splitHyphen(string s){
    vector<string> ans;
    string temp = "";
    for (int i = 0; i < s.size(); i++){
        if (s[i] == '-'){
            ans.push_back(temp);
            temp = "";
        }
        else{
            temp += s[i];
        }
    }
    ans.push_back(temp);
    return ans;
}
Route* addRoute(string s){
    Route* r = new Route();
    vector<string> temp = splitHyphen(s);
    r->station = make_pair(temp[0], stoi(temp[1]));
    return r;
}
Coach* addCoach(string s){
    Coach *c = new Coach();
    vector<string> temp = splitHyphen(s);
    c->category = temp[0][0];
    c->coachNum = stoi(temp[0].substr(1));
    //cout<<c->category<<",,"<<c->coachNum<<endl;
    int seatCount = stoi(temp[1]);
    //cout<<seatCount<<endl;
    vector<Seat *> coachSeats(seatCount);
    for (int i = 0; i < seatCount; i++){
        Seat* currSeat = new Seat(i+1);
        coachSeats[i] = currSeat;
    }
    c->seats = coachSeats;
    // for(auto i:c->seats){
    //     cout<<i->number<<",";
    // }
    // cout<<"reached here"<<endl;
    return c;
}
// void printSeats(vector<Seat*> s){
//     //cout<<"inside seat\n";
//     for(auto i:s){
//         cout<<i->number<<","<<i->src<<","<<i->dest;
//         for(auto j:i->dates){
//             cout<<j<<",";
//         }
//     }
//     cout<<endl;
//     return;
// }
// void printCoach(vector<Coach*> c){
//     cout<<"inside coach :"<<c.size()<<endl;
//     for (auto i : c){
//         cout << i->category << "," << i->coachNum << endl;
//         printSeats(i->seats);
//     }

//     return;
// }
// void printRoutes(vector<Route *> r){
//     for (auto i : r){
//         cout << i->station.first << "," << i->station.second << endl;
//     }
//     return;
// }
// void printTrain(Train *t){
//     cout << "Number :" << t->number << endl;
//     cout << "Coaches :";
//     printCoach(t->coaches);
//     cout << "Routes : \n";
//     printRoutes(t->routes);
//     return;
// }
int routeAvailable(vector<Train *> t, string src, string dest){
    for (int i = 0; i < t.size(); i++){
        if (t[i]->routes[0]->station.first == src && t[i]->routes[1]->station.first == dest){
            return i;
        }
    }
    return -1;
}
bool dateFree(Seat *s, string d){
    for (auto date : s->dates){
        if (date == d){
            return false;
        }
    }
    return true;
}
bool checkVacancy(Train *t, string date, string cate, int quant){
    unordered_map<string, char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';

    int seatsBooked = 0;
    int vacantSeats = 0;
    for (auto coach : t->coaches){
        if (coach->category == mp[cate]){
            for (auto seat : coach->seats){
                if (vacantSeats >= quant) return true;
                if (dateFree(seat, date)){
                    vacantSeats++;
                }
            }
        }
    }
    return (vacantSeats >= quant);
}
void bookTicket(Train *t, string date, string cate, int quant){
    unordered_map<string, char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';
    int seatsBooked = 0;
    vector<string> bookSeat;
    for (auto coach : t->coaches){
        if (coach->category == mp[cate]){
            for (auto seat : coach->seats){
                if (seatsBooked == quant) return;
                if (dateFree(seat, date)){
                    seat->dates.push_back(date);
                    seatsBooked++;
                }
            }
        }
    }
    return;
}
int findRate(Train *t, string cate, int quant){
    unordered_map<string, int> mp;
    mp["SL"] = 1;
    mp["3A"] = 2;
    mp["2A"] = 3;
    mp["1A"] = 4;
    int dist = t->routes[t->routes.size() - 1]->station.second;
    return (dist * quant * mp[cate]);
}
// void printVector(vector<string>& v){
//     for(auto i:v) cout<<i<<",,";
//     cout<<endl;
//     return;
// }

int main(){
    // data of all trains
    vector<Train *> trains;

    int tt;
    cin >> tt;
    cin.ignore();
    while(tt--){
        string trainDetailStr;
        getline(cin, trainDetailStr);

        // train details
        vector<string> trainDetails = split(trainDetailStr);
        Train* t = new Train();
        t->number = trainDetails[0];
        t->routes.push_back(addRoute(trainDetails[1]));
        t->routes.push_back(addRoute(trainDetails[2]));

        // for(auto i:t->routes){
        //     cout<<i->station.first<<","<<i->station.second<<endl;
        // }

        // coach details
        // cin.ignore();
        string coachDetailStr;
        getline(cin, coachDetailStr);

        vector<string> coachDetails = split(coachDetailStr);
        //printVector(coachDetails);
        for (int i = 1; i < coachDetails.size(); i++){
            //cout<<coachDetails[i]<<endl;
            t->coaches.push_back(addCoach(coachDetails[i]));
        }
        // for(auto i:t->coaches){
        //     cout<<i->category<<",,"<<i->coachNum<<"\n Seats \n";
        //     for(auto j:i->seats){
        //         cout<<j->number<<",,"<<j->src<<",,"<<j->dest<<endl;
        //         for(auto k:j->dates) cout<<k<<"||";
        //         cout<<endl;
        //     }
        // }
        trains.push_back(t);
        //printTrain(t);
    }
    // request start
    int PNR = 100000001;
    while (true){
        
        string requestStr;
        getline(cin, requestStr);

        vector<string> request = split(requestStr);
        //printVector(request);
        int idx = routeAvailable(trains, request[0], request[1]);
        if (idx != -1)
        {
            // cout<<"inside idx \n";
            // train,src,dest,date,class,num
            //vector<string> bookThis = checkVacancy(trains[idx], request[2], request[3], stoi(request[4]));

            if (checkVacancy(trains[idx], request[2], request[3], stoi(request[4]))){
                bookTicket(trains[idx], request[2], request[3], stoi(request[4]));
                cout << PNR << " " << findRate(trains[idx], request[3], stoi(request[4])) << endl;
                PNR++;
            }
            else
            {
                cout << "No Seats Available" << endl;
            }
        }
        else
        {
            cout << "No Train Available" << endl;
        }
        // cout<<"here\n";
    }
    return 0;
}