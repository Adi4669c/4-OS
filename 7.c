#include <stdio.h>

void fifo(int pages[], int n, int frames) {
    int frame[frames], i, j, k = 0, faults = 0;

    for(i = 0; i < frames; i++)
        frame[i] = -1;

    printf("\n--- FIFO ---\n");

    for(i = 0; i < n; i++) {
        int found = 0;

        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            frame[k] = pages[i];
            k = (k + 1) % frames;
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        for(j = 0; j < frames; j++)
            printf("%d ", frame[j]);
        printf("\n");
    }

    printf("FIFO Page Faults = %d\n", faults);
}

void lru(int pages[], int n, int frames) {
    int frame[frames], time[frames];
    int i, j, pos, faults = 0;

    for(i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = -1;
    }

    printf("\n--- LRU ---\n");

    for(i = 0; i < n; i++) {
        int found = 0;

        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                time[j] = i;
                break;
            }
        }

        if(!found) {
            pos = 0;

            for(j = 1; j < frames; j++) {
                if(time[j] < time[pos])
                    pos = j;
            }

            frame[pos] = pages[i];
            time[pos] = i;
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        for(j = 0; j < frames; j++)
            printf("%d ", frame[j]);
        printf("\n");
    }

    printf("LRU Page Faults = %d\n", faults);
}

void optimal(int pages[], int n, int frames) {
    int frame[frames];
    int i, j, k, pos, faults = 0;

    for(i = 0; i < frames; i++)
        frame[i] = -1;

    printf("\n--- Optimal ---\n");

    for(i = 0; i < n; i++) {
        int found = 0;

        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            int farthest = -1;
            pos = 0;

            for(j = 0; j < frames; j++) {
                int next = -1;

                for(k = i + 1; k < n; k++) {
                    if(frame[j] == pages[k]) {
                        next = k;
                        break;
                    }
                }

                if(next == -1) {
                    pos = j;
                    break;
                }

                if(next > farthest) {
                    farthest = next;
                    pos = j;
                }
            }

            frame[pos] = pages[i];
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        for(j = 0; j < frames; j++)
            printf("%d ", frame[j]);
        printf("\n");
    }

    printf("Optimal Page Faults = %d\n", faults);
}

int main() {
    int n, frames, i;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];

    printf("Enter page reference string: ");
    for(i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    fifo(pages, n, frames);
    lru(pages, n, frames);
    optimal(pages, n, frames);

    return 0;
}
