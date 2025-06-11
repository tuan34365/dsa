#include "Menu.h"
#include "bits/stdc++.h"

int main()
{
    cout << fixed << setprecision(1);
    initVehicleGraphs();
    loadDataFromFile("routes.txt");
    menu();
    return 0;
}
