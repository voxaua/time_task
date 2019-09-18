#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/types.h>



MODULE_AUTHOR("Volodymyr Ivashchenko <>");
MODULE_DESCRIPTION("Hello, world");
MODULE_LICENSE("Dual BSD/GPL");

static struct workqueue_struct *wq;
static struct work_struct *ws;
static atomic_t run = ATOMIC_INIT(1);

static void tick_tack(struct work_struct *);

static int __init tick_init(void)
{
    _Bool res;
    wq = create_singlethread_workqueue("tick_q");
    if (wq) {
        ws = (struct work_struct*)kmalloc(sizeof(struct work_struct), GFP_KERNEL);
        INIT_WORK(ws, tick_tack);
        res = queue_work(wq, ws);
        if(!res) {
            pr_err("Failed to start work queue.\n");
            goto failed_qwork;
        }
        return 0;
    } else {
        goto failed_create;
    }

failed_qwork:
    destroy_workqueue(wq);
    return -1;
failed_create:
    return -1;
}

static void __exit tick_exit(void)
{
        atomic_set(&run, 0);
        flush_workqueue(wq);
        destroy_workqueue(wq);
}

static void tick_tack(struct work_struct *work)
{
    for(;;) {
        if (atomic_read(&run)) {
            msleep_interruptible(1000);
            pr_err("Tick tack\n");
        } else {
            break;
        }
    }
}

module_init(tick_init);
module_exit(tick_exit);
