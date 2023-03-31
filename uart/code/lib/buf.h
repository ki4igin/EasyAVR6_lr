#ifndef BUF_H
#define BUF_H

#define buf_declare(_id, _size) \
    struct _id                  \
    {                           \
        uint8_t count;          \
        uint8_t data[_size];    \
    } _id

#endif
