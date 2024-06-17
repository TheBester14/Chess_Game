#include "Methods.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "vendor/stb_image/stb_images.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const float windowWidth = 1280;
const float windowHeight = 720;
Methods::ChessPiece Methods::redPawns[5];
Methods::ChessPiece Methods::bluePawns[5];
Methods::ChessPiece Methods::redHorse[1];
Methods::ChessPiece Methods::blueHorse[1];
Methods::ChessPiece Methods::redBishop[1];
Methods::ChessPiece Methods::blueBishop[1];
Methods::ChessPiece Methods::redRook[1];
Methods::ChessPiece Methods::blueRook[1];
Methods::ChessPiece Methods::redQueen[1];
Methods::ChessPiece Methods::blueQueen[1];
Methods::ChessPiece Methods::redKing[1];
Methods::ChessPiece Methods::blueKing[1];

bool Methods::gameOver = false;
Methods::Player Methods::winner = Methods::RED;
bool Methods::pawnSelected = false;
int Methods::selectedPawnIndex = -1;
Methods::PieceType Methods::selectedType = Methods::PAWN;
Methods::Player Methods::currentPlayer = Methods::RED;
Methods::Player Methods::selectedPlayer = Methods::RED;

unsigned int Methods::blackVAO = 0;
unsigned int Methods::blackVBO = 0;
unsigned int Methods::whiteVAO = 0;
unsigned int Methods::whiteVBO = 0;
unsigned int Methods::textureVAO = 0;
unsigned int Methods::textureVBO = 0;
unsigned int Methods::texture = 0;
unsigned int Methods::redPawnTexture = 0;
unsigned int Methods::redRookTexture = 0;
unsigned int Methods::redHorseTexture = 0;
unsigned int Methods::redBishopTexture = 0;
unsigned int Methods::redQueenTexture = 0;
unsigned int Methods::redKingTexture = 0;

unsigned int Methods::bluePawnTexture = 0;
unsigned int Methods::blueRookTexture = 0;
unsigned int Methods::blueHorseTexture = 0;
unsigned int Methods::blueBishopTexture = 0;
unsigned int Methods::blueQueenTexture = 0;
unsigned int Methods::blueKingTexture = 0;

void Methods::initializeBlackSquareResources()
{
    float vertices[] = {
        // positions         // colors (R, G, B)
        -100 / 2, 100 / 2, 0.0f,     0.5f, 0.5f, 0.5f, // top left
         100 / 2, 100 / 2, 0.0f,     0.5f, 0.5f, 0.5f, // top right
         100 / 2, -100 / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom right

         100 / 2, -100 / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom right (duplicate, likely meant to be part of a triangle fan or strip)
        -100 / 2, -100 / 2, 0.0f,    0.5f, 0.5f, 0.5f, // bottom left
        -100 / 2, 100 / 2, 0.0f,     0.5f, 0.5f, 0.5f  // top left (closing the loop)
    };

    glGenBuffers(1, &blackVBO);
    glGenVertexArrays(1, &blackVAO);

    glBindVertexArray(blackVAO);

    glBindBuffer(GL_ARRAY_BUFFER, blackVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Methods::initializeWhiteSquareResources()
{
    float vertices[] = {
        //  positions                           colors
         -100 / 2, 100 / 2, 0.0f,          1.0f, 1.0f, 1.0f, // top left
          100 / 2, 100 / 2, 0.0f,          1.0f, 1.0f, 1.0f, // top right
          100 / 2, -100 / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom right

          100 / 2, -100 / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom right
         -100 / 2, -100 / 2, 0.0f,         1.0f, 1.0f, 1.0f, // bottom left
         -100 / 2, 100 / 2, 0.0f,          1.0f, 1.0f, 1.0f  // top left
    };

    glGenBuffers(1, &whiteVBO);
    glGenVertexArrays(1, &whiteVAO);

    glBindVertexArray(whiteVAO);

    glBindBuffer(GL_ARRAY_BUFFER, whiteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Methods::drawWhiteSquareMatrix(float xPosition, float yPosition, float size) {
    // Create matrix to use screen space
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPosition + size / 2, yPosition + size / 2, 0.0f)); // Center the square
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

    Shader matrixShader("shaders/matrix.vertex", "shaders/matrix.fragment");
    matrixShader.use();
    matrixShader.setMat4("model", model);
    matrixShader.setMat4("view", view);
    matrixShader.setMat4("projection", projection);

    glBindVertexArray(whiteVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error Drawing: " << err << std::endl;
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

    glBindVertexArray(blackVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error Drawing: " << err << std::endl;
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

unsigned int Methods::loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGBA;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Methods::initializeTextureResources()
{
    redPawnTexture = loadTexture("textures/redpawn.png");
    redRookTexture = loadTexture("textures/redrook.png");
    redHorseTexture = loadTexture("textures/redhorse.png");
    redBishopTexture = loadTexture("textures/redbishop.png");
    redQueenTexture = loadTexture("textures/redqueen.png");
    redKingTexture = loadTexture("textures/redking.png");

    bluePawnTexture = loadTexture("textures/bluepawn.png");
    blueRookTexture = loadTexture("textures/bluerook.png");
    blueHorseTexture = loadTexture("textures/bluehorse.png");
    blueBishopTexture = loadTexture("textures/bluebishop.png");
    blueQueenTexture = loadTexture("textures/blue queen.png");
    blueKingTexture = loadTexture("textures/blue king.png");
}

void Methods::drawChessPiece(unsigned int textureID, float xPosition, float yPosition, float size)
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

    glBindTexture(GL_TEXTURE_2D, textureID);

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices, which make up two triangles
    glBindVertexArray(0); // Free space
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error Loading Texture: " << err << std::endl;
    }
}

void Methods::drawRedPieces() {
    for (int i = 0; i < 5; i++) {
        if (redPawns[i].isAlive) {
            drawChessPiece(redPawnTexture, redPawns[i].xPosition, redPawns[i].yPosition, 100);
        }
    }
    if (redHorse[0].isAlive) {
        drawChessPiece(redHorseTexture, redHorse[0].xPosition, redHorse[0].yPosition, 100);
    }
    if (redBishop[0].isAlive) {
        drawChessPiece(redBishopTexture, redBishop[0].xPosition, redBishop[0].yPosition, 100);
    }
    if (redRook[0].isAlive) {
        drawChessPiece(redRookTexture, redRook[0].xPosition, redRook[0].yPosition, 100);
    }
    if (redQueen[0].isAlive) {
        drawChessPiece(redQueenTexture, redQueen[0].xPosition, redQueen[0].yPosition, 100);
    }
    if (redKing[0].isAlive) {
        drawChessPiece(redKingTexture, redKing[0].xPosition, redKing[0].yPosition, 100);
    }
}

void Methods::drawBluePieces() {
    for (int i = 0; i < 5; i++) {
        if (bluePawns[i].isAlive) {
            drawChessPiece(bluePawnTexture, bluePawns[i].xPosition, bluePawns[i].yPosition, 100);
        }
    }
    if (blueHorse[0].isAlive) {
        drawChessPiece(blueHorseTexture, blueHorse[0].xPosition, blueHorse[0].yPosition, 100);
    }
    if (blueBishop[0].isAlive) {
        drawChessPiece(blueBishopTexture, blueBishop[0].xPosition, blueBishop[0].yPosition, 100);
    }
    if (blueRook[0].isAlive) {
        drawChessPiece(blueRookTexture, blueRook[0].xPosition, blueRook[0].yPosition, 100);
    }
    if (blueQueen[0].isAlive) {
        drawChessPiece(blueQueenTexture, blueQueen[0].xPosition, blueQueen[0].yPosition, 100);
    }
    if (blueKing[0].isAlive) {
        drawChessPiece(blueKingTexture, blueKing[0].xPosition, blueKing[0].yPosition, 100);
    }
}


// Initialize the game
void Methods::initializeGame() {
    gameOver = false;
    winner = RED;
    float initialXPosition = 360.0f;
    float redYPosition = 250.0f;
    float blueYPosition = 450.0f;

    for (int i = 0; i < 5; ++i) {
        redPawns[i].xPosition = initialXPosition + (i * 100);
        redPawns[i].yPosition = redYPosition;
        redPawns[i].isAlive = true;

        bluePawns[i].xPosition = initialXPosition + (i * 100);
        bluePawns[i].yPosition = blueYPosition;
        bluePawns[i].isAlive = true;
    }

    redHorse[0].isAlive = true;
    redHorse[0].xPosition = initialXPosition + 100;
    redHorse[0].yPosition = 150;

    blueHorse[0].isAlive = true;
    blueHorse[0].xPosition = initialXPosition + 100;
    blueHorse[0].yPosition = 550;

    redBishop[0].isAlive = true;
    redBishop[0].xPosition = initialXPosition + 200;
    redBishop[0].yPosition = 150;

    blueBishop[0].isAlive = true;
    blueBishop[0].xPosition = initialXPosition + 200;
    blueBishop[0].yPosition = 550;

    redRook[0].isAlive = true;
    redRook[0].xPosition = initialXPosition;
    redRook[0].yPosition = 150;

    blueRook[0].isAlive = true;
    blueRook[0].xPosition = initialXPosition;
    blueRook[0].yPosition = 550;

    redQueen[0].isAlive = true;
    redQueen[0].xPosition = initialXPosition + 300;
    redQueen[0].yPosition = 150;

    blueQueen[0].isAlive = true;
    blueQueen[0].xPosition = initialXPosition + 300;
    blueQueen[0].yPosition = 550;

    redKing[0].isAlive = true;
    redKing[0].xPosition = initialXPosition + 400;
    redKing[0].yPosition = 150;

    blueKing[0].isAlive = true;
    blueKing[0].xPosition = initialXPosition + 400;
    blueKing[0].yPosition = 550;
}


// Handle mouse button click
void Methods::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        Methods::handleClick(static_cast<float>(xPos), static_cast<float>(yPos));
    }
}

// Handle mouse click logic
void Methods::handleClick(float mouseX, float mouseY) {
    if (gameOver) {
        std::cout << "Game over! " << (winner == RED ? "Red" : "Blue") << " wins!" << std::endl;
        return;
    }

    bool moveSuccessful = false;
    mouseY = windowHeight - mouseY; // Convert to OpenGL coordinates

    std::cout << "Mouse clicked at: (" << mouseX << ", " << mouseY << ")" << std::endl;

    if (!pawnSelected) {
        selectPiece(mouseX, mouseY);
    }
    else {
        std::cout << "Attempting to move piece..." << std::endl;
        moveSuccessful = movePiece(mouseX, mouseY);

        if (moveSuccessful) {
            if (isKingInCheck(currentPlayer)) {
                if (isKingInCheckMate(currentPlayer)) {
                    std::cout << "Checkmate!" << std::endl;
                    gameOver = true;
                    winner = (currentPlayer == RED) ? BLUE : RED;
                }
                else {
                    std::cout << "Check!" << std::endl;
                }
            }

            currentPlayer = (currentPlayer == RED) ? BLUE : RED;
            std::cout << (currentPlayer == RED ? "Red Turn" : "Blue Turn") << std::endl;
        }
        else {
            std::cout << "Move failed." << std::endl;
        }

        pawnSelected = false;
    }
}

void Methods::selectPiece(float mouseX, float mouseY) {
    ChessPiece* pieces = nullptr;
    int pieceCount;

    // Check all pieces for the current player
    for (int type = PAWN; type <= KING; ++type) {
        switch (type) {
        case PAWN:
            pieces = (currentPlayer == RED) ? redPawns : bluePawns;
            pieceCount = 5;
            break;
        case HORSE:
            pieces = (currentPlayer == RED) ? redHorse : blueHorse;
            pieceCount = 1;
            break;
        case BISHOP:
            pieces = (currentPlayer == RED) ? redBishop : blueBishop;
            pieceCount = 1;
            break;
        case ROOK:
            pieces = (currentPlayer == RED) ? redRook : blueRook;
            pieceCount = 1;
            break;
        case QUEEN:
            pieces = (currentPlayer == RED) ? redQueen : blueQueen;
            pieceCount = 1;
            break;
        case KING:
            pieces = (currentPlayer == RED) ? redKing : blueKing;
            pieceCount = 1;
            break;
        }

        for (int i = 0; i < pieceCount; ++i) {
            if (pieces[i].isAlive && isWithinBounds(pieces[i], mouseX, mouseY)) {
                selectedPawnIndex = i;
                selectedType = static_cast<PieceType>(type);
                pawnSelected = true;
                selectedPlayer = currentPlayer;
                std::cout << (currentPlayer == RED ? "Red " : "Blue ") << pieceTypeToString(selectedType) << " Selected" << std::endl;
                return;
            }
        }
    }
}

std::string Methods::pieceTypeToString(PieceType type) {
    switch (type) {
    case PAWN: return "Pawn";
    case HORSE: return "Horse";
    case BISHOP: return "Bishop";
    case ROOK: return "Rook";
    case QUEEN: return "Queen";
    case KING: return "King";
    default: return "Unknown";
    }
}


bool Methods::movePiece(float mouseX, float mouseY) {
    switch (selectedType) {
    case PAWN: return movePawn(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    case HORSE: return moveHorse(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    case BISHOP: return moveBishop(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    case ROOK: return moveRook(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    case QUEEN: return moveQueen(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    case KING: return moveKing(selectedPawnIndex, selectedPlayer, mouseX, mouseY);
    default: return false;
    }
}



// Check if a position is occupied
bool Methods::isPositionOccupied(float x, float y) {
    for (int i = 0; i < 5; ++i) {
        if (redPawns[i].isAlive && redPawns[i].xPosition == x && redPawns[i].yPosition == y) {
            return true;
        }
    }
    if (redHorse[0].isAlive && redHorse[0].xPosition == x && redHorse[0].yPosition == y) {
        return true;
    }
    if (redBishop[0].isAlive && redBishop[0].xPosition == x && redBishop[0].yPosition == y) {
        return true;
    }
    if (redRook[0].isAlive && redRook[0].xPosition == x && redRook[0].yPosition == y) {
        return true;
    }
    if (redQueen[0].isAlive && redQueen[0].xPosition == x && redQueen[0].yPosition == y) {
        return true;
    }
    if (redKing[0].isAlive && redKing[0].xPosition == x && redKing[0].yPosition == y) {
        return true;
    }

    for (int i = 0; i < 5; ++i) {
        if (bluePawns[i].isAlive && bluePawns[i].xPosition == x && bluePawns[i].yPosition == y) {
            return true;
        }
    }
    if (blueHorse[0].isAlive && blueHorse[0].xPosition == x && blueHorse[0].yPosition == y) {
        return true;
    }
    if (blueBishop[0].isAlive && blueBishop[0].xPosition == x && blueBishop[0].yPosition == y) {
        return true;
    }
    if (blueRook[0].isAlive && blueRook[0].xPosition == x && blueRook[0].yPosition == y) {
        return true;
    }
    if (blueQueen[0].isAlive && blueQueen[0].xPosition == x && blueQueen[0].yPosition == y) {
        return true;
    }
    if (blueKing[0].isAlive && blueKing[0].xPosition == x && blueKing[0].yPosition == y) {
        return true;
    }
    return false;
}

// Check if position is occupied by opponent
bool Methods::isPositionOccupiedByOpponent(float x, float y, Player opponent) {
    if (opponent == RED) {
        for (int i = 0; i < 5; ++i) {
            if (redPawns[i].isAlive && redPawns[i].xPosition == x && redPawns[i].yPosition == y) {
                return true;
            }
        }
        if (redHorse[0].isAlive && redHorse[0].xPosition == x && redHorse[0].yPosition == y) {
            return true;
        }
        if (redBishop[0].isAlive && redBishop[0].xPosition == x && redBishop[0].yPosition == y) {
            return true;
        }
        if (redRook[0].isAlive && redRook[0].xPosition == x && redRook[0].yPosition == y) {
            return true;
        }
        if (redQueen[0].isAlive && redQueen[0].xPosition == x && redQueen[0].yPosition == y) {
            return true;
        }
        if (redKing[0].isAlive && redKing[0].xPosition == x && redKing[0].yPosition == y) {
            return true;
        }
    }
    else {
        for (int i = 0; i < 5; ++i) {
            if (bluePawns[i].isAlive && bluePawns[i].xPosition == x && bluePawns[i].yPosition == y) {
                return true;
            }
        }
        if (blueHorse[0].isAlive && blueHorse[0].xPosition == x && blueHorse[0].yPosition == y) {
            return true;
        }
        if (blueBishop[0].isAlive && blueBishop[0].xPosition == x && blueBishop[0].yPosition == y) {
            return true;
        }
        if (blueRook[0].isAlive && blueRook[0].xPosition == x && blueRook[0].yPosition == y) {
            return true;
        }
        if (blueQueen[0].isAlive && blueQueen[0].xPosition == x && blueQueen[0].yPosition == y) {
            return true;
        }
        if (blueKing[0].isAlive && blueKing[0].xPosition == x && blueKing[0].yPosition == y) {
            return true;
        }
    }
    return false;
}

// Check if within bounds
bool Methods::isWithinBounds(ChessPiece& piece, float mouseX, float mouseY) {
    float pieceSize = 100.0f; // Size of each piece
    float padding = 10.0f; // Add padding for more precise click detection

    float xStart = piece.xPosition;
    float xEnd = piece.xPosition + pieceSize;
    float yStart = piece.yPosition;
    float yEnd = piece.yPosition + pieceSize;

    return (mouseX >= xStart && mouseX <= xEnd && mouseY >= yStart && mouseY <= yEnd);
}


// Move pawn logic
bool Methods::movePawn(int index, Player player, float mouseX, float mouseY) {
    float originalX = 0.0, originalY = 0.0;
    float newX = 0.0, newY = 0.0;

    if (player == RED) {
        originalX = redPawns[index].xPosition;
        originalY = redPawns[index].yPosition;
        newX = originalX;
        newY = originalY + 100;
    }
    else if (player == BLUE) {
        originalX = bluePawns[index].xPosition;
        originalY = bluePawns[index].yPosition;
        newX = originalX;
        newY = originalY - 100;
    }

    if (std::abs(mouseX - newX) < 60 && std::abs(mouseY - newY) < 60) {
        if (!isPositionOccupied(newX, newY)) {
            if (player == RED) {
                if (redPawns[index].yPosition + 100 <= 600) {
                    redPawns[index].yPosition += 100;
                    std::cout << "Red Pawn moves to (" << newX << ", " << newY << ")" << std::endl;
                    return true;
                }
                else {
                    std::cout << "The move is out of bounds (Red Pawn)" << std::endl;
                    return false;
                }
            }
            else if (player == BLUE) {
                if (bluePawns[index].yPosition - 100 >= 150) {
                    bluePawns[index].yPosition -= 100;
                    std::cout << "Blue Pawn moves to (" << newX << ", " << newY << ")" << std::endl;
                    return true;
                }
                else {
                    std::cout << "The move is out of bounds (Blue Pawn)" << std::endl;
                    return false;
                }
            }
        }
        else {
            std::cout << "The move is blocked by another piece" << std::endl;
            return false;
        }
    }

    if (player == RED) {
        if (std::abs(mouseX - (newX - 100)) < 60 && std::abs(mouseY - newY) < 60) {
            if (canCapturePawn(newX - 100, newY, player)) {
                redPawns[index].xPosition = newX - 100;
                redPawns[index].yPosition = newY;
                std::cout << "Red Pawn captures a Blue Pawn" << std::endl;
                return true;
            }
        }
        if (std::abs(mouseX - (newX + 100)) < 60 && std::abs(mouseY - newY) < 60) {
            if (canCapturePawn(newX + 100, newY, player)) {
                redPawns[index].xPosition = newX + 100;
                redPawns[index].yPosition = newY;
                std::cout << "Red Pawn captures a Blue Pawn" << std::endl;
                return true;
            }
        }
    }
    else if (player == BLUE) {
        if (std::abs(mouseX - (newX - 100)) < 60 && std::abs(mouseY - newY) < 60) {
            if (canCapturePawn(newX - 100, newY, player)) {
                bluePawns[index].xPosition = newX - 100;
                bluePawns[index].yPosition = newY;
                std::cout << "Blue Pawn captures a Red Pawn" << std::endl;
                return true;
            }
        }
        if (std::abs(mouseX - (newX + 100)) < 60 && std::abs(mouseY - newY) < 60) {
            if (canCapturePawn(newX + 100, newY, player)) {
                bluePawns[index].xPosition = newX + 100;
                bluePawns[index].yPosition = newY;
                std::cout << "Blue Pawn captures a Red Pawn" << std::endl;
                return true;
            }
        }
    }

    return false;
}

bool Methods::moveHorse(int index, Player player, float mouseX, float mouseY) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redHorse[index].xPosition;
        originalY = redHorse[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueHorse[index].xPosition;
        originalY = blueHorse[index].yPosition;
    }

    float moveX[] = { originalX + 100, originalX + 100, originalX - 100, originalX - 100, originalX + 200, originalX + 200, originalX - 200, originalX - 200 };
    float moveY[] = { originalY + 200, originalY - 200, originalY + 200, originalY - 200, originalY + 100, originalY - 100, originalY + 100, originalY - 100 };

    for (int i = 0; i < 8; ++i) {
        if (std::abs(mouseX - moveX[i]) < 60 && std::abs(mouseY - moveY[i]) < 60) {
            if (!isPositionOccupied(moveX[i], moveY[i])) {
                if (player == RED) {
                    redHorse[index].xPosition = moveX[i];
                    redHorse[index].yPosition = moveY[i];
                    std::cout << "Red Horse moves to (" << moveX[i] << ", " << moveY[i] << ")" << std::endl;
                }
                else if (player == BLUE) {
                    blueHorse[index].xPosition = moveX[i];
                    blueHorse[index].yPosition = moveY[i];
                    std::cout << "Blue Horse moves to (" << moveX[i] << ", " << moveY[i] << ")" << std::endl;
                }
                return true;
            }
            else if (isPositionOccupiedByOpponent(moveX[i], moveY[i], player == RED ? BLUE : RED)) {
                capturePiece(moveX[i], moveY[i], player == RED ? BLUE : RED); // Capture the opponent piece
                if (player == RED) {
                    redHorse[index].xPosition = moveX[i];
                    redHorse[index].yPosition = moveY[i];
                    std::cout << "Red Horse captures opponent piece" << std::endl;
                }
                else if (player == BLUE) {
                    blueHorse[index].xPosition = moveX[i];
                    blueHorse[index].yPosition = moveY[i];
                    std::cout << "Blue Horse captures opponent piece" << std::endl;
                }
                return true;
            }
        }
    }

    return false;
}


void Methods::capturePiece(float x, float y, Player opponent) {
    ChessPiece* opponentPawns = (opponent == RED) ? redPawns : bluePawns;
    for (int i = 0; i < 5; ++i) {
        if (opponentPawns[i].isAlive && opponentPawns[i].xPosition == x && opponentPawns[i].yPosition == y) {
            opponentPawns[i].isAlive = false;
            std::cout << "Pawn captured at (" << x << ", " << y << ")" << std::endl;
            return;
        }
    }
    ChessPiece* opponentHorse = (opponent == RED) ? redHorse : blueHorse;
    if (opponentHorse[0].isAlive && opponentHorse[0].xPosition == x && opponentHorse[0].yPosition == y) {
        opponentHorse[0].isAlive = false;
        std::cout << "Horse captured at (" << x << ", " << y << ")" << std::endl;
        return;
    }
    ChessPiece* opponentBishop = (opponent == RED) ? redBishop : blueBishop;
    if (opponentBishop[0].isAlive && opponentBishop[0].xPosition == x && opponentBishop[0].yPosition == y) {
        opponentBishop[0].isAlive = false;
        std::cout << "Bishop captured at (" << x << ", " << y << ")" << std::endl;
        return;
    }
    ChessPiece* opponentRook = (opponent == RED) ? redRook : blueRook;
    if (opponentRook[0].isAlive && opponentRook[0].xPosition == x && opponentRook[0].yPosition == y) {
        opponentRook[0].isAlive = false;
        std::cout << "Rook captured at (" << x << ", " << y << ")" << std::endl;
        return;
    }
    ChessPiece* opponentQueen = (opponent == RED) ? redQueen : blueQueen;
    if (opponentQueen[0].isAlive && opponentQueen[0].xPosition == x && opponentQueen[0].yPosition == y) {
        opponentQueen[0].isAlive = false;
        std::cout << "Queen captured at (" << x << ", " << y << ")" << std::endl;
        return;
    }
    ChessPiece* opponentKing = (opponent == RED) ? redKing : blueKing;
    if (opponentKing[0].isAlive && opponentKing[0].xPosition == x && opponentKing[0].yPosition == y) {
        opponentKing[0].isAlive = false;
        std::cout << "King captured at (" << x << ", " << y << ")" << std::endl;
        return;
    }
}

bool Methods::moveBishop(int index, Player player, float mouseX, float mouseY) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redBishop[index].xPosition;
        originalY = redBishop[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueBishop[index].xPosition;
        originalY = blueBishop[index].yPosition;
    }

    if (std::abs(mouseX - originalX) == std::abs(mouseY - originalY)) {
        float xDir = (mouseX > originalX) ? 100 : -100;
        float yDir = (mouseY > originalY) ? 100 : -100;

        for (float x = originalX + xDir, y = originalY + yDir; std::abs(x - mouseX) >= 60; x += xDir, y += yDir) {
            if (isPositionOccupied(x, y)) return false;
        }

        if (player == RED) {
            redBishop[index].xPosition = mouseX;
            redBishop[index].yPosition = mouseY;
        }
        else if (player == BLUE) {
            blueBishop[index].xPosition = mouseX;
            blueBishop[index].yPosition = mouseY;
        }
        return true;
    }
    return false;
}

bool Methods::moveRook(int index, Player player, float mouseX, float mouseY) {
    // Retrieve the original position of the rook based on the player and index
    float originalX = (player == RED) ? redRook[index].xPosition : blueRook[index].xPosition;
    float originalY = (player == RED) ? redRook[index].yPosition : blueRook[index].yPosition;

    // Log the attempted movement
    std::cout << "Attempting to move Rook from (" << originalX << ", " << originalY << ") to (" << mouseX << ", " << mouseY << ")" << std::endl;

    // Snapping mouse coordinates to the nearest grid intersection
    float targetX = round(mouseX / 100) * 100;
    float targetY = round(mouseY / 100) * 100;
    std::cout << "Snapped to grid: (" << targetX << ", " << targetY << ")" << std::endl;

    // Check if the move is within the bounds of the chessboard
    if (targetX < 360 || targetX >= 860 || targetY < 150 || targetY >= 650) {
        std::cout << "Move out of bounds: Target position (" << targetX << ", " << targetY << ") is outside the chessboard limits." << std::endl;
        return false;
    }

    // Check if the move is valid (either vertical or horizontal but not both)
    if (originalX != targetX && originalY != targetY) {
        std::cout << "Invalid rook move: Rook moves must be strictly horizontal or vertical." << std::endl;
        return false;
    }

    // Check for any pieces blocking the path of the rook
    if (originalX == targetX) {  // Vertical movement
        float stepY = (targetY > originalY) ? 100 : -100;
        for (float y = originalY + stepY; y != targetY; y += stepY) {
            if (isPositionOccupied(originalX, y)) {
                std::cout << "Movement blocked by a piece at (" << originalX << ", " << y << ")." << std::endl;
                return false;
            }
        }
    }
    else if (originalY == targetY) {  // Horizontal movement
        float stepX = (targetX > originalX) ? 100 : -100;
        for (float x = originalX + stepX; x != targetX; x += stepX) {
            if (isPositionOccupied(x, originalY)) {
                std::cout << "Movement blocked by a piece at (" << x << ", " << originalY << ")." << std::endl;
                return false;
            }
        }
    }

    // If all checks pass, move the rook
    if (player == RED) {
        redRook[index].xPosition = targetX;
        redRook[index].yPosition = targetY;
    }
    else {
        blueRook[index].xPosition = targetX;
        blueRook[index].yPosition = targetY;
    }

    std::cout << "Rook moved successfully to (" << targetX << ", " << targetY << ")." << std::endl;
    return true;
}




bool Methods::moveQueen(int index, Player player, float mouseX, float mouseY) {
    std::cout << "Attempting to move Queen" << std::endl;

    // Try to move as Rook first
    if (moveRook(index, player, mouseX, mouseY)) {
        std::cout << "Queen moved as Rook successfully" << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to move Queen as Rook" << std::endl;
    }

    // Try to move as Bishop second
    if (moveBishop(index, player, mouseX, mouseY)) {
        std::cout << "Queen moved as Bishop successfully" << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to move Queen as Bishop" << std::endl;
    }

    return false;
}





bool Methods::moveKing(int index, Player player, float mouseX, float mouseY) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redKing[index].xPosition;
        originalY = redKing[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueKing[index].xPosition;
        originalY = blueKing[index].yPosition;
    }

    float moveX[] = { originalX + 100, originalX - 100, originalX, originalX, originalX + 100, originalX + 100, originalX - 100, originalX - 100 };
    float moveY[] = { originalY, originalY, originalY + 100, originalY - 100, originalY + 100, originalY - 100, originalY - 100, originalY + 100 };

    for (int i = 0; i < 8; ++i) {
        if (std::abs(mouseX - moveX[i]) < 60 && std::abs(mouseY - moveY[i]) < 60) {
            if (!isPositionOccupied(moveX[i], moveY[i]) && !isKingInCheckAfterMove(index, KING, player, moveX[i], moveY[i])) {
                if (player == RED) {
                    redKing[index].xPosition = moveX[i];
                    redKing[index].yPosition = moveY[i];
                    std::cout << "Red King moves to (" << moveX[i] << ", " << moveY[i] << ")" << std::endl;
                }
                else if (player == BLUE) {
                    blueKing[index].xPosition = moveX[i];
                    blueKing[index].yPosition = moveY[i];
                    std::cout << "Blue King moves to (" << moveX[i] << ", " << moveY[i] << ")" << std::endl;
                }
                return true;
            }
            else if (isPositionOccupiedByOpponent(moveX[i], moveY[i], player == RED ? BLUE : RED) && !isKingInCheckAfterMove(index, KING, player, moveX[i], moveY[i])) {
                capturePiece(moveX[i], moveY[i], player == RED ? BLUE : RED); // Capture the opponent piece
                if (player == RED) {
                    redKing[index].xPosition = moveX[i];
                    redKing[index].yPosition = moveY[i];
                    std::cout << "Red King captures opponent piece" << std::endl;
                }
                else if (player == BLUE) {
                    blueKing[index].xPosition = moveX[i];
                    blueKing[index].yPosition = moveY[i];
                    std::cout << "Blue King captures opponent piece" << std::endl;
                }
                return true;
            }
        }
    }

    return false;
}


// Check if King is in check
bool Methods::isKingInCheck(Player player) {
    ChessPiece* king = (player == RED) ? redKing : blueKing;
    ChessPiece* opponentPawns = (player == RED) ? bluePawns : redPawns;
    ChessPiece* opponentHorse = (player == RED) ? blueHorse : redHorse;
    ChessPiece* opponentBishop = (player == RED) ? blueBishop : redBishop;
    ChessPiece* opponentRook = (player == RED) ? blueRook : redRook;
    ChessPiece* opponentQueen = (player == RED) ? blueQueen : redQueen;

    // Check opponent pawns
    for (int i = 0; i < 5; ++i) {
        if (opponentPawns[i].isAlive) {
            if (std::abs(opponentPawns[i].xPosition - king[0].xPosition) == 100 &&
                std::abs(opponentPawns[i].yPosition - king[0].yPosition) == 100) {
                return true;
            }
        }
    }

    // Check opponent horse
    if (opponentHorse[0].isAlive) {
        float moveX[] = { king[0].xPosition + 100, king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition - 100, king[0].xPosition + 200, king[0].xPosition + 200, king[0].xPosition - 200, king[0].xPosition - 200 };
        float moveY[] = { king[0].yPosition + 200, king[0].yPosition - 200, king[0].yPosition + 200, king[0].yPosition - 200, king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition + 100, king[0].yPosition - 100 };
        for (int i = 0; i < 8; ++i) {
            if (opponentHorse[0].xPosition == moveX[i] && opponentHorse[0].yPosition == moveY[i]) {
                return true;
            }
        }
    }

    // Check opponent bishop
    if (opponentBishop[0].isAlive) {
        float moveX[] = { king[0].xPosition + 100, king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition - 100 };
        float moveY[] = { king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition + 100, king[0].yPosition - 100 };
        for (int i = 0; i < 4; ++i) {
            if (opponentBishop[0].xPosition == moveX[i] && opponentBishop[0].yPosition == moveY[i]) {
                return true;
            }
        }
    }

    // Check opponent rook
    if (opponentRook[0].isAlive) {
        float moveX[] = { king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition, king[0].xPosition };
        float moveY[] = { king[0].yPosition, king[0].yPosition, king[0].yPosition + 100, king[0].yPosition - 100 };
        for (int i = 0; i < 4; ++i) {
            if (opponentRook[0].xPosition == moveX[i] && opponentRook[0].yPosition == moveY[i]) {
                return true;
            }
        }
    }

    // Check opponent queen
    if (opponentQueen[0].isAlive) {
        float moveX[] = { king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition, king[0].xPosition, king[0].xPosition + 100, king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition - 100 };
        float moveY[] = { king[0].yPosition, king[0].yPosition, king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition - 100, king[0].yPosition + 100 };
        for (int i = 0; i < 8; ++i) {
            if (opponentQueen[0].xPosition == moveX[i] && opponentQueen[0].yPosition == moveY[i]) {
                return true;
            }
        }
    }

    return false;
}

// Check if King is in checkmate
bool Methods::isKingInCheckMate(Player player) {
    if (!isKingInCheck(player)) {
        return false;
    }

    if (canKingMove(player)) {
        return false;
    }

    if (canBlockOrCapture(player)) {
        return false;
    }

    return true;
}

// Check if King can move
bool Methods::canKingMove(Player player) {
    ChessPiece* king = (player == RED) ? redKing : blueKing;

    float moveX[] = { king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition, king[0].xPosition, king[0].xPosition + 100, king[0].xPosition + 100, king[0].xPosition - 100, king[0].xPosition - 100 };
    float moveY[] = { king[0].yPosition, king[0].yPosition, king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition + 100, king[0].yPosition - 100, king[0].yPosition - 100, king[0].yPosition + 100 };

    for (int i = 0; i < 8; ++i) {
        if (!isPositionOccupied(moveX[i], moveY[i]) && !isKingInCheckAfterMove(0, KING, player, moveX[i], moveY[i])) {
            return true;
        }
    }

    return false;
}

// Check if a piece can block or capture
bool Methods::canBlockOrCapture(Player player) {
    ChessPiece* opponentPawns = (player == RED) ? bluePawns : redPawns;
    ChessPiece* opponentHorse = (player == RED) ? blueHorse : redHorse;
    ChessPiece* opponentBishop = (player == RED) ? blueBishop : redBishop;
    ChessPiece* opponentRook = (player == RED) ? blueRook : redRook;
    ChessPiece* opponentQueen = (player == RED) ? blueQueen : redQueen;

    // Check if any piece can capture opponent piece or block check
    for (int i = 0; i < 5; ++i) {
        if (opponentPawns[i].isAlive) {
            if (isValidMove(opponentPawns[i], opponentPawns[i].xPosition, opponentPawns[i].yPosition, player)) {
                return true;
            }
        }
    }

    if (opponentHorse[0].isAlive) {
        if (isValidMove(opponentHorse[0], opponentHorse[0].xPosition, opponentHorse[0].yPosition, player)) {
            return true;
        }
    }

    if (opponentBishop[0].isAlive) {
        if (isValidMove(opponentBishop[0], opponentBishop[0].xPosition, opponentBishop[0].yPosition, player)) {
            return true;
        }
    }

    if (opponentRook[0].isAlive) {
        if (isValidMove(opponentRook[0], opponentRook[0].xPosition, opponentRook[0].yPosition, player)) {
            return true;
        }
    }

    if (opponentQueen[0].isAlive) {
        if (isValidMove(opponentQueen[0], opponentQueen[0].xPosition, opponentQueen[0].yPosition, player)) {
            return true;
        }
    }

    return false;
}

// Check if valid pawn move
bool Methods::isValidPawnMove(int index, Player player, float targetX, float targetY, bool simulateCapture) {
    float originalX = 0.0, originalY = 0.0;
    float newX = 0.0, newY = 0.0;

    if (player == RED) {
        originalX = redPawns[index].xPosition;
        originalY = redPawns[index].yPosition;
        newX = originalX;
        newY = originalY + 100;
    }
    else if (player == BLUE) {
        originalX = bluePawns[index].xPosition;
        originalY = bluePawns[index].yPosition;
        newX = originalX;
        newY = originalY - 100;
    }

    if (std::abs(targetX - newX) < 60 && std::abs(targetY - newY) < 60) {
        if (!isPositionOccupied(newX, newY)) {
            return true;
        }
    }

    if (player == RED) {
        if (std::abs(targetX - (newX - 100)) < 60 && std::abs(targetY - newY) < 60) {
            if (simulateCapture || canCapturePawn(newX - 100, newY, player)) {
                return true;
            }
        }
        if (std::abs(targetX - (newX + 100)) < 60 && std::abs(targetY - newY) < 60) {
            if (simulateCapture || canCapturePawn(newX + 100, newY, player)) {
                return true;
            }
        }
    }
    else if (player == BLUE) {
        if (std::abs(targetX - (newX - 100)) < 60 && std::abs(targetY - newY) < 60) {
            if (simulateCapture || canCapturePawn(newX - 100, newY, player)) {
                return true;
            }
        }
        if (std::abs(targetX - (newX + 100)) < 60 && std::abs(targetY - newY) < 60) {
            if (simulateCapture || canCapturePawn(newX + 100, newY, player)) {
                return true;
            }
        }
    }

    return false;
}

// Check if valid horse move
bool Methods::isValidHorseMove(int index, Player player, float targetX, float targetY, bool simulateCapture) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redHorse[index].xPosition;
        originalY = redHorse[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueHorse[index].xPosition;
        originalY = blueHorse[index].yPosition;
    }

    float moveX[] = { originalX + 100, originalX + 100, originalX - 100, originalX - 100, originalX + 200, originalX + 200, originalX - 200, originalX - 200 };
    float moveY[] = { originalY + 200, originalY - 200, originalY + 200, originalY - 200, originalY + 100, originalY - 100, originalY + 100, originalY - 100 };

    for (int i = 0; i < 8; ++i) {
        if (std::abs(targetX - moveX[i]) < 60 && std::abs(targetY - moveY[i]) < 60) {
            if (!isPositionOccupied(moveX[i], moveY[i])) {
                return true;
            }
            else if (simulateCapture || isPositionOccupiedByOpponent(moveX[i], moveY[i], player == RED ? BLUE : RED)) {
                return true;
            }
        }
    }

    return false;
}

// Check if valid bishop move
bool Methods::isValidBishopMove(int index, Player player, float targetX, float targetY, bool simulateCapture) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redBishop[index].xPosition;
        originalY = redBishop[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueBishop[index].xPosition;
        originalY = blueBishop[index].yPosition;
    }

    float moveX[] = { originalX + 100, originalX + 100, originalX - 100, originalX - 100 };
    float moveY[] = { originalY + 100, originalY - 100, originalY + 100, originalY - 100 };

    for (int i = 0; i < 4; ++i) {
        if (std::abs(targetX - moveX[i]) < 60 && std::abs(targetY - moveY[i]) < 60) {
            if (!isPositionOccupied(moveX[i], moveY[i])) {
                return true;
            }
            else if (simulateCapture || isPositionOccupiedByOpponent(moveX[i], moveY[i], player == RED ? BLUE : RED)) {
                return true;
            }
        }
    }

    return false;
}

// Check if valid rook move
bool Methods::isValidRookMove(int index, Player player, float targetX, float targetY, bool simulateCapture) {
    float originalX = 0.0, originalY = 0.0;
    if (player == RED) {
        originalX = redRook[index].xPosition;
        originalY = redRook[index].yPosition;
    }
    else if (player == BLUE) {
        originalX = blueRook[index].xPosition;
        originalY = blueRook[index].yPosition;
    }

    float moveX[] = { originalX + 100, originalX - 100, originalX, originalX };
    float moveY[] = { originalY, originalY, originalY + 100, originalY - 100 };

    for (int i = 0; i < 4; ++i) {
        if (std::abs(targetX - moveX[i]) < 60 && std::abs(targetY - moveY[i]) < 60) {
            if (!isPositionOccupied(moveX[i], moveY[i])) {
                return true;
            }
            else if (simulateCapture || isPositionOccupiedByOpponent(moveX[i], moveY[i], player == RED ? BLUE : RED)) {
                return true;
            }
        }
    }

    return false;
}

// Check if valid queen move
bool Methods::isValidQueenMove(int index, Player player, float targetX, float targetY, bool simulateCapture) {
    if (isValidRookMove(index, player, targetX, targetY, simulateCapture)) {
        return true;
    }
    if (isValidBishopMove(index, player, targetX, targetY, simulateCapture)) {
        return true;
    }
    return false;
}

// Check if King is in check after a move
bool Methods::isKingInCheckAfterMove(int pieceIndex, PieceType type, Player player, float targetX, float targetY) {
    ChessPiece* king = (player == RED) ? redKing : blueKing;

    // Store the original positions
    float originalX = 0.0, originalY = 0.0;
    bool isAliveBackup = false;
    switch (type) {
    case PAWN:
        if (player == RED) {
            originalX = redPawns[pieceIndex].xPosition;
            originalY = redPawns[pieceIndex].yPosition;
            redPawns[pieceIndex].xPosition = targetX;
            redPawns[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = bluePawns[pieceIndex].xPosition;
            originalY = bluePawns[pieceIndex].yPosition;
            bluePawns[pieceIndex].xPosition = targetX;
            bluePawns[pieceIndex].yPosition = targetY;
        }
        break;
    case HORSE:
        if (player == RED) {
            originalX = redHorse[pieceIndex].xPosition;
            originalY = redHorse[pieceIndex].yPosition;
            redHorse[pieceIndex].xPosition = targetX;
            redHorse[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = blueHorse[pieceIndex].xPosition;
            originalY = blueHorse[pieceIndex].yPosition;
            blueHorse[pieceIndex].xPosition = targetX;
            blueHorse[pieceIndex].yPosition = targetY;
        }
        break;
    case BISHOP:
        if (player == RED) {
            originalX = redBishop[pieceIndex].xPosition;
            originalY = redBishop[pieceIndex].yPosition;
            redBishop[pieceIndex].xPosition = targetX;
            redBishop[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = blueBishop[pieceIndex].xPosition;
            originalY = blueBishop[pieceIndex].yPosition;
            blueBishop[pieceIndex].xPosition = targetX;
            blueBishop[pieceIndex].yPosition = targetY;
        }
        break;
    case ROOK:
        if (player == RED) {
            originalX = redRook[pieceIndex].xPosition;
            originalY = redRook[pieceIndex].yPosition;
            redRook[pieceIndex].xPosition = targetX;
            redRook[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = blueRook[pieceIndex].xPosition;
            originalY = blueRook[pieceIndex].yPosition;
            blueRook[pieceIndex].xPosition = targetX;
            blueRook[pieceIndex].yPosition = targetY;
        }
        break;
    case QUEEN:
        if (player == RED) {
            originalX = redQueen[pieceIndex].xPosition;
            originalY = redQueen[pieceIndex].yPosition;
            redQueen[pieceIndex].xPosition = targetX;
            redQueen[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = blueQueen[pieceIndex].xPosition;
            originalY = blueQueen[pieceIndex].yPosition;
            blueQueen[pieceIndex].xPosition = targetX;
            blueQueen[pieceIndex].yPosition = targetY;
        }
        break;
    case KING:
        if (player == RED) {
            originalX = redKing[pieceIndex].xPosition;
            originalY = redKing[pieceIndex].yPosition;
            redKing[pieceIndex].xPosition = targetX;
            redKing[pieceIndex].yPosition = targetY;
        }
        else {
            originalX = blueKing[pieceIndex].xPosition;
            originalY = blueKing[pieceIndex].yPosition;
            blueKing[pieceIndex].xPosition = targetX;
            blueKing[pieceIndex].yPosition = targetY;
        }
        break;
    }

    // Check if the King is in check after the move
    bool inCheck = isKingInCheck(player);

    // Revert the simulated move
    switch (type) {
    case PAWN:
        if (player == RED) {
            redPawns[pieceIndex].xPosition = originalX;
            redPawns[pieceIndex].yPosition = originalY;
        }
        else {
            bluePawns[pieceIndex].xPosition = originalX;
            bluePawns[pieceIndex].yPosition = originalY;
        }
        break;
    case HORSE:
        if (player == RED) {
            redHorse[pieceIndex].xPosition = originalX;
            redHorse[pieceIndex].yPosition = originalY;
        }
        else {
            blueHorse[pieceIndex].xPosition = originalX;
            blueHorse[pieceIndex].yPosition = originalY;
        }
        break;
    case BISHOP:
        if (player == RED) {
            redBishop[pieceIndex].xPosition = originalX;
            redBishop[pieceIndex].yPosition = originalY;
        }
        else {
            blueBishop[pieceIndex].xPosition = originalX;
            blueBishop[pieceIndex].yPosition = originalY;
        }
        break;
    case ROOK:
        if (player == RED) {
            redRook[pieceIndex].xPosition = originalX;
            redRook[pieceIndex].yPosition = originalY;
        }
        else {
            blueRook[pieceIndex].xPosition = originalX;
            blueRook[pieceIndex].yPosition = originalY;
        }
        break;
    case QUEEN:
        if (player == RED) {
            redQueen[pieceIndex].xPosition = originalX;
            redQueen[pieceIndex].yPosition = originalY;
        }
        else {
            blueQueen[pieceIndex].xPosition = originalX;
            blueQueen[pieceIndex].yPosition = originalY;
        }
        break;
    case KING:
        if (player == RED) {
            redKing[pieceIndex].xPosition = originalX;
            redKing[pieceIndex].yPosition = originalY;
        }
        else {
            blueKing[pieceIndex].xPosition = originalX;
            blueKing[pieceIndex].yPosition = originalY;
        }
        break;
    }

    return inCheck;
}


// Check if can capture pawn
bool Methods::canCapturePawn(float x, float y, Player player) {
    ChessPiece* opponentPawns = (player == RED) ? bluePawns : redPawns;
    for (int i = 0; i < 5; ++i) {
        if (opponentPawns[i].isAlive && opponentPawns[i].xPosition == x && opponentPawns[i].yPosition == y) {
            opponentPawns[i].isAlive = false; // Mark the captured piece as dead
            std::cout << "Pawn captured at (" << x << ", " << y << ")" << std::endl;
            return true;
        }
    }
    return false;
}


// Check if a move is valid
bool Methods::isValidMove(ChessPiece& piece, float newX, float newY, Player player) {
    int pieceIndex = getPieceIndex(piece, player); // Implement this function to return the correct index
    switch (piece.type) {
    case PAWN:
        return isValidPawnMove(pieceIndex, player, newX, newY, false);
    case HORSE:
        return isValidHorseMove(pieceIndex, player, newX, newY, false);
    case BISHOP:
        return isValidBishopMove(pieceIndex, player, newX, newY, false);
    case ROOK:
        return isValidRookMove(pieceIndex, player, newX, newY, false);
    case QUEEN:
        return isValidQueenMove(pieceIndex, player, newX, newY, false);
    case KING:
        return moveKing(pieceIndex, player, newX, newY);
    default:
        return false;
    }
}

int Methods::getPieceIndex(ChessPiece& piece, Player player) {
    ChessPiece* pieces;
    int size;

    switch (piece.type) {
    case PAWN:
        pieces = (player == RED) ? redPawns : bluePawns;
        size = 5;
        break;
    case HORSE:
        pieces = (player == RED) ? redHorse : blueHorse;
        size = 1;
        break;
    case BISHOP:
        pieces = (player == RED) ? redBishop : blueBishop;
        size = 1;
        break;
    case ROOK:
        pieces = (player == RED) ? redRook : blueRook;
        size = 1;
        break;
    case QUEEN:
        pieces = (player == RED) ? redQueen : blueQueen;
        size = 1;
        break;
    case KING:
        pieces = (player == RED) ? redKing : blueKing;
        size = 1;
        break;
    default:
        return -1;
    }

    for (int i = 0; i < size; ++i) {
        if (&pieces[i] == &piece) {
            return i;
        }
    }

    return -1;
}


// Check if a capture is valid
bool Methods::isValidCapture(ChessPiece& piece, float newX, float newY, Player player) {
    switch (piece.type) {
    case PAWN:
        return isValidPawnMove(0, player, newX, newY, true);
    case HORSE:
        return isValidHorseMove(0, player, newX, newY, true);
    case BISHOP:
        return isValidBishopMove(0, player, newX, newY, true);
    case ROOK:
        return isValidRookMove(0, player, newX, newY, true);
    case QUEEN:
        return isValidQueenMove(0, player, newX, newY, true);
    case KING:
        return moveKing(0, player, newX, newY);
    default:
        return false;
    }
}

