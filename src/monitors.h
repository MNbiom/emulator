#pragma once

extern int ips;

extern bool showIpsWindow;
extern bool resetIpsPos;

extern bool showRegsWindow;
extern bool resetRegsPos;
extern bool resetRegsSize;

extern bool showFlagsWindow;
extern bool resetFlagsPos;
extern bool resetFlagsSize;

extern bool showRamWindow;
extern bool resetRamPos;
extern bool resetRamSize;

extern bool showDStackWindow;
extern bool resetDStackPos;
extern bool resetDStackSize;

extern bool showCStackWindow;
extern bool resetCStackPos;
extern bool resetCStackSize;

extern bool showOutWindow;
extern bool resetOutPos;
extern bool resetOutSize;

void get_change();

void do_ips();

void do_regs();

void do_flags();

void do_ram();

void do_dstack();

void do_cstack();

void do_out();