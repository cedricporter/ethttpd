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

#include "ethttpd.h"


#define et_process_events et_event_actions.process_events
#define et_add_event et_event_actions.add
#define et_del_event et_event_actions.del


#define ET_READ_EVENT 0
#define ET_WRITE_EVENT 1

#define INVALID_INDEX 99999999

#define  ET_OK          0
#define  ET_ERROR      -1
#define  ET_AGAIN      -2
#define  ET_BUSY       -3
#define  ET_DONE       -4
#define  ET_DECLINED   -5
#define  ET_ABORT      -6


extern et_event_t *et_posted_accept_events;
extern et_event_t *et_posted_events;


struct et_event_s
{
    void 	*data;
    
    unsigned write:1;

    unsigned accept:1;

    unsigned ready:1;

    unsigned int index;

    et_event_handler_pt  handler;

    et_event_t	*next;
    et_event_t	**prev;
    
};


typedef struct {
    void  (*add)(et_event_t *ev, int event);
    void  (*del)(et_event_t *ev, int event);

    void  (*process_events)();

    void  (*init)();
} et_event_actions_t;



typedef struct {
    et_event_actions_t		actions;
} et_event_module_t;


extern et_event_module_t et_event_module;
extern et_event_actions_t et_event_actions;

void et_event_accept(et_event_t *ev);
void et_event_process_posted(et_event_t **posted);
void et_delete_posted_event(et_event_t *ev);



#endif /* ET_EVENT_H */
