#include <iostream>
#include <cmath>

#include "piece.h"
#include "board.h"

bool isPathClear(Position from, Position to, Piece piece, const Board& board);

bool Piece::isValidMove(Position from, Position to, const Board& board) const {

	Piece* dest = board.board[to.row][to.col];
	if (dest != nullptr && dest->colour == colour) {
		return false;
	}

	
	if (type == PieceType::Knight) {
		int dr = std::abs(from.row - to.row);
		int dc = std::abs(from.col - to.col);

		return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
	}

	if (type == PieceType::King) {

		return std::abs(from.row - to.row) <= 1 && std::abs(from.col - to.col) <= 1 || (std::abs(from.row - to.row) == 0
			&& std::abs(from.col - to.col) == 2);
	}

	if (type == PieceType::Queen) {

		return (isPathClear(from, to, *this, board));
	}

	if (type == PieceType::Rook) {

		return (isPathClear(from, to, *this, board));
	}

	if (type == PieceType::Bishop) {
		
		return (isPathClear(from, to, *this, board));
	}

	if (type == PieceType::Pawn) {

		int dr = to.row - from.row;
		int dc = std::abs(from.col - to.col);

		int direction = (colour == PieceColour::White) 
			? board.WHITE_MOVING_DIRECTION 
			: board.BLACK_MOVING_DIRECTION;

		if (dc == 0 && dr == direction && dest == nullptr) {
			return true;
		}

		if (dc == 0 && dr == direction * 2 && board.board[from.row + direction][from.col] == nullptr) {
			if ((colour == PieceColour::White && from.row == board.WHITE_PAWN_ROW)
				|| colour == PieceColour::Black && from.row == board.BLACK_PAWN_ROW) {
				return true;
			}
		}
		if (dc == 1 && dr == direction && dest != nullptr) {
			return true;
		}
		if (dc == 1 && dr == direction && dest == nullptr) {
			if (board.enPassantTarget.has_value() &&
				board.enPassantTarget.value().row == to.row &&
				board.enPassantTarget.value().col == to.col) {
				return true;
			}
		}

		return false;
	}


	return false;
}

bool isPathClear(Position from, Position to, Piece piece, const Board& board) {

	int dr = to.row - from.row;
	int dc = to.col - from.col;

	if (piece.type == PieceType::Rook && 
		dr != 0 && dc != 0){
		return false;
	} 
	else
	if (piece.type == PieceType::Queen && 
		(!(dr == 0 || dc == 0 || std::abs(dr) == std::abs(dc)))) {
		return false;
	}
	else
		if (piece.type == PieceType::Bishop &&
			std::abs(dr) != std::abs(dc)) {
			return false;
	}

	int rowStep = (dr == 0) ? 0 : (dr > 0) ? 1 : -1;
	int colStep = (dc == 0) ? 0 : (dc > 0) ? 1 : -1;

	int r = from.row + rowStep;
	int c = from.col + colStep;

	while (r != to.row || c != to.col) {
		if (board.board[r][c] != nullptr)
			return false;
		r += rowStep;
		c += colStep;
	}
	return true;
}