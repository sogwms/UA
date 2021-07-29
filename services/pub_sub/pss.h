/**
 * Copyright (c) 2019, sogwyms@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * @version 0.1.0
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-01     sogwms       The first version
 */

#ifndef __PSS_H__
#define __PSS_H__

enum pss_topic_e {
    PSS_TOPIC_INVALID = 0,
    PSS_TOPIC_PSS_TEST,
    PSS_TOPIC_END,
};

typedef void (*pss_cb_f)(unsigned int topic, void *pd);

int pss_subscribe(unsigned int topic, pss_cb_f callback);
int pss_unsubcribe(unsigned int topic, pss_cb_f callback);
int pss_publish(unsigned int topic, void *pd);

void pss_report_memory_info(void);

#endif /* __PSS_H__ */
