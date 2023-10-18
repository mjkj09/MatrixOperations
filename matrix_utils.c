//
// Created by Mikołaj on 03.04.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "matrix_utils.h"

int matrix_create(struct matrix_t *m, int width, int height) {
    if (m == NULL || width <= 0 || height <= 0) return 1;

    m->height = height;
    m->width = width;

    m->ptr = calloc(height, sizeof(int *));
    if (m->ptr == NULL) return 2;

    for (int i = 0; i < height; ++i) {
        *(m->ptr + i) = calloc(width, sizeof(int));
        if (*(m->ptr + i) == NULL) {
            for (int j = 0; j < height; ++j) {
                free(*(m->ptr + j));
            }
            free(m->ptr);
            return 2;
        }
    }

    return 0;
}

int matrix_read(struct matrix_t *m) {
    if (m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0) return 1;

    printf("\nGive me numbers for the matrix:");
    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            if (scanf("%d", (*(m->ptr + i) + j)) != 1) {
                return 2;
            }
        }
    }
    while (getchar() != '\n');

    return 0;
}

void matrix_display(const struct matrix_t *m) {
    if (m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0) return;

    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            printf("%d ", *(*(m->ptr + i) + j));
        }
        if (i != m->height - 1) printf("\n");
    }
}

void matrix_destroy(struct matrix_t *m) {
    if (m == NULL) return;

    for (int j = 0; j < m->height; ++j) {
        free(*(m->ptr + j));
    }
    free(m->ptr);
}

struct matrix_t *matrix_create_struct(int width, int height) {
    if (width <= 0 || height <= 0) return NULL;

    struct matrix_t *m = calloc(1, sizeof(struct matrix_t));
    if (m == NULL) return NULL;

    switch (matrix_create(m, width, height)) {
        case 0:
            return m;
        default:
            free(m);
            return NULL;
    }
}

void matrix_destroy_struct(struct matrix_t **m) {
    if (m == NULL || *m == NULL) return;

    if ((*m)->height <= 0 || (*m)->width <= 0) return;

    matrix_destroy(*m);
    free(*m);
    *m = NULL;
}

struct matrix_t *matrix_transpose(const struct matrix_t *m) {
    if (m == NULL || m->ptr == NULL) return NULL;

    int newwidth = m->height;
    int newheight = m->width;
    struct matrix_t *transposed = matrix_create_struct(newwidth, newheight);
    if (transposed == NULL) return NULL;

    for (int i = 0; i < newheight; ++i) {
        for (int j = 0; j < newwidth; ++j) {
            *(*(transposed->ptr + i) + j) = *(*(m->ptr + j) + i);
        }
    }

    return transposed;
}

int matrix_save_b(const struct matrix_t *m, const char *filename) {
    if (m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0
        || filename == NULL)
        return 1;

    FILE *f = fopen(filename, "wb");
    if (f == NULL) return 2;

    if (fwrite(&m->width, sizeof(int), 1, f) != 1) {
        fclose(f);
        return 3;
    }
    if (fwrite(&m->height, sizeof(int), 1, f) != 1) {
        fclose(f);
        return 3;
    }

    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            if (fwrite(*(m->ptr + i) + j, sizeof(int), 1, f) != 1) {
                fclose(f);
                return 3;
            }
        }
    }

    fclose(f);
    return 0;
}

int matrix_save_t(const struct matrix_t *m, const char *filename) {
    if (m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0
        || filename == NULL)
        return 1;

    FILE *f = fopen(filename, "wt");
    if (f == NULL) return 2;

    if (fprintf(f, "%d %d\n", m->width, m->height) < 0) {
        fclose(f);
        return 3;
    }

    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            if (fprintf(f, "%d ", *(*(m->ptr + i) + j)) < 0) {
                fclose(f);
                return 3;
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}

struct matrix_t *matrix_copy(const struct matrix_t *copy) {
    if (copy == NULL || copy->ptr == NULL) return NULL;

    struct matrix_t *copied = matrix_create_struct(copy->width, copy->height);
    if (copied == NULL) return NULL;

    for (int i = 0; i < copy->height; ++i) {
        for (int j = 0; j < copy->width; ++j) {
            *(*(copied->ptr + i) + j) = *(*(copy->ptr + i) + j);
        }
    }

    return copied;
}

struct matrix_t *matrix_load_b(const char *filename, int *err_code) {
    if (filename == NULL) {
        if (err_code != NULL) {
            *err_code = 1;
            return NULL;
        } else {
            return NULL;
        }
    }

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        if (err_code != NULL) {
            *err_code = 2;
            return NULL;
        } else {
            return NULL;
        }
    }

    int width, height;

    if (fread(&width, sizeof(int), 1, f) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }
    if (fread(&height, sizeof(int), 1, f) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    if (width <= 0 || height <= 0) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    struct matrix_t *output = matrix_create_struct(width, height);
    if (output == NULL) {
        if (err_code != NULL) {
            *err_code = 4;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (fread(*(output->ptr + i) + j, sizeof(int), 1, f) != 1) {
                if (err_code != NULL) {
                    *err_code = 3;
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                } else {
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                }
            }
        }
    }

    if (err_code != NULL) *err_code = 0;

    fclose(f);
    return output;
}

struct matrix_t *matrix_load_t(const char *filename, int *err_code) {
    if (filename == NULL) {
        if (err_code != NULL) {
            *err_code = 1;
            return NULL;
        } else {
            return NULL;
        }
    }

    FILE *f = fopen(filename, "rt");
    if (f == NULL) {
        if (err_code != NULL) {
            *err_code = 2;
            return NULL;
        } else {
            return NULL;
        }
    }

    int width, height;

    if (fscanf(f, "%d", &width) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }
    if (fgetc(f) != ' ') {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }
    if (fscanf(f, "%d", &height) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }
    if (fgetc(f) != '\n') {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    if (width <= 0 || height <= 0) {
        if (err_code != NULL) {
            *err_code = 3;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    struct matrix_t *output = matrix_create_struct(width, height);
    if (output == NULL) {
        if (err_code != NULL) {
            *err_code = 4;
            fclose(f);
            return NULL;
        } else {
            fclose(f);
            return NULL;
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (fscanf(f, "%d", *(output->ptr + i) + j) != 1) {
                if (err_code != NULL) {
                    *err_code = 3;
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                } else {
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                }
            }
            if (fgetc(f) != ' ') {
                if (err_code != NULL) {
                    *err_code = 3;
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                } else {
                    fclose(f);
                    matrix_destroy_struct(&output);
                    return NULL;
                }
            }
        }
        if (fgetc(f) != '\n') {
            if (err_code != NULL) {
                *err_code = 3;
                fclose(f);
                matrix_destroy_struct(&output);
                return NULL;
            } else {
                fclose(f);
                matrix_destroy_struct(&output);
                return NULL;
            }
        }
    }

    if (err_code != NULL) *err_code = 0;
    fclose(f);
    return output;
}

struct matrix_t *matrix_add(const struct matrix_t *m1, const struct matrix_t *m2) {
    if (m1 == NULL || m1->ptr == NULL || m2 == NULL || m2->ptr == NULL
        || m1->height != m2->height || m1->width != m2->width)
        return NULL;

    struct matrix_t *sum = matrix_create_struct(m1->width, m1->height);
    if (sum == NULL) return NULL;

    for (int i = 0; i < m1->height; ++i) {
        for (int j = 0; j < m1->width; ++j) {
            *(*(sum->ptr + i) + j) = *(*(m1->ptr + i) + j) + *(*(m2->ptr + i) + j);
        }
    }

    return sum;
}

struct matrix_t *matrix_subtract(const struct matrix_t *m1, const struct matrix_t *m2) {
    if (m1 == NULL || m1->ptr == NULL || m2 == NULL || m2->ptr == NULL
        || m1->height != m2->height || m1->width != m2->width)
        return NULL;

    struct matrix_t *minus = matrix_create_struct(m1->width, m1->height);
    if (minus == NULL) return NULL;

    for (int i = 0; i < m1->height; ++i) {
        for (int j = 0; j < m1->width; ++j) {
            *(*(minus->ptr + i) + j) = *(*(m1->ptr + i) + j) - *(*(m2->ptr + i) + j);
        }
    }

    return minus;
}

struct matrix_t *matrix_multiply(const struct matrix_t *m1, const struct matrix_t *m2) {
    if (m1 == NULL || m1->ptr == NULL || m2 == NULL || m2->ptr == NULL
        || m1->width != m2->height)
        return NULL;

    int newwidth = m1->height;
    int newheight = m2->width;

    struct matrix_t *multiplied = matrix_create_struct(newwidth, newheight);
    if (multiplied == NULL) return NULL;

    for (int i = 0; i < newheight; ++i) {
        for (int j = 0; j < newwidth; ++j) {
            for (int k = 0; k < m2->height; ++k) {
                *(*(multiplied->ptr + i) + j) += (*(*(m1->ptr + i) + k)) * (*(*(m2->ptr + k) + j));
            }
        }
    }

    return multiplied;
}
