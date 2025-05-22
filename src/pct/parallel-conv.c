#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pct/filters.h>
#include <pct/utils.h>

void* parallel_convolution(void* arg) {
    struct convolution_task* task = (struct convolution_task*)arg;

    for (int y = task->start_y; y < task->end_y; y++) {
        for (int x = task->start_x; x < task->end_x; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;

            for (int filterY = 0; filterY < task->filter.size; filterY++) {
                for (int filterX = 0; filterX < task->filter.size; filterX++) {
                    int imageX = (x - task->filter.size / 2 + filterX + task->w) % task->w;
                    int imageY = (y - task->filter.size / 2 + filterY + task->h) % task->h;

                    red += task->image[imageY * task->w + imageX].r *
                           task->filter.filter[filterY][filterX];
                    green += task->image[imageY * task->w + imageX].g *
                             task->filter.filter[filterY][filterX];
                    blue += task->image[imageY * task->w + imageX].b *
                            task->filter.filter[filterY][filterX];
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

void pixelwise_convolution(struct image_info* info, struct filter filter, int num_threads) {
    info->result = malloc(info->width * info->height * sizeof(struct rgb_image));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    const int w = info->width;
    const int h = info->height;
    const int total_pixels = w * h;

    int pixels_per_thread = total_pixels / num_threads;
    int remaining_pixels = total_pixels % num_threads;

    int start_pixel = 0;
    for (int i = 0; i < num_threads; i++) {
        int end_pixel = start_pixel + pixels_per_thread + (i < remaining_pixels ? 1 : 0);

        tasks[i].start_x = start_pixel % w;
        tasks[i].start_y = start_pixel / w;
        tasks[i].end_x = (end_pixel - 1) % w + 1;
        tasks[i].end_y = end_pixel / w;

        if (tasks[i].end_y >= h) {
            tasks[i].end_y = h;
            tasks[i].end_x = w;
        }

        tasks[i].image = info->image;
        tasks[i].result = info->result;
        tasks[i].w = w;
        tasks[i].h = h;
        tasks[i].filter = filter;

        pthread_create(&threads[i], NULL, parallel_convolution, &tasks[i]);

        start_pixel = end_pixel;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(tasks);
}

void row_convolution(struct image_info* info, struct filter filter, int num_threads) {
    info->result = malloc(info->width * info->height * sizeof(struct rgb_image));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int w = info->width;
    int h = info->height;

    int rows_per_thread = h / num_threads;
    int extra_rows = h % num_threads;

    for (int i = 0; i < num_threads; i++) {
        tasks[i].image = info->image;
        tasks[i].result = info->result;
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

    free(threads);
    free(tasks);
}

void column_convolution(struct image_info* info, struct filter filter, int num_threads) {
    info->result = malloc(info->width * info->height * sizeof(struct rgb_image));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int w = info->width;
    int h = info->height;

    int cols_per_thread = w / num_threads;
    int extra_cols = w % num_threads;

    for (int i = 0; i < num_threads; i++) {
        tasks[i].image = info->image;
        tasks[i].result = info->result;
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

    free(threads);
    free(tasks);
}

void grid_convolution(struct image_info* info, struct filter filter, int num_threads) {
    info->result = malloc(info->width * info->height * sizeof(struct rgb_image));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct convolution_task* tasks = malloc(num_threads * sizeof(struct convolution_task));

    int w = info->width;
    int h = info->height;

    int grid_width = 1;
    int grid_height = num_threads;

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

            tasks[thread_idx].image = info->image;
            tasks[thread_idx].result = info->result;
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

    free(threads);
    free(tasks);
}

void parallel_run(const struct pct_options options, const struct filter* filters) {
    struct image_info info = load_image(options.read_path);

    double start_time = get_time_in_seconds();
    if (options.mode == pixel_mode) {
        pixelwise_convolution(&info, filters[options.filter_type], options.threads);
    } else if (options.mode == row_mode) {
        row_convolution(&info, filters[options.filter_type], options.threads);
    } else if (options.mode == column_mode) {
        column_convolution(&info, filters[options.filter_type], options.threads);
    } else if (options.mode == grid_mode) {
        grid_convolution(&info, filters[options.filter_type], options.threads);
    }
    double end_time = get_time_in_seconds();

    if (options.log) {
        printf("convolution time: %lf\n", end_time - start_time);
    }

    dump_image(options.write_path, info);
}
