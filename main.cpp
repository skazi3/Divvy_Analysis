/*main.cpp*/
//
// Divvy analysis: station and ride analysis.
//
// Sarah Kazi
// <<Visual Studio>>
// U. of Illinois, Chicago
// CS341, Spring 2017
// Project #02
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>

using namespace std;

class Station
{
private:
    int    ID;
    string Name;
    double Lat;
    double Long;
    int    Capacity;
    double distance;
    int totalRides;
    
public:
    
    Station(int id, string name, double lat, double longitude, int cap)
    :ID(id), Name(name), Lat(lat), Long(longitude), Capacity(cap)
    {
        totalRides = 0;
    }
    
    int getID() const{
        return ID;
    }
    string getName() const{
        return Name;
    }
    double getLat() const{
        return Lat;
    }
    double getLong() const{
        return Long;
    }
    int getCapacity() const{
        return Capacity;
    }
    
    void setDistance(double dist) {
        distance = dist;
    }
    void increaseTotalRides(){
        totalRides++;
        
    }
    int getTotalRides()  const{
        return totalRides;
    }
    double getDistance() const {
        return distance;
    }
    
    
};

class Rider
{
private:
    int    TripID;
    int    StartT;
    string StopT;
    int    BikeID;
    int    Duration;
    int    FromStationID;
    string FromStationName;
    int    ToStationID;
    string ToStationName;
    
    
public:
    
    Rider(int id, int start, string stop, int bikeID, int time, int fSID, string fSN, int tSID, string tSN)
    :TripID(id), StartT(start), StopT(stop), BikeID(bikeID), Duration(time), FromStationID(fSID), FromStationName(fSN), ToStationID(tSID), ToStationName(tSN)
    {}
    
    int getID() const{
        return TripID;
    }
    int getBikeID() const{
        return BikeID;
    }
    int getStartTime()  const{
        return StartT;
    }
    string getStopTime()   const {
        return StopT;
    }
    int getDuration()      const {
        return Duration;
    }
    int getFromStationID() const {
        return FromStationID;
        
    }
    int getToStationID()   const {
        return ToStationID;
        
    }
    string getFromStationName() const {
        return FromStationName;
        
    }
    string getToStationName() const {
        return ToStationName;
        
    }
    
};
//
// DistBetween2Points: returns the distance in miles between 2 points
// (lat1, long1) and (lat2, long2).
//
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
    //
    // Reference: http://www8.nau.edu/cvm/latlon_formula.html
    //
    constexpr double PI = 3.14159265;
    constexpr double earth_rad = 3963.1;  // statue miles:
    
    double lat1_rad = lat1 * PI / 180.0;
    double long1_rad = long1 * PI / 180.0;
    double lat2_rad = lat2 * PI / 180.0;
    double long2_rad = long2 * PI / 180.0;
    
    double dist = earth_rad * acos(
                                   (cos(lat1_rad)*cos(long1_rad)*cos(lat2_rad)*cos(long2_rad))
                                   +
                                   (cos(lat1_rad)*sin(long1_rad)*cos(lat2_rad)*sin(long2_rad))
                                   +
                                   (sin(lat1_rad)*sin(lat2_rad))
                                   );
    
    return dist;
}
//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
string getFileName()
{
    string filename;
    
    // input filename from the keyboard:
    getline(cin, filename);
    
    // make sure filename exists and can be opened:
    ifstream file(filename);
    if (!file.good())
    {
        cout << "**Error: cannot open file: " << filename << endl;
        std::terminate();
    }
    
    return filename;
}


vector<Station> InputStations(string filename)
{
    cout << std::fixed;
    cout << std::setprecision(4);
    ifstream file(filename);
    vector<Station> stations;
    string line;
    
    if (!file.good())
    {
        cout << "**Error: cannot open file: " << filename << endl;
        std::terminate();
    }
    
    getline(file, line);  // discard first line (column headers):
    
    while(getline(file, line)){
        stringstream ss(line);
        
        string id, name, latitude, longitude, cap;
        
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, latitude, ',');
        getline(ss, longitude, ',');
        getline(ss, cap, ',');
        getline(ss, line);
        
        Station S(stoi(id), name, stod(latitude), stod(longitude), stoi(cap));
        
        stations.push_back(S);
        
    }
    
    
    
    return stations;
    
}
vector<Rider> inputRiders(string filename) {
    
    
    ifstream file(filename);
    vector<Rider> riders;
    string line;
    
    if (!file.good())
    {
        cout << "**Error: cannot open file: " << filename << endl;
        std::terminate();
    }
    
    getline(file, line);  // discard first line (column headers):
    
    while(getline(file, line)){
        stringstream ss(line);
        
        string tripID, start, stop, bikeID, time, fromStationID, toStationID, fromStationName, toStationName;
        
        getline(ss, tripID, ',');
        getline(ss, start, ',');
        
        auto startTime = stoi(start.substr(start.find(" ") + 1));
        
        getline(ss, stop, ',');
        getline(ss, bikeID, ',');
        getline(ss, time, ',');
        getline(ss, fromStationID, ',');
        getline(ss, fromStationName, ',');
        getline(ss, toStationID, ',');
        getline(ss, toStationName, ',');
        //discard rest if the line
        getline(ss, line);
        
        
        Rider r(stoi(tripID), startTime, stop, stoi(bikeID), stod(time), stoi(fromStationID), fromStationName, stoi(toStationID), toStationName);
        riders.push_back(r);
        
    }
    
    
    
    return riders;
    
}
//return a vector of all stations less than distance input by user
vector<Station> findDistance(vector<Station>& S, double latitude, double longitude, double dist){
    
    vector<Station> V;
    auto iter = S.begin();
    while(iter != S.end()) {
        //find all stations within distance input
        iter = std::find_if(iter, S.end(), [=] (const Station& x){
            double stationDist =distBetween2Points( latitude, longitude, x.getLat(), x.getLong());
            if(stationDist <= dist){
                return true;
            }
            else
                return false;
            
        }
                            );
        
        if(iter != S.end()){
            //push back onto vector after setting distance
            Station tmp = *iter;
            double stationDist = distBetween2Points(latitude, longitude, tmp.getLat(), tmp.getLong());
            tmp.setDistance(stationDist);
            V.push_back(tmp);
            iter++;
        }
    }//end of while
    
    //sort the vector before sending it back
    std::sort(V.begin(), V.end(), [](Station s1, Station s2){
        if(s1.getDistance() < s2.getDistance())
            return true;
        else
            return false;
    }
              );
    
    return V;
}

//function to display station info
void printStationInfo(vector<Station>& S, vector<Rider>& R, int id){
    //count for tacking how many to/from
    int count = 0;
    
    //insert into map
    map<int, Station> stationMap;
    for(auto& x : S){
        auto key = pair<int, Station>(x.getID(), x);
        stationMap.insert(key);
    }
    
    //find id from the map
    auto lookup = stationMap.find(id);
    if(lookup == stationMap.end()){
        cout << "**Not found... " << endl;
    }
    else {
        //variables for time slots
        int countMorning = 0;
        int countEvening = 0;
        int countNight   = 0;
        
        auto key = lookup->first;
        auto s = lookup->second;
        for(const Rider& r: R){
            //convert time to int and calculate what time slot it belongs to
            if(key == r.getFromStationID()){
                if(r.getStartTime() >= 6 && r.getStartTime() < 9)
                    countMorning++;
                else if(r.getStartTime() >= 12 && r.getStartTime() < 13)
                    countEvening++;
                else if(r.getStartTime() >= 15 && r.getStartTime() < 19)
                    countNight++;
                
            }//end of if
            //calculate to/from times
            if(key == r.getToStationID() && key == r.getFromStationID())
                count+= 2;
            else if(key == r.getToStationID() || key == r.getFromStationID())
                count++;
            
        } //end for
        //print out info
        cout << "Name:     '" << s.getName() << "'" << endl;
        cout << "Position: (" << s.getLat() << ", " << s.getLong() << ")" << endl;
        cout << "Capacity:  " << s.getCapacity()<< endl;
        cout << "Total Rides to/from: " << count << endl;
        
        cout << "6-9 am   : " << countMorning << endl;
        cout << "Noon- 1pm: " << countEvening << endl;
        cout << "3-7 pm   : " << countNight   << endl;
    }
    
}

//find top N
//TODO//

void findTopN(vector<Station>& S, vector<Rider>& R, int N){
    
    for(auto& r: R){
        for(auto& s: S){
            if(r.getFromStationID() == s.getID())
                s.increaseTotalRides();
            if(r.getToStationID() == s.getID())
                s.increaseTotalRides();
            
        }
    }
    sort(S.begin(), S.end(), [](Station s1, Station s2){
        if(s1.getTotalRides() > s2.getTotalRides())
            return true;
        else
            return false;
    }
         );
    
    if(N > R.size())
        N = R.size()-1;
    for(int i = 0; i < N; i++){
        cout <<i+1 << ". Station " << S.at(i).getID() << ": "<< S.at(i).getTotalRides() << " rides @ '" << S.at(i).getName() << "'" << endl;
    }
    
    
}

void findHourly(vector<Rider> R) {
    
    
    
}


int main()
{
    // setup output format:
    cout << std::fixed;
    cout << std::setprecision(4);
    
    // get filenames from the user/stdin: *CHANGE LATER*
    string StationsFileName = "stations.csv";
    string RidesFilename = "rides.csv";
    
    
    vector<Station> stations = InputStations(StationsFileName);
    vector<Rider> riders = inputRiders(RidesFilename);
    //create a map of the stations
    map<int, Station> stationMap;
    for(auto& x : stations){
        auto key = pair<int, Station>(x.getID(), x);
        stationMap.insert(key);
    }
    
    
    cout << "**Num stations: " << stations.size() << endl;
    cout << "**Num riders: " << riders.size() << endl;
    
    //
    // allow user to enter 0 or more commands, stop when
    // the user inputs the command "exit":
    //
    string command, restOfLine;
    
    cin >> command;
    
    while (command != "exit")
    {
        if (command == "find")
        {
            //
            // find latitude longitude distance
            //
            // Example: find 41.86 -87.62 0.5
            //
            double latitude, longitude, distance;
            
            cin >> latitude;
            cin >> longitude;
            cin >> distance;
            
            cout << "find " << latitude << " " << longitude << " " << distance << endl;
            vector<Station> findStations = findDistance(stations, latitude, longitude, distance);
            
            
            //loop through the vector with distances less than user input
            for(const Station& s : findStations)
            {
                cout << "Station " << s.getID() << ": " << s.getDistance() << endl;
            }
            
            
        }
        else if (command == "info")
        {
            //
            // info stationID:
            //
            // Example: info 338
            //
            int stationID;
            
            cin >> stationID;
            
            printStationInfo(stations, riders, stationID);
            
        }
        else if (command == "top")
        {
            //
            // top N:
            //
            // Example: top 10
            //
            int N;
            
            cin >> N;
            
            if (N < 1)  // invalid input?  Default to top 10:
                N = 10;
            
            findTopN(stations, riders, N);
            
            
        }
        else if (command == "hourly")
        {
            
            findHourly(riders);
            
            
        }
        else
        {
            cout << "**Invalid, try again" << endl;
        }
        
        getline(cin, restOfLine);  // discard rest of line:
        
        //
        // input next command and repeat:
        cin >> command;
    }
    
    //
    // done:
    //
    return 0;
}
