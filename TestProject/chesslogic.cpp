#include <iostream>
#include "board.h"

void promote(Piece* piece);
Piece* enPassant(Position from, Position to, Piece* piece, Piece* capturedPiece, std::optional<Position>& enPassantCapturedPosition, Board & board);

bool Board::tryMove(Position from, Position to) {
	if (from == to) return false;

	Piece* piece = board[from.row][from.col];
	if (piece == nullptr) return false;

	if (!piece->isValidMove(from, to, *this)) return false;

	if (simulateMove(from, to)) return false;

	Piece* capturedPiece = board[to.row][to.col];
	Piece* capturedPieceClone = capturedPiece ? capturedPiece->clone() : nullptr;

	bool castlingOccured = false;
	std::optional<Position> RookFrom;
	std::optional<Position> RookTo;
	auto enPassantTargetBefore = enPassantTarget;
	std::optional<Position> enPassantCapturedPosition;
	Piece* RookPtr = nullptr;
	bool originalRookHasMoved = false;


	if (piece->type == PieceType::King && std::abs(to.col - from.col) == 2) {
		bool kingSide = to.col > from.col;
		if (!canCastle(piece->colour, kingSide))
			return false;

		castlingOccured = true;
		int RookStart = kingSide ? KINGSIDE_ROOK_COL : QUEENSIDE_ROOK_COL;
		int RookEnd = kingSide ? KINGSIDE_ROOK_END : QUEENSIDE_ROOK_END;

		RookFrom = { from.row, RookStart };
		RookTo = { from.row, RookEnd };
		RookPtr = board[from.row][RookStart];
		originalRookHasMoved = RookPtr->hasMoved;
		
		castling(from.row, kingSide);
	}

	if (board[to.row][to.col] != nullptr) {
		delete board[to.row][to.col];
	}
	
	board[to.row][to.col] = piece;
	board[from.row][from.col] = nullptr;

	bool promotion = false;
	if (piece->type == PieceType::Pawn) {
		Piece* enPassantCaptured = (enPassant(from, to, piece, capturedPiece, enPassantCapturedPosition, *this));

		if (enPassantCaptured != capturedPiece) {
			delete capturedPieceClone;
			capturedPieceClone = enPassantCaptured ? enPassantCaptured->clone() : nullptr ;
		}

		if (piece->colour == PieceColour::White && to.row == BLACK_BACK_ROW
			|| piece->colour == PieceColour::Black && to.row == WHITE_BACK_ROW) {
			promote(piece);
			promotion = true;
		}
	}
	else {
		enPassantTarget.reset();
	}
	
	bool hasMoved = piece->hasMoved;
	piece->hasMoved = true;

	MoveHistory.emplace_back(
		from,
		to,
		piece,
		capturedPieceClone,
		promotion,
		hasMoved,
		originalRookHasMoved,
		promotion ? std::optional<PieceType>{PieceType::Pawn} : std::nullopt,
		RookFrom,
		RookTo,
		RookPtr,
		enPassantTargetBefore,
		enPassantCapturedPosition
	);
	
	return true;
}

void Board::castling(int row, bool kingSide) {

	int RookFromCol = kingSide ? KINGSIDE_ROOK_COL : QUEENSIDE_ROOK_COL ;
	int RookToCol = kingSide ? KINGSIDE_ROOK_END : QUEENSIDE_ROOK_END ;

	Piece* rook = board[row][RookFromCol];
	if (!rook) {
		std::cerr << "Virhe: Torni puuttuu tornituksesta\n";
		return;
	}
	board[row][RookFromCol] = nullptr;
	board[row][RookToCol] = rook;
	rook->hasMoved = true;


}

bool Board::isInCheck(PieceColour colour) const {
	Position kingPosition = findKing(colour);

	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			Piece* p = board[r][c];
			if (p != nullptr && p->colour != colour) {
				if (p->type == PieceType::King) {
					if (std::abs(r - kingPosition.row) <= 1 && (std::abs(c - kingPosition.col) <= 1)){
						return true;
					}
				}
					else
				if (p->isValidMove({ r,c }, kingPosition, *this)) {
					return true;
				}
			}
		}
	}
	return false;
}

Position Board::findKing(PieceColour colour)const {

	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			Piece* p = board[r][c];
			if (p && p->type == PieceType::King && p->colour == colour) {
				return { r,c };
			}
		}
	}
	throw std::runtime_error("King not found");
}

Piece* enPassant(Position from, Position to, Piece* piece, Piece* capturedPiece, std::optional<Position> &enPassantCapturedPosition, Board& board) {

	int startRow = (piece->colour == PieceColour::White) 
		? board.WHITE_PAWN_ROW 
		: board.BLACK_PAWN_ROW;
	int direction = (piece->colour == PieceColour::White) 
		? board.WHITE_MOVING_DIRECTION
		: board.BLACK_MOVING_DIRECTION;

	if (from.row == startRow && to.row == from.row + direction * 2) {
		board.enPassantTarget = { from.row + direction, from.col };
		return capturedPiece;
	}
	else
		if (board.enPassantTarget.has_value() &&
			board.enPassantTarget.value() == to) {

			int victimRow = to.row - direction;
			Piece* victim = board.board[victimRow][to.col];

			if (victim) {
				enPassantCapturedPosition = { victimRow, to.col };
				board.board[victimRow][to.col] = nullptr;
				board.enPassantTarget.reset();
				return victim;
			}
		}
	board.enPassantTarget.reset();
	return capturedPiece;
}

bool Board::canCastle(PieceColour colour, bool kingSide)  {
	
	int row = (colour == PieceColour::White) 
		? WHITE_BACK_ROW 
		: BLACK_BACK_ROW;

	int RookCol = (kingSide) 
		? KINGSIDE_ROOK_COL 
		: QUEENSIDE_ROOK_COL;

	int direction = (kingSide) 
		? 1 // Oikealle
		: -1; // Vasemmalle


	Piece* king = board[row][KING_START_COL];
	Piece* Rook = board[row][RookCol];

	if (!king || !Rook || king->hasMoved || Rook->hasMoved)
		return false;

	for (int c = KING_START_COL + direction; c != RookCol; c += direction) {
		if (board[row][c] != nullptr)
			return false;
	}
	for (int i = 1; i <= 2; i++) {
		int testCol = KING_START_COL + i * direction;
		if (simulateMove({ row, KING_START_COL }, { row, testCol }))
			return false;
	}

	return true;
}

void promote(Piece* piece) {

	char answer;
	std::cout << '\n' << "What type would you want your new piece to be? (Q/N/B/R)\n";
	std::cin >> answer;

	do {
		if (answer == 'i') {
			std::cout << "\nPlease enter (Q/N/B/R)\n";
			std::cin >> answer;
		}
		switch (answer) {
		case 'q': case 'Q': piece->type = PieceType::Queen; break;
		case 'n': case 'N': piece->type = PieceType::Knight; break;
		case 'b': case 'B': piece->type = PieceType::Bishop; break;
		case 'r': case 'R': piece->type = PieceType::Rook; break;
		default: answer = 'i';
		}
		std::cin.clear();
		std::cin.ignore(100, '\n');
	} while (answer == 'i');

}

bool Board::hasLegalMoves(PieceColour colour) {

	for (int r1 = 0; r1 < BOARD_SIZE; r1++) {
		for (int c1 = 0; c1 < BOARD_SIZE; c1++) {
			Piece* piece = board[r1][c1];
			if (!piece || piece->colour != colour)	continue;

			for (int r2 = 0; r2 < BOARD_SIZE; r2++) {
				for (int c2 = 0; c2 < BOARD_SIZE; c2++) {

					Position from = { r1,c1 };
					Position to = { r2,c2 };

					if (!(piece->isValidMove(from, to, *this))) continue;

					bool stillInCheck = simulateMove(from, to);

					if (!stillInCheck)
						return true;
				}
			}


		}
	}
	return false;
}

bool Board::simulateMove(Position from, Position to) {

	if (from == to) return false;

	Piece* piece = board[from.row][from.col];
	Piece* captured = board[to.row][to.col];
	board[to.row][to.col] = piece;
	board[from.row][from.col] = nullptr;

	bool Checked = isInCheck(piece->colour);

	// perutaan simuloitu siirto
	board[from.row][from.col] = piece;
	board[to.row][to.col] = captured;

	return Checked;
}

void Board::undoMove(){
	if (MoveHistory.empty()) return;

	Move last = MoveHistory.back();
	MoveHistory.pop_back();

	enPassantTarget = last.enPassantTargetBefore;

	board[last.from.row][last.from.col] = last.movedpiece;
	board[last.to.row][last.to.col] = nullptr;

	if (last.enPassantCapturedPosition.has_value()) {
		Position captured = last.enPassantCapturedPosition.value();
		board[captured.row][captured.col] = last.capturedPiece;
	}
	else if (last.capturedPiece != nullptr){
		board[last.to.row][last.to.col] = last.capturedPiece;
	}

	if (last.wasPromoted && last.originalType.has_value()) {
		last.movedpiece->type = last.originalType.value();
	}

	if (last.castlingFrom && last.castlingTo && last.castlingPiece) {
		board[last.castlingFrom->row][last.castlingFrom->col] = last.castlingPiece;
		board[last.castlingTo->row][last.castlingTo->col] = nullptr;
		last.castlingPiece->hasMoved = last.castlingHasMoved;
	}


	last.movedpiece->hasMoved = last.hasMoved;
}