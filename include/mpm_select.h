/**
 * @file   mpm_select.h
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#ifndef MPM_SELECT_H
#define MPM_SELECT_H

#include "ethttpd.h"
#include "et_event.h"

int mpm_select(int listenfd);

extern et_event_module_t et_select_module;

void
et_locked_post_event(et_event_t *ev, et_event_t **queue);


#endif /* MPM_SELECT_H */
