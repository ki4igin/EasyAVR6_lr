#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

struct fifo
{
    uint8_t head;
    uint8_t tail;
    uint8_t data[];
};

#define fifo_declarate(_id, _size) \
    struct _id                     \
    {                              \
        uint8_t head;              \
        uint8_t tail;              \
        uint8_t data[_size];       \
    } _id

#define fifo_inc_mask(_id, _val) \
    ((_id)->_val = ((_id)->_val + 1) & (sizeof((_id)->data) - 1))

#define fifo_push(_id, _data)               \
    do                                      \
    {                                       \
        (_id)->data[(_id)->head] = (_data); \
        fifo_inc_mask(_id, head);           \
    }                                       \
    while (0)

#define fifo_pop(_id)                       \
    ({                                      \
        typeof((_id)->data[0]) _return;     \
        _return = (_id)->data[(_id)->tail]; \
        fifo_inc_mask(_id, tail);           \
        _return;                            \
    })

#define fifo_is_empty(_id) ((_id)->head == (_id)->tail)
#define fifo_is_full(_id) \
    ((((_id)->head + 1) & (sizeof((_id)->data) - 1)) == (_id)->tail)

// #undef fifo_inc_mask

#endif
