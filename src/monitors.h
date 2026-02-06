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

void get_change();

void do_ips();

void do_regs();

void do_flags();