#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //sleep
#include <fcntl.h>
#include <sys/socket.h>
#include <poll.h>
#include <wait.h>

#include "logging.h"
#include "message.h"

#define MYBOMBER 1
#define MYBOMB 2
#define BOMB_BOMBER 3
#define DEAD_BOMBER 4
#define DEAD_BOMBER_BOMB 5
#define MYOBSTACLE 7
#define WINNER 8
#define WINNER_BOMB 9

void print_map();
void setup();
void create_bombers();

void controller_loop();

void bomber_messages(int index);
void bomb_messages(int index);

int is_one_step_away(int currect_x, int current_y, int new_x, int new_y);
int find_bomber_index(int x, int y);

void start(int index);
void see(int index);
void plant(long interval, int radius, int index);
void move(int incom_x, int incom_y, int index);
int is_dead(int index);

void explode_remaining(int index);
void cleanup();


//map
int **map;
int map_width, map_height;

//obstacles
int obstacle_count;
struct obstacle_data* obstacles;

//bombers
int bomber_count;
int only_alive_bombers;
int alive_and_not_notified_bombers;
struct pollfd* bomber_fds;
pid_t* bomber_pids;
int *bomber_x;
int *bomber_y;
int *die_dist;
int winner_index=-1;

//bombs
struct pollfd* bomb_fds;
pid_t* bomb_pids;
int *bomb_x;
int *bomb_y;
int *bomb_rad;
int bomb_count = 0;
int alive_bomb_count = 0;



int main(int argc, char* argv[])
{
    setup();
    create_bombers();
    controller_loop();
    cleanup();
    close(0);
    close(1);
    close(2);
    exit(0);
}

void controller_loop(){
    while(alive_and_not_notified_bombers>0){
        int poll_return_bomb = poll(bomb_fds, bomb_count, 0);
        if(poll_return_bomb > 0){
            for(int i = 0; i < bomb_count; i++){
                if(bomb_fds[i].revents & POLLIN){
                    bomb_messages(i);
                }
            }
        }
        int poll_return_bomber = poll(bomber_fds, bomber_count, 0);
        if(poll_return_bomber > 0){
            for(int i = 0; i < bomber_count; i++){
                if(bomber_fds[i].revents & POLLIN){
                    bomber_messages(i);
                }
            }
        }
        //6.9- Sleep for 1 millisecond ???
        usleep(1000);
    }
    while(alive_bomb_count>0){
        int poll_return_bomb = poll(bomb_fds, bomb_count, 0);
        if(poll_return_bomb > 0){
            for(int i = 0; i < bomb_count; i++){
                if(bomb_fds[i].revents & POLLIN){
                    explode_remaining(i);
                }
            }
        }
    }
}

void print_map(){
    for (int i = 0; i < map_height; i++){
        for (int j = 0; j < map_width; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

void setup()
{
    scanf("%d %d %d %d", &map_width, &map_height, &obstacle_count, &bomber_count);
    alive_and_not_notified_bombers = bomber_count;
    only_alive_bombers = bomber_count;
    //map
    map = (int**)malloc(map_height * sizeof(int*));
    for (int i = 0; i < map_height; i++)
        map[i] = (int*)malloc(map_width * sizeof(int));
    //obstacles
    obstacles = calloc(obstacle_count, sizeof(struct obstacle_data));
    for (int i = 0; i < obstacle_count ; i++ ){
        int location_x,location_y,durability;
        scanf("%d %d %d", &location_x, &location_y, &durability);
        map[location_y][location_x] = MYOBSTACLE;
        obstacles[i].position.x = location_x;
        obstacles[i].position.y = location_y;
        obstacles[i].remaining_durability = durability;
    }
    //bombers
    bomber_fds = calloc(bomber_count, sizeof(struct pollfd));
    bomber_pids = calloc(bomber_count, sizeof(pid_t));
    bomber_x = calloc(bomber_count, sizeof(int));
    bomber_y = calloc(bomber_count, sizeof(int));
    die_dist = calloc(bomber_count, sizeof(int));
    for(int i =0;i<bomber_count;i++){
        die_dist[i] =-1;
    }
    //bombs
    bomb_fds = calloc(map_height*map_width*3, sizeof(struct pollfd));
    bomb_pids = calloc(map_height*map_width*3, sizeof(pid_t));
    bomb_x = calloc(map_height*map_width*3, sizeof(int));
    bomb_y = calloc(map_height*map_width*3, sizeof(int));
    bomb_rad = calloc(map_height*map_width*3, sizeof(int));
}



void create_bombers(){
    for (int i = 0; i < bomber_count ; i++ ){
        int b_x, b_y, argument_count;
        scanf("%d %d %d", &b_x, &b_y, &argument_count);
        
        pid_t pid;
        int fd[2];
        PIPE(fd);
        char **child_argv;
        child_argv = calloc(argument_count+1, sizeof(char*));
        for(int i = 0; i < argument_count; i++){
            scanf("%ms", &child_argv[i]);
        }
        child_argv[argument_count] = NULL;
        if( !(pid = fork()) ){
            close(fd[0]);
            dup2(fd[1], 0);
            dup2(fd[1], 1);
            close(fd[1]);
            execv(child_argv[0], child_argv);
        }
        else {
            for(int i = 0; i <= argument_count; i++)
                free(child_argv[i]);
            free(child_argv);
        }
        close(fd[1]);
        bomber_fds[i].fd = fd[0];
        bomber_fds[i].events = POLLIN;
        bomber_pids[i] = pid;
        bomber_x[i] = b_x;
        bomber_y[i] = b_y;
    }
}



void explode_remaining(int index){
    imp incom;
    omp outmes;
    im incoming_message;  memset(&incoming_message , 0, sizeof(imt));
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    read_data(bomb_fds[index].fd, &incoming_message);
    incom.pid = bomb_pids[index];
    incom.m = &incoming_message;
    if(incoming_message.type== BOMB_EXPLODE){
        print_output(&incom, NULL, NULL, NULL);
        alive_bomb_count--;
        bomb_fds[index].events ^=POLLIN;
        int stat;
        waitpid(bomb_pids[index],&stat,0);
        close(bomb_fds[index].fd);
    }
}

int find_bomber_index(int x, int y){
    for(int i =0; i<bomber_count;i++){
        if(bomber_x[i] == x && bomber_y[i]==y)
            return i;
    }
}

void bomb_messages(int index){
    int dead=0;
    imp incom;
    im incoming_message;  memset(&incoming_message , 0, sizeof(imt));
    read_data(bomb_fds[index].fd, &incoming_message);
    incom.pid = bomb_pids[index];
    incom.m = &incoming_message;
    
    if(incoming_message.type== BOMB_EXPLODE){
        print_output(&incom, NULL, NULL, NULL);

        int radius = bomb_rad[index];
        int origin_x = bomb_x[index];
        int origin_y = bomb_y[index];

        if(map[origin_y][origin_x] == BOMB_BOMBER ){
            map[origin_y][origin_x] = DEAD_BOMBER;
            only_alive_bombers--;
            die_dist[find_bomber_index(origin_x,origin_y)] = 0;
        }
 
        else if(map[origin_y][origin_x] == MYBOMB){
            map[origin_y][origin_x] = 0;
        }
        else if(map[origin_y][origin_x] == DEAD_BOMBER_BOMB){
            map[origin_y][origin_x] = DEAD_BOMBER;
            die_dist[find_bomber_index(origin_x,origin_y)] = 0;
        }
            

        for(int i = 1; i<=radius ;i++){
            if( map_width > origin_x+i){
                if((map[origin_y][origin_x+i]) == MYOBSTACLE){
                        for (int j = 0; j< obstacle_count; j++){
                            if(obstacles[j].position.x == origin_x+i && obstacles[j].position.y == origin_y){
                                if(obstacles[j].remaining_durability == 1){
                                    obstacles[j].remaining_durability = 0;
                                    map[obstacles[j].position.y][obstacles[j].position.x] =0;
                                }
                                else if(obstacles[j].remaining_durability == -1){
                                
                                }
                                else
                                obstacles[j].remaining_durability--;
                                
                                obsd damaged_obst;
                                damaged_obst.remaining_durability =  obstacles[j].remaining_durability;
                                damaged_obst.position.x = obstacles[j].position.x;
                                damaged_obst.position.y = obstacles[j].position.y;
                                print_output(NULL, NULL, &damaged_obst, NULL);
                            }
                        }
                   break;
                }
                else if((map[origin_y][origin_x+i]) == MYBOMBER){
                    map[origin_y][origin_x+i] = DEAD_BOMBER; // dead bomber
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x+i,origin_y)] = i;
                }
                else if((map[origin_y][origin_x+i]) == BOMB_BOMBER){
                    map[origin_y][origin_x+i] = DEAD_BOMBER_BOMB; //dead bomber + bomb
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x+i,origin_y)] = i;
                }
                else;
            }
        }


        for(int i = 1; i<=radius ;i++){
            if( 0<= origin_x-i){
                if((map[origin_y][origin_x-i]) == MYOBSTACLE){
                        for (int j = 0; j< obstacle_count; j++){
                            if(obstacles[j].position.x == origin_x-i && obstacles[j].position.y == origin_y){
                                if(obstacles[j].remaining_durability == 1){
                                    obstacles[j].remaining_durability = 0;
                                    map[obstacles[j].position.y][obstacles[j].position.x] =0;
                                }
                                else if(obstacles[j].remaining_durability == -1){
                                    
                                }
                                else
                                obstacles[j].remaining_durability--;

                                obsd damaged_obst;
                                damaged_obst.remaining_durability =  obstacles[j].remaining_durability;
                                damaged_obst.position.x = obstacles[j].position.x;
                                damaged_obst.position.y = obstacles[j].position.y;
                                print_output(NULL, NULL, &damaged_obst, NULL);
                            }
                        }

                   break;
                }
                else if((map[origin_y][origin_x-i]) == MYBOMBER){
                    map[origin_y][origin_x-i] = DEAD_BOMBER; // dead bomber
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x-i,origin_y)] = i;
                }
                else if((map[origin_y][origin_x-i]) == BOMB_BOMBER){
                    map[origin_y][origin_x-i] = DEAD_BOMBER_BOMB; //dead bomber + bomb
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x-i,origin_y)] = i;
                }
                else;
            }
        }


        for(int i = 1; i<=radius ;i++){
            if( map_height > origin_y+i){
                if((map[origin_y+i][origin_x]) == MYOBSTACLE){
                    for (int j = 0; j< obstacle_count; j++){
                        if(obstacles[j].position.x == origin_x && obstacles[j].position.y == origin_y+i){
                            if(obstacles[j].remaining_durability == 1){
                                obstacles[j].remaining_durability = 0;
                                map[obstacles[j].position.y][obstacles[j].position.x] =0;
                            }                                
                            else if(obstacles[j].remaining_durability == -1){
                               
                            }
                            else
                                obstacles[j].remaining_durability--;
                            
                                obsd damaged_obst;
                                damaged_obst.remaining_durability =  obstacles[j].remaining_durability;
                                damaged_obst.position.x = obstacles[j].position.x;
                                damaged_obst.position.y = obstacles[j].position.y;
                                print_output(NULL, NULL, &damaged_obst, NULL);
                        }
                    }
                   break;
                }
                else if((map[origin_y+i][origin_x]) == MYBOMBER){
                    map[origin_y+i][origin_x] = DEAD_BOMBER; // dead bomber
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x,origin_y+i)] = i;
                }
                else if((map[origin_y+i][origin_x]) == BOMB_BOMBER){
                    map[origin_y+i][origin_x] = DEAD_BOMBER_BOMB; //dead bomber + bomb
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x,origin_y+i)] = i;
                }
                else;
            }
        }

        for(int i = 1; i<=radius ;i++){
            if( 0<= origin_y-i){
                if((map[origin_y-i][origin_x]) == MYOBSTACLE){
                    for (int j = 0; j< obstacle_count; j++){
                            if(obstacles[j].position.x == origin_x && obstacles[j].position.y == origin_y-i){
                                if(obstacles[j].remaining_durability == 1){
                                    obstacles[j].remaining_durability = 0;
                                    map[obstacles[j].position.y][obstacles[j].position.x] =0;
                                }
                                else if(obstacles[j].remaining_durability == -1){
                                   
                                }
                                else
                                obstacles[j].remaining_durability--;

                                obsd damaged_obst;
                                damaged_obst.remaining_durability =  obstacles[j].remaining_durability;
                                damaged_obst.position.x = obstacles[j].position.x;
                                damaged_obst.position.y = obstacles[j].position.y;
                                print_output(NULL, NULL, &damaged_obst, NULL);
                            }
                        }
                   break;
                }
                else if((map[origin_y-i][origin_x]) == MYBOMBER){
                    map[origin_y-i][origin_x] = DEAD_BOMBER; // dead bomber
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x,origin_y-i)] = i;
                }
                else if((map[origin_y-i][origin_x]) == BOMB_BOMBER){
                    map[origin_y-i][origin_x] = DEAD_BOMBER_BOMB; //dead bomber + bomb
                    only_alive_bombers--;
                    die_dist[find_bomber_index(origin_x,origin_y-i)] = i;
                }
                else;
            }
        }
        
        alive_bomb_count--;
        bomb_fds[index].events ^=POLLIN;
        int stat;
        waitpid(bomb_pids[index],&stat,0);
        close(bomb_fds[index].fd);


        if(only_alive_bombers == 1 ){
            for (int i = 0; i < map_height; i++){
                for (int j = 0; j < map_width; j++){
                    if(map[i][j] == MYBOMBER){
                        map[i][j] = WINNER;
                        break;
                    }
                    else if(map[i][j] == BOMB_BOMBER){
                        map[i][j] = WINNER_BOMB;
                        break;
                    }
                }
            }
        }
        else if(only_alive_bombers == 0){
           // printf("multiple dead \n");
            int max_dist=-1;
            for(int i =0;i<bomber_count;i++){
                if(die_dist[i]>max_dist){
                    max_dist = die_dist[i];
                winner_index = i;
                }
            } 
            if(map[bomber_y[winner_index]][bomber_x[winner_index]] == DEAD_BOMBER)
                map[bomber_y[winner_index]][bomber_x[winner_index]] = WINNER;
            else if(map[bomber_y[winner_index]][bomber_x[winner_index]] == DEAD_BOMBER_BOMB)
                map[bomber_y[winner_index]][bomber_x[winner_index]] = WINNER_BOMB;
        }
        else{
            for(int i =0;i<bomber_count;i++)
                die_dist[i] =-1;
        }

    }
    //print_map();
}


void start(int index){
    omp outmes;
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    outgoing_message.type = BOMBER_LOCATION;
    outgoing_message.data.new_position.x = bomber_x[index];
    outgoing_message.data.new_position.y = bomber_y[index];
    send_message(bomber_fds[index].fd, &outgoing_message);
    if(bomber_count==1)
        map[bomber_y[index]][ bomber_x[index]] = WINNER;
    else
        map[bomber_y[index]][ bomber_x[index]] = MYBOMBER;
    outmes.pid = bomber_pids[index];
    outmes.m = &outgoing_message;
    print_output(NULL, &outmes, NULL, NULL); 

}
int is_dead(int index){
    omp outmes;
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    int origin_x = bomber_x[index];
    int origin_y = bomber_y[index];

    if((map[origin_y][origin_x]) == DEAD_BOMBER ||(map[origin_y][origin_x]) == DEAD_BOMBER_BOMB || 
        (map[origin_y][origin_x]) == WINNER || (map[origin_y][origin_x]) == WINNER_BOMB ){

        if(((map[origin_y][origin_x]) == WINNER) || (map[origin_y][origin_x]) == WINNER_BOMB )
            outgoing_message.type =BOMBER_WIN;
        else
            outgoing_message.type =BOMBER_DIE;

        send_message(bomber_fds[index].fd, &outgoing_message);
        outmes.pid = bomber_pids[index];
        outmes.m = &outgoing_message;
        print_output(NULL, &outmes, NULL, NULL);
        alive_and_not_notified_bombers--;
        bomber_fds[index].events ^=POLLIN;
        int stat;
        waitpid(bomber_pids[index],&stat,0);
        close(bomber_fds[index].fd);

        if((map[origin_y][origin_x]) == DEAD_BOMBER || (map[origin_y][origin_x]) == WINNER)
            map[origin_y][origin_x] = 0; 
        else
            map[origin_y][origin_x] = 2;
        return 1;
    }
    else 
        return 0;

}



void see(int index){
    int obj_count=0;
    od* objects = (od*)malloc(25 * sizeof(od));
    od obj, obj2;
    omp outmes;
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    int origin_x = bomber_x[index];
    int origin_y = bomber_y[index];
    if((map[origin_y][origin_x]) == BOMB_BOMBER ){
        obj.position.x = origin_x;
        obj.position.y = origin_y;
        obj.type = BOMB;
        objects[obj_count] = obj;
         obj_count ++;
    }
    for(int i = 1; i<=3 ;i++){
        if( map_width > origin_x+i && (map[origin_y][origin_x+i]) != MYOBSTACLE){
            if((map[origin_y][origin_x+i]) != 0){
                obj.position.x = origin_x+i;
                obj.position.y = origin_y;
                if((map[origin_y][origin_x+i]) == BOMB_BOMBER){
                    obj2.position.x = origin_x+i;
                    obj2.position.y = origin_y;
                    obj2.type = BOMB;
                    objects[obj_count] = obj2;
                    obj_count ++;
                    obj.type = BOMBER;
                    }
                else if((map[origin_y][origin_x+i]) == MYBOMB || (map[origin_y][origin_x+i]) == DEAD_BOMBER_BOMB)
                    obj.type = BOMB;
                else
                    obj.type = BOMBER;
                objects[obj_count] = obj;
                obj_count ++;
            }
        }
        else
            break;
    }
    for(int i = 1; i<=3 ;i++){
        if(0 <= origin_x-i && (map[origin_y][origin_x-i]) != MYOBSTACLE){
            if((map[origin_y][origin_x-i]) != 0){
                obj.position.x = origin_x-i;
                obj.position.y = origin_y;
                if((map[origin_y][origin_x-i]) == BOMB_BOMBER){
                    obj2.position.x = origin_x-i;
                    obj2.position.y = origin_y;
                    obj2.type = BOMB;
                    objects[obj_count] = obj2;
                    obj_count ++;
                    obj.type = BOMBER;
                }
                else if((map[origin_y][origin_x-i]) == MYBOMB || (map[origin_y][origin_x-i]) == DEAD_BOMBER_BOMB )
                    obj.type = BOMB;
                else
                    obj.type = BOMBER;
                objects[obj_count] = obj;
                obj_count ++;
            }
        }
        else
            break;
    }
    for(int i = 1; i<=3 ;i++){
        if(map_height > origin_y+i &&(map[origin_y+i][origin_x]) != MYOBSTACLE){
            if((map[origin_y+i][origin_x]) != 0){
                obj.position.x = origin_x;
                obj.position.y = origin_y+i;
                if((map[origin_y+i][origin_x]) == BOMB_BOMBER){
                    obj2.position.x = origin_x;
                    obj2.position.y = origin_y+i;
                    obj2.type = BOMB;
                    objects[obj_count] = obj2;
                    obj_count ++;
                    obj.type = BOMBER;
                }
                else if((map[origin_y+i][origin_x]) == MYBOMB || (map[origin_y+i][origin_x]) == DEAD_BOMBER_BOMB )
                    obj.type = BOMB;
                else
                    obj.type = BOMBER;
                objects[obj_count] = obj;
                obj_count ++;
            }
        }
        else
            break;
    }
    for(int i = 1; i<=3 ;i++){
        if( 0<= origin_y-i && (map[origin_y-i][origin_x]) != MYOBSTACLE){
            if((map[origin_y-i][origin_x]) != 0){
                obj.position.x = origin_x;
                obj.position.y = origin_y-i;
                if((map[origin_y-i][origin_x]) == BOMB_BOMBER){
                    obj2.position.x = origin_x;
                    obj2.position.y = origin_y-i;
                    obj2.type = BOMB;
                    objects[obj_count] = obj2;
                    obj_count ++;
                    obj.type = BOMBER;
                }
                else if((map[origin_y-i][origin_x]) == MYBOMB || (map[origin_y-i][origin_x]) == DEAD_BOMBER_BOMB )
                    obj.type = BOMB;
                else
                    obj.type = BOMBER;
                objects[obj_count] = obj;
                obj_count ++;
            }
        }
        else
            break;
    }
    outgoing_message.type = BOMBER_VISION;
    outgoing_message.data.object_count = obj_count;
    send_message(bomber_fds[index].fd, &outgoing_message);
    if(obj_count>0)
        send_object_data(bomber_fds[index].fd, obj_count, objects);
    outmes.pid = bomber_pids[index];
    outmes.m = &outgoing_message;
    if(obj_count>0)
        print_output(NULL, &outmes, NULL, objects);
    else
        print_output(NULL, &outmes, NULL, NULL);
}

void move(int incom_x, int incom_y, int index){
    omp outmes;
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    int origin_x = bomber_x[index];
    int origin_y = bomber_y[index];
    if((( incom_x < map_width) && ( incom_y < map_height) && ( incom_x >= 0) && ( incom_y >= 0) 
        && is_one_step_away(bomber_x[index],bomber_y[index], incom_x, incom_y))
        &&( map[incom_y][incom_x] != MYOBSTACLE ) &&( map[incom_y][incom_x] != MYBOMBER )&&( map[incom_y][incom_x] != BOMB_BOMBER )
        &&( map[incom_y][incom_x] != DEAD_BOMBER ) &&( map[incom_y][incom_x] != DEAD_BOMBER_BOMB ) 
        ){
            if(map[origin_y][origin_x] == BOMB_BOMBER)
                map[origin_y][origin_x] = MYBOMB;
            else
                map[origin_y][origin_x] = 0;
            if(map[incom_y][incom_x] == MYBOMB)
                map[incom_y][incom_x] = BOMB_BOMBER;
            else
                map[incom_y][incom_x] = MYBOMBER;
            outgoing_message.data.new_position.x = incom_x;
            outgoing_message.data.new_position.y = incom_y;
            bomber_x[index] = incom_x;
            bomber_y[index] = incom_y;
    }
    else{
        outgoing_message.data.new_position.x = origin_x;
        outgoing_message.data.new_position.y = origin_y;
    }
    outgoing_message.type = BOMBER_LOCATION;
    send_message(bomber_fds[index].fd,&outgoing_message);
    outmes.pid = bomber_pids[index];
    outmes.m = &outgoing_message;
    print_output(NULL, &outmes, NULL, NULL);
}


void plant(long interval, int radius, int index){
    omp outmes;
    om outgoing_message; memset(&outgoing_message, 0, sizeof(omt));
    int origin_x = bomber_x[index];
    int origin_y = bomber_y[index];
    if(map[bomber_y[index]][bomber_x[index]] == MYBOMB || map[bomber_y[index]][bomber_x[index]] == BOMB_BOMBER)
        outgoing_message.data.planted=0;
    else{
        map[bomber_y[index]][bomber_x[index]] = BOMB_BOMBER;
        pid_t pid;
        int fd[2];
        PIPE(fd);
        char **child_argv = calloc(3, sizeof(char*));
        child_argv[0] = "./bomb"; 
        long number = interval; int n=0; 
        do { number /= 10; ++n; } while (number != 0); number = interval;
        char *interval = calloc(n, sizeof(char));
        for (int i = n-1; i >= 0; --i, number /= 10) interval[i] = (number % 10) + '0';
        child_argv[1] = interval;
        child_argv[2] = NULL;
        if( !(pid = fork()) ){
            close(fd[0]);
            dup2(fd[1], 0);
            dup2(fd[1], 1);
            close(fd[1]);
            execv(child_argv[0], child_argv);
        }
        close(fd[1]);
        bomb_fds[bomb_count].fd = fd[0];
        bomb_fds[bomb_count].events = POLLIN;
        bomb_pids[bomb_count] = pid;
        bomb_x[bomb_count] = bomber_x[index];
        bomb_y[bomb_count] = bomber_y[index];
        bomb_rad[bomb_count]=radius;
        bomb_count++;
        alive_bomb_count++;
        outgoing_message.data.planted=1;
        }
    outgoing_message.type = BOMBER_PLANT_RESULT;
    send_message(bomber_fds[index].fd,&outgoing_message);
    outmes.pid = bomber_pids[index];
    outmes.m = &outgoing_message;
    print_output(NULL, &outmes, NULL, NULL);
}

void bomber_messages(int index){
    omp outmes;
    om outgoing_message; 
    int origin_x,origin_y;
    
    imp incom;
    im incoming_message;  memset(&incoming_message , 0, sizeof(imt));
    read_data(bomber_fds[index].fd, &incoming_message);
    incom.pid = bomber_pids[index];
    incom.m = &incoming_message;
    print_output(&incom, NULL, NULL, NULL);
    //print_map();
    if(!is_dead(index)){
        switch(incoming_message.type)
        {
            case BOMBER_START:
                start(index);
                break;

            case BOMBER_SEE:
                see(index);
                break;

            case BOMBER_MOVE:
                move(incoming_message.data.target_position.x,incoming_message.data.target_position.y,index);
                break;

            case BOMBER_PLANT:
                plant(incoming_message.data.bomb_info.interval,incoming_message.data.bomb_info.radius, index);
                break;

            default:
                break;
        }
    }
    
    //print_map();

}


int is_one_step_away(int currect_x, int current_y, int new_x, int new_y){
    int sum = abs(currect_x-new_x) + abs(current_y-new_y);
    if (sum<=1) return 1;
    else return 0;
}

void cleanup()
{
    free(obstacles);
    free(bomber_fds);
    free(bomber_pids);
    free(bomber_x);
    free(bomber_y);
    free(bomb_pids);
    free(bomb_x);
    free(bomb_y);
    free(bomb_rad);
    for (int i = 0; i < map_height; i++)
        free( map[i]);
}