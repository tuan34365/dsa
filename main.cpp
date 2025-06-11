#include "Menu.h"
#include "bits/stdc++.h"

int main()
{
    cout << fixed << setprecision(1);
    initVehicleGraphs();
    loadDataFromFile("data.txt");
    menu();
    return 0;
}
