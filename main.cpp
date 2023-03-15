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

//function to split string by space
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
//function to split string by hyphen
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
/*
function to add new route
input : Rajkot-100
*/
Route* addRoute(string s){
    Route* r = new Route();
    vector<string> temp = splitHyphen(s); //separate station name and distance
    r->station = make_pair(temp[0], stoi(temp[1]));
    return r;
}
/*
function to add new coach
input : S1-10
*/
Coach* addCoach(string s){
    Coach *c = new Coach();
    vector<string> temp = splitHyphen(s);
    c->category = temp[0][0];
    c->coachNum = stoi(temp[0].substr(1));
    int seatCount = stoi(temp[1]);
    //adding seats in the coach
    vector<Seat *> coachSeats(seatCount);
    for (int i = 0; i < seatCount; i++){
        Seat* currSeat = new Seat(i+1);
        coachSeats[i] = currSeat;
    }
    c->seats = coachSeats;
    return c;
}
/*function to check for route availability
input : array of all train, source, destination
output : index of train if route found, else -1
*/
int routeAvailable(vector<Train *> t, string src, string dest){
    for (int i = 0; i < t.size(); i++){
        if (t[i]->routes[0]->station.first == src && t[i]->routes[1]->station.first == dest){
            return i;
        }
    }
    return -1;
}
//function to check is the given seat is booked for a particular date
bool dateFree(Seat *s, string d){
    for (auto date : s->dates){
        if (date == d){
            return false;
        }
    }
    return true;
}
//function to check for if vacant seats >= given booking request
bool checkVacancy(Train *t, string date, string cate, int quant){
    unordered_map<string, char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';

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
//main ticket booking function
void bookTicket(Train *t, string date, string cate, int quant){
    unordered_map<string, char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';
    int seatsBooked = 0;
    vector<string> bookSeat;//to store the booked seats (for future use)
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
//function to calculate total fare
int findRate(Train *t, string cate, int quant){
    unordered_map<string, int> mp;
    mp["SL"] = 1;
    mp["3A"] = 2;
    mp["2A"] = 3;
    mp["1A"] = 4;
    int dist = t->routes[t->routes.size() - 1]->station.second;
    return (dist * quant * mp[cate]);
}


int main(){
    // data of all trains
    vector<Train *> trains;

    int tt;
    cin >> tt;
    cin.ignore();
    while(tt--){
        string trainDetailStr;
        getline(cin, trainDetailStr);

        //storing all train details
        vector<string> trainDetails = split(trainDetailStr);
        Train* t = new Train();
        t->number = trainDetails[0];
        t->routes.push_back(addRoute(trainDetails[1]));
        t->routes.push_back(addRoute(trainDetails[2]));

        //coach detail input
        string coachDetailStr;
        getline(cin, coachDetailStr);
        vector<string> coachDetails = split(coachDetailStr);
        
        //adding coaches in the train
        for (int i = 1; i < coachDetails.size(); i++){
            t->coaches.push_back(addCoach(coachDetails[i]));
        }
        trains.push_back(t); 
    }
    //user request start
    int PNR = 100000001;
    while (true){
        
        string requestStr;
        getline(cin, requestStr);
        vector<string> request = split(requestStr);
        
        int idx = routeAvailable(trains, request[0], request[1]);//check for route availability
        if (idx != -1){
            //check for vacancy
            if(checkVacancy(trains[idx], request[2], request[3], stoi(request[4]))){
                bookTicket(trains[idx], request[2], request[3], stoi(request[4]));
                cout << PNR << " " << findRate(trains[idx], request[3], stoi(request[4])) << endl;
                PNR++;
            }
            else{
                cout << "No Seats Available" << endl;
            }
        }
        else{
            cout << "No Train Available" << endl;
        }
    }
    return 0;
}