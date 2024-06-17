#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Add this line

class Methods {
public:
    struct ChessPiece {
        float xPosition;
        float yPosition;
        bool isAlive;
        int type; // added type to represent the piece type
    };

    enum Player { RED, BLUE };
    enum PieceType { PAWN, HORSE, BISHOP, ROOK, QUEEN, KING };

    static ChessPiece redPawns[5];
    static ChessPiece bluePawns[5];
    static ChessPiece redHorse[1];
    static ChessPiece blueHorse[1];
    static ChessPiece redBishop[1];
    static ChessPiece blueBishop[1];
    static ChessPiece redRook[1];
    static ChessPiece blueRook[1];
    static ChessPiece redQueen[1];
    static ChessPiece blueQueen[1];
    static ChessPiece redKing[1];
    static ChessPiece blueKing[1];

    static bool gameOver;
    static Player winner;
    static bool pawnSelected;
    static int selectedPawnIndex;
    static PieceType selectedType;
    static Player currentPlayer;
    static Player selectedPlayer;

    static unsigned int blackVAO;
    static unsigned int blackVBO;
    static unsigned int whiteVAO;
    static unsigned int whiteVBO;
    static unsigned int textureVAO;
    static unsigned int textureVBO;
    static unsigned int texture;
    static unsigned int redPawnTexture;
    static unsigned int redRookTexture;
    static unsigned int redHorseTexture;
    static unsigned int redBishopTexture;
    static unsigned int redQueenTexture;
    static unsigned int redKingTexture;
    static unsigned int bluePawnTexture;
    static unsigned int blueRookTexture;
    static unsigned int blueHorseTexture;
    static unsigned int blueBishopTexture;
    static unsigned int blueQueenTexture;
    static unsigned int blueKingTexture;

    static bool movePawn(int index, Player player, float mouseX, float mouseY);
    static bool moveHorse(int index, Player player, float mouseX, float mouseY);
    static bool moveBishop(int index, Player player, float mouseX, float mouseY);
    static bool moveRook(int index, Player player, float mouseX, float mouseY);
    static bool moveQueen(int index, Player player, float mouseX, float mouseY);
    static bool moveKing(int index, Player player, float mouseX, float mouseY);
    static bool isPositionOccupied(float x, float y);
    static bool isKingInCheck(Player player);
    static bool canKingMove(Player player);
    static bool canBlockOrCapture(Player player);
    static bool isValidPawnMove(int index, Player player, float targetX, float targetY, bool simulateCapture);
    static bool isValidHorseMove(int index, Player player, float targetX, float targetY, bool simulateCapture);
    static bool isValidBishopMove(int index, Player player, float targetX, float targetY, bool simulateCapture);
    static bool isValidRookMove(int index, Player player, float targetX, float targetY, bool simulateCapture);
    static bool isValidQueenMove(int index, Player player, float targetX, float targetY, bool simulateCapture);
    static bool isKingInCheckAfterMove(int pieceIndex, PieceType type, Player player, float targetX, float targetY);
    static bool isKingInCheckMate(Player player);
    static bool canCapturePawn(float x, float y, Player player);

    static void initializeBlackSquareResources();
    static void initializeWhiteSquareResources();
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void handleClick(float mouseX, float mouseY);
    static void selectPiece(float mouseX, float mouseY);
    static bool movePiece(float mouseX, float mouseY);
    static bool isPositionOccupiedByOpponent(float x, float y, Player opponent);
    static bool isWithinBounds(ChessPiece& piece, float mouseX, float mouseY);

    static void drawWhiteSquareMatrix(float xPosition, float yPosition, float size);
    static void drawBlackSquareMatrix(float xPosition, float yPosition, float size);
    static void drawWhiteRow(float xPosition, float yPosition);
    static void drawBlackRow(float xPosition, float yPosition);
    static void drawChessboard();
    static unsigned int loadTexture(const char* path);
    static void initializeTextureResources();
    static void drawChessPiece(unsigned int textureID, float xPosition, float yPosition, float size);
    static void drawRedPieces();
    static void drawBluePieces();
    static void initializeGame();
    static void capturePiece(float x, float y, Player opponent);
    static bool isValidMove(ChessPiece& piece, float newX, float newY, Player player); // Add this line
    static bool isValidCapture(ChessPiece& piece, float newX, float newY, Player player); // Add this line
    static int getPieceIndex(ChessPiece& piece, Player player);
    static std::string pieceTypeToString(PieceType type);

};
