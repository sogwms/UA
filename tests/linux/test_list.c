#include <stdlib.h>
#include <ua_def.h>

#define LOG_TAG "test-can-bus"
#include <elog.h>

#define NUM_OF_NODES 32

typedef struct {
    int value;
    int id;
    ua_list_t link;
}test_list_t;

int test_list_entry(void)
{
    log_i("Startup");
    test_list_t list_head;

    list_head.value = -1;
    ua_list_init(&list_head.link);

    log_i("Going to insert %d new nodes", NUM_OF_NODES);
    for (int i = 0; i < NUM_OF_NODES; i++) {
        test_list_t *p = malloc(sizeof(test_list_t));
        p->value = i;
        p->id = i+1;
        ua_list_insert_before(&list_head.link, &p->link);
    }
    log_i("Done");
    unsigned int length = ua_list_len(&list_head.link);
    log_i("Current length: %d", length);

    ua_list_t *node;
    test_list_t *ins;
#if 0
    ua_list_for_each(node, &list_head.link) {
        ins = ua_list_entry(node, test_list_t, link);
        log_i("node%d, value: %d", ins->id, ins->value);
    }
#else
    ua_list_for_each_entry(ins, &list_head.link, link) {
        log_i("node%d, value: %d", ins->id, ins->value);
    }
#endif
    // ua_list_remove();

}

UA_EXPORT_INIT_APP(test_list_entry);
