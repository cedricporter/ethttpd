/**
 * @file   mpm_epoll.h
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#ifndef MPM_EPOLL_H
#define MPM_EPOLL_H


#include "ethttpd.h"
#include "et_event.h"

int mpm_epoll(int listenfd);

extern et_event_module_t et_epoll_module;


#endif /* MPM_EPOLL_H */
