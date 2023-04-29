//Includes/Namespace://
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
//#include <stdio.h>
#include <unistd.h>

#include <mutex> //new

using namespace std;

//Defines://
#define N 5

#define STR(s) #s
#define TO_STR(s) STR(s)

#define THINKING 3 //2
#define EATING 2 //0
#define HUNGRY 1
#define DEAD 0

#define MAX_STARVATION_TIME 10

#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

//Global Variables://
int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
int starvation_time[N] = {0, 0, 0, 0, 0}; // counter for each philosopher's starvation time

mutex cout_mutex; //new

sem_t sem_mutex;
sem_t S[N];


int counter = 0;
int num = 0;
//Functions://

void test(int phnum) {
  /*
  counter++;

  cout << "times entering the test function:" << counter << endl; //only entering test funct\
ion 7 times???

  std::lock_guard<std::mutex> lock(cout_mutex); //new

  for (int i = 0; i < 5; i++) {
    cout << "Philosopher " << (i + 1) << "[" << starvation_time[phnum]++ << "]: ";

    if (starvation_time[phnum] == 0) {
      cout << "Hunger meter: [|||||||||||||||||||||] > Well-fed :)" << endl;
    } else if (starvation_time[phnum] == 1) {
      cout << "Hunger meter: [--|||||||||||||||||||] > Well-fed :)" << endl;//flush;
    } else if (starvation_time[phnum] == 2) {
      cout << "Hunger meter: [----|||||||||||||||||] > Well-fed :)" << endl;//flush;
    } else if (starvation_time[phnum] == 3) {
      cout << "Hunger meter: [------|||||||||||||||] >   Hungry :|" << endl; //flush;
    } else if (starvation_time[phnum] == 4) {
      cout << "Hunger meter: [-------||||||||||||||] >   Hungry :|" << endl; //flush;
    } else if (starvation_time[phnum] == 5) {
      cout << "Hunger meter: [----------|||||||||||] >   Hungry :|" << endl; //flush;
    } else if (starvation_time[phnum] == 6) {
      cout << "Hunger meter: [------------|||||||||] > Very hungry :(" << endl; //flush;
    } else if (starvation_time[phnum] == 7) {
      cout << "Hunger meter: [--------------|||||||] > Very hungry :(" << endl; //flush;
    } else if (starvation_time[phnum] == 8) {
      cout << "Hunger meter: [-----------------||||] > Very hungry :(" << endl; //flush;
    } else if (starvation_time[phnum] == 9) {
      cout << "Hunger meter: [-------------------||] > Starving! :,(" << endl; //flush;
    } else if (starvation_time[phnum] >= 10) {
      cout << "Hunger meter: [---------------------] > Died from death (x_x)" << endl; //flu\
sh;
    }
  }

  cout << "\033[5A";

  //  cout << "\033[5B";


  if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    state[phnum] = EATING; //state that eating;

    sleep(2);

    //    cout << "Philosopher " << (phnum + 1) << " takes fork " << (LEFT + 1) << " and " <\
< (phnum + 1) << endl;
    //cout << "Philosopher " << (phnum + 1) << " is eating" << endl;

    sem_post(&S[phnum]); //used to wake up hungry philosophers during putfork
  }

  if (state[phnum] == HUNGRY) { //check for starvation;
    starvation_time[phnum]++;

    //cout << "Philosopher " << (phnum + 1) << "[" << starvation_time[phnum]++ << "]: ";
    //cout << "[" << starvation_time[phnum]++ << "]: ";

    if (starvation_time[phnum] >= MAX_STARVATION_TIME) {
      //cout << "Philosopher " << (phnum + 1) << " died from starvation!" << endl;
      state[phnum] = DEAD;
      pthread_exit(NULL);
    }
  }
  */
}

void take_fork(int phnum) { //take up fork;
  sem_wait(&sem_mutex);

  state[phnum] = HUNGRY; //state that hungry;

  //cout << "Philosopher " << (phnum + 1) << " is hungry" << endl;

  test(phnum); //eat if neighbours aren't eating;
  sem_post(&sem_mutex);
  sem_wait(&S[phnum]); //if unable to eat, wait to be signalled to eat;

  sleep(1);
}

void put_fork(int phnum) { //put down fork;
  sem_wait(&sem_mutex);

  state[phnum] = THINKING; //state that thinking;

  cout << num++;

  //cout << "Philosopher " << (phnum + 1) << " is putting down forks " << (LEFT + 1) << " an\
d " << (phnum + 1) << endl;
  //cout << "Philosopher " << (phnum + 1) << " is thinking" << endl;

  test(LEFT);
  test(RIGHT);

  sem_post(&sem_mutex);
}

void* philosopher(void* num) {
  while (1) {
    int* i = static_cast<int*>(num); //    int* i = num doesn't work, needs to be static_cas\
t;

    sleep(5);
    take_fork(*i);
    sleep(0);
    put_fork(*i);
  }
}

//Main Function://
int main() {
  int i;
  pthread_t thread_id[N];

  sem_init(&sem_mutex, 0, 1); //initialize the semaphore;

  for (i = 0; i < N; i++) {
    sem_init(&S[i], 0, 0);
  }

  for (i = 0; i < N; i++) {
    pthread_create(&thread_id[i], NULL, philosopher, &phil[i]); //creating philosopher proce\
sses;
    //    cout << "Philosopher " << (i + 1) << " is thinking" << endl;
  }

  for (i = 0; i < N; i++) {
    pthread_join(thread_id[i], NULL);
  }

  cout << "\033[5B";

  //Return://
  return 0;
}
