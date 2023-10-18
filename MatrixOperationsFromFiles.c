#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix_utils.h"

int main() {
    //Firstly, let's make our own file with matrix
    //WARNING! They will be transposed :)

    while (1) {
        printf("\nType in the matrix size (WIDTH HEIGHT):");
        int width, height;
        if (scanf("%d%d", &width, &height) != 2) {
            printf("Incorrect input");
            return 1;
        }
        if (height <= 0 || width <= 0) {
            printf("Incorrect input data");
            return 2;
        }

        struct matrix_t *matrixPointer = matrix_create_struct(width, height);
        if (matrixPointer == NULL) {
            printf("Failed to allocate memory");
            matrix_destroy_struct(&matrixPointer);
            return 8;
        }

        if (matrix_read(matrixPointer) == 2) {
            printf("Incorrect input");
            matrix_destroy_struct(&matrixPointer);
            return 1;
        }

        char *filename = calloc(40, sizeof(char));
        if (filename == NULL) {
            printf("Failed to allocate memory");
            matrix_destroy_struct(&matrixPointer);
            return 8;
        }
        printf("\nType in the filename in which you\nwant to save your matrix (.txt/.bin):");
        scanf("%39s", filename);
        while (getchar() != '\n');
        int length = (int) strlen(filename);

        if (length < 4) {
            free(filename);
            matrix_destroy_struct(&matrixPointer);
            printf("Unsupported file format");
            return 7;
        }

        if (*(filename + length - 1) == 'n' && *(filename + length - 2) == 'i'
            && *(filename + length - 3) == 'b' && *(filename + length - 4) == '.') {

            struct matrix_t *transposed = matrix_transpose(matrixPointer);
            if (transposed == NULL) {
                free(filename);
                printf("Failed to allocate memory");
                matrix_destroy_struct(&matrixPointer);
                return 8;
            }

            if (matrix_save_b(transposed, filename) == 2) {
                free(filename);
                matrix_destroy_struct(&transposed);
                matrix_destroy_struct(&matrixPointer);
                printf("Couldn't create file");
                return 5;
            } else {
                printf("\nFile saved");
                matrix_destroy_struct(&transposed);
            }

        } else if (*(filename + length - 1) == 't' && *(filename + length - 2) == 'x'
                   && *(filename + length - 3) == 't' && *(filename + length - 4) == '.') {

            struct matrix_t *transposed = matrix_transpose(matrixPointer);
            if (transposed == NULL) {
                free(filename);
                printf("Failed to allocate memory");
                matrix_destroy_struct(&matrixPointer);
                return 8;
            }

            if (matrix_save_t(transposed, filename) == 2) {
                free(filename);
                matrix_destroy_struct(&transposed);
                matrix_destroy_struct(&matrixPointer);
                printf("Couldn't create file");
                return 5;
            } else {
                printf("\nFile saved");
                matrix_destroy_struct(&transposed);
            }

        } else {
            free(filename);
            matrix_destroy_struct(&matrixPointer);
            printf("Unsupported file format");
            return 7;
        }

        free(filename);
        matrix_destroy_struct(&matrixPointer);

        printf("\n\nAre you done with creating files? (Y/N):");
        char choice;
        scanf("%c", &choice);
        if (choice == 'Y') {
            while (getchar() != '\n');
            break;
        } else if (choice == 'N') {
            while (getchar() != '\n');
            continue;
        } else {
            printf("Incorrect input");
            return 1;
        }
    }

    //Now, when we are done with making our matrix files, let's see some operations on them

    char *fileName1 = calloc(20, sizeof(char));
    if (fileName1 == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }
    printf("\nType in your first filename (.txt/.bin):");
    scanf("%19[^\n]s", fileName1);
    int length1 = (int) strlen(fileName1);
    int error1;
    struct matrix_t *firstFileMatrix;

    if (*(fileName1 + length1 - 4) == '.' && *(fileName1 + length1 - 3) == 't'
        && *(fileName1 + length1 - 2) == 'x' && *(fileName1 + length1 - 1) == 't') {
        firstFileMatrix = matrix_load_t(fileName1, &error1);
        switch (error1) {
            case 2:
                printf("Couldn't open file");
                free(fileName1);
                return 4;
            case 3:
                printf("File corrupted");
                free(fileName1);
                return 6;
            case 4:
                printf("Failed to allocate memory");
                free(fileName1);
                return 8;
            default:
                free(fileName1);
                break;
        }
    } else if (*(fileName1 + length1 - 4) == '.' && *(fileName1 + length1 - 3) == 'b'
               && *(fileName1 + length1 - 2) == 'i' && *(fileName1 + length1 - 1) == 'n') {
        firstFileMatrix = matrix_load_b(fileName1, &error1);

        switch (error1) {
            case 2:
                printf("Couldn't open file");
                free(fileName1);
                return 4;
            case 3:
                printf("File corrupted");
                free(fileName1);
                return 6;
            case 4:
                printf("Failed to allocate memory");
                free(fileName1);
                return 8;
            default:
                free(fileName1);
                break;
        }
    } else {
        free(fileName1);
        printf("Unsupported file format");
        return 7;
    }

    char *fileName2 = calloc(20, sizeof(char));
    if (fileName2 == NULL) {
        printf("Failed to allocate memory");
        matrix_destroy_struct(&firstFileMatrix);
        return 8;
    }
    printf("\nType in your second filename (.txt/.bin):");
    while (getchar() != '\n');
    scanf("%19[^\n]s", fileName2);
    int length2 = (int) strlen(fileName2);
    int error2;
    struct matrix_t *secondFileMatrix;

    if (*(fileName2 + length2 - 4) == '.' && *(fileName2 + length2 - 3) == 't'
        && *(fileName2 + length2 - 2) == 'x' && *(fileName2 + length2 - 1) == 't') {
        secondFileMatrix = matrix_load_t(fileName2, &error2);
        switch (error2) {
            case 2:
                printf("Couldn't open file");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 4;
            case 3:
                printf("File corrupted");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 6;
            case 4:
                printf("Failed to allocate memory");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 8;
            default:
                free(fileName2);
                break;
        }
    } else if (*(fileName2 + length2 - 4) == '.' && *(fileName2 + length2 - 3) == 'b'
               && *(fileName2 + length2 - 2) == 'i' && *(fileName2 + length2 - 1) == 'n') {
        secondFileMatrix = matrix_load_b(fileName2, &error2);
        switch (error2) {
            case 2:
                printf("Couldn't open file");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 4;
            case 3:
                printf("File corrupted");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 6;
            case 4:
                printf("Failed to allocate memory");
                matrix_destroy_struct(&firstFileMatrix);
                free(fileName2);
                return 8;
            default:
                free(fileName2);
                break;
        }
    } else {
        free(fileName2);
        matrix_destroy_struct(&firstFileMatrix);
        printf("Unsupported file format");
        return 7;
    }

    printf("\nAdd:\n");

    struct matrix_t *add = matrix_add(firstFileMatrix, secondFileMatrix);
    if (add == NULL) {
        printf("Error\n");
    } else {
        for (int i = 0; i < add->height; ++i) {
            for (int j = 0; j < add->width; ++j) {
                printf("%d ", *(*(add->ptr + i) + j));
            }
            printf("\n");
        }
        matrix_destroy_struct(&add);
    }

    printf("\nSubtract:\n");

    struct matrix_t *subtract = matrix_subtract(firstFileMatrix, secondFileMatrix);
    if (subtract == NULL) {
        printf("Error\n");
    } else {
        for (int i = 0; i < subtract->height; ++i) {
            for (int j = 0; j < subtract->width; ++j) {
                printf("%d ", *(*(subtract->ptr + i) + j));
            }
            printf("\n");
        }
        matrix_destroy_struct(&subtract);
    }

    printf("\nMultiply:\n");

    struct matrix_t *multiply = matrix_multiply(firstFileMatrix, secondFileMatrix);
    if (multiply == NULL) {
        printf("Error\n");
    } else {
        for (int i = 0; i < multiply->height; ++i) {
            for (int j = 0; j < multiply->width; ++j) {
                printf("%d ", *(*(multiply->ptr + i) + j));
            }
            printf("\n");
        }
        matrix_destroy_struct(&multiply);
    }

    printf("\nNow you can also see the files in cmake-build-debug folder");
    matrix_destroy_struct(&firstFileMatrix);
    matrix_destroy_struct(&secondFileMatrix);
    return 0;
}
