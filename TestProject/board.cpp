#include "board.h"
#include <iostream>
#include <cstdlib>

void clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}

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
	clearScreen();
	std::cout << "    A   B   C   D   E   F   G   H  " << '\n';
	std::cout << "  +---+---+---+---+---+---+---+---+" << '\n';
	for (int i = 0; i < BOARD_SIZE; i++) {
		std::cout << BOARD_SIZE - i << " ";
		for (int j = 0; j < BOARD_SIZE; j++) {
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
	board[BLACK_BACK_ROW][0] = new Piece{ PieceType::Rook, PieceColour::Black };
	board[BLACK_BACK_ROW][1] = new Piece{ PieceType::Knight, PieceColour::Black };
	board[BLACK_BACK_ROW][2] = new Piece{ PieceType::Bishop, PieceColour::Black };
	board[BLACK_BACK_ROW][3] = new Piece{ PieceType::Queen, PieceColour::Black };
	board[BLACK_BACK_ROW][4] = new Piece{ PieceType::King, PieceColour::Black };
	board[BLACK_BACK_ROW][5] = new Piece{ PieceType::Bishop, PieceColour::Black };
	board[BLACK_BACK_ROW][6] = new Piece{ PieceType::Knight, PieceColour::Black };
	board[BLACK_BACK_ROW][7] = new Piece{ PieceType::Rook, PieceColour::Black };

	for (int i = 0; i < BOARD_SIZE; i++) {
		board[BLACK_PAWN_ROW][i] = new Piece{ PieceType::Pawn, PieceColour::Black };
		board[WHITE_PAWN_ROW][i] = new Piece{ PieceType::Pawn, PieceColour::White };
	}

	board[WHITE_BACK_ROW][0] = new Piece{ PieceType::Rook, PieceColour::White };
	board[WHITE_BACK_ROW][1] = new Piece{ PieceType::Knight, PieceColour::White };
	board[WHITE_BACK_ROW][2] = new Piece{ PieceType::Bishop, PieceColour::White };
	board[WHITE_BACK_ROW][3] = new Piece{ PieceType::Queen, PieceColour::White };
	board[WHITE_BACK_ROW][4] = new Piece{ PieceType::King, PieceColour::White };
	board[WHITE_BACK_ROW][5] = new Piece{ PieceType::Bishop, PieceColour::White };
	board[WHITE_BACK_ROW][6] = new Piece{ PieceType::Knight, PieceColour::White };
	board[WHITE_BACK_ROW][7] = new Piece{ PieceType::Rook, PieceColour::White };


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