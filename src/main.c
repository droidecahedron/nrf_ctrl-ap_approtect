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

    printk("Start approtect sample.\n");
    NRF_CTRLAP_S->APPROTECT.DISABLE = 0x00000055;

    // Somehow, reading the value back does not work.
    // printk("NRF_CTRLAP_S->ERASEPROTECT.DISABLE: %08x\n",NRF_CTRLAP_S->ERASEPROTECT.DISABLE);
    //NRF_CTRLAP_S->SECUREAPPROTECT.DISABLE = 0x00000001;

    if(!NRF_UICR_S->APPROTECT){
        config_nvmc(NVMC_CONFIG_WEN_Wen);
        NRF_UICR_S->APPROTECT=0x000000FF;
        config_nvmc(NVMC_CONFIG_WEN_Ren);
        NVIC_SystemReset();
    }

    printk("Entering forever loop.\n");
    printk("Disclaimer: This example does not lock netcore, and can be unlocked by recovering netcore!\n See README for more information.\n");
    while(1){
        k_sleep(K_SECONDS(1));
    }
}
