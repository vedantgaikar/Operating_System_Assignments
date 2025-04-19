#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <map>

using namespace std;

class Process
{
public:
    int p_id, arrivalTime, burstTime, turnaroundTime, responseTime, waitingTime, completionTime, priority;
    Process(int p_id, int arrivalTime, int burstTime, int priority = 0)
    {
        this->p_id = p_id;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        turnaroundTime = waitingTime = completionTime = 0;
        responseTime = -1;
        this->priority = priority;
    }
};

// funct to print the table
void printtable(vector<Process> &pcs, int totalRT, int totalWT, int totalTAT, int n)
{
    // Print process table
    cout << "\nProcess Scheduling Table:\n";
    cout << setw(10) << "PID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time"
         << setw(15) << "Completion" << setw(15) << "Turnaround" << setw(15) << "Waiting"
         << setw(15) << "Response" << setw(15) << "Priority" << endl;
    ;

    for (auto &p : pcs)
    {
        cout << setw(10) << p.p_id
             << setw(15) << p.arrivalTime
             << setw(15) << p.burstTime
             << setw(15) << p.completionTime
             << setw(15) << p.turnaroundTime
             << setw(15) << p.waitingTime
             << setw(15) << p.responseTime
             << setw(15) << p.priority << endl;
    }

    // Print averages
    cout << "\nAverage Turnaround Time: " << totalTAT / n << endl;
    cout << "Average Waiting Time: " << totalWT / n << endl;
    cout << "Average Response Time: " << totalRT / n << endl;
}

// FCFS Scheduling Algorithm
void fcfs(vector<Process> &pcs, int p_no)
{
    sort(pcs.begin(), pcs.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });

    int currentTime = 0;
    map<int, int> completeTime;
    map<int, int> TAT;
    map<int, int> WT;
    map<int, int> responseTime;
    double avgTAT = 0, avgWT = 0, avgRT = 0;

    // Find the complete time, turnaround time, waiting time, and response time
    for (auto it : pcs)
    {
        if (currentTime < it.arrivalTime)
        {
            currentTime = it.arrivalTime;
        }

        // Completion Time
        currentTime += it.burstTime;
        completeTime[it.p_id] = currentTime;

        // Turnaround Time
        TAT[it.p_id] = completeTime[it.p_id] - it.arrivalTime;

        // Waiting Time
        WT[it.p_id] = TAT[it.p_id] - it.burstTime;

        // Response Time (for FCFS, Response Time is the time when the process starts)
        responseTime[it.p_id] = completeTime[it.p_id] - it.burstTime - it.arrivalTime;
        // Calculate averages
        avgTAT += TAT[it.p_id];
        avgWT += WT[it.p_id];
        avgRT += responseTime[it.p_id];
    }
    printtable(pcs, avgRT, avgWT, avgTAT, p_no);
}

struct ComparePriority
{
    bool operator()(const Process *a, const Process *b)
    {
        if (a->priority == b->priority)
            return a->arrivalTime > b->arrivalTime;
        return a->priority > b->priority;
    }
};

// Priority Scheduling Algorithm
void priorityScheduling(vector<Process> &pcs, int n)
{
    int currTime = 0, completedCount = 0;
    double totalWT = 0, totalRT = 0, totalTAT = 0;
    vector<Process> proc;

    priority_queue<Process *, vector<Process *>, ComparePriority> pq;

    // Sort processes by arrival time
    sort(pcs.begin(), pcs.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });

    int i = 0;
    while (completedCount < n)
    {
        while (i < n && pcs[i].arrivalTime <= currTime)
        {
            pq.push(&pcs[i]);
            i++;
        }

        if (pq.empty())
        {

            currTime = pcs[i].arrivalTime;
            continue;
        }
        Process *pc = pq.top();
        pq.pop();
        // cout << "PID: " << pc->p_id << endl;
        if (pc->responseTime == -1)
            pc->responseTime = currTime - pc->arrivalTime;

        currTime += pc->burstTime;
        pc->completionTime = currTime;
        pc->turnaroundTime = pc->completionTime - pc->arrivalTime;
        pc->waitingTime = pc->turnaroundTime - pc->burstTime;

        totalWT += pc->waitingTime;
        totalRT += pc->responseTime;
        totalTAT += pc->turnaroundTime;

        proc.push_back(*pc);
        completedCount++;
    }
    printtable(pcs, totalRT, totalWT, totalTAT, n);
}

struct CompareSjf
{
    bool operator()(const Process *a, const Process *b)
    {
        if (a->burstTime == b->burstTime)
            return a->arrivalTime > b->arrivalTime; // Break ties with arrival time
        return a->burstTime > b->burstTime;
    }
};

// Shortest Job First (SJF) Scheduling Algorithm
void sjf(vector<Process> &pcs, int n)
{
    int currTime = 0, completedCount = 0;
    double totalWT = 0, totalRT = 0, totalTAT = 0;

    priority_queue<Process *, vector<Process *>, CompareSjf> pq;

    // Sort processes by arrival time
    sort(pcs.begin(), pcs.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });

    int i = 0;
    while (completedCount < n)
    {
        // Push all available processes into the priority queue
        while (i < n && pcs[i].arrivalTime <= currTime)
        {
            pq.push(&pcs[i]);
            i++;
        }

        if (pq.empty())
        {
            // If no process is available, move time to next process arrival
            currTime = pcs[i].arrivalTime;
            continue;
        }

        // Pick the shortest job available
        Process *pc = pq.top();
        pq.pop();

        if (pc->responseTime == -1)
            pc->responseTime = currTime - pc->arrivalTime;

        currTime += pc->burstTime;
        pc->completionTime = currTime;
        pc->turnaroundTime = pc->completionTime - pc->arrivalTime;
        pc->waitingTime = pc->turnaroundTime - pc->burstTime;

        totalWT += pc->waitingTime;
        totalRT += pc->responseTime;
        totalTAT += pc->turnaroundTime;

        completedCount++;
    }
    printtable(pcs, totalRT, totalWT, totalTAT, n);
}

// Round Robin Scheduling Algorithm
void rr(vector<Process> &pcs, int p_no, int TQ)
{
    sort(pcs.begin(), pcs.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });

    queue<Process *> readyQ;
    map<int, int> BT;
    vector<int> gnatt;

    for (int i = 0; i < p_no; i++)
    {
        BT[pcs[i].p_id] = pcs[i].burstTime;
    }

    int i = 0;
    int currentTime = 0;

    while (!readyQ.empty() || i < p_no)
    {
        while (i < p_no && pcs[i].arrivalTime <= currentTime)
        {
            readyQ.push(&pcs[i]);
            i++;
        }

        if (readyQ.empty())
        {
            if (i < p_no)
                currentTime = pcs[i].arrivalTime;
            continue;
        }

        Process *p = readyQ.front();
        readyQ.pop();
        gnatt.push_back(p->p_id);

        if (p->responseTime == -1)
        {
            p->responseTime = currentTime - p->arrivalTime;
        }

        if (BT[p->p_id] <= TQ)
        {
            currentTime += BT[p->p_id];
            BT[p->p_id] = 0;
            p->completionTime = currentTime;
            p->turnaroundTime = p->completionTime - p->arrivalTime;
            p->waitingTime = p->turnaroundTime - p->burstTime;
        }
        else
        {
            BT[p->p_id] -= TQ;
            currentTime += TQ;
            while (i < p_no && pcs[i].arrivalTime <= currentTime)
            {
                readyQ.push(&pcs[i]);
                i++;
            }
            if (BT[p->p_id] > 0)
                readyQ.push(p);
        }
    }

    int totalTAT = 0, totalWT = 0, totalRT = 0;
    for (auto &p : pcs)
    {
        totalTAT += p.turnaroundTime;
        totalRT += p.responseTime;
        totalWT += p.waitingTime;
    }

    cout << "Printing the Gantt chart" << endl;
    for (auto it : gnatt)
    {
        cout << "P" << it << "--";
    }
    cout << endl;

    printtable(pcs, totalRT, totalWT, totalTAT, p_no);
}

int main()
{
    int choice;
    do
    {
        cout << "\nCPU Scheduling Algorithms";
        cout << "\n1. First Come First Serve (FCFS)";
        cout << "\n2. Shortest Job First (SJF)";
        cout << "\n3. Priority Scheduling";
        cout << "\n4. Round Robin (RR)";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 5)
            break;

        int p_no;
        cout << "Enter the number of processes: ";
        cin >> p_no;
        vector<Process> pcs;

        for (int i = 0; i < p_no; i++)
        {
            int p_id, arrivalTime, burstTime, priority = 0;
            cout << "Enter Process ID: ";
            cin >> p_id;
            cout << "Enter Arrival Time: ";
            cin >> arrivalTime;
            cout << "Enter Burst Time: ";
            cin >> burstTime;
            if (choice == 3)
            {
                cout << "Enter Priority: ";
                cin >> priority;
            }
            pcs.emplace_back(p_id, arrivalTime, burstTime, priority);
        }

        if (choice == 1)
            fcfs(pcs, p_no);
        else if (choice == 2)
            sjf(pcs, p_no);
        else if (choice == 3)
            priorityScheduling(pcs, p_no);
        else if (choice == 4)
        {
            int TQ;
            cout << "Enter Time Quantum: ";
            cin >> TQ;
            rr(pcs, p_no, TQ);
        }
    } while (choice != 5);

    return 0;
}
