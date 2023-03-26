#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void loadFiles();
int page_table[PAGE_TABLE_SIZE];

const int PAGE_SIZE = 4;
const int LOGICAL_MEMORY_SIZE = 16;  // 4 pages of 4 bytes each
const int PHYSICAL_MEMORY_SIZE = 32; // 8 pages of 4 bytes each
const int PAGE_TABLE_SIZE = LOGICAL_MEMORY_SIZE;
string logical_memory[LOGICAL_MEMORY_SIZE][PAGE_SIZE];
string physical_memory[PHYSICAL_MEMORY_SIZE / PAGE_SIZE][PAGE_SIZE];

void loadFiles()
{
    ifstream infile("input.txt");
    string line;
    int page_index = 0;
    int offset = 0;
    while (getline(infile, line))
    {
        if (offset == PAGE_SIZE)
        {
            offset = 0;
            page_index++;
        }
        logical_memory[page_index][offset] = line;
        offset++;
    }
}

void initialize_page_table()
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        page_table[i] = -1; // initialize all entries to -1
    }
}

int get_free_frame()
{
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        bool is_free = true;
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (physical_memory[i][j].empty())
            {
                is_free = false;
                break;
            }
        }
        if (is_free)
        {
            return i;
        }
    }
    return -1; // no free frame available
}

void allocate_page(int logical_address)
{
    int page_index = logical_address / PAGE_SIZE;
    int offset = logical_address % PAGE_SIZE;
    int frame_index = page_table[page_index];
    if (frame_index == -1)
    {
        frame_index = get_free_frame();
        if (frame_index == -1)
        {
            cout << "Error: Physical memory full!" << endl;
            return;
        }
        page_table[page_index] = frame_index;
        for (int i = 0; i < PAGE_SIZE; i++)
        {
            physical_memory[frame_index][i] = logical_memory[page_index][i];
        }
        cout << "Page fault occurred, loaded page " << page_index << " into frame " << frame_index << endl;
    }
    cout << "String: " << logical_memory[page_index][offset] << " allocated to frame " << frame_index << endl;
}

void write_data_to_file()
{
    ofstream outfile("output.txt");
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (!physical_memory[i][j].empty())
            {
                int logical_address = i * PAGE_SIZE + j;
                int frame_index = page_table[logical_address / PAGE_SIZE];
                outfile << "String: " << physical_memory[i][j] << " in frame " << frame_index << " allocated using ";
                if (frame_index == logical_address / PAGE_SIZE)
                {
                    outfile << "optimal paging." << endl;
                }
                else
                {
                    outfile << "FIFO paging." << endl;
                }
            }
        }
    }
    outfile.close();
}

int main()
{
    loadFiles();
    initialize_page_table();
    allocate_page(2);
    allocate_page(7);
    allocate_page(12);
    allocate_page(1);
    write_data_to_file();
    return 0;
}