#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int** newArray(int, int);
void freeArray(int***, int);
void printArray(int***, int, int);

int nextInt(char*);
int nextBombInt(int, int);

int rngInt(int, int);

void prepareTable(int**, int, int, int);
void convertInt2Pos(int**, int, int, int*, int*);

void startGame(int**, int, int);

void createFile(int**, int, int);

int main(int argc, const char** argv) {
    /*  Notes: 
     *  ■  : hidden
     *  1-8: bombs nearby
     *  0  : no bombs nearby
     *  x  : bomb
    */
    char play = 'y';
    while (play == 'y') {  
        system("clear");
        srand(time(NULL));

        printf("Welcome to Minesweeper!\n");
        int r = nextInt("Please enter the desired number of rows."); // table's rows
        int c = nextInt("Now please enter the desired number of columns."); // table's columns
        int k = nextBombInt(r, c); // number of bombs
        printf("Generating table...\n");

        int** table = newArray(r, c);

        prepareTable(table, r, c, k);

        createFile(table, r, c);

        startGame(table, r, c);

        freeArray(&table, r);

        printf("Do you want to play again? [y/n]\n ➤ ");
        scanf("%c", &play);
        while (getchar() != '\n'); // clear stdin (just in case)
        if (play == 'Y') {
            play = 'y';
        }
    }

    return 0;
}

int** newArray(int r, int c) {
    int** array = (int**)calloc(r, sizeof(int*));
    for (int i = 0; i < r; i++) {
        *(array + i) = (int*)calloc(c, sizeof(int));
    }

    return array;
}

void freeArray(int*** array, int r) {
    for (int i = 0; i < r; i++) {
        free(*(*array + i));
    }
    free(*array);
    *array = NULL;
}

void printArray(int*** array, int r, int c) {
    for (int i = 0; i < r; i++) {
        printf("\t");
        for (int j = 0; j < c; j++) {
            printf("%2d ", *(*(*array + i) + j));
        }
        printf("\n");
    }
}

int nextInt(char* str) {
    int in;
    printf("%s\n ➤ ", str);
    scanf("%d", &in);
    while (in <= 1) {
        printf("Invalid number.\n ➤ ");
        scanf("%d", &in);
    }
    while (getchar() != '\n'); // clear stdin (just in case)
    return in;
}

int nextBombInt(int r, int c) {
    int max_bombs = (r * c) - 1;
    printf("You may fit up to %d bombs in this table. ", max_bombs);
    int k = nextInt("Please enter the number of bombs.");
    while (k > max_bombs || k <= 0) {
        if (k > max_bombs) {
            printf("It's impossible to fit %d bombs assuming we'll be using this table!\nRemember, you may fit up to %d bombs in this table. ", k, max_bombs);
        }
        k = nextInt("Please enter a proper number.");
    }
    return k;
}

int rngInt(int a, int b) {
    return rand() % (b - a + 1) + a;
}

void prepareTable(int** table, int r, int c, int k) {
    // Bomb placement
    for (int it = 0; it < k; it++) {
        int location, i, j;
        do {
            location = rngInt(0, (r * c) - 1); // why? 0 is the first position of the table and (r * c) - 1 the last position
            /*  Example of a table

                Data:       Place:

                1 -1  1     0  1  2
                1  1  1     3  4  5
                0  0  0     6  7  8
                
            Therefore (3 * 3) - 1 = 8 which is in fact the last position of the table.
            Now we need to convert this into coordinates. These will be saved in "i" and "j".
            */
            convertInt2Pos(table, c, location, &i, &j);
        }while (*(*(table + i) + j) == -1);
        *(*(table + i) + j) = -1;

        // Determining location characteristics
        int top = i == 0;
        int bottom = i == r - 1;
        int left = j == 0;
        int right = j == c - 1;

        if (!top && !left) {
            if (*(*(table + i - 1) + j - 1) != -1) {
                (*(*(table + i - 1) + j - 1))++;
            }
        }
        if (!top) {
            if (*(*(table + i - 1) + j) != -1) {
                (*(*(table + i - 1) + j))++;
            }
        }
        if (!top && !right) {
            if (*(*(table + i - 1) + j + 1) != -1) {
                (*(*(table + i - 1) + j + 1))++;
            }
        }
        if (!left) {
            if (*(*(table + i) + j - 1) != -1) {
                (*(*(table + i) + j - 1))++;
            }
        }
        if (!right) {
            if (*(*(table + i) + j + 1) != -1) {
                (*(*(table + i) + j + 1))++;
            }
        }
        if (!bottom && !left) {
            if (*(*(table + i + 1) + j - 1) != -1) {
                (*(*(table + i + 1) + j - 1))++;
            }
        }
        if (!bottom) {
            if (*(*(table + i + 1) + j) != -1) {
                (*(*(table + i + 1) + j))++;
            }
        }
        if (!bottom && !right) {
            if (*(*(table + i + 1) + j + 1) != -1) {
                (*(*(table + i + 1) + j + 1))++;
            }
        }
    }
}

void convertInt2Pos(int** array, int c, int pos, int* i, int* j) { // This takes a number that denotes it's place (see above) and uses "i" and "j" to express the same thing. By doing this, I'm able to retrieve any data that's in there. Also, it makes it easier to place the bombs in our table.
    *j = pos % c;
    *i = pos / c;
}

void startGame(int** table, int r, int c) {
    int** stage = newArray(r, c);
    int flag = 0;
    int bomb = 0;
    while (!flag) {
        system("clear");
        printf(" -- == Minesweeper == --\nSee what's under a block by typing the coordinates of it, starting from 0. The row and column values are separated with one space character. For example, \"1 1\" . Good luck!\n");
        for (int i = 0; i < r; i++) { // draw the table
            printf("\t");
            for (int j = 0; j < c; j++) {
                if (*(*(stage + i) + j) == 0) {
                    printf("■ ");
                } else {
                    if (*(*(table + i) + j) == -1) {
                        printf("x ");
                    } else {
                        printf("%d ", *(*(table + i) + j));
                    }
                }
            }
            printf("\n");
        }
        int full = 1;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (*(*(stage + i) + j) == 0 && *(*(table + i) + j) != -1) {
                    full = 0;
                    break;
                }
            }
        }
        if (bomb || full) {
            flag = 1;
        }
        if (!flag) {
            int x, y; // x = row, y = column
            printf("Coordinates ➤ ");
            scanf("%d %d", &x, &y);
            while (getchar() != '\n'); // clear stdin (just in case)
            while ((x > r - 1 || x < 0) || (y > c - 1 || y < 0)) {
                printf("Invalid input ➤ ");
                scanf("%d %d", &x, &y);
                while (getchar() != '\n'); // clear stdin (just in case)
            }
            *(*(stage + x) + y) = 1;
            if (*(*(table + x) + y) == -1) {
                bomb = 1;
            }
        } else if(bomb && flag){
            printf("Oh no! Now've done it!\n");
        } else {
            printf("Nice.\n");
        }
    }
    freeArray(&stage, r);
}

void createFile(int** table, int r, int c) {
    time_t curr = time(NULL);
    char* input = ctime(&curr);

    char* i = input; // Apparently, an extra '\n' seems to be there so this gets rid of it
    while (*i != '\0') { 
        if (*i == '\n') {
            *i = '\0';
        }
        i++;
    }

    char* filename = (char*)malloc(strlen(input) + 4 + 1); // why +4? Because ".txt", why +1? Because '\0'
    sprintf(filename, "%s.txt", input);
    FILE* tableFile = fopen(filename, "w+");
    if (!tableFile) {
        free(filename);
        return;
    }

    fprintf(tableFile, "Minesweeper game %s\n", input);
    for (int i = 0; i < r; i++) {
        fprintf(tableFile, "\t");
        for (int j = 0; j < c; j++) {
            fprintf(tableFile, "%2d ", *(*(table + i) + j));
        }
        fprintf(tableFile, "\n");
    }

    fclose(tableFile);
    free(filename);
}
