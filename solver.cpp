/* 
 * 2021 Decision Making B - Exercise problem 03  
 * solver.cpp
 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define rep(i, n) for (int i = 0; i < (int)(n); i++) // loop: 0 to n-1

//rate[1's choice][2's choice][i's] = rating
string dom_strategy(const int r[2][2][2]) {
    string res = "1's DS: ";
    int i1_x = r[0][0][0] - r[1][0][0], i1_y = r[0][1][0] - r[1][1][0];
    if (i1_x >= 0 && i1_y >= 0) res += "a1";
    else if (i1_x <= 0 && i1_y <= 0) res += "b1";
    else res += "None";

    res += "\n2's DS: ";
    int i2_x = r[0][0][1] - r[0][1][1], i2_y = r[1][0][1] - r[1][1][1];
    if (i2_x >= 0 && i2_y >= 0) res += "a2";
    else if (i2_x <= 0 && i2_y <= 0) res += "b2";
    else res += "None";

    return res;
}

string dom_strategy_eql(const int r[2][2][2]) {
    string res = "DSE: ", ds = dom_strategy(r);
    if (ds.find("None") != -1) res += "None";
    else res += "(" + ds.substr(8, 2) + ", " + ds.substr(19, 2) + ")";
    return res;
}

string nash_eql(const int r[2][2][2]) {
    string res = "";
    rep(ch0, 2) {
        rep(ch1, 2) {
            if (r[ch0][ch1][0] >= r[1 - ch0][ch1][0] && r[ch0][ch1][1] >= r[ch0][1 - ch1][1]) {
                if (res != "") res += ", ";
                string c1 = (!ch0 ? "a" : "b"), c2 = (!ch1 ? "a" : "b");
                res += "(" + c1 + "1, " + c2 + "2)";
            }
        }
    }
    if (res == "") res = "None";
    res = "NE:  " + res;
    return res;
}

typedef std::pair<int, int> pr;

string pareto_op(const int r[2][2][2]) {
    string res = "";
    std::vector<pr> p;
    p.push_back(pr(r[0][0][0], r[0][0][1])); //(a1,a2) -> p[0]
    p.push_back(pr(r[0][1][0], r[0][1][1])); //(a1,b2) -> p[1]
    p.push_back(pr(r[1][0][0], r[1][0][1])); //(b1,a2) -> p[2]
    p.push_back(pr(r[1][1][0], r[1][1][1])); //(b1,b2) -> p[3]
    rep(i, 4) {
        bool pareto = true;
        rep(j, 4) {
            if (p[i].first < p[j].first && p[i].second < p[j].second)
                pareto = false;
        }
        if (pareto){
            if (res != "") res += ", ";
            string c1 = (i > 1 ? "b" : "a"), c2 = (i % 2 ? "b" : "a");
            res += "(" + c1 + "1, " + c2 + "2)";
        }
    }
    if (res == "") res = "None";
    res = "PO:  " + res;
    return res;
}


int main(void) {
    ifstream ifs("./ex03question.dat"); // question (01-78)
    ofstream ofs("./ex03ans.dat"); // output answer
    string buf;

    while (getline(ifs, buf)) {
        cout << buf << endl;
        ofs << buf << endl;
        ofs << "(1,2)  a2  b2" << endl;
        int rate[2][2][2] = {}; //rate[1's choice][2's choice][i's] = rating
                                // a1,b1,i=1 -> 0, a2,b2,i=2 -> 1 (0-indexed)
        rep(i, 2) {
            getline(ifs, buf);
            ofs << (!i ? "  a1  " : "  b1  ");
            ofs << buf << endl;
            (void)sscanf(buf.c_str(), "%d,%d %d,%d", \
                &rate[i][0][0], &rate[i][0][1], &rate[i][1][0], &rate[i][1][1]);
        }
        
        ofs << dom_strategy(rate) << endl;
        ofs << dom_strategy_eql(rate) << endl;
        ofs << nash_eql(rate) << endl;
        ofs << pareto_op(rate) << endl << endl;

        getline(ifs, buf); // empty line
    }
    cout << "Done." << endl;
    return 0;
}