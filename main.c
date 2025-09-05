//Sorry for not commenting, i don't do that much. Also this is my first real program so sorry if it's a bit messy.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_USAGE 100
#define MAX_STR_LEN 22
#define FILE_NAME "/home/deck/CLionProjects/pasword/Passwordlist.txt"

void generate_random_string(char *str, int length) {
    for (int i = 0; i < length; i++) {
        str[i] = (rand() % (126 - 32) + 1) + 32;
    }
    str[length] = '\0';
}

void print_file() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No entries yet.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);
    }
    fclose(file);
}

void add_entry() {
    char usage[MAX_USAGE];
    char random_str[MAX_STR_LEN + 1];

    printf("Enter usage: ");
    if (!fgets(usage, sizeof(usage), stdin)) return;
    usage[strcspn(usage, "\n")] = '\0';

    generate_random_string(random_str, MAX_STR_LEN);

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) return;

    fprintf(file, "%s : %s\n", usage, random_str);
    fclose(file);

    printf("Saved: %s : %s\n", usage, random_str);
}

void change_password() {
    char usage[MAX_USAGE];
    char new_password[MAX_STR_LEN + 1];

    printf("Enter usage to change: ");
    if (!fgets(usage, sizeof(usage), stdin)) return;
    usage[strcspn(usage, "\n")] = '\0';

    printf("Enter new password (leave empty to generate random): ");
    fgets(new_password, sizeof(new_password), stdin);
    new_password[strcspn(new_password, "\n")] = '\0';

    if (strlen(new_password) == 0) generate_random_string(new_password, MAX_STR_LEN);

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No entries found.\n");
        return;
    }

    char **lines = NULL;
    size_t count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        lines = realloc(lines, (count + 1) * sizeof(char *));
        lines[count] = strdup(buffer);
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }
    fclose(file);

    int found = 0;
    for (size_t i = 0; i < count; i++) {
        char *sep = strstr(lines[i], " : ");
        if (sep) {
            *sep = '\0';
            if (strcmp(lines[i], usage) == 0) {
                sprintf(lines[i], "%s : %s", usage, new_password);
                found = 1;
                break;
            } else {
                *sep = ':';
            }
        }
    }

    if (!found) {
        lines = realloc(lines, (count + 1) * sizeof(char *));
        lines[count] = malloc(256);
        sprintf(lines[count], "%s : %s", usage, new_password);
        count++;
    }

    file = fopen(FILE_NAME, "w");
    if (!file) return;
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "%s\n", lines[i]);
        free(lines[i]);
    }
    free(lines);
    fclose(file);

    printf("Password updated.\n");
}

void delete_entry() {
    char usage[MAX_USAGE];
    printf("Enter usage to delete: ");
    if (!fgets(usage, sizeof(usage), stdin)) return;
    usage[strcspn(usage, "\n")] = '\0';

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No entries found.\n");
        return;
    }

    char **lines = NULL;
    size_t count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        lines = realloc(lines, (count + 1) * sizeof(char *));
        lines[count] = strdup(buffer);
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }
    fclose(file);

    size_t new_count = 0;
    for (size_t i = 0; i < count; i++) {
        char *sep = strstr(lines[i], " : ");
        if (sep) *sep = '\0';
        if (strcmp(lines[i], usage) != 0) {
            if (sep) *sep = ':';
            lines[new_count++] = lines[i];
        } else {
            free(lines[i]);
        }
    }

    if (new_count == count) {
        printf("Usage not found.\n");
        free(lines);
        return;
    }

    file = fopen(FILE_NAME, "w");
    if (!file) return;
    for (size_t i = 0; i < new_count; i++) fprintf(file, "%s\n", lines[i]);
    fclose(file);

    for (size_t i = 0; i < new_count; i++) free(lines[i]);
    free(lines);

    printf("Entry deleted successfully.\n");
}

int main() {
    srand(time(NULL));

    printf("\033[1;31mDO NOT CALL TWO PASSWORDS THE SAME NAME!\033[0m\n");

    while (1) {
        printf("\nOptions:\n1) Add entry\n2) Change password\n3) Show all entries\n4) Delete entry\n5) Exit\nChoose: ");
        int choice;
        if (scanf("%d", &choice) != 1) break;
        while (getchar() != '\n');

        switch (choice) {
            case 1: add_entry(); break;
            case 2: change_password(); break;
            case 3: print_file(); break;
            case 4: delete_entry(); break;
            case 5: return 0;
            default: printf("Invalid choice.\n"); break;
        }
    }
    return 0;
}
