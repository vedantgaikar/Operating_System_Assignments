#include <iostream>
#include <vector>
#include <limits>
using namespace std;

void print_frames(const vector<int> &frames)
{
    for (int f : frames)
    {
        if (f == -1)
            cout << "  ";
        else
            cout << f << " ";
    }
    cout << endl;
}

void fifo(const vector<int> &pages, int frame_cnt)
{
    vector<int> frames(frame_cnt, -1);
    int index = -1, page_hits = 0;

    for (int symbol : pages)
    {
        bool hit = false;
        for (int frame : frames)
        {
            if (frame == symbol)
            {
                hit = true;
                break;
            }
        }

        cout << "Symbol: " << symbol << "  Frame: ";
        if (hit)
        {
            page_hits++;
        }
        else
        {
            index = (index + 1) % frame_cnt;
            frames[index] = symbol;
        }
        print_frames(frames);
    }

    cout << "\nPage hits: " << page_hits;
    cout << "\nPage misses: " << pages.size() - page_hits;
    cout << "\nRatio of Page Hit: " << (float)page_hits / pages.size();
    cout << "\nRatio of Page Miss: " << (float)(pages.size() - page_hits) / pages.size() << endl;
}

void lru(const vector<int> &pages, int frame_cnt)
{
    vector<int> frames(frame_cnt, -1);
    vector<int> recent(frame_cnt, -1); // To store the last used index
    int page_hits = 0;

    for (int i = 0; i < pages.size(); i++)
    {
        int symbol = pages[i];
        bool hit = false;

        for (int j = 0; j < frame_cnt; j++)
        {
            if (frames[j] == symbol)
            {
                hit = true;
                recent[j] = i;
                break;
            }
        }

        cout << "Symbol: " << symbol << "  Frame: ";
        if (hit)
        {
            page_hits++;
        }
        else
        {
            int replace_idx = -1;
            for (int j = 0; j < frame_cnt; j++)
            {
                if (frames[j] == -1)
                {
                    replace_idx = j;
                    break;
                }
            }

            if (replace_idx == -1)
            {
                int lru_index = i;
                for (int j = 0; j < frame_cnt; j++)
                {
                    if (recent[j] < lru_index)
                    {
                        lru_index = recent[j];
                        replace_idx = j;
                    }
                }
            }

            frames[replace_idx] = symbol;
            recent[replace_idx] = i;
        }
        print_frames(frames);
    }

    cout << "\nPage hits: " << page_hits;
    cout << "\nPage misses: " << pages.size() - page_hits;
    cout << "\nRatio of Page Hit: " << (float)page_hits / pages.size();
    cout << "\nRatio of Page Miss: " << (float)(pages.size() - page_hits) / pages.size() << endl;
}

void optimal(const vector<int> &pages, int frame_cnt)
{
    vector<int> frames(frame_cnt, -1);
    int page_hits = 0;

    for (int i = 0; i < pages.size(); i++)
    {
        int symbol = pages[i];
        bool hit = false;

        for (int frame : frames)
        {
            if (frame == symbol)
            {
                hit = true;
                break;
            }
        }

        cout << "Symbol: " << symbol << "  Frame: ";
        if (hit)
        {
            page_hits++;
        }
        else
        {
            int replace_idx = -1;
            int farthest = i + 1;

            for (int j = 0; j < frame_cnt; j++)
            {
                if (frames[j] == -1)
                {
                    replace_idx = j;
                    break;
                }

                int k;
                for (k = i + 1; k < pages.size(); k++)
                {
                    if (pages[k] == frames[j])
                        break;
                }

                if (k == pages.size())
                {
                    replace_idx = j;
                    break;
                }

                if (k > farthest)
                {
                    farthest = k;
                    replace_idx = j;
                }
            }

            frames[replace_idx] = symbol;
        }
        print_frames(frames);
    }

    cout << "\nPage hits: " << page_hits;
    cout << "\nPage misses: " << pages.size() - page_hits;
    cout << "\nRatio of Page Hit: " << (float)page_hits / pages.size();
    cout << "\nRatio of Page Miss: " << (float)(pages.size() - page_hits) / pages.size() << endl;
}

int main()
{
    int n, frame_cnt;
    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter the page reference sequence: ";
    for (int i = 0; i < n; i++)
    {
        cin >> pages[i];
    }

    cout << "Enter the number of frames: ";
    cin >> frame_cnt;

    int choice;
    while (true)
    {
        cout << "\nEnter your choice:\n";
        cout << "1. FIFO\n2. LRU\n3. Optimal\n4. Quit\n--> ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            fifo(pages, frame_cnt);
            break;
        case 2:
            lru(pages, frame_cnt);
            break;
        case 3:
            optimal(pages, frame_cnt);
            break;
        case 4:
            cout << "Quitting program...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
