
SECTIONS
{
    .text :
    {
        /* section information for initial. */
        . = ALIGN(4);
        __ua_init_start = .;
        KEEP(*(SORT(*.uai_fn*)))
        __ua_init_end = .;
    }
}