#ifndef __MD5_H__
#define __MD5_H__

typedef struct md5_context
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
} md5_context_t;
                                    
void md5_init(md5_context_t *context);
void md5_update(md5_context_t *context, const unsigned char *input, unsigned int inputlen);
void md5_final(md5_context_t *context, unsigned char digest[16]);

#endif //__MD5_H__
