#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_ROOMS 5
#define NUM_COMPUTERS 3
pthread_mutex_t room_mutex[NUM_ROOMS];
pthread_mutex_t computer_mutex[NUM_COMPUTERS];

sem_t room_sem;
sem_t computer_sem;

void* reserve_study_room(void* arg) {
    int id = *(int*)arg;
    int room_id;
    sem_wait(&room_sem);
    for (room_id = 0; room_id < NUM_ROOMS; room_id++) {
        if (pthread_mutex_trylock(&room_mutex[room_id]) == 0) {
            printf("User %d reserved study room %d.\n", id, room_id);
            sleep(rand() % 3 + 1);
            pthread_mutex_unlock(&room_mutex[room_id]);
            printf("User %d released study room %d.\n", id, room_id);
            break;
        }
    }
    sem_post(&room_sem);

    return NULL;
}

void* reserve_computer(void* arg) {
    int id = *(int*)arg;
    int computer_id;

    sem_wait(&computer_sem);
    for (computer_id = 0; computer_id < NUM_COMPUTERS; computer_id++) {
        if (pthread_mutex_trylock(&computer_mutex[computer_id]) == 0) {
            printf("User %d reserved computer %d.\n", id, computer_id);
            sleep(rand() % 3 + 1); 
            pthread_mutex_unlock(&computer_mutex[computer_id]);
            printf("User %d released computer %d.\n", id, computer_id);
            break;
        }
    }
    sem_post(&computer_sem);

    return NULL;
}

int main() {
    pthread_t threads[10];
    int user_ids[10];


    for (int i = 0; i < NUM_ROOMS; i++) {
        pthread_mutex_init(&room_mutex[i], NULL);
    }
    for (int i = 0; i < NUM_COMPUTERS; i++) {
        pthread_mutex_init(&computer_mutex[i], NULL);
    }
    sem_init(&room_sem, 0, NUM_ROOMS);
    sem_init(&computer_sem, 0, NUM_COMPUTERS);


    for (int i = 0; i < 10; i++) {
        user_ids[i] = i + 1;
        if (rand() % 2) {
            pthread_create(&threads[i], NULL, reserve_study_room, &user_ids[i]);
        } else {
            pthread_create(&threads[i], NULL, reserve_computer, &user_ids[i]);
        }
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }


    for (int i = 0; i < NUM_ROOMS; i++) {
        pthread_mutex_destroy(&room_mutex[i]);
    }
    for (int i = 0; i < NUM_COMPUTERS; i++) {
        pthread_mutex_destroy(&computer_mutex[i]);
    }
    sem_destroy(&room_sem);
    sem_destroy(&computer_sem);

    return 0;
}
 
