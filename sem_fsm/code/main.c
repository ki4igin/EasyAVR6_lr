#include "avr/io.h"
#include "avr/interrupt.h"
#include <avr/pgmspace.h>
#include <assert.h>

#include "main.h"

enum state
{
    STATE_NONE = 0,
    STATE_TIME_SHOW,
    STATE_TIME_SET,
    STATE_TEMP_SHOW,
    STATE_TEMP_SET,
    STATE_HUM_SHOW,
    STATE_CNT
};

enum event
{
    EVENT_NONE = 0,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_SET,
    EVENT_BACK,
    EVENT_CNT
};

const enum state fsm_state_table[STATE_CNT][EVENT_CNT] PROGMEM = {

    [STATE_TIME_SHOW][EVENT_UP] = STATE_TEMP_SHOW,
    [STATE_TIME_SHOW][EVENT_DOWN] = STATE_HUM_SHOW,
    [STATE_TIME_SHOW][EVENT_SET] = STATE_TIME_SET,

    [STATE_TEMP_SHOW][EVENT_UP] = STATE_HUM_SHOW,
    [STATE_TEMP_SHOW][EVENT_DOWN] = STATE_TIME_SHOW,
    [STATE_TEMP_SHOW][EVENT_SET] = STATE_TEMP_SET,

    [STATE_HUM_SHOW][EVENT_UP] = STATE_TIME_SHOW,
    [STATE_HUM_SHOW][EVENT_DOWN] = STATE_TEMP_SHOW,

    [STATE_TIME_SET][EVENT_BACK] = STATE_TIME_SHOW,

    [STATE_TEMP_SET][EVENT_BACK] = STATE_TEMP_SHOW};

typedef void (*vfprt_t)(void);

// void (*const fsm_action_table[STATE_CNT])(void) PROGMEM = {
//     [STATE_TIME_SHOW] = time_show,
//     [STATE_TIME_SET] = time_set,
//     [STATE_TEMP_SHOW] = temp_show,
//     [STATE_TEMP_SET] = temp_set,
//     [STATE_HUM_SHOW] = hum_show,
// };
const vfprt_t fsm_action_table[STATE_CNT] PROGMEM = {
    [STATE_TIME_SHOW] = time_show,
    [STATE_TIME_SET] = time_set,
    [STATE_TEMP_SHOW] = temp_show,
    [STATE_TEMP_SET] = temp_set,
    [STATE_HUM_SHOW] = hum_show,
};

static enum state state;
static enum event event;

// static enum state fsm_next_state(enum state state_old, enum event event)
// {
//     switch (state_old)
//     {
//         case STATE_TIME_SHOW:
//             switch (event)
//             {
//                 case EVENT_UP:
//                     state_old = STATE_TEMP_SHOW;
//                     break;
//                 case EVENT_DOWN:
//                     state_old = STATE_HUM_SHOW;
//                     break;
//                 case EVENT_SET:
//                     state_old = STATE_TIME_SET;
//                     break;
//                 case EVENT_BACK:
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         case STATE_TIME_SET:
//             switch (event)
//             {
//                 case EVENT_UP:
//                     break;
//                 case EVENT_DOWN:
//                     break;
//                 case EVENT_SET:
//                     break;
//                 case EVENT_BACK:
//                     state_old = STATE_TIME_SHOW;
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         case STATE_TEMP_SHOW:
//             switch (event)
//             {
//                 case EVENT_UP:
//                     state_old = STATE_HUM_SHOW;
//                     break;
//                 case EVENT_DOWN:
//                     state_old = STATE_TIME_SHOW;
//                     break;
//                 case EVENT_SET:
//                     state_old = STATE_TEMP_SET;
//                     break;
//                 case EVENT_BACK:
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         case STATE_TEMP_SET:
//             switch (event)
//             {
//                 case EVENT_UP:
//                     break;
//                 case EVENT_DOWN:
//                     break;
//                 case EVENT_SET:
//                     break;
//                 case EVENT_BACK:
//                     state_old = STATE_TEMP_SHOW;
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         case STATE_HUM_SHOW:
//             switch (event)
//             {
//                 case EVENT_UP:
//                     state_old = STATE_TIME_SHOW;
//                     break;
//                 case EVENT_DOWN:
//                     state_old = STATE_TEMP_SHOW;
//                     break;
//                 case EVENT_SET:
//                     break;
//                 case EVENT_BACK:
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         default:
//             break;
//     }

//     return state_old;
// }

static enum state fsm_next_state(enum state state_old, enum event event)
{
    enum state state_new = pgm_read_byte(&fsm_state_table[state_old][event]);

    return state_new == STATE_NONE ? state_old : state_new;
}

// void fsm_action(enum state state)
// {
//     switch (state)
//     {
//         case STATE_HUM_SHOW:
//             hum_show();
//             break;
//         case STATE_TEMP_SET:
//             temp_set();
//             break;
//         case STATE_TEMP_SHOW:
//             temp_show();
//             break;
//         case STATE_TIME_SET:
//             time_set();
//             break;
//         case STATE_TIME_SHOW:
//             time_show();
//             break;
//         default:
//             break;
//     }
// }

void fsm_action(enum state state)
{
    vfprt_t func = (vfprt_t)pgm_read_word(&fsm_action_table[state]);
    func();
}

int main(void)
{
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);
    sei();

    state = STATE_TIME_SHOW;

    while (1)
    {
        enum state state_new = fsm_next_state(state, event);
        fsm_action(state_new);
        state = state_new;
        event = EVENT_NONE;
    }
}

ISR(TIMER2_OVF_vect)
{
    static uint8_t btn_lock = 0;

    uint8_t pin = PINA & 0x0F;
    if (pin)
    {
        if (btn_lock == 0)
        {
            btn_lock = 0;
            if (pin & (1 << PINA0))
            {
                event = EVENT_UP;
            }
            else if (pin & (1 << PINA1))
            {
                event = EVENT_DOWN;
            }
            else if (pin & (1 << PINA2))
            {
                event = EVENT_SET;
            }
            else if (pin & (1 << PINA3))
            {
                event = EVENT_BACK;
            }
        }
    }
    else
    {
        btn_lock = 0;
    }
}

/* End File */