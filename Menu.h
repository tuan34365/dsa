#ifndef MENU_H
#define MENU_H

#include "Graph.h"
#include "PathFinder.h"
#include "FileHandler.h"
#include "bits/stdc++.h"

#define pb push_back
#define pu push
#define mt empty
#define task "data"

using namespace std;
typedef long long ll;
typedef double db;
typedef float fl;
typedef pair<int,int> ii;
typedef vector<int> vi;

// Hàm hiển thị menu và xử lý lựa chọn người dùng
void menu()
{
    while(true)
    {
        // Hiển thị menu
        cout << "\n=== Menu Lap Ke Hoach Du Lich ===\n";
        cout << "1. Doc cac tuyen duong tu file\n";
        cout << "2. Liet ke tat ca cac duong di\n";
        cout << "3. Liet ke tat ca cac duong di va sap xep theo tieu chi\n";
        cout << "4. Liet ke tat ca cac duong di qua dia diem trung gian\n";
        cout << "5. Liet ke tat ca cac duong di bang mot phuong tien\n";
        cout << "6. Thoat\n";
        cout << "Chon: ";

        int choice;
        if(!(cin >> choice)) // Kiểm tra đầu vào hợp lệ
        {
            cout << "Lua chon khong hop le! Vui long nhap so tu 1 den 6." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if(choice == 6) break; // Thoát chương trình

        string start, end, vehicle, middle, criterion;

        switch(choice)
        {
            case 1: // Đọc và in tuyến đường từ file
            {
                printRoutesFromFile(task".txt");
                break;
            }
            case 2: // Liệt kê tất cả các đường đi
            {
                cout << "Nhap diem bat dau: ";
                cin.ignore();
                getline(cin, start);
                cout << "Nhap diem ket thuc: ";
                getline(cin, end);
                listAllPaths(start, end, "all", "", "");
                break;
            }
            case 3: // Liệt kê đường đi và sắp xếp theo tiêu chí
            {
                cout << "Nhap diem bat dau: ";
                cin.ignore();
                getline(cin, start);
                cout << "Nhap dien ket thuc: ";
                getline(cin, end);
                cout << "Chon tieu chi (distance/time/cost): ";
                cin >> criterion;
                if(criterion != "distance" && criterion != "time" && criterion != "cost")
                {
                    cout << "Tieu chi khong hop le! Phai la distance, time hoac cost." << endl;
                    break;
                }
                listAllPaths(start, end, "all", "", criterion);
                break;
            }
            case 4: // Liệt kê đường đi qua điểm trung gian
            {
                cout << "Nhap diem bat dau: ";
                cin.ignore();
                getline(cin, start);
                cout << "Nhap diem ket thuc: ";
                getline(cin, end);
                cout << "Nhap dia diem trung gian: ";
                getline(cin, middle);
                if(!isLocationExist(middle))
                {
                    cout << "Dia diem trung gian khong ton tai!" << endl;
                    break;
                }
                listAllPaths(start, end, "all", middle, "");
                break;
            }
            case 5: // Liệt kê đường đi bằng một phương tiện cụ thể
            {
                cout << "Nhap diem bat dau: ";
                cin.ignore();
                getline(cin, start);
                cout << "Nhap diem ket thuc: ";
                getline(cin, end);
                cout << "Nhap phuong tien: ";
                getline(cin, vehicle);
                bool validVehicle = false;
                string mappedVehicle = mapVehicle(vehicle); // Ánh xạ phương tiện
                for(const string& v : vehicles)
                {
                    if(v == mappedVehicle)
                    {
                        validVehicle = true;
                        break;
                    }
                }
                if(!validVehicle)
                {
                    cout << "Phuong tien khong hop le!" << endl;
                    break;
                }
                if(!isLocationExist(start) || !isLocationExist(end))
                {
                    cout << "Mot hoac ca hai dia diem khong ton tai!" << endl;
                    break;
                }
                listAllPaths(start, end, mappedVehicle, "", "");
                break;
            }
            default: // Xử lý lựa chọn không hợp lệ
            {
                cout << "Lua chon khong hop le! Vui long nhap so tu 1 den 6." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
    }
}

#endif
