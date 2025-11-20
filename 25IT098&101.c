#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bus {
    int busNo;
    char start[50];
    char destination[50];
    char startTime[10];
    char endTime[10];
};

void addRoute();
void displayRoutes();
void searchRoute();
void saveToFile(struct Bus b);

int main() {
    int choice;

    while (1) {
        printf("\n====== BUS ROUTE SCHEDULE SYSTEM ======\n");
        printf("1. Add Bus Route\n");
        printf("2. Display All Routes\n");
        printf("3. Search Bus by Number\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addRoute();
                break;

            case 2:
                displayRoutes();
                break;

            case 3:
                searchRoute();
                break;

            case 4:
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

void addRoute() {
    struct Bus b;
    getchar();  // clear buffer after scanf

    printf("\nEnter Bus Number: ");
    scanf("%d", &b.busNo);
    getchar();

    printf("Enter Starting Point: ");
    fgets(b.start, sizeof(b.start), stdin);
    b.start[strcspn(b.start, "\n")] = 0; // remove newline

    printf("Enter Destination: ");
    fgets(b.destination, sizeof(b.destination), stdin);
    b.destination[strcspn(b.destination, "\n")] = 0;

    printf("Enter Start Time (HH:MM): ");
    scanf("%s", b.startTime);

    printf("Enter End Time (HH:MM): ");
    scanf("%s", b.endTime);

    saveToFile(b);

    printf("\nRoute added successfully!\n");
}

void saveToFile(struct Bus b) {
    FILE *fp = fopen("routes.txt", "a");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Use | separator to safely store strings with spaces
    fprintf(fp, "%d|%s|%s|%s|%s\n",
            b.busNo,
            b.start,
            b.destination,
            b.startTime,
            b.endTime);

    fclose(fp);
}

void displayRoutes() {
    FILE *fp = fopen("routes.txt", "r");
    struct Bus b;

    if (fp == NULL) {
        printf("\nNo routes available.\n");
        return;
    }

    printf("\n======= ALL BUS ROUTES =======\n");
    printf("BusNo   Start             Destination        StartTime  EndTime\n");
    printf("--------------------------------------------------------------------\n");

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  &b.busNo,
                  b.start,
                  b.destination,
                  b.startTime,
                  b.endTime) != EOF) {

        printf("%-7d %-17s %-18s %-10s %-10s\n",
               b.busNo,
               b.start,
               b.destination,
               b.startTime,
               b.endTime);
    }

    fclose(fp);
}

void searchRoute() {
    FILE *fp = fopen("routes.txt", "r");
    struct Bus b;
    int busNo, found = 0;

    if (fp == NULL) {
        printf("\nNo routes available.\n");
        return;
    }

    printf("Enter Bus Number to search: ");
    scanf("%d", &busNo);

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  &b.busNo,
                  b.start,
                  b.destination,
                  b.startTime,
                  b.endTime) != EOF) {

        if (b.busNo == busNo) {
            printf("\nRoute Found!\n");
            printf("Bus Number   : %d\n", b.busNo);
            printf("Start Point  : %s\n", b.start);
            printf("Destination  : %s\n", b.destination);
            printf("Start Time   : %s\n", b.startTime);
            printf("End Time     : %s\n", b.endTime);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No route found for Bus Number %d.\n", busNo);
    }

    fclose(fp);
}
