#include <bits/stdc++.h>
using namespace std;

class Route{
    public:
    pair<string, int> station;
};
class Ticket{
    public:
    pair<string,int> src;
    pair<string,int> dest;
    string date;
};
class UserTicket{
    public:
    int pnrNumber;
    string trainNumber;
    string src;
    string dest;
    string date;
    int totalFare;
    vector<string> seatNumbers;
};
class Seat{
    public:
    int number;
    vector<Ticket*> tickets;
    //unordered_set<string> dates;
    Seat(int number){
        this->number = number;
    }
};
class Coach{
    public:
    char category;//letter of the coach : S,B,A,H
    string coachClass; //string of SL,3A,2A,1A
    int coachNum;
    vector<Seat *> seats;
};
class Train{
    public:
    string number;
    unordered_map<string,int> routes;
    vector<Coach*> coaches;
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
pair<string,int> addRoute(string s){
    vector<string> temp = splitHyphen(s); //separate station name and distance
    return make_pair(temp[0], stoi(temp[1]));
}
/*
function to add new coach
input : S1-10
*/
Coach* addCoach(string s){
    unordered_map<char,string> mp;
    mp['S'] = "SL";
    mp['B'] = "3A";
    mp['A'] = "2A";
    mp['H'] = "1A";
    Coach *c = new Coach();
    vector<string> temp = splitHyphen(s);
    c->category = temp[0][0];
    c->coachClass = mp[c->category];
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
//function to calculate total fare
int findFare(Train *trn,vector<string>& userRequest){
    string src = userRequest[0];
    string dest = userRequest[1];
    string category = userRequest[3];
    int quantity = stoi(userRequest[4]);
    unordered_map<string, int> mp;
    mp["SL"] = 1;
    mp["3A"] = 2;
    mp["2A"] = 3;
    mp["1A"] = 4;
    int srcDistance = trn->routes[src];
    int destDistance = trn->routes[dest];
    int distanceTravelled = destDistance - srcDistance;
    return (distanceTravelled * quantity * mp[category]);
}
bool checkRouteDateCollision(Train* trn,Ticket* tkt,string src,string dest,string d){
    //if dates are different then no collision
    if(tkt->date != d) return true;
    //current source and destination distance
    int srcDistance = trn->routes[src];
    int destDistance = trn->routes[dest];
    
    if(srcDistance >= tkt->dest.second) return true;
    if(destDistance <= tkt->src.second) return true;
    return false;
}
//function to check is the given seat is booked for a particular date
bool isSeatNotReserved(Train* t,Seat *s,string date,string src,string dest){
    //if date is free then no need to check for route clash
    bool status = true;//here true means seat is free
    //traversing all tickets for a given seat
    for(auto tkt:s->tickets){
        status = status && checkRouteDateCollision(t,tkt,src,dest,date);
    }
    return status; 
}
//function to check for if vacant seats >= given booking request
bool checkVacancy(string trainNum,unordered_map<string,Train *> trn, string src,string dest,string date,string cate, int quant){
    Train* currTrain = trn[trainNum];
    int vacantSeats = 0;
    for (auto coach : currTrain->coaches){
        if (coach->coachClass == cate){
            for (auto seat : coach->seats){
                if (vacantSeats >= quant) return true;
                if (isSeatNotReserved(currTrain,seat,date,src,dest)){
                    vacantSeats++;
                }
            }
        }
    }
    return (vacantSeats >= quant);
}
/*function to check for route availability
input : array of all train, source, destination
output : index of train if route found, else -1
*/
vector<string> getAvailableTrains(unordered_map<string,Train*> trn, string src, string dest,string coachCategory){
    vector<string> ans;
    for(auto i:trn){
        
        if( (i.second->routes.find(src) != i.second->routes.end()) && (i.second->routes.find(dest) != i.second->routes.end())){
            //check for reverse journey
            int srcDistance = i.second->routes[src];
            int destDistance = i.second->routes[dest];
            if(destDistance > srcDistance){
                //checking for coach category
                for(auto j:i.second->coaches){
                    if(j->coachClass == coachCategory){
                        ans.push_back(i.first);
                        break;
                    }
                }
            }
        }
    }
    return ans;
}
set<string> checkBookingRequest(vector<string>& userRequest,unordered_map<string, Train*> trains){
    set<string> ans;
    string src = userRequest[0];
    string dest = userRequest[1];
    string journeyDate = userRequest[2];
    string coachCategory = userRequest[3];
    int totalSeats = stoi(userRequest[4]);

    //first finding trains based on source and destination and coach
    vector<string> trainsAvailable = getAvailableTrains(trains,src,dest,coachCategory);
    if(trainsAvailable.size()){
        for(auto trainNumber:trainsAvailable){
            if(checkVacancy(trainNumber,trains,src,dest,journeyDate,coachCategory,totalSeats)){
                ans.insert(trainNumber);
            }
        }
        if(ans.size() == 0) cout<<"No Seats Available\n";
    }else cout<<"No Trains Available\n";
    return ans;
}
//main ticket booking function
vector<string> bookTicket(Train *trn,vector<string>& userRequest){
    vector<string> bookedSeatsNumber;
    unordered_map<string, char> mp;
    mp["SL"] = 'S';
    mp["3A"] = 'B';
    mp["2A"] = 'A';
    mp["1A"] = 'H';
    string src = userRequest[0];
    string dest = userRequest[1];
    string journeyDate = userRequest[2];
    string coachCategory = userRequest[3];
    int totalSeats = stoi(userRequest[4]);

    int seatsBooked = 0;
    vector<string> bookSeat;//to store the booked seats (for future use)
    for (auto coach : trn->coaches){
        if (coach->category == mp[coachCategory]){
            for (auto seat : coach->seats){
                if (seatsBooked == totalSeats) return bookedSeatsNumber;
                if (isSeatNotReserved(trn,seat,journeyDate,src,dest)){
                    seatsBooked++;
                    string seatNumber = coach->category + to_string(coach->coachNum) + "-" + to_string(seat->number);
                    bookedSeatsNumber.push_back(seatNumber);
                    Ticket* currTicket = new Ticket();
                    seat->tickets.push_back(currTicket);
                    currTicket->src = make_pair(src,trn->routes[src]);
                    currTicket->dest = make_pair(dest,trn->routes[dest]);
                    currTicket->date = journeyDate;
                }
            }
        }
    }
    return bookedSeatsNumber;
}
void getDetailsByPNR(int pnr,unordered_map<int, UserTicket*>& tickets){
    if(tickets.find(pnr) == tickets.end()){
        cout<<"Invalid PNR\n";
        return;
    }
    UserTicket* currTicket = tickets[pnr];
    cout<<currTicket->trainNumber<<" ";
    cout<<currTicket->src<<" ";
    cout<<currTicket->dest<<" ";
    cout<<currTicket->date<<" ";
    cout<<currTicket->totalFare<<" ";
    for(auto seats:currTicket->seatNumbers) cout<<seats<<" ";
    cout<<endl;
    return;
}
void generateReport(unordered_map<int, UserTicket*>& tickets){
    priority_queue<int,vector<int>, greater<int>> pq;
    for(auto i:tickets) pq.push(i.first);
    cout<<"PNR, DATE, TRAIN, FROM, TO, FARE, SEATS"<<endl;
    while(!pq.empty()){
        UserTicket* tkt = tickets[pq.top()];
        pq.pop();
        cout<<tkt->pnrNumber<<", ";
        cout<<tkt->date<<", ";
        cout<<tkt->trainNumber<<", ";
        cout<<tkt->src<<", ";
        cout<<tkt->dest<<", ";
        cout<<tkt->totalFare<<", ";
        for(auto seat:tkt->seatNumbers) cout<<seat<<" ";
        cout<<endl;
    }
    return;
}
int main(){
    // data of all trains
    unordered_map<string,Train*> trains;

    //maping of pnr number with user ticket
    unordered_map<int,UserTicket*> allTicketsBooked;

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
        for(int i=1;i<trainDetails.size();i++){
            pair<string,int> p = addRoute(trainDetails[i]);
            t->routes[p.first] = p.second;
        }

        // for(auto i:t->routes){
        //     cout<<i->station.first<<"--"<<i->station.second<<endl;
        // }

        //coach detail input
        string coachDetailStr;
        getline(cin, coachDetailStr);
        vector<string> coachDetails = split(coachDetailStr);
        
        //adding coaches in the train
        for (int i = 1; i < coachDetails.size(); i++){
            t->coaches.push_back(addCoach(coachDetails[i]));
        }
        trains[t->number] = t; 
    }
    //user request start
    int PNR = 100000001;
    while (true){
        //sample input : Ahmedabad Surat 2023-03-15 1A 2
        string requestStr;
        getline(cin, requestStr);
        vector<string> userRequest = split(requestStr);

        //ticket booking request
        if(userRequest.size() == 5){
            set<string> trainsAvailables = checkBookingRequest(userRequest,trains);
            if(trainsAvailables.size() != 0){
                for(auto i:trainsAvailables){
                    cout<<i<<" ";
                }cout<<endl;
                string trainNumber;
                getline(cin,trainNumber);

                //checking if the inputed train number is valid or not
                if(trainsAvailables.find(trainNumber) == trainsAvailables.end()){
                    cout<<"Invalid train number\n";
                    continue;
                }
                Train* userDemandTrain = trains[trainNumber];
                UserTicket* currTicket = new UserTicket();
                //adding ticket to pnr->ticket map
                allTicketsBooked[PNR] = currTicket;

                currTicket->pnrNumber = PNR++;
                currTicket->trainNumber = trainNumber;
                currTicket->src = userRequest[0];
                currTicket->dest = userRequest[1];
                currTicket->date = userRequest[2];
                currTicket->seatNumbers = bookTicket(userDemandTrain,userRequest);
                currTicket->totalFare = findFare(userDemandTrain,userRequest);

                cout<<currTicket->pnrNumber<<" "<<currTicket->totalFare<<endl;
            }
        }
        //pnr input request
        else if(userRequest.size() == 1 && userRequest[0].size() == 9){
            int inputPNR = stoi(userRequest[0]);
            getDetailsByPNR(inputPNR,allTicketsBooked);
            
        }
        //report generation request
        else if(userRequest.size() == 1 && userRequest[0] == "REPORT"){
            generateReport(allTicketsBooked);
        }
        else{
            cout<<"Invalid Input\n";
        }
    }
    return 0;
}