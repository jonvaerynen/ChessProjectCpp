#pragma once
#include <array>
#include <string>
#include "piece.h"
#include <optional>
#include <vector>
#include "move.h"

class Board {
public:

	static constexpr int BOARD_SIZE = 8;
	static constexpr int WHITE_PAWN_ROW = 6;
	static constexpr int BLACK_PAWN_ROW = 1;
	static constexpr int WHITE_BACK_ROW = 7;
	static constexpr int BLACK_BACK_ROW = 0;
	static constexpr int KING_START_COL = 4;
	static constexpr int QUEENSIDE_ROOK_COL = 0;
	static constexpr int KINGSIDE_ROOK_COL = 7;
	static constexpr int KINGSIDE_ROOK_END = 5;
	static constexpr int QUEENSIDE_ROOK_END = 3;
	static constexpr int WHITE_MOVING_DIRECTION = -1;
	static constexpr int BLACK_MOVING_DIRECTION = 1;


	std::array<std::array<Piece*, BOARD_SIZE>, BOARD_SIZE> board;
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
