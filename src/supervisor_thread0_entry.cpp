#include "supervisor_thread0.h"

/* Supervisor entry function */
void supervisor_thread0_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (1);
    }
}
