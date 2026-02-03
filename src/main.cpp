#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "consola_font.h"

#include "gui.h"
#include "display.h"


int main(int argc, char const *argv[]){

    int windowWidth = 1600;
	int windowHeight = 900;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(windowWidth, windowHeight, "MNemulator");
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

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



	while (!WindowShouldClose()){
		BeginDrawing(); //raylib

		ClearBackground(Color{24, 24, 24, 255});

		rlImGuiBegin(); //imgui

		update_display();

		gui();

		rlImGuiEnd();
		EndDrawing();
	}
	shutdown_display();

	rlImGuiShutdown();
	CloseWindow();
	
	return 0;
}

