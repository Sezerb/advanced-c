#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CLI 10
#define MAX_TELLER 3
#define MAX_CLI_THREAD MAX_CLI
#define MAX_TELLER_THREAD MAX_TELLER
#define MAX_SEAT 10
#define AVAIL 1
#define NAVAIL 0

#define LONG_WAIT (1000*1000)
#define SHORT_WAIT (100*1000)


void exit_sys(const char *msg);
void exit_fail(const char *msg);
void exit_sys_errno(const char *msg, int errno);

void *client_thread_proc(void *param);
void *teller_thread_proc(void *param);

size_t allocate_booth(size_t* id);
void deallocate_booth(size_t booth_id);
size_t process_request();
size_t is_theater_full();

size_t g_booth_arr[MAX_TELLER] = {AVAIL, AVAIL, AVAIL};
/** Client mutexes */
pthread_mutex_t g_mutex_booth;
pthread_mutex_t g_mutex_ticketOp;

/** Teller mutexes */
pthread_mutex_t g_mutex_teller;

typedef struct tagCLI_THREAD_PARAM {
    char name[256];
    int arrivalTime;
    int serviceTime;
    int seatNumber;
} CLI_THREAD_PARAM;

typedef enum tagEN_PROCESS_T {
    E_IDLE = 0,
    E_REQUESTED = 1,
    E_PROCESSING = 2,
    E_RESPONDED = 3
}EN_PROCESS_T;

/* Communication channel between client and teller */
typedef struct tagBANKO {
    char name[256];
    int serviceTime;
    int seatNumber;
    EN_PROCESS_T processType;
}BANKO;

BANKO g_banko_arr[MAX_TELLER] = { 
    {"", 0, 0, E_IDLE },
    {"", 0, 0, E_IDLE},
    {"", 0, 0, E_IDLE}
};

size_t *g_seat_arr;

/* Input for Test */
static const CLI_THREAD_PARAM testInput[] = {
    {"Client1", 10, 50, 5},
    {"Client2", 20, 20, 1},
    {"Client3", 30, 20, 5},
    {"Client4", 45, 10, 1},
    {"Client5", 65, 10, 5},
    {"Client6", 70, 10, 10},
    {"Client7", 72, 10, 10},
    {"Client8", 100, 20, 10},
    {"Client9", 95, 20, 10},
    {"Client10", 90, 20, 10}
} ;
/* Expected Output ----->

Welcome to the Sync-Ticket!
Teller-0 has arrived with id:0
Teller-1 has arrived with id:1
Teller-2 has arrived with id:2
Client2 requests seat 1, reserves seat 1, Signed by Teller-1
Client3 requests seat 5, reserves seat 2, Signed by Teller-2
Client4 requests seat 1, reserves seat 3, Signed by Teller-1
Client1 requests seat 5, reserves seat 5, Signed by Teller-0
Client5 requests seat 5, reserves seat 4, Signed by Teller-1
Client6 requests seat 10, reserves seat 10, Signed by Teller-0
Client7 requests seat 10, reserves seat 6, Signed by Teller-2
Client10 requests seat 10, reserves seat 7, Signed by Teller-0
Client9 requests seat 10, reserves seat 8, Signed by Teller-1
Client8 requests seat 10, reserves seat 9, Signed by Teller-2
All clients received!
---------------------------->*/

int main(int argc, char **argv)
{
    pthread_t teller_tids[MAX_TELLER_THREAD];
    pthread_t cli_tids[MAX_CLI_THREAD];
    int result;
    int i;
    int *id;
    
    printf("\nWelcome to the Sync-Ticket!");

    /* Create Seats */
    if ((g_seat_arr = (size_t *) malloc(MAX_SEAT * sizeof(size_t))) == NULL)
        exit_fail("Can not allocate memory\n");

    for(i = 0; i < MAX_SEAT; i++) {
        g_seat_arr[i] = 1;
    }


    /* Create Teller Threads */
    for (i = 0; i < MAX_TELLER_THREAD; ++i)
    {
        id = i;
        if ((result = pthread_create(&teller_tids[i], NULL, teller_thread_proc, id)) != 0)
            exit_sys_errno("pthread_create", result);

        usleep(SHORT_WAIT);
    }
    

    /* Wait for a while now */
    usleep(LONG_WAIT);

    /* Create Client Threads */
    for (i = 0; i < MAX_CLI_THREAD; ++i) 
    {
        CLI_THREAD_PARAM *clientParam;
    
        if ((clientParam = (CLI_THREAD_PARAM *)malloc(sizeof(CLI_THREAD_PARAM))) == NULL)
            exit_fail("can not allocate memory\n");
        
        memcpy(clientParam, &testInput[i], sizeof(CLI_THREAD_PARAM));
        
        if ((result = pthread_create(&cli_tids[i], NULL, client_thread_proc, clientParam)) != 0)
            exit_sys_errno("pthread_create", result);
    }       
   

    for (i = 0; i < MAX_TELLER_THREAD; ++i) 
        if ((result = pthread_join(teller_tids[i], NULL)) != 0)
            exit_sys_errno("pthread_join", result); 

    for (i = 0; i < MAX_CLI_THREAD; ++i) 
        if ((result = pthread_join(cli_tids[i], NULL)) != 0)
            exit_sys_errno("pthread_join", result);             


    if ((result = pthread_mutex_destroy(&g_mutex_booth)) != 0)
        exit_sys_errno("pthread_mutex_destroy", result);

    if ((result = pthread_mutex_destroy(&g_mutex_ticketOp)) != 0)
        exit_sys_errno("pthread_mutex_destroy", result);

    if ((result = pthread_mutex_destroy(&g_mutex_teller)) != 0)
        exit_sys_errno("pthread_mutex_destroy", result);

    printf("\nAll clients received!\n");     

    return 0;
}


void *client_thread_proc(void *param)
{
    CLI_THREAD_PARAM *clientParam = (CLI_THREAD_PARAM *)param;
    size_t booth_id;
    int result;

    // printf("\n name:%s", clientParam->name);
    // printf("\n arrival time:%d", clientParam->arrivalTime);
    // printf("\n service time:%d", clientParam->serviceTime);
    // printf("\n seat:%d", clientParam->seatNumber);

    /* Wait for arrival time */
    usleep(clientParam->arrivalTime * 1000);


    /* First find a Teller booth for yourself */
    if ((result = pthread_mutex_lock(&g_mutex_booth)) != 0)
        exit_sys_errno("pthread_mutex_lock", result);

    for (;;) {

        if(allocate_booth(&booth_id)) {
            
            break;
        }
    }

    if ((result = pthread_mutex_unlock(&g_mutex_booth)) != 0)
        exit_sys_errno("pthread_mutex_lock", result);


    /* Demand a seat from teller */
    if ((result = pthread_mutex_lock(&g_mutex_ticketOp)) != 0)
        exit_sys_errno("pthread_mutex_lock", result);

    memcpy(g_banko_arr[booth_id].name, clientParam->name, 256);
    g_banko_arr[booth_id].seatNumber = clientParam->seatNumber;
    g_banko_arr[booth_id].serviceTime = clientParam->serviceTime;
    g_banko_arr[booth_id].processType = E_REQUESTED;

    if ((result = pthread_mutex_unlock(&g_mutex_ticketOp)) != 0)
        exit_sys_errno("pthread_mutex_lock", result);

    while(g_banko_arr[booth_id].processType != E_RESPONDED)
    {
        ;   
    }

    deallocate_booth(booth_id);
    
    free(clientParam);

    return NULL;
}

void *teller_thread_proc(void *param)
{
    int id = (int *)param;
    int result;

    printf("\nThread-%d has arrived with id", id);


    for (;;) 
    {
        if(g_banko_arr[tellerParam->id].processType != E_REQUESTED)
        {
            continue;
        }
        
        if ((result = pthread_mutex_lock(&g_mutex_teller)) != 0)
            exit_sys_errno("pthread_mutex_lock", result);

        size_t seatNum = process_request(tellerParam->id);

        if ((result = pthread_mutex_unlock(&g_mutex_teller)) != 0)
            exit_sys_errno("pthread_mutex_lock", result);    


        usleep(g_banko_arr[tellerParam->id].serviceTime * 1000);
        g_banko_arr[tellerParam->id].processType = E_RESPONDED;

        printf("\n%s requests seat %d, reserves seat %d, Signed by Thread-%d",g_banko_arr[tellerParam->id].name, g_banko_arr[tellerParam->id].seatNumber, seatNum, id);
        fflush(stdout);

        if(is_theater_full())
            break;
    }   



    free(tellerParam);
    return NULL;
}

void exit_fail(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void exit_sys_errno(const char *msg, int errno)
{
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

size_t allocate_booth(size_t* boot_id)
{
    int i;

    for(i = 0; i < MAX_TELLER; i++) {
        if(g_booth_arr[i]) {
            *boot_id = i;
            g_booth_arr[i] = NAVAIL;
            return 1;
        }
    }

    return 0;
}

void deallocate_booth(size_t booth_id)
{
    g_booth_arr[booth_id] = AVAIL;
}

size_t process_request(int teller_id)
{
    int i;
    int id;
    id = g_banko_arr[teller_id].seatNumber;
    if(g_seat_arr[id-1]) {
        g_seat_arr[id-1] = NAVAIL;
        return id;    
    }else{
       for(i = 0; i < MAX_SEAT; i++){
           if(g_seat_arr[i]) {
               g_seat_arr[i] = NAVAIL;
               return i+1;
           }
       } 
    }

    return -1;

}

size_t is_theater_full()
{
    int i;

    for(i = 0; i < MAX_SEAT; i++)
    {
        if(g_seat_arr[i])
            return 0;
    }

    return 1;
}
