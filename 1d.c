#include <stdio.h>

struct Process {
    int pid, at, bt, ct, tat, wt, rt;
    int remaining, started;
};

int main() {

    int n, tq;
    struct Process p[20];

    printf("Enter number of processes: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        p[i].pid = i+1;
        printf("Enter AT and BT for P%d: ",p[i].pid);
        scanf("%d%d",&p[i].at,&p[i].bt);

        p[i].remaining = p[i].bt;
        p[i].started = 0;
    }

    printf("Enter Time Quantum: ");
    scanf("%d",&tq);

    int time = 0, completed = 0;

    int queue[100], front = 0, rear = 0;
    int visited[20] = {0};

    int gantt_pid[100], gantt_time[100];
    int g = 0;
    gantt_time[g] = 0;

    float total_ct=0, total_tat=0, total_wt=0, total_rt=0;

    for(int i=0;i<n;i++){
        if(p[i].at == 0){
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while(completed < n){

        if(front == rear){
            time++;
            for(int i=0;i<n;i++){
                if(p[i].at <= time && !visited[i]){
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int i = queue[front++];

        if(p[i].started == 0){
            p[i].rt = time - p[i].at;
            total_rt += p[i].rt;
            p[i].started = 1;
        }

        gantt_pid[g] = p[i].pid;

        if(p[i].remaining > tq){
            time += tq;
            p[i].remaining -= tq;
        } else {
            time += p[i].remaining;
            p[i].remaining = 0;

            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;

            total_ct += p[i].ct;
            total_tat += p[i].tat;
            total_wt += p[i].wt;

            completed++;
        }

        g++;
        gantt_time[g] = time;

        for(int j=0;j<n;j++){
            if(p[j].at <= time && !visited[j]){
                queue[rear++] = j;
                visited[j] = 1;
            }
        }

        if(p[i].remaining > 0){
            queue[rear++] = i;
        }
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
        p[i].pid,p[i].at,p[i].bt,
        p[i].ct,p[i].tat,p[i].wt,p[i].rt);
    }

    printf("\nAverage CT = %.2f",total_ct/n);
    printf("\nAverage TAT = %.2f",total_tat/n);
    printf("\nAverage WT = %.2f",total_wt/n);
    printf("\nAverage RT = %.2f\n",total_rt/n);

    printf("\nGantt Chart:\n");

    for(int i=0;i<g;i++) printf("-------");
    printf("-\n|");

    for(int i=0;i<g;i++) printf(" P%d |",gantt_pid[i]);

    printf("\n");

    for(int i=0;i<g;i++) printf("-------");
    printf("-\n");

    for(int i=0;i<=g;i++) printf("%d\t",gantt_time[i]);

    printf("\n");

    return 0;
}
