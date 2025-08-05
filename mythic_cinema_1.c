#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define total_movies 10
#define total_branches 5
#define total_halls 5
#define max_showtime 5
#define max_booking 50


typedef struct {
    char username[30];
    int phone_number;
    char password[20];
    char birthdate[12];
} Registration;

typedef struct {
    char title[30];
    char release_date[30];
    char duration[12];
    char genre[50];
    char classification[8];
    float price;
} Movie;

typedef struct {
    char time[10];
} Time;

typedef struct {
    char format[12];
    char hall[20];
    Time time[max_showtime];
    char** seat;
    int row,col;
} Hall;

typedef struct {
    char branch_name[50];
    Hall hall_location[total_halls];
} Location;

typedef struct {
    Movie *movie;
    Hall *halls;
    Time *times;
    Location *location;
    int row,col;
} BookingTicket;

typedef struct {
    Movie movies[total_movies];
    Location locations[total_branches];
} Cinema;


void bookingProcessStep1 (Cinema* movie,int movieIndex, BookingTicket booking[], int* bookingCount);
void bookingProcessStep2 (Cinema* movie,int indexLocation, int movieIndex, BookingTicket booking[], int* bookingCount);
void bookingProcessStep3 (Cinema* movie, int movieIndex, int indexLocation, int indexFormatnHall, int indexTime, BookingTicket booking[], int* bookingCount);
void bookingSeat (Cinema* movie, int movieIndex, int indexLocation, int indexFormatnHall, int indexTime, BookingTicket booking[], int* bookingCount);

void initializeSeat(Hall *halls) {
    int rows = 0, cols = 0;

    if (strcmp(halls->hall, "Gold Class") == 0)       { rows = 5; cols = 10; }
    else if (strcmp(halls->hall, "Diamond Class") == 0) { rows = 4; cols = 8; }
    else if (strcmp(halls->hall, "Regular Hall") == 0)  { rows = 6; cols = 12; }
    else if (strcmp(halls->hall, "Premium Hall") == 0)  { rows = 6; cols = 10; }
    else if (strcmp(halls->hall, "SCREEN X") == 0)      { rows = 6; cols = 12; }

    halls->seat = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        halls->seat[i] = malloc(cols * sizeof(char));
        for (int j = 0; j < cols; j++) {
            halls->seat[i][j] = 'O'; // 'O' for available seat
        }
    }
}

void printSeat(Hall *hall) {
    int rows = 0, cols = 0;

    // Set rows and cols based on hall type
    if (strcmp(hall->hall, "Gold Class") == 0)       { rows = 5; cols = 10; }
    else if (strcmp(hall->hall, "Diamond Class") == 0) { rows = 4; cols = 8; }
    else if (strcmp(hall->hall, "Regular Hall") == 0)  { rows = 6; cols = 12; }
    else if (strcmp(hall->hall, "Premium Hall") == 0)  { rows = 6; cols = 10; }
    else if (strcmp(hall->hall, "SCREEN X") == 0)      { rows = 6; cols = 12; }

    printf("\n\n   ");
    
    // Print column headers
    for (int j = 0; j < cols; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    // Print each row with its label (A, B, ...)
    for (int i = 0; i < rows; i++) {
        printf("%c: ", 'A' + i);
        for (int j = 0; j < cols; j++) {
            printf(" %c ", hall->seat[i][j]); // 'O' = available, 'X' = booked
        }
        printf("\n");
    }
}
////////////////////////1.  Home /////////////////////////////////// 
void Home (Cinema *cinema, BookingTicket booking[], int *bookingCount) {  

    int choice;
    do {
        printf("\n=== Mythic Cinema ===\n");
        printf("  Now Showing\n");

        for (int i=0; i<total_movies; i++){
            printf("%d. %s ",i+1,cinema->movies[i].title);
            printf("(%s) Rate:%s\n",cinema->movies[i].release_date,cinema->movies[i].classification);
        }
        printf("\n0. Exit the Home Screen\n");
        
        printf("\nEnter choice: ");
        scanf("%d",&choice);
        int movieIndex = choice-1;

        if (choice > 0 && choice <= total_movies) {
            bookingProcessStep1(cinema,movieIndex,booking,bookingCount);

        }else if (choice==0) {
            printf("Exiting Home...\n");
            break;
        }else {
            printf("Invalid Choice Try Again\n");
        }


    }while (choice != 0);
}

void bookingProcessStep1 (Cinema* movie,int movieIndex, BookingTicket booking[], int* bookingCount) { 
    int choice;
    do {
        printf("\n%s\n",movie->movies[movieIndex].title);
        printf("Format: 2D 3D SCREEN_X\n");
        printf("Genre: %s\n",movie->movies[movieIndex].genre);
        printf("Duration: %s\n",movie->movies[movieIndex].duration);
        printf("Release: %s\n",movie->movies[movieIndex].release_date);
        printf("Classification: %s\n",movie->movies[movieIndex].classification);
    
        printf("\nAll Cinemas:\n");
        for (int i=0; i<total_branches; i++){
            printf("%d. %s\n",i+1,movie->locations[i].branch_name);
        }
    
        printf("Enter choice: ");
        scanf("%d",&choice);
        int indexLocation= choice-1;

        if (choice > 0 && choice <= total_branches) {bookingProcessStep2(movie,indexLocation,movieIndex,booking,bookingCount);} ////////choice///////////
        else if (choice == 0) {printf("\nExiting .....\n");break;}
        else {printf("Invalid option! Try Again!\n");}

    }while (choice != 0);
    
}

void bookingProcessStep2 (Cinema* movie,int indexLocation, int movieIndex, BookingTicket booking[], int* bookingCount) { 
    int choice;
    do {
        printf("\n%s\n",movie->locations[indexLocation].branch_name);
        for (int i=0; i<total_halls; i++){
            printf("%d. %s|%s\n",i+1,movie->locations[indexLocation].hall_location[i].format,movie->locations[indexLocation].hall_location[i].hall);
        } 
        printf("\nEnter choice: ");
        scanf("%d",&choice);
        int indexFormatnHall = choice-1;
        if (choice > 0 && choice <= total_halls) {
            int choice_1;
            int count=0;
            do {
                printf("\n%s\n",movie->locations[indexLocation].branch_name);
                printf("%s|%s\n",movie->locations[indexLocation].hall_location[indexFormatnHall].format,movie->locations[indexLocation].hall_location[indexFormatnHall].hall);
                for (int i=0; i<max_showtime; i++){
                    if (strlen(movie->locations[indexLocation].hall_location[indexFormatnHall].time[i].time) == 0 ) continue;
                    printf("%d. %s\t",i+1,movie->locations[indexLocation].hall_location[indexFormatnHall].time[i].time);
                    count++;
                }
                printf("\n\nEnter choice: ");
                scanf("%d",&choice_1);
                int indexTime = choice_1 - 1;
                if (choice_1 > 0 && choice_1 <= count) {bookingProcessStep3(movie,movieIndex,indexLocation,indexFormatnHall,indexTime,booking,bookingCount);} ////////choice///////////
                else if (choice_1 == 0) {printf("\nExiting .....\n");break;}
                else {printf("Invalid option! Try Again!\n");}


            } while (choice_1 != 0);

        } ////////choice///////////
        else if (choice == 0) {printf("\nExiting.....\n");break;}
        else {printf("Invalid option! Try Again!\n");}

    }  while (choice != 0) ;
   
}

void bookingProcessStep3 (Cinema* movie, int movieIndex, int indexLocation, int indexFormatnHall, int indexTime, BookingTicket booking[], int* bookingCount) {
    Hall *seat = &movie->locations[indexLocation].hall_location[indexFormatnHall];
    int choice;

    do {
        printf("\n   ==========SCREEN=========");
        printSeat(seat);
        printf("1. Book seat\n2. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);
        if (choice == 1){
            bookingSeat(movie,movieIndex,indexLocation,indexFormatnHall,indexTime,booking,bookingCount);
        }else if (choice == 2) {
            printf("Exiting...\n");
            break;
        }else{
            printf("Invalid Choice Try Again\n");
        }

    } while (choice != 0);

}

void bookingSeat (Cinema* movie, int movieIndex, int indexLocation, int indexFormatnHall, int indexTime, BookingTicket booking[], int* bookingCount) {
    char seatInput[5];
    int row, col;
    Hall *hall = &movie->locations[indexLocation].hall_location[indexFormatnHall];

    printf("Enter seat (e.g., A1): ");
    scanf("%s", seatInput);

    row = seatInput[0] - 'A';
    col = atoi(&seatInput[1]) - 1;

    if (row < 0 || col < 0) {
        printf("\nInvalid seat!\n");
        return;
    }

    int rows = 0, cols = 0;
    if (strcmp(hall->hall, "Gold Class") == 0)       { rows = 5; cols = 10; }
    else if (strcmp(hall->hall, "Diamond Class") == 0) { rows = 4; cols = 8; }
    else if (strcmp(hall->hall, "Regular Hall") == 0)  { rows = 6; cols = 12; }
    else if (strcmp(hall->hall, "Premium Hall") == 0)  { rows = 6; cols = 10; }
    else if (strcmp(hall->hall, "SCREEN X") == 0)      { rows = 6; cols = 12; }

    if (row >= rows || col >= cols) {
        printf("\nSeat out of range!\n");
        return;
    }

    if (hall->seat[row][col] == 'X') {
        printf("\nSeat already booked!\n");
    } else {
        hall->seat[row][col] = 'X'; ////////// X for taken seat ////////////
        booking[*bookingCount].halls = hall;
        booking[*bookingCount].movie = &movie->movies[movieIndex];
        booking[*bookingCount].location = &movie->locations[indexLocation];
        booking[*bookingCount].times = &movie->locations[indexLocation].hall_location[indexFormatnHall].time[indexTime];
        booking[*bookingCount].row = row;
        booking[*bookingCount].col = col;
        
        printf("\nSuccessfully Booked!!\n");
        printf("\nBooking Detail:\n");
        printf("Movie name: %s\n", booking[*bookingCount].movie->title);
        printf("Time: %s\n", booking[*bookingCount].times->time);
        printf("Location: %s\n", booking[*bookingCount].location->branch_name);
        printf("Format: %s\n", booking[*bookingCount].halls->format);
        printf("Hall: %s\n", booking[*bookingCount].halls->hall);
        printf("Seat: %c%d\n", 'A' + row, col + 1);
        printf("Price: %.2f$\n", movie->movies[movieIndex].price);

        (*bookingCount)++;

    }

}
/// ////////////////////////////////////////////////2. Cinema//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cinemas (Cinema *cinema, BookingTicket booking[], int* bookingCount);
void bookingProcessStep1_1(Cinema* cinema, int indexBranch, BookingTicket booking[], int* bookingCount); 
void bookingProcessStep2_1 (Cinema* cinema, int indexBranch, int indexMovie, BookingTicket booking[], int* bookingCount);

void Cinemas (Cinema *cinema, BookingTicket booking[], int* bookingCount) {
    int choice=0;
    do {
        printf("\n=== Mythic Cinema ===\n");
        for (int i=0; i<total_branches; i++) {
            printf("%d. %s\n",i+1,cinema->locations[i].branch_name);
        }
        printf("Enter choice: ");
        scanf("%d",&choice);
        int indexBranch = choice-1;
        if (choice == 0) {printf("Exiting Cinema...\n");break;}
        else if (choice>0 && choice<=total_branches) {bookingProcessStep1_1(cinema,indexBranch,booking,bookingCount);}
        else {printf("Invalid Choice\n");}

    } while (choice != 0);   

}

void bookingProcessStep1_1(Cinema* cinema, int indexBranch, BookingTicket booking[], int* bookingCount) {
    int choice;
    do {
        printf("\n%s\n",cinema->locations[indexBranch].branch_name);
        printf("  Now Showing\n");

        for (int i=0; i<total_movies; i++){
            printf("%d. %s ",i+1,cinema->movies[i].title);
            printf("(%s) Rate:%s\n",cinema->movies[i].release_date,cinema->movies[i].classification);
        }
   
        printf("Enter choice: ");
        scanf("%d",&choice);
        int indexMovie =choice - 1;
        if (choice == 0) {printf("Exiting\n");break;}
        else if (choice>0 && choice<=total_movies) {bookingProcessStep2_1(cinema,indexBranch,indexMovie,booking,bookingCount);}
        else {printf("Invalid Choice\n");}
    }while (choice != 0);
}

void bookingProcessStep2_1 (Cinema* cinema, int indexBranch, int indexMovie, BookingTicket booking[], int* bookingCount) {
    int choice;
    do {
        printf("\n%s\n",cinema->movies[indexMovie].title);
        printf("Format: 2D|3D|SCREEN X\n");
        printf("Genre: %s\n",cinema->movies[indexMovie].genre);
        printf("Duration: %s\n",cinema->movies[indexMovie].duration);
        printf("Release: %s\n",cinema->movies[indexMovie].release_date);
        printf("Classification: %s\n",cinema->movies[indexMovie].classification);
    
        printf("\n%s:\n",cinema->locations[indexBranch].branch_name);
        for (int i=0; i<total_halls; i++) {
            printf("%d. %s|%s\n",i+1,cinema->locations[indexBranch].hall_location[i].format,cinema->locations[indexBranch].hall_location[i].hall);

        }

        printf("Enter choice: ");
        scanf("%d",&choice);
        int indexFormatnHall= choice-1;

        if (choice > 0 && choice <= total_branches) {
            int choice_1;
            int count=0;
            do {
                printf("\n%s\n",cinema->locations[indexBranch].branch_name);
                printf("%s|%s\n",cinema->locations[indexBranch].hall_location[indexFormatnHall].format,cinema->locations[indexBranch].hall_location[indexFormatnHall].hall);
                for (int i=0; i<max_showtime; i++){
                    if (strlen(cinema->locations[indexBranch].hall_location[indexFormatnHall].time[i].time) == 0 ) continue;
                    printf("%d. %s\t",i+1,cinema->locations[indexBranch].hall_location[indexFormatnHall].time[i].time);
                    count++;
                }
                printf("\n\nEnter choice: ");
                scanf("%d",&choice_1);
                int indexTime = choice_1 - 1;
                if (choice_1 > 0 && choice_1 <= count) {bookingProcessStep3(cinema,indexMovie,indexBranch,indexFormatnHall,indexTime,booking,bookingCount);} ////////choice///////////
                else if (choice_1 == 0) {printf("\nExiting.....\n");break;}
                else {printf("Invalid option! Try Again!\n");}


            } while (choice_1 != 0);

        } ////////choice///////////
        else if (choice == 0) {printf("\nExiting.....\n");break;}
        else {printf("Invalid option! Try Again!\n");}

    }while (choice != 0);
}
/// ////////// ///////////////////////////////3. More//////////////////////////////////////////////////////////////

void More (Registration* user, BookingTicket booking[], int* bookingCount) ;
void Profile (Registration* user);
void trimNewline(char *str);
void EditAccount(Registration* user);
void BookingHistory(BookingTicket booking[], int* bookingCount);


void More (Registration* user, BookingTicket booking[], int* bookingCount) {
    int choice;
    do {
        printf("\nYour Profile: %s\n",user->username);
        printf("1. Account\n");
        printf("2. Purchase History\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch (choice) {
            case 0: printf("Exiting More...\n");break;
            case 1: Profile(user);break;
            case 2: BookingHistory(booking,bookingCount);
            
            default: printf("Invalid Choice Try Again\n");
        }
    } while (choice != 0);
    
}

void Profile (Registration* user) {
    int choice;
    do {
        printf("\nPersonal Information:\n");
        printf("Username: %s\n",user->username);
        printf("Date of Birth: %s\n",user->birthdate);
        printf("\nContact Information:\n");
        printf("Phone Number: %d\n",user->phone_number);
        printf("\n1. Edit information\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);
        switch (choice) {
            case 1: EditAccount(user);break;
            case 2: printf("Exiting...\n");break;
            default: printf("Invalid Choice Try Again\n");
        }

    } while (choice != 2);
    
}

void trimNewline(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
}

void EditAccount(Registration* user) {
    int choice;
    do {
        printf("1. Username\n");
        printf("2. Date of Birth (DD-MM-YYYY)\n");
        printf("3. Phone Number\n");
        printf("0. Exiting\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // flush leftover newline

        switch (choice) {
            case 0:
                printf("Exiting...\n");
                break;
            case 1:
                printf("Username: ");
                fgets(user->username, sizeof(user->username), stdin);
                trimNewline(user->username);
                break;
            case 2:
                printf("Date of Birth (DD-MM-YYYY): ");
                fgets(user->birthdate, sizeof(user->birthdate), stdin);
                trimNewline(user->birthdate);
                break;
            case 3:
                printf("Phone Number: ");
                scanf("%d", &user->phone_number);
                while (getchar() != '\n'); // flush newline
                break;
            default:
                printf("Invalid Choice Try Again\n");
        }

    } while (choice != 0);
}

void BookingHistory(BookingTicket booking[], int* bookingCount) {
    int choice; 
    do {
        printf("\n=== Booking History ===\n");

        if (*bookingCount == 0) {
            printf("No bookings found.\n");
        } else {
            for (int i = 0; i < *bookingCount; i++) {
                printf("\nBooking #%d\n", i + 1);
                printf("Movie name: %s\n", booking[i].movie->title);
                printf("Time: %s\n", booking[i].times->time);
                printf("Location: %s\n", booking[i].location->branch_name);
                printf("Format: %s\n", booking[i].halls->format);
                printf("Hall: %s\n", booking[i].halls->hall);
                printf("Seat: %c%d\n", 'A' + booking[i].row, booking[i].col + 1);
                printf("Price: %.2f$\n", booking[i].movie->price);
            }
        }

        printf("\n0. Exit Booking History\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

    } while (choice != 0);
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
int main () {
    //////////////Registration////////////////////////
    Registration user;
    printf("=== Sign in ===\n");
    printf("Enter username: ");
    fgets(user.username,sizeof(user.username),stdin);
    trimNewline(user.username);

    printf("Enter Birth of date (DD-MM-YYYY): ");
    fgets(user.birthdate,sizeof(user.birthdate),stdin);
    trimNewline(user.birthdate);

    printf("Enter phone number: ");
    scanf("%d",&user.phone_number);

    printf("Enter password: ");
    getchar();
    fgets(user.password,sizeof(user.password),stdin);
    trimNewline(user.password);
    
    Cinema cinema;
    BookingTicket booking[max_booking];
    int bookingCount = 0;

    ////////////////Initialize movies//////////////////
    cinema.movies[0] = (Movie){"M3GAN","27-June-2025","2h 00min","Horror","NC15",4}; ////////////compound literal/////////
    cinema.movies[1] = (Movie){"F1","26-June-2025","2h 32mins","Drama","G",5};
    cinema.movies[2] = (Movie){"Titanic","19-December-1997","3h 15mins","Romance,Drama","PG-13",4}; 
    cinema.movies[3] = (Movie){"Frozen","27-November-2013","1h 42mins","Animation, Family, Fantasy","PG",2};
    cinema.movies[4] = (Movie){"Barbie","21-July-2023","1h 54mins","Comedy,Adventure,Fantasy","PG-13",3};
    cinema.movies[5] = (Movie){"Oppenheimer","21-July-2023","3h 00min","Biography,drama,History","R",3};
    cinema.movies[6] = (Movie){"Spider-Man: No Way Home","17-December-2021","2h 28mins","Superhero,Action,Fantasy","PG-13",4};
    cinema.movies[7] = (Movie){"Finding Nemo","30-May-2003","1h 40mins","Animation,Family,Adventure","G",4};
    cinema.movies[8] = (Movie){"Joker","04-October-2019","2h 02mins","Drama,Thriller,Crime","R",3.25};
    cinema.movies[9] = (Movie){"Exuhma","22-February-2024","2h 14mins","Supernatural Horror,Mystery,Thriller","R",2.5};
    
    ////////////Initialize Location of movie theatre////////////
    cinema.locations[0] = (Location){"Mythic Olympia",{{"2D","Gold Class",{"03:35 PM","09:30 PM"}}, {"3D","Gold Class",{"10:40 AM","01:25 PM","06:40 PM"}},
    {"SCREEN X","Regular Hall",{"10:20 AM","01:05 PM","03:50 PM","06:35 PM","09:20 PM"}}, {"3D","Premium Hall",{"10:00 AM","12:45 PM","03:30 PM","6:15 PM","08:40 PM"}},
    {"2D","Diamond Class",{"08:25 PM","11:10 PM"}}}};
    cinema.locations[1] = (Location){"Mythic Aeon Mall",{{"2D","Gold Class",{"03:35 PM","09:30 PM"}}, {"3D","Gold Class",{"10:40 AM","01:25 PM","06:40 PM"}},
    {"SCREEN X","Regular Hall",{"10:20 AM","01:05 PM","03:50 PM","06:35 PM","09:20 PM"}}, {"3D","Premium Hall",{"10:00 AM","12:45 PM","03:30 PM","6:15 PM","08:40 PM"}},
    {"2D","Diamond Class",{"08:25 PM","11:10 PM"}}}};
    cinema.locations[2] = (Location){"Mythic TK",{{"2D","Gold Class",{"03:35 PM","09:30 PM"}}, {"3D","Gold Class",{"10:40 AM","01:25 PM","06:40 PM"}},
    {"SCREEN X","Regular Hall",{"10:20 AM","01:05 PM","03:50 PM","06:35 PM","09:20 PM"}}, {"3D","Premium Hall",{"10:00 AM","12:45 PM","03:30 PM","6:15 PM","08:40 PM"}},
    {"2D","Diamond Class",{"08:25 PM","11:10 PM"}}}};
    cinema.locations[3] = (Location){"Mythic Siem Reap",{{"2D","Gold Class",{"03:35 PM","09:30 PM"}}, {"3D","Gold Class",{"10:40 AM","01:25 PM","06:40 PM"}},
    {"SCREEN X","Regular Hall",{"10:20 AM","01:05 PM","03:50 PM","06:35 PM","09:20 PM"}}, {"3D","Premium Hall",{"10:00 AM","12:45 PM","03:30 PM","6:15 PM","08:40 PM"}},
    {"2D","Diamond Class",{"08:25 PM","11:10 PM"}}}};
    cinema.locations[4] = (Location){"Mythic Cinema Sihanoukville",{{"2D","Gold Class",{"03:35 PM","09:30 PM"}}, {"3D","Gold Class",{"10:40 AM","01:25 PM","06:40 PM"}},
    {"SCREEN X","Regular Hall",{"10:20 AM","01:05 PM","03:50 PM","06:35 PM","09:20 PM"}}, {"3D","Premium Hall",{"10:00 AM","12:45 PM","03:30 PM","6:15 PM","08:40 PM"}},
    {"2D","Diamond Class",{"08:25 PM","11:10 PM"}}}};

    ///////////////Initialize seat/////////////////
    for (int i = 0; i < total_branches; i++) {
        for (int j = 0; j < total_halls; j++) {
            initializeSeat(&cinema.locations[i].hall_location[j]);
        }
    }

    int choice;
    do {
        printf("\n=== Welcome to Mythic Cinema ===\n");
        printf("1. Home\n");
        printf("2. Cinema\n");
        printf("3. More\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch (choice) {
            case 1: Home(&cinema,booking,&bookingCount); break;
            case 2: Cinemas(&cinema,booking,&bookingCount); break;
            case 3: More(&user,booking,&bookingCount); break;
            case 0: printf("Exiting...Bye!!");break;
            default:printf("Invalid option! Try again!\n");
        }

    } while (choice != 0);

    // Free all allocated seat memory
    for (int i = 0; i < total_branches; i++) {
        for (int j = 0; j < total_halls; j++) {
            Hall *hall = &cinema.locations[i].hall_location[j];

            // Determine number of rows
            int rows = 0;
            if (strcmp(hall->hall, "Gold Class") == 0)         { rows = 5; }
            else if (strcmp(hall->hall, "Diamond Class") == 0) { rows = 4; }
            else if (strcmp(hall->hall, "Regular Hall") == 0)  { rows = 6; }
            else if (strcmp(hall->hall, "Premium Hall") == 0)  { rows = 6; }
            else if (strcmp(hall->hall, "SCREEN X") == 0)      { rows = 6; }

            // Free each row
            for (int r = 0; r < rows; r++) {
                free(hall->seat[r]);
            }

            // Free the seat pointer array
            free(hall->seat);
        }
    }

    return 0;
}