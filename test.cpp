#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

using namespace std;

class Piece {
public:
    char type;
    bool color;

    Piece() {}

    Piece(char type, bool color) : type(type), color(color) {}
};

class Board {
public:
    Piece pieces[8][8];

    void print() {
        cout << "  1  2  3  4  5  6  7  8" << endl;
        for (int i = 0; i < 8; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; j++) {
                if (pieces[i][j].type) {
                    cout << (pieces[i][j].color ? 'w' : 'b') << pieces[i][j].type << ' ';
                }
                else {
                    cout << "-- ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};

struct Move {
    int startX, startY, endX, endY;

    Move(int startX, int startY, int endX, int endY) :
        startX(startX), startY(startY), endX(endX), endY(endY) {}
};

bool isKingMoveValid(Board& board, int startX, int startY, int endX, int endY) {
    int diffX = abs(startX - endX);
    int diffY = abs(startY - endY);

    if ((diffX == 1 && diffY <= 1) || (diffX <= 1 && diffY == 1)) {
        if (board.pieces[endX][endY].type == 0 || board.pieces[endX][endY].color != board.pieces[startX][startY].color) {
            return true;
        }
    }

    return false;
}

vector<Move> generatePossibleMoves(Board& board, bool color) {
    vector<Move> possibleMoves;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board.pieces[x][y].color == color && board.pieces[x][y].type == 'K') {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                            if (i != 0 || j != 0) {
                                int newX = x + i;
                                    int newY = y + j;

                                    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && isKingMoveValid(board, x, y, newX, newY)) {
                                        possibleMoves.push_back(Move(x, y, newX, newY));
                                    }
                            }
                    }
                }
            }
        }
    }

    return possibleMoves;
}

int getPieceValue(Piece& piece) {
    switch (piece.type) {
    case 'K':
        return 10000;
    case 'Q':
        return 900;
    case 'R':
        return 500;
    case 'N':
        return 320;
    case 'B':
        return 330;
    case 'P':
        return 100;
    default:
        return 0;
    }
}
int evaluateBoard(Board& board) {
    int score = 0;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board.pieces[x][y].type != 0) {
                int value = getPieceValue(board.pieces[x][y]);
                if (board.pieces[x][y].color) {
                    score += value;
                }
                else {
                    score -= value;
                }
            }
        }
    }

    return score;
}



int negamax(Board& board, int depth, int alpha, int beta, bool color) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    int maxScore = -100000;

    vector<Move> possibleMoves = generatePossibleMoves(board, color);
    for (Move& move : possibleMoves) {
        Board newBoard = board;

        newBoard.pieces[move.endX][move.endY] = newBoard.pieces[move.startX][move.startY];
        newBoard.pieces[move.startX][move.startY] = Piece();

        int score = -negamax(newBoard, depth - 1, -beta, -alpha, !color);
        maxScore = max(maxScore, score);
        alpha = max(alpha, score);

        if (alpha >= beta) {
            break;
        }
    }

    return maxScore;
}

Move getBestMove(Board& board, int depth, bool color) {
    int maxScore = -100000;
    Move bestMove(-1, -1, -1, -1);

    vector<Move> possibleMoves = generatePossibleMoves(board, color);
    for (Move& move : possibleMoves) {
        Board newBoard = board;

        newBoard.pieces[move.endX][move.endY] = newBoard.pieces[move.startX][move.startY];
        newBoard.pieces[move.startX][move.startY] = Piece();

        int score = -negamax(newBoard, depth - 1, -100000, 100000, !color);
        if (score > maxScore) {
            maxScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

void playGame(Board& board) {
    bool color = true;

    while (true) {
        system("cls");
        board.print();

        Move move(-1, -1, -1, -1);

        if (color) {
            while (move.startX < 1 || move.startX > 8 || move.startY < 1 || move.startY > 8 || move.endX < 1 || move.endY > 8 ||
                !isKingMoveValid(board, move.startX - 1, move.startY - 1, move.endX - 1, move.endY - 1)) {
                cout << "Enter start and end coordinates: ";
                cin >> move.startX >> move.startY >> move.endX >> move.endY;
                continue;
            }
        }
        else {
            move = getBestMove(board, 3, color);
            // Our board representation follows 0-based present the row and column from 1 to 8.
            cout << "Row: " << move.startX + 1 << " Column: " << move.startY + 1 << endl;
            cout << "Move to Row: " << move.endX + 1 << " Column: " << move.endY + 1 << endl;
        }

        board.pieces[move.endY][move.endX] = board.pieces[move.startY][move.startX];
        board.pieces[move.startY][move.startX] = Piece();
        color = !color;
    }
}


int main() {
    Board board;

    board.pieces[0][0] = Piece('R', true);
    board.pieces[0][1] = Piece('N', true);
    board.pieces[0][2] = Piece('B', true);
    board.pieces[0][3] = Piece('Q', true);
    board.pieces[0][4] = Piece('K', true);
    board.pieces[0][5] = Piece('B', true);
    board.pieces[0][6] = Piece('N', true);
    board.pieces[0][7] = Piece('R', true);

    for (int i = 0; i < 8; i++) {
        board.pieces[1][i] = Piece('P', true);
    }

    board.pieces[7][0] = Piece('R', false);
    board.pieces[7][1] = Piece('N', false);
    board.pieces[7][2] = Piece('B', false);
    board.pieces[7][3] = Piece('Q', false);
    board.pieces[7][4] = Piece('K', false);
    board.pieces[7][5] = Piece('B', false);
    board.pieces[7][6] = Piece('N', false);
    board.pieces[7][7] = Piece('R', false);

    for (int i = 0; i < 8; i++) {
        board.pieces[6][i] = Piece('P', false);
    }

    playGame(board);

    return 0;
}
