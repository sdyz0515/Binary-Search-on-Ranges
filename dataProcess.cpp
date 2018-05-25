#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct entry {
    uint32_t start;
    uint32_t end;
    string nextHop;
};

vector<entry> rangesTable;

uint32_t strToInt(const string& str){
    // convert an IP string to int

    uint32_t res = 0;
    uint32_t shifter = 1 << 24;
    stringstream ss(str);
    string number;
    while(getline(ss, number, '.')){
        res += stoi(number) * shifter;
        shifter >>= 8;
    }
    return res;
}

pair<uint32_t, uint32_t> parsePrefix(const string& str){
    // return the start and end points of a prefix

    uint32_t ip, mask;
    uint32_t start, end;
    pair<uint32_t, uint32_t> p;
    int shifter;

    bool hasMask = false;
    for (int i=0; i < str.length(); i++){
        if (str[i] == '/'){
            hasMask = true;
            ip = strToInt(str.substr(0,i));
            shifter = 32 - stoi(str.substr(i+1));
            mask = UINT32_MAX >> shifter << shifter;
        }
    }
    if (!hasMask){
        ip = strToInt(str);
        mask = UINT32_MAX;
    }
    start = ip & mask;
    end = ip | (~mask);
    p = make_pair(start, end);
    return p;
}

void parseLine(const string& line){
    // add one entry to rangesTable

    if (line[3]==' '){
        return;
    }

    string prefixStr, nextHopStr;
    stringstream ss(line.substr(3));
    ss >> prefixStr >> nextHopStr;
    pair<uint32_t,uint32_t> p = parsePrefix(prefixStr);

    entry e;
    e.start = p.first;
    e.end = p.second;
    e.nextHop = nextHopStr;
    rangesTable.push_back(e);
}

int main(int argc, const char *argv[]) {
    ifstream inputFile ("bgptable.txt");
    string line;
    while(getline(inputFile, line)){
        parseLine(line);
    }
    /*
    for(int i = 0; i < 10; i++){
        cout << rangesTable[i].start << " ";
        cout << rangesTable[i].end << " ";
        cout << rangesTable[i].nextHop << endl;
    }
    */
    return 0;
}