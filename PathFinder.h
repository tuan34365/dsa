#ifndef PATHFINDER_H
#define PATHFINDER_H

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


struct Path // Cấu trúc lưu trữ một đường đi
{
    vector<string> nodes; // Danh sách các nút trong đường đi
    vector<Edge> edges;   // Danh sách các cạnh trong đường đi
};

// Hàm tìm tất cả đường đi từ start đến end bằng thuật toán BFS
void bfsAllPaths(const string& start, const string& end, vector<Path>& allPaths, const string& vehicle = "all", const string& middle = "")
{
    string normalizedStart = mapLocation(start); // Chuẩn hóa điểm xuất phát
    string normalizedEnd = mapLocation(end); // Chuẩn hóa điểm đến
    string normalizedMiddle = middle.mt() ? "" : mapLocation(middle); // Chuẩn hóa điểm trung gian (nếu có)
    string mappedVehicle = vehicle == "all" ? "all" : mapVehicle(vehicle); // Ánh xạ phương tiện

    // Chọn đồ thị: fullGraph nếu dùng tất cả phương tiện, vehicleGraphs nếu chỉ dùng một phương tiện
    map<string, Node>& graph = (mappedVehicle == "all") ? fullGraph : vehicleGraphs[mappedVehicle];
    if(graph.find(normalizedStart) == graph.end() || graph.find(normalizedEnd) == graph.end()) return; // Nếu không tồn tại điểm xuất phát hoặc điểm đến

    queue<Path> q; // Hàng đợi cho BFS
    Path initialPath;
    initialPath.nodes.pb(normalizedStart); // Bắt đầu từ điểm xuất phát
    q.pu(initialPath);

    while(!q.mt())
    {
        Path path = q.front(); // Lấy đường đi đầu tiên từ hàng đợi
        q.pop();
        string current = path.nodes.back(); // Nút hiện tại là nút cuối cùng trong đường đi

        if(current == normalizedEnd) // Nếu đến được điểm đích
        {
            // Kiểm tra điều kiện điểm trung gian (nếu có)
            if(normalizedMiddle.mt() || find(path.nodes.begin(), path.nodes.end(), normalizedMiddle) != path.nodes.end())
                allPaths.pb(path); // Thêm đường đi vào danh sách
            continue;
        }

        // Duyệt qua các cạnh kề của nút hiện tại
        for(const Edge& edge : graph[current].adjList)
        {
            if(find(path.nodes.begin(), path.nodes.end(), edge.destination) == path.nodes.end()) // Nếu điểm đến của cạnh chưa được thăm
            {
                Path newPath = path; // Tạo đường đi mới
                newPath.nodes.pb(edge.destination); // Thêm điểm đến
                newPath.edges.pb(edge); // Thêm cạnh
                q.pu(newPath); // Thêm vào hàng đợi
            }
        }
    }
}

// Hàm in danh sách tất cả các đường đi từ start đến end
void listAllPaths(const string& start, const string& end, const string& vehicle, const string& middle, const string& criterion)
{
    string normalizedStart = mapLocation(start); // Chuẩn hóa điểm xuất phát
    string normalizedEnd = mapLocation(end); // Chuẩn hóa điểm đến

    // Kiểm tra xem địa điểm có tồn tại không
    if(!isLocationExist(start) || !isLocationExist(end))
    {
        cout << "Dia diem khong ton tai!" << endl;
        return;
    }

    vector<Path> allPaths; // Danh sách các đường đi tìm được
    bfsAllPaths(start, end, allPaths, vehicle, middle); // Tìm tất cả đường đi

    if(allPaths.mt())
    {
        cout << "Khong tim thay duong di nao tu " << normalizedStart << " den " << normalizedEnd << "!" << endl;
        return;
    }

    // Sắp xếp đường đi theo tiêu chí nếu có
    if(!criterion.mt())
    {
        sort(allPaths.begin(), allPaths.end(), [&](const Path& path1, const Path& path2)
        {
            db total1 = 0, total2 = 0;
            for(const Edge& edge : path1.edges)
            {
                if(criterion == "distance") total1 += edge.distance;
                else if(criterion == "time") total1 += edge.time;
                else total1 += edge.cost;
            }
            for(const Edge& edge : path2.edges)
            {
                if(criterion == "distance") total2 += edge.distance;
                else if(criterion == "time") total2 += edge.time;
                else total2 += edge.cost;
            }
            return total1 < total2; // Sắp xếp tăng dần theo tiêu chí
        });
    }

    // In thông tin các đường đi
    cout << "\nCac duong di tu " << normalizedStart << " den " << normalizedEnd;
    if(!vehicle.mt() && vehicle != "all") cout << " (phuong tien: " << vehicle << ")";
    if(!middle.mt()) cout << " qua diem trung gian " << mapLocation(middle);
    if(!criterion.mt()) cout << " (tieu chi: " << criterion << ")";
    cout << ":\n";
    cout << "+----------------------------------------------------------+\n";

    for(size_t i = 0; i < allPaths.size(); ++i)
    {
        const Path& path = allPaths[i];
        cout << "| Duong di " << (i + 1) << ":\n";
        cout << "| ";
        for(size_t j = 0; j < path.nodes.size(); ++j)
        {
            cout << path.nodes[j];
            if(j < path.nodes.size() - 1) cout << " -> ";
        }
        cout << "\n";

        db totalDistance = 0;
        int totalTime = 0;
        int totalCost = 0;

        // Tính tổng các giá trị
        for(const Edge& edge : path.edges)
        {
            totalDistance += edge.distance;
            totalTime += edge.time;
            totalCost += edge.cost;
        }

        // In chi tiết từng đoạn
        cout << "| Chi tiet tung doan:\n";
        for(size_t j = 0; j < path.edges.size(); ++j)
        {
            const Edge& edge = path.edges[j];
            cout << "|   Diem di -> Diem den: " << path.nodes[j] << " -> " << path.nodes[j + 1] << "\n";
            cout << "|   Khoang cach: " << fixed << setprecision(1) << edge.distance << " km\n";
            cout << "|   Thoi gian: " << edge.time << " phut\n";
            cout << "|   Chi phi: " << edge.cost << " VND\n";
            cout << "|   Phuong tien: " << edge.vehicle << "\n";
            if(j < path.edges.size() - 1) cout << "| \n"; // Dòng trống giữa các đoạn
        }

        // In tổng nếu có nhiều hơn 1 đoạn
        if(path.edges.size() > 1)
        {
            cout << "| Tong khoang cach: " << fixed << setprecision(1) << totalDistance << " km\n";
            cout << "| Tong thoi gian: " << totalTime << " phut\n";
            cout << "| Tong chi phi: " << totalCost << " VND\n";
        }

        cout << "+----------------------------------------------------------+\n";
    }
}

#endif
