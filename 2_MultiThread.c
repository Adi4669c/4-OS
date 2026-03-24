#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    char type[10];
} Process;

typedef struct {
    Process queue[MAX];
    int front, rear;
} Queue;

void initQueue(Queue *q) { q->front = q->rear = -1; }
int isEmpty(Queue *q) { return q->front == -1; }
void enqueue(Queue *q, Process p) {
    if (q->rear == MAX - 1) return;
    if (q->front == -1) q->front = 0;
    q->queue[++q->rear] = p;
}
Process dequeueFCFS(Queue *q) {
    Process p = q->queue[q->front];
    if (q->front == q->rear) q->front = q->rear = -1;
    else q->front++;
    return p;
}
Process dequeueSJF(Queue *q) {
    int minIndex = q->front;
    for (int j = q->front; j <= q->rear; j++)
        if (q->queue[j].remaining_time < q->queue[minIndex].remaining_time)
            minIndex = j;
    Process p = q->queue[minIndex];
    for (int j = minIndex; j < q->rear; j++) q->queue[j] = q->queue[j+1];
    q->rear--;
    if (q->rear < q->front) q->front = q->rear = -1;
    return p;
}
int compareArrival(const void *a, const void *b) {
    return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
}

void printQueueSummary(Process *list, int n, const char *type) {
    printf("%s Queue: ", type);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(list[i].type, type) == 0) {
            printf("P%d ", list[i].process_id);
            found = 1;
        }
    }
    if (!found) printf("empty");
    printf("\n");
}

int main() {
    int n;
    printf("Enter total number of processes: ");
    scanf("%d", &n);

    Process process_list[MAX];
    for (int i = 0; i < n; i++) {
        printf("Enter process_id, arrival_time, burst_time, type (system/user): ");
        scanf("%d %d %d %s", &process_list[i].process_id,
              &process_list[i].arrival_time,
              &process_list[i].burst_time,
              process_list[i].type);
        process_list[i].remaining_time = process_list[i].burst_time;
    }

    qsort(process_list, n, sizeof(Process), compareArrival);

    Queue systemQueue, userQueue;
    initQueue(&systemQueue);
    initQueue(&userQueue);

    int current_time = 0, completed = 0, i = 0;
    Process *current_process = NULL;
    int timeline[MAX*10], timeline_len = 0;

    while (completed < n) {
        while (i < n && process_list[i].arrival_time <= current_time) {
            if (strcmp(process_list[i].type, "system") == 0)
                enqueue(&systemQueue, process_list[i]);
            else
                enqueue(&userQueue, process_list[i]);
            i++;
        }

        if (current_process != NULL) {
            if (strcmp(current_process->type, "user") == 0 && !isEmpty(&systemQueue)) {
                enqueue(&userQueue, *current_process);
                current_process = NULL;
            }
        }

        if (current_process == NULL) {
            if (!isEmpty(&systemQueue)) {
                Process p = dequeueFCFS(&systemQueue);
                current_process = (Process*)malloc(sizeof(Process));
                *current_process = p;
            } else if (!isEmpty(&userQueue)) {
                Process p = dequeueSJF(&userQueue);
                current_process = (Process*)malloc(sizeof(Process));
                *current_process = p;
            } else {
                timeline[timeline_len++] = -1;
                current_time++;
                continue;
            }
        }

        timeline[timeline_len++] = current_process->process_id;
        current_process->remaining_time--;
        current_time++;

        if (current_process->remaining_time == 0) {
            current_process->completion_time = current_time;
            current_process->turnaround_time = current_process->completion_time - current_process->arrival_time;
            current_process->waiting_time = current_process->turnaround_time - current_process->burst_time;
            for (int k = 0; k < n; k++)
                if (process_list[k].process_id == current_process->process_id)
                    process_list[k] = *current_process;
            completed++;
            free(current_process);
            current_process = NULL;
        }
    }

    printf("\nGantt Chart:\n ");
    for (int t = 0; t < timeline_len; t++)
        printf(timeline[t] == -1 ? " idle " : " P%d ", timeline[t]);
    printf("\n0");
    for (int t = 1; t <= timeline_len; t++) printf("   %d", t);
    printf("\n");

    double avgWT = 0, avgTAT = 0;
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\tType\n");
    for (int j = 0; j < n; j++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
               process_list[j].process_id,
               process_list[j].arrival_time,
               process_list[j].burst_time,
               process_list[j].completion_time,
               process_list[j].turnaround_time,
               process_list[j].waiting_time,
               process_list[j].type);
        avgWT += process_list[j].waiting_time;
        avgTAT += process_list[j].turnaround_time;
    }
    printf("\nAverage Waiting Time = %.2f\n", avgWT/n);
    printf("Average Turnaround Time = %.2f\n", avgTAT/n);

    printf("\nQueues Summary:\n");
    printQueueSummary(process_list, n, "system");
    printQueueSummary(process_list, n, "user");

    return 0;
}
