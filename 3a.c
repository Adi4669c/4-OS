#include <stdio.h>
#include <math.h>

#define N 3
#define MAX_TIME 100

struct Process {
    int id;
    int bt;
    int period;
    int deadline;

    int remaining;
    int next_arrival;
    int abs_deadline;

    int ct, wt, tat;
};

int gcd(int a, int b) {
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

int find_hyperperiod(struct Process p[]) {
    int h = p[0].period;
    for (int i = 1; i < N; i++)
        h = lcm(h, p[i].period);
    return h;
}

void check_schedulability(struct Process p[]) {
    float U = 0;
    for (int i = 0; i < N; i++)
        U += (float)p[i].bt / p[i].period;

    printf("\nCPU Utilization = %.3f\n", U);

    float rm_bound = N * (pow(2, (float)1/N) - 1);

    printf("RM Bound = %.3f\n", rm_bound);

    if (U <= 1)
        printf("EDF: Schedulable\n");
    else
        printf("EDF: Not Schedulable\n");

    if (U <= rm_bound)
        printf("RM: Schedulable\n");
    else
        printf("RM: Not Schedulable\n");
}

void reset(struct Process p[], struct Process orig[]) {
    for (int i = 0; i < N; i++) {
        p[i] = orig[i];
        p[i].remaining = 0;
        p[i].next_arrival = 0;
        p[i].abs_deadline = p[i].deadline;
        p[i].ct = p[i].wt = p[i].tat = 0;
    }
}

void EDF(struct Process p[], int hyper) {
    printf("\n===== EDF Scheduling =====\n");

    for (int t = 0; t < hyper; t++) {

        for (int i = 0; i < N; i++) {
            if (t == p[i].next_arrival) {
                p[i].remaining = p[i].bt;
                p[i].abs_deadline = t + p[i].deadline;
                p[i].next_arrival += p[i].period;
            }
        }
        int idx = -1;
        int min_deadline = 1e5;

        for (int i = 0; i < N; i++) {
            if (p[i].remaining > 0 && p[i].abs_deadline < min_deadline) {
                min_deadline = p[i].abs_deadline;
                idx = i;
            }
        }

        if (idx != -1) {
            p[idx].remaining--;

            if (p[idx].remaining == 0) {
                p[idx].ct = t + 1;
                p[idx].tat = p[idx].ct - (p[idx].next_arrival - p[idx].period);
                p[idx].wt = p[idx].tat - p[idx].bt;
            }
        }
    }

    printf("ID\tBT\tDL\tCT\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].bt, p[i].deadline,
               p[i].ct, p[i].wt, p[i].tat);
    }
}

void RM(struct Process p[], int hyper) {
    printf("\n===== Rate Monotonic Scheduling =====\n");

    for (int t = 0; t < hyper; t++) {

        for (int i = 0; i < N; i++) {
            if (t == p[i].next_arrival) {
                p[i].remaining = p[i].bt;
                p[i].next_arrival += p[i].period;
            }
        }
        int idx = -1;
        int min_period = 1e5;

        for (int i = 0; i < N; i++) {
            if (p[i].remaining > 0 && p[i].period < min_period) {
                min_period = p[i].period;
                idx = i;
            }
        }

        if (idx != -1) {
            p[idx].remaining--;

            if (p[idx].remaining == 0) {
                p[idx].ct = t + 1;
                p[idx].tat = p[idx].ct - (p[idx].next_arrival - p[idx].period);
                p[idx].wt = p[idx].tat - p[idx].bt;
            }
        }
    }

    printf("ID\tBT\tDL\tCT\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].bt, p[i].deadline,
               p[i].ct, p[i].wt, p[i].tat);
    }
}

int main() {

    struct Process original[N] = {
        {1, 2, 3, 3},
        {2, 4, 4, 4},
        {3, 3, 6, 6}
    };

    struct Process p[N];

    int hyper = find_hyperperiod(original);

    printf("Hyperperiod = %d\n", hyper);

    check_schedulability(original);

    reset(p, original);
    EDF(p, hyper);

    reset(p, original);
    RM(p, hyper);

    return 0;
}
