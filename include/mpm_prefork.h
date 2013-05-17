/**
 * @file   mpm_prefork.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef MPM_PREFORK_H
#define MPM_PREFORK_H

#include "et_event.h"

int mpm_prefork(int listenfd);

extern et_event_module_t et_empty_module;

#endif /* MPM_PREFORK_H */
