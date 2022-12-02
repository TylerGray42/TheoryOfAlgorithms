#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


// размер рюкзака
// название вес стоимость


typedef struct {
    string name;
    int weight, cost;
    double value;
} Things;



int main() {
    
    ifstream f("things.txt");
    if (!f.is_open()) return 0;

    vector<Things> things;

    int weightOfPack, w, c, allCost = 0;
    string n;
    f >> weightOfPack;

    while(f >> n >> w >> c) {
        things.push_back({n, w, c, (double)c/w});
    }

    f.close();

    sort(things.begin(), things.end(), [](const Things& a, const Things& b) {return a.value > b.value;});

    for (auto i : things) {
        if (i.weight <= weightOfPack) {
            cout << i.name << endl;
            weightOfPack -= i.weight;
            allCost += i.cost;
        } 
    }

    cout << endl << allCost << endl;

    return 0;
}