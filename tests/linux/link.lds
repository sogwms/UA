SECTIONS {
    .text : { 
        KEEP(*(SORT(.cus_auto_fn*)))
        /* section information for finsh shell */
        . = ALIGN(4);
        PROVIDE_HIDDEN(__fsymtab_start = .);
        KEEP(*(FSymTab));
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);
        KEEP(*(.rodata.name))

        /* section information for initial. */
        . = ALIGN(4);
        __ua_init_start = .;
        KEEP(*(SORT(.uai_fn*)))
        __ua_init_end = .;
    }
    
}