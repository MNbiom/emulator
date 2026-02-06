#pragma once
#include "TextEditor.h"

extern TextEditor editor;

extern bool showEditorWindow;
extern bool resetEditorPos;
extern bool resetEditorSize;

extern bool isPaused;
extern int rtps;
extern bool rtpsLimit;
extern bool stepInstruction;
extern bool highlightCurrentLine;
extern bool followHighlights;

void do_editor();

void highlight_line();