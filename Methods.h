#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "vendor/stb_image/stb_images.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Methods
{

public:
	static unsigned int whiteVAO, blackVAO;
	static unsigned int whiteVBO, blackVBO;
	static void initializeResources();
	static void drawWhiteSquareMatrix(float xPosition, float yPosition, float size);
	static void drawBlackSquareMatrix(float xPosition, float yPosition, float size);
	static void drawWhiteRow(float xPosition, float yPosition);
	static void drawBlackRow(float xPosition, float yPosition);
	static void drawChessboard();
	static void drawChessPiece(char filePath[128], float xPosition, float yPosition, float size);
	static void drawRedPieces();
	static void drawBluePieces();
	struct ChessPiece
	{
		bool isAlive = true;
		float xPosition = 360;
		float yPosition = 250;
	};
	static ChessPiece redPawns[5];
	static void initializeGame();
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void handleClick(float mouseX, float mouseY);
	static void movePawn(int index);

};
