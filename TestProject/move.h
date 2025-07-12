#pragma once
class Board;

struct Move {
	Position from;
	Position to;
	Piece* movedpiece;
	Piece* capturedPiece;
	bool hasMoved;

	// castling
	bool castlingHasMoved;
	std::optional<Position> castlingFrom;
	std::optional<Position> castlingTo;
	Piece* castlingPiece;

	// Pawn
	bool wasPromoted;
	std::optional<PieceType> originalType;

	// enPassant
	std::optional<Position> enPassantTargetBefore;
	std::optional<Position> enPassantCapturedPosition;

	// konstruktori historian tallennukseen
	Move(Position from, Position to, Piece* movediece, Piece* capturedPiece, bool wasPromoted = false,
		bool hasMoved = false, bool castlingHasMoved = false,
		std::optional<PieceType> originalType = std::nullopt,
		std::optional<Position> castlingFrom = std::nullopt,
		std::optional<Position> castlingTo = std::nullopt,
		Piece* castlingPiece = nullptr,
		std::optional<Position> enPassantTargetBefore = std:: nullopt,
		std::optional<Position> enPassantCapturedPosition = std::nullopt
		)
		:from(from), to(to), movedpiece(movediece), capturedPiece(capturedPiece), 
		wasPromoted(wasPromoted), hasMoved(hasMoved), castlingHasMoved(castlingHasMoved),
		originalType(originalType), castlingFrom(castlingFrom), castlingTo(castlingTo), castlingPiece(castlingPiece),
		enPassantTargetBefore(enPassantTargetBefore), enPassantCapturedPosition(enPassantCapturedPosition) {}

};