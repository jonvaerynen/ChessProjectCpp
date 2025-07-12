#pragma once

#include "position.h"

class Board;

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn };
enum class PieceColour { White, Black };

struct Piece {
	PieceType type;
	PieceColour colour;

	bool hasMoved = false;

	bool isValidMove(Position from, Position to, const Board& board) const;

	Piece* clone() const{
		Piece* copy = new Piece;
		copy->type = this->type;
		copy->colour = this->colour;
		copy->hasMoved = this->hasMoved;
		return copy;
	
	}
};