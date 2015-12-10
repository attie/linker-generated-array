SECTIONS {
	__lga_start = .;
	.lga : {
		KEEP(*(SORT(.lga_*)));
	}
	__lga_end = .;
}
INSERT BEFORE .bss;
