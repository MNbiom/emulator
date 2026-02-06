#pragma once
#include "TextEditor.h"

extern TextEditor editor;

extern bool showEditorWindow;

extern bool isPaused;
extern int rtps;
extern bool rtpsLimit;
extern bool stepInstruction;
extern bool highlightCurrentLine;

void do_editor();

void highlight_line();