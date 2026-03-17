#include <stdio.h>

struct Process {
    int pid;        
    int bt; 
    int at;    
    int pr;       
    int ct;    
    int tat;     
    int wt;    
    int rt;    
    int finished;
};

struct Gantt {
    int pid;
    int start;
    int end;
};


void print_gantt_chart(struct Gantt g[], int count) {
    printf("\n--- Gantt Chart ---\n");


    printf(" ");
    for (int i = 0; i < count; i++) {
        for (int j = g[i].start; j < g[i].end; j++) printf("--");
        printf(" ");
    }
    printf("\n|");


    for (int i = 0; i < count; i++) {
        int len = g[i].end - g[i].start;
        for (int j = 0; j < len - 1; j++) printf(" ");
        printf("P%d", g[i].pid);
        for (int j = 0; j < len - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");


    for (int i = 0; i < count; i++) {
        for (int j = g[i].start; j < g[i].end; j++) printf("--");
        printf(" ");
    }
    printf("\n");


    printf("%d", g[0].start);
    for (int i = 0; i < count; i++) {
        int len = g[i].end - g[i].start;
        for (int j = 0; j < len; j++) printf("  ");
        printf("%d", g[i].end);
    }
    printf("\n");
}

void priority_non_preemptive(struct Process p[], int n) {
    int time = 0, completed = 0;
    struct Gantt g[100];
    int gcount = 0;

    while (completed < n) {
        int idx = -1;
        int highest = 9999;
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].at <= time) {
                if (p[i].pr < highest) {
                    highest = p[i].pr;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        g[gcount].pid = p[idx].pid;
        g[gcount].start = time;
        time += p[idx].bt;
        g[gcount].end = time;
        gcount++;

        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].finished = 1;
        completed++;
    }

    printf("\n--- Non-preemptive Priority Scheduling ---\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);
    }

    print_gantt_chart(g, gcount);
}

void priority_preemptive(struct Process p[], int n) {
    int time = 0, completed = 0;
    struct Gantt g[100];
    int gcount = 0;
    int current = -1;

    while (completed < n) {
        int idx = -1;
        int highest = 9999;
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].at <= time) {
                if (p[i].pr < highest) {
                    highest = p[i].pr;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }

        if (current != idx) {
            if (current != -1) {
                g[gcount].end = time;
                gcount++;
            }
            g[gcount].pid = p[idx].pid;
            g[gcount].start = time;
            current = idx;
        }

        p[idx].rt--;
        time++;

        if (p[idx].rt == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].finished = 1;
            completed++;

            g[gcount].end = time;
            gcount++;
            current = -1;
        }
    }

    printf("\n--- Preemptive Priority Scheduling ---\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);
    }

    print_gantt_chart(g, gcount);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter AT, BT, Priority for P%d: ", i + 1);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
        p[i].rt = p[i].bt;
        p[i].finished = 0;
    }

    struct Process p1[n], p2[n];
    for (int i = 0; i < n; i++) {
        p1[i] = p[i];
        p2[i] = p[i];
    }

    priority_non_preemptive(p1, n);
    priority_preemptive(p2, n);

    return 0;
}
