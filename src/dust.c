/*
 * dust.c
 *
 *  Created on: Dec 2, 2012
 *      Author: vic
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dust.h"
#include "icp.h"

uint8_t dust_occupancy_read_idx = 0;
uint32_t dust_occupancy[2] = {0xffffffff, 0xffffffff};

void dust_init(){
    dust_occupancy[0] = 0xffffffff;
    dust_occupancy[1] = 0xffffffff;
}

uint32_t get_dust_occupancy(){
    return dust_occupancy[dust_occupancy_read_idx];
}

void dust_enq(){
    uint8_t dust_occupancy_write_idx;
    dust_occupancy_read_idx = 1 - dust_occupancy_read_idx;
    dust_occupancy_write_idx = 1 - dust_occupancy_read_idx;
    dust_occupancy[dust_occupancy_write_idx] = 0;
}

void dust_process(){
    uint8_t dust_occupancy_write_idx = 1 - dust_occupancy_read_idx;
    icp_sample_t sample = icp_rx();
    const icp_sample_t empty_q = (icp_sample_t) -1;
    if(sample != empty_q){
        dust_occupancy[dust_occupancy_write_idx] += sample;
    }
}

