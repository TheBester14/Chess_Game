#include "Methods.h"
const float windowWidth = 1280;
const float windowHeight = 720;
Methods::ChessPiece Methods::redPawns[5];



void Methods::drawWhiteSquareMatrix(float xPosition, float yPosition, float size) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPosition + size / 2, yPosition + size / 2, 0.0f)); // Center the square
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

    Shader matrixShader("shaders/matrix.vertex", "shaders/matrix.fragment");
    matrixShader.use();
    matrixShader.setMat4("model", model);
    matrixShader.setMat4("view", view);
    matrixShader.setMat4("projection", projection);

    float vertices[] = {
        // positions                          // colors
        -size / 2, size / 2, 0.0f,          1.0f, 1.0f, 1.0f, // top left
         size / 2, size / 2, 0.0f,          1.0f, 1.0f, 1.0f, // top right
         size / 2, -size / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom right
         size / 2, -size / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom right
        -size / 2, -size / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom left
        -size / 2, size / 2, 0.0f,          1.0f, 1.0f, 1.0f  // top left
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }
}

void Methods::drawBlackSquareMatrix(float xPosition, float yPosition, float size)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPosition + size / 2, yPosition + size / 2, 0.0f)); // Center the square
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

    Shader matrixShader("shaders/matrix.vertex", "shaders/matrix.fragment");
    matrixShader.use();
    matrixShader.setMat4("model", model);
    matrixShader.setMat4("view", view);
    matrixShader.setMat4("projection", projection);

    float vertices[] = {
        // positions         // colors (R, G, B)
        -size / 2, size / 2, 0.0f,     0.5f, 0.5f, 0.5f, // top left
         size / 2, size / 2, 0.0f,     0.5f, 0.5f, 0.5f, // top right
         size / 2, -size / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom right

         size / 2, -size / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom right (duplicate, likely meant to be part of a triangle fan or strip)
        -size / 2, -size / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom left
        -size / 2, size / 2, 0.0f,     0.5f, 0.5f, 0.5f  // top left (closing the loop)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }
}

void Methods::drawWhiteRow(float xPosition, float yPosition)
{
    drawWhiteSquareMatrix(xPosition, yPosition, 100);
    drawBlackSquareMatrix(xPosition + 100, yPosition, 100);
    drawWhiteSquareMatrix(xPosition + 200, yPosition, 100);
    drawBlackSquareMatrix(xPosition + 300, yPosition, 100);
    drawWhiteSquareMatrix(xPosition + 400, yPosition, 100);
}

void Methods::drawBlackRow(float xPosition, float yPosition)
{
    drawBlackSquareMatrix(xPosition, yPosition, 100);
    drawWhiteSquareMatrix(xPosition + 100, yPosition, 100);
    drawBlackSquareMatrix(xPosition + 200, yPosition, 100);
    drawWhiteSquareMatrix(xPosition + 300, yPosition, 100);
    drawBlackSquareMatrix(xPosition + 400, yPosition, 100);

}

void Methods::drawChessboard()
{
    drawBlackRow(360, 550);
    drawWhiteRow(360, 450);
    drawBlackRow(360, 350);
    drawWhiteRow(360, 250);
    drawBlackRow(360, 150);
}

void Methods::drawChessPiece(char filePath[128], float xPosition, float yPosition, float size)
{
    // Set up The matrices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPosition + size / 2, yPosition + size / 2, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);


    Shader textureShader("shaders/texture.vertex", "shaders/texture.fragment");
    textureShader.use();
    textureShader.setMat4("model", model);
    textureShader.setMat4("view", view);
    textureShader.setMat4("projection", projection);

    // Set up vertices to draw a square
    float vertices[] = {
        // positions         // colors   // texture coords
        -size / 2, size / 2, 0.0f,       0, 0, 0,         0.0f, 1.0f, // top left
         size / 2, size / 2, 0.0f,       0, 0, 0,         1.0f, 1.0f, // top right
         size / 2, -size / 2, 0.0f,      0, 0, 0,         1.0f, 0.0f, // bottom right

         size / 2, -size / 2, 0.0f,      0, 0, 0,         1.0f, 0.0f, // bottom right
        -size / 2, -size / 2, 0.0f,      0, 0, 0,         0.0f, 0.0f, // bottom left
        -size / 2, size / 2, 0.0f,       0, 0, 0,         0.0f, 1.0f  // top left
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Prepare to draw the square
    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Loading the texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(filePath, & width, &height, &nrChannels, 0);

    if (data)
    {
        // Use RGBA when loading png images or else the textures load in weirdly since they have an alpha channel compared to the jpeg images
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles

    // Free space
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Methods::drawRedPieces()
{
    char redPawnfilePath[120] =     "textures/redpawn.png";
    char redRookfilePath[120] =     "textures/redrook.png";
    char redHorsefilePath[120] =    "textures/redhorse.png";
    char redBishopfilePath[120] =   "textures/redbishop.png";
    char redQueenfilePath[120] =    "textures/redqueen.png";
    char redKingfilePath[120] =     "textures/redking.png";

    float initialXPosition = 360;

    // Drawing Pawns
    for (int i = 0; i < 5; i++) 
    {
        if (redPawns[i].isAlive) 
        {
            drawChessPiece(redPawnfilePath, redPawns[i].xPosition + (i * 103), redPawns[i].yPosition, 100);
        }
    }

    // Drawing other pieces
    drawChessPiece(redRookfilePath,     initialXPosition, 150, 100);
    drawChessPiece(redHorsefilePath,    initialXPosition + 103, 150, 100);
    drawChessPiece(redBishopfilePath,   initialXPosition + 203, 150, 100);
    drawChessPiece(redQueenfilePath,    initialXPosition + 303, 150, 100);
    drawChessPiece(redKingfilePath,      initialXPosition + 403, 150, 100);
}

void Methods::drawBluePieces()
{
    char bluePawnfilePath[120] = "textures/bluepawn.png";
    char blueRookfilePath[120] = "textures/bluerook.png";
    char blueHorsefilePath[120] = "textures/bluehorse.png";
    char blueBishopfilePath[120] = "textures/bluebishop.png";
    char blueQueenfilePath[120] = "textures/blue queen.png";
    char blueKingfilePath[120] = "textures/blue king.png";

    float initialXPosition = 360;
    // Drawing Pawns
    drawChessPiece(bluePawnfilePath, initialXPosition, 450, 100);
    drawChessPiece(bluePawnfilePath, initialXPosition + 103, 450, 100);
    drawChessPiece(bluePawnfilePath, initialXPosition + 203, 450, 100);
    drawChessPiece(bluePawnfilePath, initialXPosition + 303, 450, 100);
    drawChessPiece(bluePawnfilePath, initialXPosition + 403, 450, 100);

    // Drawing other pieces
    drawChessPiece(blueRookfilePath, initialXPosition, 550, 100);
    drawChessPiece(blueHorsefilePath, initialXPosition + 103, 550, 100);
    drawChessPiece(blueBishopfilePath, initialXPosition + 203, 550, 100);
    drawChessPiece(blueQueenfilePath, initialXPosition + 303, 550, 100);
    drawChessPiece(blueKingfilePath, initialXPosition + 403, 550, 100);
}

// This is the method to link my drawings to what is going to happen in the game logic. // We need to initalize all the pieces
void Methods::initializeGame()
{
    // Initialize the redPawns
    float initialXPosition = 360;
    float yPosition = 250;
    for (int i = 0; i < 5; i++)
    {
     //   redPawns[i].xPosition += (i * 103);
      //  redPawns[i].yPosition = yPosition;
    }
}

// CallBack function to move 
void Methods::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        Methods::handleClick(static_cast<float> (xPos), static_cast<float> (yPos));
    }
}

void Methods::handleClick(float mouseX, float mouseY)
{
    // Convert screen coordinates to openGl coordinates
    float normalizedX = mouseX;
    float normalizedY = 720.0f - mouseY;

    for (int i = 0; i < 5; ++i) 
    {
        if (redPawns[i].isAlive &&
            normalizedX >= redPawns[i].xPosition && normalizedX <= redPawns[i].xPosition + 100 &&
            normalizedY >= redPawns[i].yPosition && normalizedY <= redPawns[i].yPosition + 100)
        {
            movePawn(i);
            break;
        }
    }
}

void Methods::movePawn(int index)
{
    redPawns[index].yPosition += 103; // Move the pawn by one square forward
}
