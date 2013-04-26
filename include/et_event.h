/**
 * @file   et_event.h
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#ifndef ET_EVENT_H
#define ET_EVENT_H

struct et_event_s
{
    void *data;
    
    unsigned write:1;

    unsigned accept:1;

    unsigned ready:1;

    int index;
    
};

typedef struct et_event_s et_event_t;



#endif /* ET_EVENT_H */
