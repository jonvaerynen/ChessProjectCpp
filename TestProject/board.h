#pragma once
#include <array>
#include <string>
#include "piece.h"
#include <optional>
#include <vector>
#include "move.h"

class Board {
public:
	std::array<std::array<Piece*, 8>, 8> board;
	std::optional<Position> enPassantTarget;

	std::vector<Move> MoveHistory;

	Board();
	~Board();

	void drawBoard();
	void fillBoard();
	void undoMove();
	std::string toSymbol(Piece* piece) const;
	bool tryMove(Position from, Position to);
	bool isInCheck(PieceColour colour) const;
	Position findKing(PieceColour colour) const;
	bool hasLegalMoves(PieceColour colour);
	bool simulateMove(Position from, Position to);
	void castling(int row, bool kingSide);
	bool canCastle(PieceColour colour, bool kingSide);
};

extern Board Chessboard;
