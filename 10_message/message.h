#ifndef MESSAGE_H
#define MESSAGE_H

struct Message
{
    unsigned short type,cmd,index,total;
    unsigned int length;
    unsigned char payload[];
};

Message * Message_New(unsigned short type,
                    unsigned short cmd,
                    unsigned short index,
                    unsigned short total,
                    const char * payload,
                    unsigned int length);

#endif  // MESSAGE_H