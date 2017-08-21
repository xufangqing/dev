#include <stdio.h>                                
#include <stdlib.h>                               
#include <assert.h>                               
#include <stdint.h>                               
#include <string.h>                               
#include <time.h>                                 
#include <sys/types.h>                            
#include <sys/stat.h>                             
#include <fcntl.h>                                
#include <unistd.h>                               
#include <ctype.h>                                
                                                   
#define NS 1000000000                             
                                                   
                                                   
//  Linux/GCC                                     
                                                   
static inline uint64_t get_cycle_count()          
{                                                 
    unsigned int lo,hi;                           
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;             
}                                                 

void call_os_func(char * cmd, FILE *stream) {
    FILE * fp;
    char buffer[256];
    fp=popen(cmd, "r");
    fgets(buffer, sizeof(buffer), fp);
    fprintf(stream, "\n%s", buffer);
    pclose(fp);
    /* system(cmd); */
    fprintf(stream, "\t%s\n", cmd);
}
                                                   
int main(int argc,char ** argv)                   
{                                                 
                                                   
    int i = 0;                                    
    uint64_t old,new;                             
    int delayms = 700;                            
    for(i = 0; i < 7; i++)                      
    {                                             
        old = get_cycle_count();                  
        usleep(delayms * 1000);                   
        new = get_cycle_count();                  
        printf("%u\n",(new-old)/(delayms*1000));                                                                                                                                       
                                                   
    }
    call_os_func("cat /proc/cpuinfo  | grep \"cpu MHz\"",stdout);                                             
}
