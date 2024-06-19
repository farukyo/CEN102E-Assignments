#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct definitions
typedef struct {
    char tailNumber[50];
    char model[50];
    int year;
    int capacity;
} Airplane;

typedef struct {
    int count;
    Airplane *airplanes;
} Fleet;

// Function Prototypes
Fleet readFleetFromFile(char *filename);
void writeFleetToFile(char *filename, Fleet *fleet);
void addAirplane(Fleet *fleet, Airplane newAirplane);
void displayFleet(Fleet fleet);

int main(int argc, char *argv[]) {
    // Check if correct number of arguments is provided
    if (argc < 3) {
        printf("Usage: %s <fleet_file> <command> [<args>]\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *command = argv[2];

    // Read fleet from file
    Fleet fleet = readFleetFromFile(filename);

    // Handle add command
    if (strcmp(command, "add") == 0) {
        // Check if correct number of parameters is provided
        /* Included 8 because the example photo on pdf with boeing 777 2020 350 is faulty 
        if the terminal input was ./fleetManager fleet1.txt N98765 "Boeing 777" 2020 350 
        i would keep if(argc !=7) instead of argc < 7 || arc > 8 */
        if (argc < 7 || argc > 8) {
            printf("Usage: %s <fleetFile> add <tailNumber> <model> <year> <capacity>\n", argv[0]);
            return 1;
        }

        char tailNumber[50];
        char model[50];
        int year, capacity;

        strcpy(tailNumber, argv[3]);
        strcpy(model, argv[4]);
        if (argc == 8) { // to fix confusion mentioned above with 8 arguments; argv 4 and 5 are concatenated into one argv[4]
        //then argv 6 and 7 are placed into 5 and 6 to make them use the same code with 7 arguments
            strcat(model, " ");
            strcat(model, argv[5]);
            year = atoi(argv[6]);
            capacity = atoi(argv[7]);
        } else {
            year = atoi(argv[5]);
            capacity = atoi(argv[6]);
        }


        printf("Command: %s\n", command);
        Airplane newAirplane;
        strcpy(newAirplane.tailNumber, tailNumber);
        strcpy(newAirplane.model, model);
        newAirplane.year = year;
        newAirplane.capacity = capacity;

        printf("Adding airplane with tail number: %s\n", newAirplane.tailNumber);
        addAirplane(&fleet, newAirplane);
        writeFleetToFile(filename, &fleet);

    } 
    // Handle display command
    else if (strcmp(command, "display") == 0) {
        if (argc != 3) {
            printf("Usage: %s <fleetFile> display\n", argv[0]);
            return 1;
        }
        printf("Command: %s\n", command);
        displayFleet(fleet);
    } 
    // Handle unknown command
    else {
        printf("Unknown command: %s\n", command);
        free(fleet.airplanes);
        return 1;
    }

    free(fleet.airplanes);
    return 0;
}

// Function to read fleet from file
Fleet readFleetFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file: %s\n", filename);
        exit(1);
    }

    Fleet fleet;
    fleet.count = 0;
    fleet.airplanes = NULL;

    Airplane airplane;
    // Read airplane details from file and add them to fleet
    while (fscanf(file, "%49[^,],%49[^,],%d,%d\n", airplane.tailNumber, airplane.model, &airplane.year, &airplane.capacity) == 4) { //Checks if fscanf have 4 arguments
        addAirplane(&fleet, airplane);
    }

    fclose(file);
    return fleet;
}

// Function to write fleet to file
void writeFleetToFile(char *filename, Fleet *fleet) {
    FILE *file = fopen(filename, "a"); // Open file in append mode
    if (!file) {
        printf("Cannot open file: %s\n", filename);
        return;
    }

    // Append the last airplane
    Airplane newAirplane = fleet->airplanes[fleet->count - 1];
    fprintf(file, "\n%s,%s,%d,%d", newAirplane.tailNumber, newAirplane.model, newAirplane.year, newAirplane.capacity);

    fclose(file);
}

// Function to add airplane to fleet
void addAirplane(Fleet *fleet, Airplane newAirplane) {
    fleet->airplanes = realloc(fleet->airplanes, (fleet->count + 1) * sizeof(Airplane));
    if (!fleet->airplanes) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    fleet->airplanes[fleet->count] = newAirplane;
    fleet->count++;
}

// Function to display fleet
void displayFleet(Fleet fleet) {
    printf("Displaying fleet\n");
    for (int i = 0; i < fleet.count; i++) {
        printf("Tail Number: %s, Model: %s, Year: %d, Capacity: %d\n", fleet.airplanes[i].tailNumber, fleet.airplanes[i].model, fleet.airplanes[i].year, fleet.airplanes[i].capacity);
    }
}
