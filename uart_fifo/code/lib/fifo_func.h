#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

struct __attribute__((packed)) fifo
{
    const uint8_t mask;
    uint8_t head;
    uint8_t tail;
    uint8_t data[];
};

#define fifo_declarate(_id, _size) \
    struct fifo _id = {            \
        .mask = _size - 1,         \
        .data = {                  \
            [0 ... _size - 1] = 0}}

#define fifo_inc_mask(_id, _val) \
    ((_id)->_val = ((_id)->_val + 1) & (_id->mask))

static inline void fifo_push(struct fifo *id, uint8_t data)
{
    id->data[id->head] = data;
    fifo_inc_mask(id, head);
}

static inline uint8_t fifo_pop(struct fifo *id)
{
    uint8_t data = id->data[id->tail];
    fifo_inc_mask(id, tail);
    return data;
}

static inline uint8_t fifo_is_empty(struct fifo *id)
{
    return id->head == id->tail;
}
static inline uint8_t fifo_is_full(struct fifo *id)
{
    return ((id->head + 1) & id->mask) == id->tail;
}

#undef fifo_inc_mask

#endif