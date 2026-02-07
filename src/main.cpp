#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "consola_font.h"

#include "gui.h"
#include "display.h"
#include "assembler.h"
#include "editor.h"
#include "monitors.h"

#include "isa.h"

int main(int argc, char const *argv[]){

	int windowWidth = 1600;
	int windowHeight = 900;
	int FPS = 60;

	double frameTime = 1/(double)FPS;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(windowWidth, windowHeight, "MNemulator");
	SetExitKey(KEY_NULL);
	SetTargetFPS(FPS);

	rlImGuiSetup(true);
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::GetStyle().FrameRounding = 3.0f;
	ImGui::GetIO().IniFilename = nullptr;
	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(consola_ttf, consola_ttf_len, 14.0f, &cfg);

	init_display();
	clear_display();
	draw_pixel(0, 0);
	draw_pixel(2, 2);
	draw_pixel(15, 15);
	draw_pixel(DISPLAY_MAX_SIZE-1, DISPLAY_MAX_SIZE-1);

	double lastInstrTime = 0;
	double ipsTimer = 0;
	int ipsCounter = 0;

	while (!WindowShouldClose()){
		BeginDrawing(); //raylib

		ClearBackground(Color{24, 24, 24, 255});

		rlImGuiBegin(); //imgui

		update_display();
		
		//execution loop
		static bool wasPaused = true;
		static bool wasRtpsLimit = true;
		if ((wasPaused && !isPaused) || (!wasRtpsLimit && rtpsLimit)) lastInstrTime = GetTime();
		wasPaused = isPaused;
		wasRtpsLimit = rtpsLimit;
		double instrInterval = CLOCK_SPEED/rtps;
		double startTime = GetTime();
		int instrCounter = 0;
		if (!isPaused){
			if (rtpsLimit){
				while (((GetTime() - lastInstrTime >= instrInterval)) && (GetTime() - startTime < frameTime)){
					exec_instr(rom[pc]);
					lastInstrTime += instrInterval;
					ipsCounter++;
				}
			}
			else{
				while (true) {
					exec_instr(rom[pc]);
					ipsCounter++;
					instrCounter++;
					if (instrCounter & 0x100000) {
						instrCounter = 0;
						if (GetTime() - startTime >= frameTime)
							break;
					}
				}
			}
			get_change();
			if (highlightCurrentLine) highlight_line();
		}
		if (stepInstruction){
			exec_instr(rom[pc]);
			get_change();
			if (highlightCurrentLine) highlight_line();
			ipsCounter++;
			stepInstruction = false;
		}

		if(GetTime() - ipsTimer >= 1){
			ipsTimer = GetTime();
			ips = ipsCounter;
			ipsCounter = 0;
		}

		gui();

		rlImGuiEnd();
		EndDrawing();
	}
	shutdown_display();

	rlImGuiShutdown();
	CloseWindow();
	
	return 0;
}

