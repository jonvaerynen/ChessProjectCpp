#include "board.h"
#include <iostream>
#include <cstdlib>


Board::Board() {
	fillBoard();
}

Board::~Board() {
	for (auto& row : board) {
		for (auto& cell : row) {
			delete cell;
		}
	}
}
void Board::drawBoard() {
	system("cls");
	std::cout << "    A   B   C   D   E   F   G   H  " << '\n';
	std::cout << "  +---+---+---+---+---+---+---+---+" << '\n';
	for (int i = 0; i < 8; i++) {
		std::cout << 8 - i << " ";
		for (int j = 0; j < 8; j++) {
			std::cout << "|";
			bool black = (i + j) % 2 == 0;
			if (black)
				std::cout << "\033[40m\033[37m" << toSymbol(board[i][j]) << "\033[0m";
			else
				std::cout << "\033[47m\033[30m" << toSymbol(board[i][j]) << "\033[0m";

			if (j == 7) {
				std::cout << "|" << '\n' << "  +---+---+---+---+---+---+---+---+" << '\n';
			}
		}
	}
}
void Board::fillBoard() {
	for (auto& row : board) {
		for (auto& cell : row) {
			cell = nullptr;
		};
	}
	board[0][0] = new Piece{ PieceType::Rook, PieceColour::Black };
	board[0][1] = new Piece{ PieceType::Knight, PieceColour::Black };
	board[0][2] = new Piece{ PieceType::Bishop, PieceColour::Black };
	board[0][3] = new Piece{ PieceType::Queen, PieceColour::Black };
	board[0][4] = new Piece{ PieceType::King, PieceColour::Black };
	board[0][5] = new Piece{ PieceType::Bishop, PieceColour::Black };
	board[0][6] = new Piece{ PieceType::Knight, PieceColour::Black };
	board[0][7] = new Piece{ PieceType::Rook, PieceColour::Black };

	for (int i = 0; i < 8; i++) {
		board[1][i] = new Piece{ PieceType::Pawn, PieceColour::Black };
		board[6][i] = new Piece{ PieceType::Pawn, PieceColour::White };
	}

	board[7][0] = new Piece{ PieceType::Rook, PieceColour::White };
	board[7][1] = new Piece{ PieceType::Knight, PieceColour::White };
	board[7][2] = new Piece{ PieceType::Bishop, PieceColour::White };
	board[7][3] = new Piece{ PieceType::Queen, PieceColour::White };
	board[7][4] = new Piece{ PieceType::King, PieceColour::White };
	board[7][5] = new Piece{ PieceType::Bishop, PieceColour::White };
	board[7][6] = new Piece{ PieceType::Knight, PieceColour::White };
	board[7][7] = new Piece{ PieceType::Rook, PieceColour::White };


}

std::string Board::toSymbol(Piece* piece)const {
	
	if (piece == nullptr) {
		return "   ";
	}

	char c = '?';
	switch (piece->type) {
	case PieceType::Pawn: c = 'P'; break;
	case PieceType::Queen: c = 'Q'; break;
	case PieceType::King: c = 'K'; break;
	case PieceType::Bishop: c = 'B'; break;
	case PieceType::Knight: c = 'N'; break;
	case PieceType::Rook: c = 'R'; break;
	}
	return (piece->colour == PieceColour::White)
		? "(" + std::string(1, c) + ")"
		: "<" + std::string(1, c) + ">";
}