#include "AutoIncreaceBuffer.h"
#include "malloc.h"
#include <string.h>


#define DEFAULT_INIT_SIZE 2048

void AutoIncreaceBuffer::init(int initSize)
{
    capacity = initSize;
    buff =(char*) malloc(capacity);
    length = 0;
}

void AutoIncreaceBuffer::clear()
{
    length = 0;
}

AutoIncreaceBuffer::AutoIncreaceBuffer(int initSize)
{
    init(initSize);
}

AutoIncreaceBuffer::AutoIncreaceBuffer()
{
    init(DEFAULT_INIT_SIZE);
}

void AutoIncreaceBuffer::addBuff(char* _buff,int _size)
{
    int freeSize =  capacity - length;
    if(freeSize<_size)
    {
        int newLength = length + _size;
        newLength = newLength>2*capacity?newLength:2*capacity;
        char* newBuff = (char*)malloc(newLength);
        memcpy(newBuff,buff,length);
        free(buff);
        buff = newBuff;
        capacity = newLength;
    }
    memcpy(buff+length,_buff,_size);
    length+=_size;
}

void AutoIncreaceBuffer::addBuff(int _size)
{
    int freeSize =  capacity - length;
    if(freeSize<_size)
    {
        int newLength = length + _size;
        newLength = newLength>2*capacity?newLength:2*capacity;
        char* newBuff = (char*)malloc(newLength);
        memcpy(newBuff,buff,length);
        free(buff);
        buff = newBuff;
        capacity = newLength;
    }
    memset(buff+length,0,_size);
    length+=_size;
}

int AutoIncreaceBuffer::getCapacity()
{
    return capacity;
}

int AutoIncreaceBuffer::getLength()
{
    return length;
}

AutoIncreaceBuffer::~AutoIncreaceBuffer()
{
   free(buff);
}
