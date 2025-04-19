#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;
void fifo(const vector<int> &requests, int head)
{
    int seek_count = 0;
    int current_head = head;
    cout << "\nOrder of execution: ";
    for (int request : requests)
    {
        cout << request << " ";
    }
    cout << "\nHead movements:\n";
    for (int request : requests)
    {
        cout << current_head << " -> " << request << " (Seek: " << abs(request - current_head) << ")\n";
        seek_count += abs(request - current_head);
        current_head = request;
    }
    cout << "Total seek time: " << seek_count << "\n";
}
void sstf(vector<int> requests, int head)
{
    int seek_count = 0;
    int current_head = head;
    vector<bool> visited(requests.size(), false);
    cout << "\nOrder of execution: ";
    for (size_t i = 0; i < requests.size(); ++i)
    {
        int min_distance = INT_MAX;
        int index = -1;
        for (size_t j = 0; j < requests.size(); ++j)
        {
            if (!visited[j] && abs(requests[j] - current_head) < min_distance)
            {
                min_distance = abs(requests[j] - current_head);
                index = j;
            }
        }
        visited[index] = true;
        cout << requests[index] << " ";
    }
    // Reset for head movement tracking
    current_head = head;
    fill(visited.begin(), visited.end(), false);
    seek_count = 0;
    cout << "\nHead movements:\n";
    for (size_t i = 0; i < requests.size(); ++i)
    {
        int min_distance = INT_MAX;
        int index = -1;
        for (size_t j = 0; j < requests.size(); ++j)
        {
            if (!visited[j] && abs(requests[j] - current_head) < min_distance)
            {
                min_distance = abs(requests[j] - current_head);
                index = j;
            }
        }
        visited[index] = true;
        cout << current_head << " -> " << requests[index] << " (Seek: " << abs(requests[index] - current_head) << ")\n";
        seek_count += abs(requests[index] - current_head);
        current_head = requests[index];
    }
    cout << "Total seek time: " << seek_count << "\n";
}
void scan(vector<int> requests, int head, int disk_size, int direction)
{
    int seek_count = 0;
    vector<int> left, right, sequence;
    for (int request : requests)
    {
        if (request < head)
            left.push_back(request);
        else
            right.push_back(request);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    if (direction == 0)
        left.insert(left.begin(), 0); // Move towards 0
    else
        right.push_back(disk_size - 1); // Move towards max disk size
    if (direction == 0)
    {
        for (int i = left.size() - 1; i >= 0; --i)
            sequence.push_back(left[i]);
        for (int r : right)
            sequence.push_back(r);
    }
    else
    {
        for (int r : right)
            sequence.push_back(r);
        for (int i = left.size() - 1; i >= 0; --i)
            sequence.push_back(left[i]);
    }
    cout << "\nOrder of execution: ";
    for (int r : sequence)
        cout << r << " ";
    cout << "\nHead movements:\n";
    int current_head = head;
    for (int request : sequence)
    {
        cout << current_head << " -> " << request << " (Seek: " << abs(request - current_head) << ")\n";
        seek_count += abs(request - current_head);
        current_head = request;
    }
    cout << "Total seek time: " << seek_count << "\n";
}
void c_scan(vector<int> requests, int head, int disk_size, int direction)
{
    int seek_count = 0;
    vector<int> left, right, sequence;
    for (int request : requests)
    {
        if (request < head)
            left.push_back(request);
        else
            right.push_back(request);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    if (direction == 0)
    {
        left.insert(left.begin(), 0);
        right.push_back(disk_size - 1);
        for (int i = left.size() - 1; i >= 0; --i)
            sequence.push_back(left[i]);
        sequence.push_back(disk_size - 1); // Jump to other end
        for (int r : right)
            if (r != disk_size - 1) // Avoid duplicate
                sequence.push_back(r);
    }
    else
    {
        right.push_back(disk_size - 1);
        left.insert(left.begin(), 0);
        for (int r : right)
            sequence.push_back(r);
        sequence.push_back(0); // Jump to start
        for (int r : left)
            if (r != 0) // Avoid duplicate
                sequence.push_back(r);
    }
    cout << "\nOrder of execution: ";
    for (int r : sequence)
        cout << r << " ";
    cout << "\nHead movements:\n";
    int current_head = head;
    for (int request : sequence)
    {
        cout << current_head << " -> " << request << " (Seek: " << abs(request - current_head) << ")\n";
        seek_count += abs(request - current_head);
        current_head = request;
    }
    cout << "Total seek time: " << seek_count << "\n";
}
int main()
{
    vector<int> requests;
    int n, head, disk_size;
    cout << "Enter number of disk requests: ";
    cin >> n;
    cout << "Enter disk requests:\n";
    for (int i = 0; i < n; ++i)
    {
        int req;
        cin >> req;
        requests.push_back(req);
    }
    cout << "Enter initial head position: ";
    cin >> head;
    cout << "Enter disk size (e.g., 200): ";
    cin >> disk_size;
    int choice;
    do
    {
        cout << "\nChoose Disk Scheduling Algorithm:\n";
        cout << "1. FIFO\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        int direction;
        switch (choice)
        {
        case 1:
            fifo(requests, head);
            break;
        case 2:
            sstf(requests, head);
            break;
        case 3:
            cout << "Enter head movement direction (0 for Low, 1 for High): ";
            cin >> direction;
            scan(requests, head, disk_size, direction);
            break;
        case 4:
            cout << "Enter head movement direction (0 for Low, 1 for High): ";
            cin >> direction;
            c_scan(requests, head, disk_size, direction);
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
    return 0;
}