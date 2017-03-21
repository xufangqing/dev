#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<sys/socket.h>  
#include<sys/types.h>  
#include<unistd.h>      /* sleep */
#include<netinet/in.h>  
#include <errno.h>
#include <signal.h>     /* sigaction */
#include <execinfo.h>   /* backtrace */
#include <fcntl.h>      /* file open O_CREAT */

#define PORT 6666

char* FindSoName(const char * soFile) {
    static char SoName[50] = {'\0'};
    FILE* file =  popen(soFile, "r");
    int n = fread(SoName, 1, 50, file);
    SoName[n-1] = 0;
    //printf("The .so name is:%s\n", SoName);
    return SoName;
}

char* FindBaseAddress(const char* SoName) {
    static char BaseAddr[20]; 
    int pid = getpid();
    char file_name[50];
    sprintf(file_name, "/proc/%d/maps", pid);
    char cmd[100];
    sprintf(cmd, "grep %s %s | head -1 | awk -F- \'{print $1}\'", SoName, file_name);
    FILE* file = popen(cmd, "r");
    int n = fread(BaseAddr, 1, 20,file);
    BaseAddr[n-1] = '\0';
    //printf("The Base address of the %s is:%s\n", SoName, BaseAddr);
    return BaseAddr;
}

int PrintLineNumber(char* So_Name, int OffsetAddress, FILE *stream) {
    char cmd[128];
    sprintf(cmd, "addr2line -e %s 0x%x", So_Name, OffsetAddress);
    
    /* system(cmd); */
    FILE * fp;
    char buffer[256];
    fp=popen(cmd, "r");
    fgets(buffer, sizeof(buffer), fp);
    fprintf(stream, "\t%s", buffer);
    pclose(fp);
    /* system(cmd); */
    fprintf(stream, "\t%s\n", cmd);
}

int PrintElfLn(char *Elf_Name, char *addr, FILE *stream) {
    char cmd[128];
    sprintf(cmd, "addr2line -e %s %s", Elf_Name, addr);
    
    /* system(cmd); */
    FILE * fp;
    char buffer[256];
    fp=popen(cmd, "r");
    fgets(buffer, sizeof(buffer), fp);
    fprintf(stream, "\t%s", buffer);
    pclose(fp);
    /* system(cmd); */
    fprintf(stream, "\t%s\n", cmd);
}

/******************************************************************************
./a.out(main+0x9) [0x400ec5]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f006bdfbec5]
./tserver[0x401620]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5)[0x7fa330034f45]
******************************************************************************/
/*    char **n_elf, char **n_func, char **n_offset, char **n_ptr) { */
void DumpBtReadable(char *pSymbolExe, char *btLine, FILE *stream) {
    if (NULL == pSymbolExe || NULL == btLine) {
        return;
    }
    
    static char elfName[128] = {0};
    static char funcName[128] = {0};
    static char funcOffset[24] = {0};
    static char funcPtr[24] = {0};
    int  i = 0;
    char *p = NULL;
    char *p_w = NULL;
    char *p_w_end = NULL;
    for (p=btLine,p_w=elfName,p_w_end=p_w+sizeof(elfName); *p && p_w<=p_w_end; ++p) {
        *p_w = *p;
        if (*p == '(') {                   // (，左边是 elf名称
            *p_w = '\0';
            p_w=funcName;
            p_w_end=p_w+sizeof(funcName);
        } else if (*p == '+') {            // +, 左边是 函数名
            *p_w = '\0';
            p_w = funcOffset;
            p_w_end=p_w+sizeof(funcOffset);
        } else if (*p == ')') {            // ), 左边是 函数偏移
            *p_w = '\0';
            p_w = funcPtr;
            p_w_end=p_w+sizeof(funcPtr);
        } else if (*p == '[') {            // [, 右边是 函数指针 开始
            p_w = funcPtr;
            p_w_end=p_w+sizeof(funcPtr);
        } else if (*p == ']') {            // ], 左边是 函数指针
            *p_w = '\0';
            break;
        } else {
            p_w++;
        }
    }
/*    *n_func = elfName;
    *n_func = funcName;
    *n_offset = funcOffset;
    *n_ptr = funcPtr; */
    
    //fprintf(stream, "\t|%s|%s|%s|%s|\n", elfName,funcName,funcOffset,funcPtr);
    /* char *So_Name = FindSoName(elfName); */
    char *So_Name = elfName;
    if (NULL == strstr(elfName, ".so")) {   //exe
        PrintElfLn(elfName, funcPtr, stream);
    } else {                                //so
        char *BaseAddress = FindBaseAddress(So_Name);
        //fprintf(stream, "The Stack Address is:%s\n", funcPtr);
        char Stack[50];
        sprintf(Stack, "%s", funcPtr);
        int OffsetAddress = strtol(Stack, NULL, 16) - strtol(BaseAddress, NULL, 16);
        //fprintf(stream, "OffsetAddress:%d\n", OffsetAddress);
        PrintLineNumber(So_Name, OffsetAddress, stream); 
    }
}

void dump(int signo, char *btfile) {  
    void *buffer[128] = {0};  
    size_t size;  
    char **strings = NULL;  
    size_t i = 0; 
    int fd;
    
    size = backtrace(buffer, 128);  
    fprintf(stdout, "Obtained %zd stack frames.nm\n", size);
    if (NULL != btfile) {
        fd = open(btfile, O_CREAT | O_RDWR);
        if (0 != fd) {
            backtrace_symbols_fd(buffer, size, fd);
            /* backtrace_symbols_fd(bt, nentries, fileno(stdout)); */
            close(fd);
        }
    }
    strings = backtrace_symbols(buffer, size);  
    if (strings == NULL) {  
        perror("backtrace_symbols.");  
        exit(EXIT_FAILURE);  
    }
    
    FILE *fp_bt;
    fp_bt = fopen(".btread.log", "w+");
    if (NULL != fp_bt) {
        for (i = 0; i < size; i++) {  
        fprintf(fp_bt, "%s", strings[i]);
        //fprintf(stdout, "\t+++%p###", buffer[i]);
        //DumpBtReadable("111", strings[i], stdout);
        DumpBtReadable("111", strings[i], fp_bt);
        }
        fclose(fp_bt);
    }
    
    free(strings);  
    strings = NULL;  
    
    /* exit(0); */
}

void dumpToFile(char *btfile) {
    int fd;
    size_t size;
    void *buffer[128] = {0};
    size = backtrace(buffer, 128);
	if (NULL != btfile) {
        fd = open(btfile, O_CREAT | O_RDWR);
        if (0 != fd) {
            backtrace_symbols_fd(buffer, size, fd);
            /* backtrace_symbols_fd(bt, nentries, fileno(stdout)); */
            close(fd);
        }
    }
}

static void SignalHandler(int nSigno)  
{
    switch(nSigno)  
    {  
    case SIGPIPE:  
        printf("########receive SIGPIPE, Process will not exit########\n");
        dump(nSigno, ".btlog");
        break;  
    default:  
        printf("%d signal unregister\n", nSigno);  
        break;  
    }  
}  

static void SigActionInit() {
    struct sigaction sa;  
    sa.sa_handler = SIG_IGN;  
    sigaction( SIGPIPE, &sa, 0 );
}
 
static void InitSignalHandler(int iDealType)  
{
    switch(iDealType) {
    case 1:
        signal(SIGPIPE, &SignalHandler);
        break;
    case 2:
        signal(SIGPIPE, SIG_IGN);
        break;
    case 3:
        SigActionInit();
        break;
    default:
        break;
    } 
    signal(SIGPIPE , &SignalHandler);  
}

  
int main(int argc,char **argv)  
{  
    int ser_sockfd,cli_sockfd;  
    int err,n,clientLoop;  
  
    struct sockaddr_in ser_addr;  
    struct sockaddr_in cli_addr;  
    char recvline[200];
    const char *sendline = "hello client";
    
    InitSignalHandler(1); 
    ser_sockfd=socket(AF_INET,SOCK_STREAM,0);  
    if(ser_sockfd==-1)  
    {  
        printf("socket error:%s\n",strerror(errno));  
        return -1;  
    }  
      
    bzero(&ser_addr,sizeof(ser_addr));  
    ser_addr.sin_family=AF_INET;  
    ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
    ser_addr.sin_port=htons(PORT);  
    err=bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));  
    if(err==-1)  
    {  
        printf("bind error:%s\n",strerror(errno));  
        return -1;  
    }  
      
    err=listen(ser_sockfd,5);  
    if(err==-1)  
    {  
        printf("listen error\n");  
        return -1;  
    }  
      
    printf("listen the port:%d\n",PORT);  
      
    while(1)  
    {     
        socklen_t addlen=sizeof(struct sockaddr);  
        cli_sockfd=accept(ser_sockfd,(struct sockaddr *)&cli_addr,&addlen);  
        if(cli_sockfd==-1)  
        {  
            printf("accept error\n");  
        }
        clientLoop = 0;
        while(1)  
        {
        	clientLoop++;
            printf("[clientLoop %3d]:waiting for client...\n", clientLoop);  
            n=recv(cli_sockfd,recvline,1024,0);
            printf( "\trecv return %2d, ",n ); 
            if(n==-1) {  
                printf("recv error, no datas\n");  
            }  
            recvline[n]='\0';  
            printf("recv data is:%s$\n",recvline);  
              
            int ret = send(cli_sockfd,sendline,strlen(sendline),0);  
            printf( "\tsend return %2d, send data is:%s$\n", ret, sendline);
            sleep(7);
        }  
        close(cli_sockfd);  
    }  
      
    close(ser_sockfd);  
      
    return 0;  
}

