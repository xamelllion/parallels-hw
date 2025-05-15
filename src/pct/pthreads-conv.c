#include <pct/filters.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct convolution_task {
    unsigned char* pixel_array;
    struct pixel* result;
    int w;
    int h;
    struct filter filter;
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};

void* parallel_convolution(void* arg) {
    struct convolution_task* task = (struct convolution_task*)arg;
    struct pixel* image = (struct pixel*)task->pixel_array;

    for (int y = task->start_y; y < task->end_y; y++) {
        for (int x = task->start_x; x < task->end_x; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;

            for (int filterY = 0; filterY < task->filter.size; filterY++) {
                for (int filterX = 0; filterX < task->filter.size; filterX++) {
                    int imageX = (x - task->filter.size / 2 + filterX + task->w) % task->w;
                    int imageY = (y - task->filter.size / 2 + filterY + task->h) % task->h;

                    red +=
                        image[imageY * task->w + imageX].r * task->filter.filter[filterY][filterX];
                    green +=
                        image[imageY * task->w + imageX].g * task->filter.filter[filterY][filterX];
                    blue +=
                        image[imageY * task->w + imageX].b * task->filter.filter[filterY][filterX];
                }
            }

            task->result[y * task->w + x].r =
                min(max((int)(task->filter.factor * red + task->filter.bias), 0), 255);
            task->result[y * task->w + x].g =
                min(max((int)(task->filter.factor * green + task->filter.bias), 0), 255);
            task->result[y * task->w + x].b =
                min(max((int)(task->filter.factor * blue + task->filter.bias), 0), 255);
        }
    }

    return NULL;
}

// 1. Попиксельное разделение
void pixelwise_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                           int num_threads) {
    struct pixel* result = malloc(w * h * sizeof(struct pixel));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int pixels_per_thread = (w * h) / num_threads;
    int extra_pixels = (w * h) % num_threads;

    int current_pixel = 0;
    for (int i = 0; i < num_threads; i++) {
        int pixels_for_this_thread = pixels_per_thread + (i < extra_pixels ? 1 : 0);

        tasks[i].pixel_array = pixel_array;
        tasks[i].result = result;
        tasks[i].w = w;
        tasks[i].h = h;
        tasks[i].filter = filter;

        // Вычисляем координаты для каждого пикселя
        int end_pixel = current_pixel + pixels_for_this_thread;
        tasks[i].start_x = current_pixel % w;
        tasks[i].start_y = current_pixel / w;
        tasks[i].end_x = end_pixel % w;
        tasks[i].end_y = end_pixel / w;

        // Корректировка для последнего пикселя
        if (i == num_threads - 1) {
            tasks[i].end_x = w;
            tasks[i].end_y = h;
        }

        pthread_create(&threads[i], NULL, parallel_convolution, &tasks[i]);
        current_pixel = end_pixel;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    memcpy(pixel_array, result, w * h * sizeof(struct pixel));
    free(result);
    free(threads);
    free(tasks);
}

// 2. Разделение по строкам
void row_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                     int num_threads) {
    struct pixel* result = malloc(w * h * sizeof(struct pixel));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int rows_per_thread = h / num_threads;
    int extra_rows = h % num_threads;

    for (int i = 0; i < num_threads; i++) {
        tasks[i].pixel_array = pixel_array;
        tasks[i].result = result;
        tasks[i].w = w;
        tasks[i].h = h;
        tasks[i].filter = filter;
        tasks[i].start_x = 0;
        tasks[i].end_x = w;

        tasks[i].start_y = i * rows_per_thread;
        tasks[i].end_y = tasks[i].start_y + rows_per_thread;

        if (i == num_threads - 1) {
            tasks[i].end_y += extra_rows;
        }

        pthread_create(&threads[i], NULL, parallel_convolution, &tasks[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    memcpy(pixel_array, result, w * h * sizeof(struct pixel));
    free(result);
    free(threads);
    free(tasks);
}

// 3. Разделение по столбцам
void column_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                        int num_threads) {
    struct pixel* result = malloc(w * h * sizeof(struct pixel));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int cols_per_thread = w / num_threads;
    int extra_cols = w % num_threads;

    for (int i = 0; i < num_threads; i++) {
        tasks[i].pixel_array = pixel_array;
        tasks[i].result = result;
        tasks[i].w = w;
        tasks[i].h = h;
        tasks[i].filter = filter;
        tasks[i].start_y = 0;
        tasks[i].end_y = h;

        tasks[i].start_x = i * cols_per_thread;
        tasks[i].end_x = tasks[i].start_x + cols_per_thread;

        if (i == num_threads - 1) {
            tasks[i].end_x += extra_cols;
        }

        pthread_create(&threads[i], NULL, parallel_convolution, &tasks[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    memcpy(pixel_array, result, w * h * sizeof(struct pixel));
    free(result);
    free(threads);
    free(tasks);
}

// 4. Произвольная прямоугольная решётка
void grid_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                      int num_threads) {
    struct pixel* result = malloc(w * h * sizeof(struct pixel));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    // Вычисляем оптимальное разделение на прямоугольники
    int grid_width = 1;
    int grid_height = num_threads;

    // Ищем наиболее квадратное разбиение
    for (int i = 1; i <= num_threads; i++) {
        if (num_threads % i == 0) {
            int other = num_threads / i;
            if (abs(i - other) < abs(grid_width - grid_height)) {
                grid_width = i;
                grid_height = other;
            }
        }
    }

    int block_width = w / grid_width;
    int block_height = h / grid_height;
    int extra_width = w % grid_width;
    int extra_height = h % grid_height;

    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            int thread_idx = i * grid_height + j;
            if (thread_idx >= num_threads) break;

            tasks[thread_idx].pixel_array = pixel_array;
            tasks[thread_idx].result = result;
            tasks[thread_idx].w = w;
            tasks[thread_idx].h = h;
            tasks[thread_idx].filter = filter;

            tasks[thread_idx].start_x = i * block_width;
            tasks[thread_idx].end_x = tasks[thread_idx].start_x + block_width;
            if (i == grid_width - 1) tasks[thread_idx].end_x += extra_width;

            tasks[thread_idx].start_y = j * block_height;
            tasks[thread_idx].end_y = tasks[thread_idx].start_y + block_height;
            if (j == grid_height - 1) tasks[thread_idx].end_y += extra_height;

            pthread_create(&threads[thread_idx], NULL, parallel_convolution, &tasks[thread_idx]);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    memcpy(pixel_array, result, w * h * sizeof(struct pixel));
    free(result);
    free(threads);
    free(tasks);
}
