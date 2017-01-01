#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <linux/types.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <list>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include "AutoIncreaceBuffer.h"

#define BUFFER_SIZE 2048
#define PORT 89898

typedef unsigned char u8;
typedef __u16 u16;
typedef __u32 u32;

#define REQ_DIR 0
#define REQ_RENAME 1
#define REQ_DELETE 2
#define REQ_MOVE 3
#define REQ_COPY 4
#define REQ_CREATE_FILE 5
#define REQ_CREATE_DIR 6

#define FUNC_DIR_TYPE_NAME 0
#define FUNC_DIR_TYPE_FILE_TYPE 1
#define FUNC_DIR_TYPE_SIZE 2
#define FUNC_DIR_TYPE_TIME 3
#define FUNC_DIR_TYPE_PERMISSION 4
#define FUNC_DIR_TYPE_USER 5
#define FUNC_DIR_TYPE_GROUP 6
#define FUNC_DIR_TYPE_LINK_TO 7

#define RES_OK 0
#define RES_ERROR 1
#define RES_FINISH 2
//enum protocolCmd{
//REQ_DIR,
//REQ_RENAME,
//REQ_DELETE,
//REQ_MOVE,
//REQ_COPY,
//REQ_CREATE_FILE,
//REQ_CREATE_DIR
//
//}
#pragma pack(1)
typedef struct{
    u32 size;
    char arg0;
    char arg1;
    char arg2;
    char arg3;
    char data;
} package;
#pragma pack()

void sendBuffer(int clientSock,char* buff,int length)
{
    int count = length/BUFFER_SIZE +1;
    int lenthSend = 0;
    for(int i=0;i<count;i++)
    {
        int lengthToSend = BUFFER_SIZE>length-lenthSend?length-lenthSend:BUFFER_SIZE;
        int lengthOk = send(clientSock,buff+lenthSend,lengthToSend,0);
        if(lengthOk<0)
        {
            perror("send fail!");
            break;
        }
        lenthSend += lengthOk;
    }
}

/*
request and respond format
byte0~3:size
byte4~7:arguments
byte8~~:data
*/


//void doDir(AutoIncreaceBuffer autoBuff,int clientSock)
//{
//    int length = autoBuff.getLength()-1;
//    if(length<=0)
//        return;
//    char* buff = autoBuff.buff+1;
//    /*
//    request format:
//    byte0~3:directory path length(little endian)
//    byte4:  flags:
//                    bit0:request size
//                    bit1:request time
//                    bit2:request permission
//                    bit3:request user
//                    bit4:request group
//                    bit5:request link to
//    byte5~7:  reserve
//    byte8~ :directory path
//    */
//    /*
//    respond format:
//    byte0:   result code:
//    byte1~2:   data length
//    byte3:    reserve
//    byte4~ :data
//                result code:
//                        0:succees:
//                                files:
//                                        items:
//                                                byte0:  type
//                                                byte1~2:items length
//                                                byte3:  reserve
//                                                byte4~ :data
//                                                    type0:name
//                                                    type1:filetype
//                                                       DT_BLK      This is a block device.
//                                                       DT_CHR      This is a character device.
//                                                       DT_DIR      This is a directory.
//                                                       DT_FIFO     This is a named pipe (FIFO).
//                                                       DT_LNK      This is a symbolic link.
//                                                       DT_REG      This is a regular file.
//                                                       DT_SOCK     This is a UNIX domain socket.
//                                                       DT_UNKNOWN  The file type is unknown.
//                                                    type2:size
//                                                    type3:time
//                                                    type4:permission
//                                                    type5:user
//                                                    type6:group
//                                                    type7:link to
//                        1~ :fail
//                                byte0~  :descript
//
//    */
//    int pathLength = *(u16*)buff;
//    char* path = malloc(pathLength);
//    memcpy(path,buff+4,pathLength);
//    DIR* dir = opendir(path);
//    free(path);
//    AutoIncreaceBuffer respondBuff;
//    u32 _space = 0;
//    respondBuff.addBuff((char*)&_space,4);
//    if(NULL == dir)
//    {
//        *respondBuff.buff = RES_ERROR;
//        perror("open dir fail:");
//        char* strErr = strerror(errno);
//        int errLength = strlen(strErr)+1;
//        respondBuff.addBuff(strErr,errLength);
//    }else
//    {
//        *respondBuff.buff = RES_OK;
//        struct dirent* ent;
//        while ((ent = readdir(dir)))
//        {
//            //for name item
//            int nameLength = strlen(ent->d_name)+1;
//            char _type = FUNC_DIR_TYPE_NAME;
//            int itemLength = 4;
//            respondBuff.addBuff(_type,1);
//            u16* pItemLength = (u16*)(respondBuff.buff+respondBuff.getLength());
//            respondBuff.addBuff(itemLength,3);//just hold the space
//            itemLength += nameLength;
//            respondBuff.addBuff(ent->d_name,nameLength);
//            *pItemLength = itemLength;
//            //for file type item
//            itemLength = 4;
//            _type = FUNC_DIR_TYPE_FILE_TYPE;
//            respondBuff.addBuff(_type,1);
//            pItemLength = (u16*)(respondBuff.buff+respondBuff.getLength());
//            respondBuff.addBuff(itemLength,3);
//            int typeLength = sizeof(ent->d_type);
//            respondBuff.addBuff(ent->d_type,typeLength);
//            itemLength += typeLength;
//            *pItemLength = itemLength;
//
//        }
//        closedir(dir);
//    }
//    u16 dataSize = respondBuff.getLength()-4;
//    memcpy(respondBuff.buff+1,&dataSize,2);
//    sendBuffer(clientSock,respondBuff.buff,respondBuff.getLength());
//
//}

/* File types for `d_type'. */

//enum
//{
//    DT_UNKNOWN = 0,
//# define DT_UNKNOWN DT_UNKNOWN
//    DT_FIFO = 1,
//# define DT_FIFO DT_FIFO
//    DT_CHR = 2,
//# define DT_CHR DT_CHR
//    DT_DIR = 4,
//# define DT_DIR DT_DIR
//    DT_BLK = 6,
//# define DT_BLK DT_BLK
//    DT_REG = 8,
//# define DT_REG DT_REG
//    DT_LNK = 10,
//# define DT_LNK DT_LNK
//    DT_SOCK = 12,
//# define DT_SOCK DT_SOCK
//    DT_WHT = 14
//# define DT_WHT DT_WHT
//};

#define FUNC_DIR_REQ_SIZE 1<<0
#define FUNC_DIR_REQ_TIME 1<<1
#define FUNC_DIR_REQ_PERM 1<<2
#define FUNC_DIR_REQ_USER 1<<3
#define FUNC_DIR_REQ_GROUP 1<<4
#define FUNC_DIR_REQ_LINK_TO 1<<5

typedef struct{
    char operationId;
    ssize_t totalNumber;
    ssize_t finishedNumber;
    char currentDescribe[100];
} OperationItem;

std::list<OperationItem*> operationList;
pthread_mutex_t listMutex = PTHREAD_MUTEX_INITIALIZER;

char *uid_to_name(uid_t uid)
{
   struct passwd *pw_ptr;
   static char numstr[10];

   if((pw_ptr=getpwuid(uid))==NULL)
   {
       sprintf(numstr,"%d",uid);
       return numstr;
   }
   else
       return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
   struct group *grp_ptr;
   static char numstr[10];
   if((grp_ptr=getgrgid(gid))==NULL)
   {
       sprintf(numstr,"%d",gid);
       return numstr;
   }
   else
       return grp_ptr->gr_name;
}

/*
dir request format:
    size:path size
    arg0:flags:
                bit0:request size
                bit1:request time
                bit2:request permission
                bit3:request user
                bit4:request group
                bit5:request link to
    data:path
*/
/*
dir respond format:
    arg0:result:
                    0:ok
                    1:fail
                    2:finish
    size:if(request==ok)
            count of files;
        else
            length of descript
    data:if(request==ok)
            file sections
        else
            decript string of the reason
file section format:
    size: count of the items
    data: item sections
item section format:
    size: size of the data
    arg0: type of the item:
                            type0:name
                            type1:filetype
                               DT_BLK      This is a block device.
                               DT_CHR      This is a character device.
                               DT_DIR      This is a directory.
                               DT_FIFO     This is a named pipe (FIFO).
                               DT_LNK      This is a symbolic link.
                               DT_REG      This is a regular file.
                               DT_SOCK     This is a UNIX domain socket.
                               DT_UNKNOWN  The file type is unknown.
                            type2:size
                            type3:time
                            type4:permission
                            type5:user
                            type6:group
                            type7:link to

*/
void doDir(char* reqBuff,int reqLength,int clientSock)
{
    package* requestSection = (package*)reqBuff;
    u8 flag = requestSection->arg0;
    //u32 pathLength = requestSection->size;
    std::string baseDir;
    baseDir = &requestSection->data;
    if(!baseDir.at(baseDir.length()-1)=='/')
    {
        baseDir+='/';
    }

    DIR* dir = opendir(&requestSection->data);
    AutoIncreaceBuffer respondBuff;
    respondBuff.addBuff(8);
    package* respondSection = (package*)respondBuff.buff;
    if(NULL == dir)
    {
        respondSection->arg0 = RES_ERROR;
        perror("open dir fail:");
        char* strErr = strerror(errno);
        int errLength = strlen(strErr)+1;
        respondSection->size = errLength;
        respondBuff.addBuff(strErr,errLength);
    }
    else
    {
        respondSection->arg0 = RES_OK;
        int fileCount = 0;
        struct dirent* ent;
        while ((ent = readdir(dir)))
        {
            package* fileSection = (package*)(respondBuff.buff+respondBuff.getLength());
            respondBuff.addBuff(8);
            int itemCount = 0;
            package* itemSection;
            //for name item
            itemCount++;
            itemSection = (package*)(respondBuff.buff+respondBuff.getLength());
            respondBuff.addBuff(8);
            itemSection->arg0 = FUNC_DIR_TYPE_NAME;
            int nameLength = strlen(ent->d_name)+1;
            respondBuff.addBuff(ent->d_name,nameLength);
            itemSection->size = nameLength;
            //for filetype item
            itemCount++;
            itemSection = (package*)(respondBuff.buff+respondBuff.getLength());
            respondBuff.addBuff(8);
            itemSection->arg0 = FUNC_DIR_TYPE_FILE_TYPE;
            int typeLength = sizeof(ent->d_type);
            respondBuff.addBuff((char*)&ent->d_type,typeLength);
            itemSection->size = typeLength;

            std::string filePath(baseDir);
            filePath+=ent->d_name;
            struct stat fileStat;
            if(stat(filePath.c_str(),&fileStat)<0)
            {
                perror("stat fail");
            }
            else
            {
                //for size item
                if(flag&FUNC_DIR_REQ_SIZE)
                {
                    itemCount++;
                    itemSection = (package*)(respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_SIZE;
                    int sizeLength = sizeof(fileStat.st_size);
                    respondBuff.addBuff((char*)&fileStat.st_size,typeLength);
                    itemSection->size = sizeLength;
                }
                //for time item
                if(flag&FUNC_DIR_REQ_TIME)
                {
                    itemCount++;
                    itemSection =(package*)( respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_TIME;
                    int timeLength = sizeof(fileStat.st_mtim);
                    respondBuff.addBuff((char*)&fileStat.st_mtim,timeLength);
                    itemSection->size = timeLength;
                }
                //for user item
                if(flag&FUNC_DIR_REQ_USER)
                {
                    itemCount++;
                    itemSection = (package*)(respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_USER;
                    char* userString = uid_to_name(fileStat.st_uid);
                    int userLength = strlen(userString)+1;
                    respondBuff.addBuff(userString,userLength);
                    itemSection->size = userLength;
                }
                //for group item
                if(flag&FUNC_DIR_REQ_GROUP)
                {
                    itemCount++;
                    itemSection =(package*)( respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_GROUP;
                    char* groupString = gid_to_name(fileStat.st_gid);
                    int groupLength = strlen(groupString)+1;
                    respondBuff.addBuff(groupString,groupLength);
                    itemSection->size = groupLength;
                }
                 //for premission item
                if(flag&FUNC_DIR_REQ_PERM)
                {
                    itemCount++;
                    itemSection = (package*)(respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_PERMISSION;
                    int modeLength = sizeof(fileStat.st_mode);
                    respondBuff.addBuff((char*)&fileStat.st_mode,modeLength);
                    itemSection->size = modeLength;
                }
                 //for link_to item
                if(flag&FUNC_DIR_REQ_LINK_TO&&ent->d_type==DT_LNK)
                {
                    itemCount++;
                    itemSection =(package*)(respondBuff.buff+respondBuff.getLength());
                    respondBuff.addBuff(8);
                    itemSection->arg0 = FUNC_DIR_TYPE_LINK_TO;
                    char linkString[512];
                    int linkLength = readlink(filePath.c_str(),linkString,511);//not end with \0
                    if(linkLength>0)
                    {
                        linkString[linkLength] = 0;
                        linkLength++;
                        respondBuff.addBuff(linkString,linkLength);
                        itemSection->size = linkLength;
                    }
                    else
                    {
                        linkString[0] = 0;
                        respondBuff.addBuff(linkString,1);
                        itemSection->size = 0;
                        perror("readlink fail");
                    }
                }
            }
            fileSection->size = itemCount;
            fileCount++;
        }
        respondSection->size = fileCount;
        closedir(dir);
    }
    sendBuffer(clientSock,respondBuff.buff,respondBuff.getLength());
}
/*
//copy report format:
//    arg0~1:number finished
//    arg2~3:number total
copy request format:
    size:dest path offset to the beginning of the data
    arg0:operation id
    data:source path + destination path
copy result format:
    arg0: state:
                0 ok
                1 fail
    size: if(error)
            descript length
    data: if(error)
            descript
*/
#define COPY_BUFF_SIZE 10*1024
void doCopy(char* reqBuff,int reqLength,int clientSock)
{
    package* requestSection = (package*)reqBuff;
    char* errString ;
    AutoIncreaceBuffer respondBuff;
    respondBuff.addBuff(8);
    package* respondSection = (package*)respondBuff.buff;
    int srcFd,dstFd;
    struct stat statTmp;
    OperationItem item ;
    srcFd = open(&requestSection->data,O_RDONLY);
    if(srcFd==-1)
    {
        perror("open source file fail");
        goto copyErr;
    }
    dstFd = open(&requestSection->data+requestSection->size,O_WRONLY);
    if(dstFd==-1)
    {
        perror("open destination file fail");
        goto copyErr;
    }
    if(fstat(srcFd,&statTmp))
    {
        item.totalNumber = statTmp.st_size;
    }else{
        item.totalNumber = 0;
        perror("stat fail");
    }
    item.finishedNumber = 0;
    item.operationId = requestSection->arg0;
    pthread_mutex_lock(&listMutex);
    operationList.push_back(&item);
    pthread_mutex_unlock(&listMutex);
    while(1)
    {
        char buff[COPY_BUFF_SIZE];
        ssize_t writeLength,readLength;
        readLength = read(srcFd,buff,COPY_BUFF_SIZE);
        if(readLength==0)
            break;
        if(readLength==-1)
          {
            perror("read err");
            goto copyErr;
          }
          writeLength = write(dstFd,buff,readLength);
          if(writeLength!=readLength)
          {
            perror("write err");
            goto copyErr;
          }
          item.finishedNumber+=readLength;
    }
    respondSection->arg0 = RES_OK;
    sendBuffer(clientSock,respondBuff.buff,respondBuff.getLength());
    pthread_mutex_lock(&listMutex);
    operationList.remove(&item);
    pthread_mutex_unlock(&listMutex);
    return;

copyErr:
    errString = strerror(errno);
    respondSection->arg0 = RES_ERROR;
    int errLength = strlen(errString)+1;
    respondBuff.addBuff(errString,errLength);
    respondSection->size = errLength;
    sendBuffer(clientSock,respondBuff.buff,respondBuff.getLength());
    pthread_mutex_lock(&listMutex);
    operationList.remove(&item);
    pthread_mutex_unlock(&listMutex);
    return;
}
/*
CreateFile request format:
    size : length of the path
    arg0 : force to creat
    arg1 : reserved
    arg23: file share mode
    data : file path
CreateFile respond format:
    arg0 : success or fail
    size : if(fail)
                length of the descript
    data : if(fail)
                the descript of the error
*/
void doCreateFile(char* reqBuff,int reqLength,int clientSock)
{
    package* requestSection = (package*)reqBuff;
    bool forceToCreate = requestSection->arg0 == 1;
    AutoIncreaceBuffer respondBuff;
    respondBuff.addBuff(8);
    package* respondSection = (package*)respondBuff.buff;
    if(access((char*)&requestSection->data,F_OK)==0)
    {
        /*file exits*/
        respondSection->arg0 = RES_ERROR;
        char* exitsErr = "the file areadly exits!";
        int exitsLength = strlen(exitsErr)+1;
        respondBuff.addBuff(exitsErr,exitsLength);
        respondSection->size = exitsLength;
        sendBuffer(clientSock,respondBuff.buff,respondBuff.getLength());
        return;
    }

}

void doCreateDir(char* reqBuff,int reqLength,int clientSock)
{

}

void doDelete(char* reqBuff,int reqLength,int clientSock)
{
}

void doMove(char* reqBuff,int reqLength,int clientSock)
{
}

void doRename(char* reqBuff,int reqLength,int clientSock)
{
}
/*
package header format:
    size:pakage size
    arg0: request code
    data(request):data of function request
*/
void doService(AutoIncreaceBuffer autoBuff,int clientSock)
{
    package* pPackage = (package*)autoBuff.buff;
    int packageLength = pPackage->size;
    switch(pPackage->arg0)
    {
     case REQ_DIR:
     doDir(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_COPY:
     doCopy(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_CREATE_FILE:
     doCreateFile(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_CREATE_DIR:
     doCreateDir(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_DELETE:
     doDelete(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_MOVE:
     doMove(&pPackage->data,packageLength-8,clientSock);
     break;
     case REQ_RENAME:
     doRename(&pPackage->data,packageLength-8,clientSock);
    }

}

void* newThread(void *_sock)
{
    int clientSock = *(int*)_sock;
    AutoIncreaceBuffer autoBuff;
    char buffer[BUFFER_SIZE];
    while(1)
    {
        int length = recv(clientSock,buffer,BUFFER_SIZE,0);
        if (length < 0)
        {
            printf("Server Recieve Data end\n");
            break;
        }
        autoBuff.addBuff(buffer,length);
    }
    if(autoBuff.getLength()>0)
        doService(autoBuff,clientSock);
    close(clientSock);
    return NULL;
}

void recvClient(int server_socket)
{
     //定义客户端的socket地址结构client_addr
        struct sockaddr client_addr;
        socklen_t length = sizeof(client_addr);
        pthread_attr_t attr_detach;
        pthread_attr_init(&attr_detach);
        pthread_attr_setdetachstate(&attr_detach,PTHREAD_CREATE_DETACHED);

        //接受一个到server_socket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            return;
        }

        pthread_t thread;
        if(pthread_create(&thread,&attr_detach,newThread,&new_server_socket)<0)
        {
            printf("create thread fail!\n");
            return;
        }

}

int main(int argc,char* argv[])
{
    int port = PORT;
    if(argc>0)
    {
         int temp = atoi(argv[0]);
         if(temp >0)
            port = temp;
    }
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket<0)
    {
        printf("create socket fail!");
        return -1;
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", port);
        return -2;
    }

       //server_socket用于监听
    if ( listen(server_socket, 10) )
    {
        printf("Server Listen Failed!");
        return -3;
    }

    while (1) //服务器端要一直运行
    {
       recvClient(server_socket);
    }

    return 0;
}
