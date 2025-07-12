#include <iostream>
#include <string>
#include "board.h"

Board Chessboard;
PieceColour currentPlayer = PieceColour::White;

void UserInput();
void switchPlayer();
bool checkGameState();

void runChess() {
	bool gaming = true;
	Chessboard.drawBoard();

	while (gaming){

		if (!checkGameState()) {
			gaming = false;
			break;
		}
		std::cout << "\nVuorossa: " << (currentPlayer == PieceColour::White ? "Valkoinen" : "Musta") << '\n';
		UserInput();
	}
}

void UserInput() {
	std::string move;
	std::cout << "Anna siirto (esim. e2b4) tai 'u' peruuttaaksesi: ";
	std::cin >> move;

	if (move == "u") {	
		Chessboard.undoMove();
		switchPlayer();
		Chessboard.drawBoard();
		return;
	}

	if (move.length() != 4) {
		std::cout << "Virheellinen syöte! Käytä muotoa esim. e2e4\n";
		return;
	}

	Position from{8 - (move[1] - '0'), move[0] - 'a'};
	Position to{8 - (move[3] - '0'), move[2] - 'a'};

	if (from.row < 0 || from.row > 7 || from.col < 0 || from.col > 7 ||
		to.row < 0 || to.row > 7 || to.col < 0 || to.col > 7) {
		std::cout << "Siirto on laudan ulkopuolella!\n";
		return;
	}

	Piece* piece = Chessboard.board[from.row][from.col];
	if (!piece || piece->colour != currentPlayer) {
		std::cout << "Et voi siirtää vastustajan nappulaa tai tyhjää ruutua!\n";
		return;
	}
	if (Chessboard.tryMove(from, to)) {
		Chessboard.drawBoard();
		switchPlayer();
	}
	else {
		std::cout << "Laiton siirto\n";
	}
}

void switchPlayer() {
	currentPlayer = (currentPlayer == PieceColour::White) ? PieceColour::Black : PieceColour::White;
}

bool checkGameState() {
	if (Chessboard.isInCheck(currentPlayer)) {
		std::cout << "\n*** SHAKKI! ***\n";

		if (!Chessboard.hasLegalMoves(currentPlayer)) {
			std::cout << "\n*** SHAKKI MATTI! ***\n";
		
		if (currentPlayer == PieceColour::White) {
			std::cout << "Musta voitti! Valkoinen hävisi.\n";
		}
		else {
			std::cout << "Valkoinen voitti! Musta hävisi.\n";
		}

		std::cout << "Peli päättyi!\n";
		return false;
		}
	}
	else {
		if (!Chessboard.hasLegalMoves(currentPlayer)) {
			std::cout <<  "\n*** PATTI ***\n";
			std::cout << "Peli päättyi tasapeliin\n";
			return false;
		}
	}
	return true;
}