#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
struct Process
{
    int pid;
    int arr_time;
    int burst_time;
    int wait_time;
    int rp_time;
    int turound_time;
    int finish_time;
    int start_time;
    int time_remain;
    int turn_time;
    bool flag;
    Process(int pid, int arr_t, int burst_t)
    {
        this->pid = pid;
        this->arr_time = arr_t;
        this->burst_time = burst_t;
        this->wait_time = 0;
        this->turound_time = 0;
        this->rp_time = 0;
        this->time_remain = burst_t;
        this->start_time = 0;
        this->finish_time = 0;
        this->turn_time = 0;
        this->flag = false;
    }
};

struct arr_priority
{
    bool operator()(const Process &Process1, const Process &Process2)
    {

        return Process1.arr_time > Process2.arr_time;
    }
};

bool compare_pid(struct Process a, struct Process b)
{
    return a.pid < b.pid;
}

int num_process;
void show(vector<Process> P)
{

    printf(" _______________________________________________________________________________________________________________ \n");
    printf("|      PID       | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Response Time |\n");
    printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");

    float avg_waiting_time = 0;
    float avg_turnaround = 0;
    float avg_respone_time = 0;
    for (int i = 0; i < num_process; i++)
    {
        printf("|%-16d|%-14d|%-12d|%-17d|%-17d|%-14d|%-15d|\n", P[i].pid, P[i].arr_time, P[i].burst_time, P[i].finish_time, P[i].turound_time, P[i].wait_time, P[i].rp_time);

        avg_waiting_time += P[i].wait_time;
        avg_turnaround += P[i].turound_time;
        avg_respone_time += P[i].rp_time;
    }

    printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
    avg_waiting_time /= num_process;
    avg_turnaround /= num_process;
    avg_respone_time /= num_process;

    printf("Average waiting time : %.2f\n", avg_waiting_time);
    printf("Average turn around time: %.2f\n", avg_turnaround);
    printf("Average respone time: %.2f\n", avg_respone_time);
}

int main()
{

    queue<Process> queue_process;
    priority_queue<Process, vector<Process>, arr_priority> input_process;
    vector<Process> result_process;
    int quantum_time;
    cout << "Enter quantum time: ";
    cin >> quantum_time;
    cout << "Enter number process: ";
    cin >> num_process;
    int arr_t, burst_t = 0;
    for (int i = 0; i < num_process; i++)
    {

        cout << "Process pid " << i << endl;
        cout << "Enter arrival time: ";
        cin >> arr_t;
        cout << "Enter busrt time: ";
        cin >> burst_t;
        Process token(i, arr_t, burst_t);
        input_process.push(token);
    }

    int remain_process = num_process;
    int time_start = 0;
    while (remain_process != 0 || !queue_process.empty())
    {
        bool exist = true;
        do
        {

            if (!input_process.empty() && input_process.top().arr_time == time_start)
            {
                Process tmp = input_process.top();
                queue_process.push(tmp);
                input_process.pop();
            }
            else
                exist = false;
        } while (exist);
        if (queue_process.empty() && remain_process != 0)
        {
            time_start++;
            continue;
        }
        Process current_p = queue_process.front();
        if (!current_p.flag)
        {
            current_p.start_time = time_start;
            current_p.flag = true;
        }
        if (!current_p.turn_time)
        {
            current_p.turn_time = quantum_time;
        }

        time_start++;
        current_p.time_remain--;
        current_p.turn_time--;
        if (current_p.time_remain > 0)
        {
            if (!current_p.turn_time)
            {
                queue_process.pop();
                if (!input_process.empty() && input_process.top().arr_time == time_start)
                {
                    Process tmp = input_process.top();
                    queue_process.push(tmp);
                    input_process.pop();
                }
                queue_process.push(current_p);
            }
            else
            {
                queue_process.front() = current_p;
                continue;
            }
        }
        else
        {
            current_p.finish_time = time_start;
            current_p.rp_time = current_p.start_time - current_p.arr_time;
            current_p.turound_time = current_p.finish_time - current_p.arr_time;
            current_p.wait_time = current_p.turound_time - current_p.burst_time;
            result_process.push_back(current_p);
            queue_process.pop();
            remain_process--;
        }
    }
    sort(result_process.begin(), result_process.end(), compare_pid);
    show(result_process);
}
