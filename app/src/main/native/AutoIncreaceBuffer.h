#ifndef AUTOINCREACEBUFFER_H
#define AUTOINCREACEBUFFER_H


class AutoIncreaceBuffer
{
    public:
        AutoIncreaceBuffer();
        AutoIncreaceBuffer(int initSize);
        virtual ~AutoIncreaceBuffer();
        int getLength();
        int getCapacity();
        char* buff;
        void addBuff(char* _buff,int _size);
        void addBuff(int _size);
        void clear();
    protected:
    private:
    void init(int initSize);
    int length;
    int capacity;

};

#endif // AUTOINCREACEBUFFER_H
