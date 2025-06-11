#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Graph.h"
#include "bits/stdc++.h"

#define pb push_back
#define pu push
#define mt empty

using namespace std;
typedef long long ll;
typedef double db;
typedef float fl;
typedef pair<int,int> ii;
typedef vector<int> vi;


// Hàm đọc và in các tuyến đường từ file
void printRoutesFromFile(const string& filename)
{
    ifstream inFile(filename); // Mở file
    if(!inFile.is_open())
    {
        cout << "Khong mo duoc file " << filename << "!" << endl;
        return;
    }

    cout << "\nDanh sach cac tuyen duong:\n";
    cout << "+----------------------------------------------------------+\n";
    string line;
    set<pair<string, pair<string, string>>> edgesSet; // Bộ lưu các cạnh để kiểm tra trùng lặp
    while(getline(inFile, line))
    {
        if(line[0] == '#') continue; // Bỏ qua dòng comment
        stringstream ss(line);
        string src, dest, vehicle;
        db distance;
        int time, cost;
        char comma;

        // Đọc dữ liệu từ dòng
        if(getline(ss, src, ',') && getline(ss, dest, ',') && (ss >> distance >> comma >> time >> comma >> cost >> comma) && getline(ss, vehicle))
        {
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
                cout << "Phuong tien khong hop le trong dong: " << line << endl;
                continue;
            }
            string normalizedSrc = mapLocation(src); // Chuẩn hóa điểm xuất phát
            string normalizedDest = mapLocation(dest); // Chuẩn hóa điểm đến
            if(edgesSet.find({normalizedSrc, {normalizedDest, mappedVehicle}}) == edgesSet.end())
            {
                // In thông tin tuyến đường
                cout << "| Diem di -> Diem den: " << src << " -> " << dest << "\n";
                cout << "| Khoang cach: " << fixed << setprecision(1) << distance << " km\n";
                cout << "| Thoi gian: " << time << " phut\n";
                cout << "| Chi phi: " << cost << " VND\n";
                cout << "| Phuong tien: " << mappedVehicle << "\n";
                cout << "| \n"; // Dòng trống giữa các tuyến
                addLocation(src); // Thêm điểm xuất phát
                addLocation(dest); // Thêm điểm đến
                addEdge(src, dest, distance, time, cost, vehicle); // Thêm cạnh
                edgesSet.insert({normalizedSrc, {normalizedDest, mappedVehicle}}); // Lưu cạnh để kiểm tra trùng lặp
            }
        }
        else cout << "Loi dinh dang dong: " << line << endl;
    }
    cout << "+----------------------------------------------------------+\n";
    inFile.close(); // Đóng file
}

// Hàm tải dữ liệu từ file vào đồ thị
void loadDataFromFile(const string& filename)
{
    ifstream inFile(filename); // Mở file
    if(!inFile.is_open())
    {
        cout << "Khong mo duoc file " << filename << " de tai du lieu ban dau! Chuong trinh se chay voi do thi rong." << endl;
        return;
    }

    string line;
    set<pair<string, pair<string, string>>> edgesSet; // Bộ lưu các cạnh để kiểm tra trùng lặp
    while(getline(inFile, line))
    {
        if(line[0] == '#') continue; // Bỏ qua dòng comment
        stringstream ss(line);
        string src, dest, vehicle;
        db distance;
        int time, cost;
        char comma;

        // Đọc dữ liệu từ dòng
        if(getline(ss, src, ',') && getline(ss, dest, ',') && (ss >> distance >> comma >> time >> comma >> cost >> comma) && getline(ss, vehicle))
        {
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
            if(!validVehicle) continue; // Bỏ qua nếu phương tiện không hợp lệ
            string normalizedSrc = mapLocation(src); // Chuẩn hóa điểm xuất phát
            string normalizedDest = mapLocation(dest); // Chuẩn hóa điểm đến
            if(edgesSet.find({normalizedSrc, {normalizedDest, mappedVehicle}}) == edgesSet.end())
            {
                addLocation(src); // Thêm điểm xuất phát
                addLocation(dest); // Thêm điểm đến
                addEdge(src, dest, distance, time, cost, vehicle); // Thêm cạnh
                edgesSet.insert({normalizedSrc, {normalizedDest, mappedVehicle}}); // Lưu cạnh để kiểm tra trùng lặp
            }
        }
    }
    inFile.close(); // Đóng file
}

#endif
