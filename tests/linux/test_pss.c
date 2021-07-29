#include <ua_def.h>
#include "pss.h"

#define LOG_TAG "TEST-PSS"
#include <elog.h>

void topic_callback(unsigned int topic, void *pd)
{
    log_i("received topic: %d,  pd: 0x%x", topic, pd);
}

void topic_callback2(unsigned int topic, void *pd)
{
    log_i("2 received topic: %d,  pd: 0x%x", topic, pd);
}

int test_pss_entry(void)
{
    log_i("Startup");
    pss_report_memory_info();

    pss_subscribe(PSS_TOPIC_PSS_TEST, topic_callback);
    pss_subscribe(PSS_TOPIC_PSS_TEST, topic_callback);
   
    pss_unsubcribe(PSS_TOPIC_PSS_TEST, topic_callback);

    pss_report_memory_info();

    pss_subscribe(PSS_TOPIC_PSS_TEST, topic_callback);

#if 1
    for (int i = 0; i < 129; i++) {
        pss_subscribe(PSS_TOPIC_PSS_TEST, topic_callback);
    }
    pss_report_memory_info();

    for (int i = 0; i < 100; i++) {
        pss_unsubcribe(PSS_TOPIC_PSS_TEST, topic_callback);
    }
    pss_report_memory_info();
#endif


    pss_subscribe(PSS_TOPIC_PSS_TEST, topic_callback2);
    pss_publish(PSS_TOPIC_PSS_TEST, (void *)1);

    pss_report_memory_info();

    return 0;
}

UA_EXPORT_INIT_APP(test_pss_entry);
