/**
 * Ideal Indirection Lab
 * CS 241 - Fall 2016
 */
#include "kernel.h"
#include "mmu.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

MMU *MMU_create() {
  MMU *mmu = calloc(1, sizeof(MMU));
  mmu->tlb = TLB_create();
  mmu->curr_pid = 0;
  return mmu;
}

void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid) {
    assert(pid < MAX_PROCESS_ID);
    if(pid != mmu->curr_pid){
        TLB_flush(&mmu->tlb);
    }
	//printf("pid:%zu\n", mmu->curr_pid);
	mmu->curr_pid = pid;
	size_t offset = (size_t)virtual_address & 0x7fff;
    size_t key = ((size_t)virtual_address >> 15) & 0xfffffffff;
    void* physical_address = TLB_get_physical_address(&(mmu->tlb), (void*)key);
    if(physical_address){
        return (char*)physical_address + offset;
    }    
        MMU_tlb_miss(mmu, virtual_address, pid);
        size_t VPN_1 = key >> 24;
        size_t VPN_2 = (key & 0x000fff000) >> 12;
        size_t VPN_3 = key & 0x000000fff;
        PageTable *pt_1 = mmu->base_pts[pid];
        void* pt_2 = PageTable_get_entry(pt_1, VPN_1);   
        if(!pt_2){
                MMU_raise_page_fault(mmu, virtual_address, pid);
                pt_2 = PageTable_create(); 
                PageTable_set_entry(pt_1, VPN_1, pt_2); 
        }
        void* pt_3 = PageTable_get_entry(pt_2, VPN_2); 
        if(!pt_3){
                MMU_raise_page_fault(mmu, virtual_address, pid);
                pt_3 = PageTable_create(); 
                PageTable_set_entry(pt_2, VPN_2, pt_3); 
        }
        physical_address = PageTable_get_entry(pt_3, VPN_3);
        if(!physical_address){
            MMU_raise_page_fault(mmu, virtual_address, pid);
            physical_address = ask_kernel_for_frame();
            PageTable_set_entry(pt_3, VPN_3, physical_address); 
        }
        TLB_add_physical_address(&(mmu->tlb), (void*)key, physical_address);
		//printf("key:%zu\n", (size_t)key);
		//printf("physical_address:%zu\n", (size_t)physical_address);
  return (char*)physical_address + offset;
}

void MMU_tlb_miss(MMU *mmu, void *address, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->num_tlb_misses++;
  printf("Process [%lu] tried to access [%p] and it couldn't find it in the "
         "TLB so the MMU has to check the PAGE TABLES\n",
         pid, address);
}

void MMU_raise_page_fault(MMU *mmu, void *address, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->num_page_faults++;
  printf(
      "Process [%lu] tried to access [%p] and the MMU got an invalid entry\n",
      pid, address);
}

void MMU_add_process(MMU *mmu, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->base_pts[pid] = PageTable_create();
}

void MMU_free_process_tables(MMU *mmu, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  PageTable *base_pt = mmu->base_pts[pid];
  Pagetable_delete_tree(base_pt);
}

void MMU_delete(MMU *mmu) {
  for (size_t i = 0; i < MAX_PROCESS_ID; i++) {
    MMU_free_process_tables(mmu, i);
  }
  TLB_delete(mmu->tlb);
  free(mmu);
}
