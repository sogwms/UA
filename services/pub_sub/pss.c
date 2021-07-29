#include <assert.h>
#include "pss.h"
#include "pss_cfg.h"

#include <ua_def.h>

#define LOG_TAG "PSS"
#include <elog.h>

struct pss_topic_subscription
{
    pss_cb_f cb;
    ua_slist_t link;
};

struct pss_topic
{
    unsigned int id;
    ua_slist_t list_head;
};

struct pss_server
{
    struct pss_topic topics[MAX_NUMBER_OF_TOPICS];
};

static unsigned char __memory_pool[MAX_NUMBER_OF_SUBSCRIPTIONS * sizeof(struct pss_topic_subscription)];
static short __memory_pool_table[MAX_NUMBER_OF_SUBSCRIPTIONS] = {0};

static struct pss_server g_pss_server = {0};

static_assert((PSS_TOPIC_END - 1) <= MAX_NUMBER_OF_TOPICS, "Fatal error. The defined topics have exceeded 'MAX_NUMBER_OF_TOPICS' ");

static struct pss_topic_subscription *__allocate(void)
{
    for (long i = 0; i < MAX_NUMBER_OF_SUBSCRIPTIONS; i++)
    {
        if (__memory_pool_table[i] == 0)
        {
            __memory_pool_table[i] = 1;
            return (struct pss_topic_subscription *)&__memory_pool[i * sizeof(struct pss_topic_subscription)];
        }
    }

    return NULL;
}

static void __free(struct pss_topic_subscription *res)
{
    unsigned char *p1, *p2;

    p1 = &__memory_pool[0];
    p2 = (unsigned char *)res;

    __memory_pool_table[(p2 - p1) / sizeof(struct pss_topic_subscription)] = 0;
}

void pss_report_memory_info(void)
{
    int len_used = 0;

    log_i("Total number of blocks: %d", MAX_NUMBER_OF_SUBSCRIPTIONS);

    for (long i = 0; i < MAX_NUMBER_OF_SUBSCRIPTIONS; i++)
    {
        if (__memory_pool_table[i] == 1)
        {
            log_i("block %d is used", i);
            len_used++;
        }
    }

    log_i("Blocks used: %d", len_used);
}

int pss_subscribe(unsigned int topic, pss_cb_f callback)
{
    // TODO
    // check Parameters

    struct pss_topic_subscription *new_sub = __allocate();

    if (new_sub == NULL)
    {
        log_a("allocate memory failed");
        return UA_ERROR;
    }

    new_sub->cb = callback;
    ua_slist_append(&g_pss_server.topics[topic].list_head, &new_sub->link);

    return UA_EOK;
}

int pss_unsubcribe(unsigned int topic, pss_cb_f callback)
{
    // TODO
    // check Parameters

    struct pss_topic_subscription *sub;

    ua_slist_for_each_entry(sub, &g_pss_server.topics[topic].list_head, link)
    {
        if (sub->cb == callback)
        {
            break;
        }
    }

    if (sub == NULL)
    {
        log_w("unsubcribing null occured");
        return UA_EOK;
    }

    ua_slist_remove(&g_pss_server.topics[topic].list_head, &sub->link);
    __free(sub);

    return UA_EOK;
}

int pss_publish(unsigned int topic, void *pd)
{
    // TODO
    // check Parameters

    struct pss_topic_subscription *sub;
    ua_slist_for_each_entry(sub, &g_pss_server.topics[topic].list_head, link)
    {
        sub->cb(topic, pd);
    }

    return UA_EOK;
}
