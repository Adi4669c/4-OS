#include <stdio.h>

struct Process {
    int id, at, bt, ct, tat, wt, rt, remaining;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Arrival Time of P%d: ", p[i].id);
        scanf("%d", &p[i].at);
        printf("Burst Time of P%d: ", p[i].id);
        scanf("%d", &p[i].bt);
        p[i].remaining = p[i].bt;
        p[i].rt = -1; // response time marker
    }

    int completed = 0, time = 0;
    int sumTAT = 0, sumWT = 0, sumRT = 0;

    while (completed != n) {
        int idx = -1;
        int minRemaining = 1e9;

        // Find process with shortest remaining time among arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining > 0) {
                if (p[i].remaining < minRemaining) {
                    minRemaining = p[i].remaining;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // First time this process is scheduled → record response time
            if (p[idx].rt == -1) {
                p[idx].rt = time - p[idx].at;
                sumRT += p[idx].rt;
            }

            p[idx].remaining--;
            time++;

            if (p[idx].remaining == 0) {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;

                sumTAT += p[idx].tat;
                sumWT += p[idx].wt;
                completed++;
            }
        } else {
            time++; // If no process has arrived yet, move time forward
        }
    }

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct,
               p[i].tat, p[i].wt, p[i].rt);
    }

    printf("\nAverage TAT: %.2f\n", (float)sumTAT / n);
    printf("Average WT: %.2f\n", (float)sumWT / n);
    printf("Average RT: %.2f\n", (float)sumRT / n);

    return 0;
}
