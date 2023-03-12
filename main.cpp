#include <bits/stdc++.h>
using namespace std;

class Route
{
public:
    pair<string, int> station;
};
class Seat
{
public:
    int number;
    vector<string> dates;
    string src;
    string dest;
};
class Coach
{
public:
    char category;
    int coachNum;
    vector<Seat *> seats;
};
class Train
{
public:
    string number;
    vector<Route *> routes;
    vector<Coach *> coaches;
};

vector<string> split(string s)
{
    vector<string> ans;
    string temp = "";
    for (auto i : s)
    {
        if (i == ' ')
        {
            ans.push_back(temp);
            temp = "";
        }
        else
            temp += i;
    }
    ans.push_back(temp);
    return ans;
}
vector<string> splitHyphen(string s)
{
    vector<string> ans;
    string temp = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '-')
        {
            ans.push_back(temp);
            temp = "";
        }
        else
        {
            temp += s[i];
        }
    }
    ans.push_back(temp);
    return ans;
}
Route *addRoute(string s)
{
    Route *r = new Route();
    vector<string> temp = splitHyphen(s);
    r->station = make_pair(temp[0], stoi(temp[1]));
    return r;
}
Coach *addCoach(string s)
{
    Coach *c = new Coach();
    vector<string> temp = splitHyphen(s);
    c->category = temp[0][0];
    c->coachNum = stoi(temp[0].substr(1));
    int seatCount = stoi(temp[1]);
    vector<Seat *> coachSeats(seatCount);
    for (int i = 0; i < seatCount; i++)
    {
        Seat *currSeat = new Seat();
        currSeat->number = i + 1;
    }
    c->seats = coachSeats;
    return c;
}
void printCoach(vector<Coach *> c)
{
    for (auto i : c)
    {
        cout << i->category << "," << i->coachNum << endl;
    }
    return;
}
void printRoutes(vector<Route *> r)
{
    for (auto i : r)
    {
        cout << i->station.first << "," << i->station.second << endl;
    }
    return;
}
void printTrain(Train *t)
{
    cout << "Number :" << t->number << endl;
    cout << "Coaches :" << endl;
    printCoach(t->coaches);
    cout << "Routes : \n";
    printRoutes(t->routes);
    return;
}
int routeAvailable(vector<Train *> t, string src, string dest)
{   
    //cout<<"inside route check\n";
    for (int i = 0; i < t.size(); i++)
    {
        if(t[i]->routes[0]->station.first == src && t[i]->routes[1]->station.first == dest){
            //cout<<"root found\n";
            return i;
        }
    }
    return -1;
}
bool dateFree(Seat* s,string d){
    for(auto date:s->dates){
        cout<<date<<endl;
        if(date == d) return false;
    }
    //cout<<"inside 1\n";
    return true;
}
vector<string> checkVacancy(Train* t,string date,string cate,int quant){
    unordered_map<string,char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';

    int seatsBooked = 0;
    vector<string> bookSeat;
    for(auto coach:t->coaches){
        if(coach->category == mp[cate]){
            for(auto seat:coach->seats){
                if(seatsBooked == quant) return bookSeat;
                if(dateFree(seat,date)){
                    //cout<<"inside 1\n";
                    string seatStr = to_string(coach->category) + to_string(coach->coachNum) + "-" + to_string(seat->number); 
                    bookSeat.push_back(seatStr);
                    seatsBooked++;
                }
            }
        }
    }
   // cout<<"in vacancy\n";
    cout<<bookSeat.size()<<endl;
    return bookSeat;
}
void bookTicket(Train* t,string date,string cate,int quant){
    unordered_map<string,char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';
    int seatsBooked = 0;
    vector<string> bookSeat;
    for(auto coach:t->coaches){
        if(coach->category == mp[cate]){
            for(auto seat:coach->seats){
                if(seatsBooked == quant) return;
                if(dateFree(seat,date)){
                    seat->dates.push_back(date);
                    seatsBooked++;
                }
            }
        }
    }
    return;

}
int findRate(Train* t,string cate,int quant){
    unordered_map<string,int> mp;
    mp["SL"] = 1;
    mp["3A"] = 2;
    mp["2A"] = 3;
    mp["1A"] = 4;
    int dist = t->routes[t->routes.size() - 1]->station.second;
    return (dist *quant * mp[cate]);
}

int main()
{
    // data of all trains
    vector<Train *> trains;

    int tt;
    cin >> tt;
    while (tt--)
    {
        cin.ignore();
        string trainDetailStr;
        getline(cin, trainDetailStr);

        // train details
        vector<string> trainDetails = split(trainDetailStr);
        Train *t = new Train();
        t->number = trainDetails[0];
        t->routes.push_back(addRoute(trainDetails[1]));
        t->routes.push_back(addRoute(trainDetails[2]));

        // coach details
        cin.ignore();
        string coachDetailStr;
        getline(cin, coachDetailStr);

        vector<string> coachDetails = split(coachDetailStr);
        for (int i = 1; i < coachDetails.size(); i++)
        {
            t->coaches.push_back(addCoach(coachDetails[i]));
        }
        trains.push_back(t);
    }
    // request start
    while (true)   
    {
        int PNR = 100000001;
        string requestStr;
        getline(cin, requestStr);
        
        vector<string> request = split(requestStr);
        int idx = routeAvailable(trains, request[0], request[1]);
        if (idx != -1)
        {   
            //cout<<"inside idx \n";
            //train,src,dest,date,class,num
            vector<string> bookThis = checkVacancy(trains[idx],request[2],request[3],stoi(request[4]));

            if(bookThis.size()){
                bookTicket(trains[idx],request[2],request[3],stoi(request[4]));
                cout<<PNR<<" "<<findRate(trains[idx],request[3],stoi(request[4]))<<endl;
                PNR++;    
            }else{
                cout<<"No Vacant Seats"<<endl;
            }
        }
        else
        {
            cout << "Train Not Available" << endl;
        }
        //cout<<"here\n";
    }
    return 0;
}