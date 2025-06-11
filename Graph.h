#ifndef GRAPH_H
#define GRAPH_H

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

struct Edge // Cấu trúc biểu diễn một cạnh trong đồ thị
{
    string destination; // Điểm đến của cạnh
    db distance;       // Khoảng cách của cạnh (tính bằng km)
    int time;          // Thời gian di chuyển của cạnh (tính bằng phút)
    int cost;          // Chi phí của cạnh (tính bằng VND)
    string vehicle;    // Loại phương tiện sử dụng cho cạnh này
};

struct Node // Cấu trúc biểu diễn một nút trong đồ thị
{
    string name;              // Tên của nút (địa điểm)
    vector<Edge> adjList;     // Danh sách kề chứa các cạnh kết nối với nút này
};

struct UserRequest // Cấu trúc lưu trữ yêu cầu của người dùng
{
    string start;           // Địa điểm bắt đầu
    string end;             // Địa điểm kết thúc
    string startTime;       // Thời gian bắt đầu chuyến đi (chưa sử dụng)
    int duration;           // Thời gian mong muốn cho chuyến đi (phút, chưa sử dụng)
    int maxCost;            // Chi phí tối đa cho phép (VND, chưa sử dụng)
    string middleLocation;  // Địa điểm trung gian (nếu có)
    string vehicle;         // Loại phương tiện ưu tiên
    string criterion;       // Tiêu chí tối ưu hóa (distance, time, hoặc cost)
};

// Biến toàn cục
map<string, Node> fullGraph; // Đồ thị chứa tất cả các cạnh, không phân biệt phương tiện
map<string, map<string, Node>> vehicleGraphs; // Đồ thị phân loại theo loại phương tiện
vector<string> vehicles = {"Xe buyt", "Tau hoa", "Oto", "May bay", "Xe may", "Xe dap"}; // Danh sách các loại phương tiện hợp lệ

// Hàm khởi tạo vehicleGraphs cho từng loại phương tiện
void initVehicleGraphs()
{
    for(const string& vehicle : vehicles)
        if(vehicleGraphs.find(vehicle) == vehicleGraphs.end())
            vehicleGraphs[vehicle] = map<string, Node>(); // Tạo đồ thị rỗng cho phương tiện
}

// Hàm chuẩn hóa chuỗi: chuyển thành in hoa và xóa khoảng trắng
string normalizeString(const string& str)
{
    string result = str;
    for(char& c : result) c = toupper(c); // Chuyển tất cả ký tự thành in hoa
    result.erase(remove(result.begin(), result.end(), ' '), result.end()); // Xóa khoảng trắng
    return result;
}

// Hàm ánh xạ tên địa điểm về dạng chuẩn hóa
string mapLocation(const string& location)
{
    return normalizeString(location); // Chuẩn hóa địa điểm
}

// Hàm ánh xạ tên phương tiện về dạng chuẩn trong danh sách vehicles
string mapVehicle(const string& vehicle)
{
    string normalized = normalizeString(vehicle); // Chuẩn hóa phương tiện
    for(const string& v : vehicles) if(normalizeString(v) == normalized) return v; // Tìm phương tiện khớp
    return vehicle; // Trả về phương tiện gốc nếu không ánh xạ được
}

// Hàm kiểm tra xem địa điểm có tồn tại trong đồ thị không
bool isLocationExist(const string& location)
{
    string normalized = mapLocation(location); // Chuẩn hóa địa điểm
    return fullGraph.find(normalized) != fullGraph.end(); // Kiểm tra trong fullGraph
}

// Hàm thêm một địa điểm mới vào đồ thị
void addLocation(const string& location)
{
    string normalized = mapLocation(location); // Chuẩn hóa địa điểm
    if(fullGraph.find(normalized) == fullGraph.end()) // Nếu địa điểm chưa tồn tại
    {
        Node newNode;
        newNode.name = normalized; // Gán tên cho nút
        fullGraph[normalized] = newNode; // Thêm vào fullGraph
        for(const string& vehicle : vehicles)
            vehicleGraphs[vehicle][normalized] = newNode; // Thêm vào vehicleGraphs của mỗi phương tiện
    }
}

// Hàm thêm một cạnh vào đồ thị
void addEdge(const string& src, const string& dest, db distance, int time, int cost, const string& vehicle)
{
    string normalizedSrc = mapLocation(src); // Chuẩn hóa điểm xuất phát
    string normalizedDest = mapLocation(dest); // Chuẩn hóa điểm đến
    string mappedVehicle = mapVehicle(vehicle); // Ánh xạ phương tiện

    // Kiểm tra phương tiện hợp lệ
    bool validVehicle = false;
    for(const string& v : vehicles)
    {
        if(v == mappedVehicle)
        {
            validVehicle = true;
            break;
        }
    }
    if(!validVehicle) return; // Không thêm cạnh nếu phương tiện không hợp lệ

    // Kiểm tra trùng lặp trong fullGraph
    Node& node = fullGraph[normalizedSrc];
    for(const Edge& edge : node.adjList) {
        if(edge.destination == normalizedDest && edge.vehicle == mappedVehicle)
            return; // Cạnh đã tồn tại, không thêm
    }

    // Kiểm tra trùng lặp trong vehicleGraphs
    if(vehicleGraphs.find(mappedVehicle) != vehicleGraphs.end())
    {
        Node& vehicleNode = vehicleGraphs[mappedVehicle][normalizedSrc];
        for(const Edge& edge : vehicleNode.adjList)
        {
            if(edge.destination == normalizedDest && edge.vehicle == mappedVehicle)
                return; // Cạnh đã tồn tại, không thêm
        }
    }

    Edge edge = {normalizedDest, distance, time, cost, mappedVehicle}; // Tạo cạnh mới

    // Thêm cạnh vào fullGraph
    fullGraph[normalizedSrc].adjList.pb(edge);

    // Thêm cạnh vào vehicleGraphs
    if(vehicleGraphs.find(mappedVehicle) != vehicleGraphs.end())
    {
        if(vehicleGraphs[mappedVehicle].find(normalizedSrc) == vehicleGraphs[mappedVehicle].end())
            addLocation(src); // Khởi tạo nút nếu chưa tồn tại
        vehicleGraphs[mappedVehicle][normalizedSrc].adjList.pb(edge);
    }
}


#endif
