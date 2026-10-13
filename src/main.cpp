#include "raylib.h"
#include <string>

int main(){

	auto windowWidth  = 800;
	auto windowHeight = 600;

    std::string title = "MinerPirates";

	InitWindow(windowWidth, windowHeight, title.c_str());
	SetTargetFPS(60);
	
	while(!WindowShouldClose()) {

		ClearBackground(RED);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}