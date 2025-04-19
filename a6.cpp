#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

void print_alloc(vector<int> alloc, int p_cnt)
{
    cout << "\nPrinting the Allocation of memory. . . " << endl;
    for (int i = 0; i < p_cnt; i++)
    {
        cout << "P" << i + 1 << " --> B" << alloc[i] + 1 << endl;
    }
}

void first_fit(vector<int> proc, vector<int> block, int p_cnt, int b_cnt)
{
    vector<int> alloc(p_cnt, -1);

    for (int i = 0; i < p_cnt; i++)
    {
        for (int j = 0; j < b_cnt; j++)
        {
            if (block[j] >= proc[i])
            {
                alloc[i] = j;
                block[j] -= proc[i];
                break;
            }
            else
            {
                if (j == b_cnt - 1)
                {
                    cout << "Insufficient memory, process cannot be executed !!!!!" << endl;
                    return;
                }
            }
        }
    }
    print_alloc(alloc, p_cnt);
}

void next_fit(vector<int> proc, vector<int> block, int p_cnt, int b_cnt)
{
    vector<int> alloc(p_cnt, -1);
    int idx = 0;
    for (int i = 0; i < p_cnt; i++)
    {
        int j = idx, count = 0;
        while (count < b_cnt)
        {
            if (block[j] >= proc[i])
            {
                alloc[i] = j;
                block[j] -= proc[i];
                idx = j; // start from here next time
                break;
            }
            j = (j + 1) % b_cnt;
            count++;
        }
        if (alloc[i] == -1)
        {
            cout << "Insufficient memory, process cannot be executed !!!!!" << endl;
            return;
        }
    }
    print_alloc(alloc, p_cnt);
}

void worst_fit(vector<int> proc, vector<int> block, int p_cnt, int b_cnt)
{
    long long max = LLONG_MIN;
    int idx = -1;
    vector<int> alloc(p_cnt, -1);
    for (int i = 0; i < p_cnt; i++)
    {
        max = LLONG_MIN; // Reset for each process
        idx = -1;
        for (int j = 0; j < b_cnt; j++)
        {
            if (block[j] < proc[i])
                continue;
            long long diff = (block[j] - proc[i]);
            if (diff > max)
            {
                max = diff;
                idx = j;
            }
        }
        if (idx == -1)
        {
            cout << "Insufficient memory, process cannot be executed !!!!!" << endl;
            return;
        }
        else
        {
            alloc[i] = idx;
            block[idx] -= proc[i];
        }
    }
    print_alloc(alloc, p_cnt);
}

void best_fit(vector<int> proc, vector<int> block, int p_cnt, int b_cnt)
{
    long long min = LLONG_MAX;
    int idx = -1;
    vector<int> alloc(p_cnt, -1);
    for (int i = 0; i < p_cnt; i++)
    {
        min = LLONG_MAX; // Reset for each process
        idx = -1;
        for (int j = 0; j < b_cnt; j++)
        {
            if (block[j] < proc[i])
                continue;
            long long diff = (block[j] - proc[i]);
            if (diff < min)
            {
                min = diff;
                idx = j;
            }
        }
        if (idx == -1)
        {
            cout << "Insufficient memory, process cannot be executed !!!!!" << endl;
            return;
        }
        else
        {
            alloc[i] = idx;
            block[idx] -= proc[i];
        }
    }
    print_alloc(alloc, p_cnt);
}

int main()
{
    int p_cnt, b_cnt;
    cout << "Enter the number of blocks: ";
    cin >> b_cnt;
    cout << "Enter the number of processes: ";
    cin >> p_cnt;
    vector<int> proc(p_cnt);
    vector<int> block(b_cnt); // ✅ Corrected line

    for (int i = 0; i < b_cnt; i++)
    {
        cout << "Enter the space for block " << i + 1 << ": ";
        cin >> block[i];
    }
    for (int i = 0; i < p_cnt; i++)
    {
        cout << "Enter the space for process " << i + 1 << ": ";
        cin >> proc[i];
    }
    int choice;
    cout << "\n\tMemory allocation techniques";
    while (true)
    {
        cout << "\nEnter the fit you want to perform";
        cout << "\n 1. First Fit \n 2. Next Fit\n 3. Best Fit \n 4. Worst Fit \n 5. QUIT\n--> ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            first_fit(proc, block, p_cnt, b_cnt);
            break;
        case 2:
            next_fit(proc, block, p_cnt, b_cnt);
            break;
        case 3:
            best_fit(proc, block, p_cnt, b_cnt);
            break;
        case 4:
            worst_fit(proc, block, p_cnt, b_cnt);
            break;
        case 5:
            cout << "Quiting program . . .";
            return 0;
        default:
            cout << "INVALID CHOICE \nEnter valid choice!!!" << endl;
            break;
        }
    }

    return 0;
}
