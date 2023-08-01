/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static void config_nvmc(uint32_t val)
{
    while (!NRF_NVMC_S->READY);
    NRF_NVMC_S->CONFIG = val;
    while (!NRF_NVMC_S->READY);
}

void main(void)
{
    uint32_t read_val;

    //NRF_UICR_S->APPROTECT = 0x00000001;



    printk("Start approtect sample.\n");
    NRF_CTRLAP_S->APPROTECT.DISABLE = 0x00000001;

    // Somehow, reading the value back does not work.
    // printk("NRF_CTRLAP_S->ERASEPROTECT.DISABLE: %08x\n",NRF_CTRLAP_S->ERASEPROTECT.DISABLE);

    if(NRF_UICR_S->APPROTECT){
        config_nvmc(NVMC_CONFIG_WEN_Wen);
        NRF_UICR_S->APPROTECT=0x00000000;
        config_nvmc(NVMC_CONFIG_WEN_Ren);
        NVIC_SystemReset();
    }

    //I dont think this is the right place for this. I tried it at the start, no dice.
    /* Prevent processor from unlocking APPROTECT soft branch after this point. */
    //NRF_CTRLAP_S->APPROTECT.LOCK = CTRLAPPERI_APPROTECT_LOCK_LOCK_Locked;

    printk("Entering forever loop.\n");
    printk("Disclaimer: This example does not lock netcore, and can be unlocked by recovering netcore!\n See README for more information.\n");
    while(1){
        k_sleep(K_SECONDS(1));
    }
}
