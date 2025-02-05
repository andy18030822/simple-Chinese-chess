#include "Board.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>

static constexpr Position Directions[4] = { {0, 1}, {1, 0}, {-1, 0}, {0, -1} };
static constexpr Position Corners[4] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
static constexpr int WorstScore = -0x999999;

Board::Board() : Board("rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR w") {}

Board::Board(const std::string& fen_string)
{
    int read = 0;
    Position position{0, 9};

    for (char fen : fen_string)
    {
        read++;
        if (fen == ' ') break;

        if (fen == '/')
        {
            position.x = 0;
            position.y--;
        }
        else if ('1' <= fen && fen <= '9')
        {
            int gap = fen - '0';
            position.x += gap;
        }
        else
        {
            pieces[position.get_index()] = Piece(fen);
            position.x++;
        }
    }

    black_turn = (fen_string[read] == 'b');
}

void Board::print_board() const
{
    std::cout << "  A B C D E F G H I" << std::endl;

    for (int y = 9; y >= 0; y--)
    {
        std::cout << y << " ";

        for (int x = 0; x < 9; x++)
        {
            int index = x + y * Width;
            std::cout << pieces[index].to_string();
        }
        
        std::cout << ' ' << y << std::endl;
    }

    std::cout << "  A B C D E F G H I" << std::endl;

    if (black_turn)
    {
        std::cout << "black turn" << std::endl;
    }
    else
    {
        std::cout << "red turn" << std::endl;
    }
}

void Board::make_move(Move move)
{   
    int begin = move.get_begin().get_index();
    int end = move.get_end().get_index();
    move_history.emplace_back(move, pieces[end]);

    Piece move_piece = pieces[begin];
    assert(not move_piece.is_empty());

    pieces[begin] = Piece();
    pieces[end] = move_piece;

    black_turn = not black_turn;
}

bool Board::undo_move()
{
    if (move_history.empty()) return false;

    auto [move, capture] = move_history.back();
    move_history.pop_back();

    int begin = move.get_begin().get_index();
    int end = move.get_end().get_index();

    pieces[begin] = pieces[end];
    pieces[end] = capture;

    black_turn = not black_turn;
    return true;
}

std::vector<Move> Board::get_moves() const
{
    std::vector<Move> moves;
    Position position;

    for (position.y = 0; position.y < Height; position.y++)
    {
        for(position.x = 0; position.x < Width; position.x++)
        {
            Piece piece = pieces[position.get_index()];
            if (piece.is_empty() || piece.is_black() != black_turn) continue;

            if (piece.equals_ignore_color(Piece::pawn())) fill_pawn_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::cannon())) fill_cannon_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::rook())) fill_rook_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::horse())) fill_horse_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::elephant())) fill_elephant_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::advisor())) fill_advisor_moves(position, moves);
            else if (piece.equals_ignore_color(Piece::king())) fill_king_moves(position, moves);
        }
    }

    return moves;
}

std::vector<Move> Board::get_legal_moves()
{
    std::vector<Move> moves = get_moves();

    auto predicate = [this](Move move)
    {
        make_move(move);

        Position king = find_king(true);
        std::vector<Move> opponent_moves = get_moves();

        for (Move move : opponent_moves)
        {
            if (move.end == king)
            {
                undo_move();
                return true;
            }
        }

        undo_move();
        return false;
    };

    moves.erase(std::remove_if(moves.begin(), moves.end(), predicate), moves.end());
    return moves;
}

Position Board::find_king(bool opponent) const
{
    bool is_black = opponent ^ black_turn;
    Position position;

    for (position.y = is_black ? 7 : 0; position.y <= (is_black ? 9 : 2); position.y++)
    {
        for (position.x = 3; position.x <= 5; position.x++)
        {
            Piece piece = pieces[position.get_index()];
            if (piece.equals_ignore_color(Piece::king()))
            {
                return position;
            }
        }
    }

    return Position();
}

uint64_t Board::perft(int depth)
{
    assert(depth >= 1);

    auto moves = get_legal_moves();
    if (depth == 1) return moves.size();
    
    uint64_t nodes = 0;

    for (Move legal_move : moves)
    {
        make_move(legal_move);
        nodes += perft(depth - 1);
        undo_move();
    }

    return nodes;
}

Position index_to_position(int i)
{
    Position position;
    position.x = i % 9;
    position.y = i / 10;
}

int Board::evaluate() const
{
    int score = 0;

    for (int y = 0; y < Height; y++)
    {
        for(int x = 0; x < Width; x++)
        {
            int index = x + y * Width;
            Piece piece = pieces[index];
            int piece_score = piece.get_evaluate_score();
            score += piece_score;
            
            //pawn
            if(piece.equals_ignore_color(Piece::pawn()))
            {
                if(x == 4)
                {
                    score += piece.get_evaluate_score()/2;
                }
                if(y <= 4 == piece.is_black())
                {
                    score += piece.get_evaluate_score();
                }
            }

            //cannon
            if(piece.equals_ignore_color(Piece::cannon()))
            {
                if(in_same_line_with_king(piece,x,y))
                    score += piece.get_evaluate_score()/5;
                
            }

            //rook
            if(piece.equals_ignore_color(Piece::rook()))
            {
                if(in_same_line_with_king(piece,x,y))
                    score += piece.get_evaluate_score()/5;
            }
        }

    }


    return black_turn ? -score : score;
}

bool Board::in_same_line_with_king(Piece piece,int x,int y) const
{
    Position king_position = piece.is_black() ? find_king(black_turn) : find_king(!black_turn);
    return king_position.x == x || king_position.y == y;
}


// Move Board::find_best_move()
// {
//     int best_score = WorstScore;
//     Move best_move;

//     auto moves = get_legal_moves();

//     for (Move move : moves)
//     {
//         make_move(move);

//         //predict steps
//         int score = -find_highest_score(5, WorstScore, -best_score);

//         if (score > best_score)
//         {
//             best_score = score;
//             best_move = move; 
//         }

//         undo_move();
//     }

//     return best_move;
// }


Move Board::find_best_move(int& score, int& depth)
{
    using Time = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;

    std::vector<Move> best;

    auto start = Time::now();

    depth = 0;

    while (std::chrono::duration_cast<ms>(Time::now() - start).count() < 1000)
    {
        best = find_best_moves(depth, WorstScore, -WorstScore, true, best, score);
        depth++;

        while (best.size() < depth) best.insert(best.begin(), Move());
    }

    return best.back();
}

int Board::find_highest_score(int depth, int alpha, int beta)
{
    if (depth == 0) return evaluate();

    auto moves = get_legal_moves();

    if (moves.empty()) return WorstScore;

    for (Move move : moves)
    {
        make_move(move);
        int score = -find_highest_score(depth - 1, -beta, -alpha);
        undo_move();

        if (score >= beta) return beta;
        if (alpha < score) alpha = score;
    }

    return alpha;
}

std::vector<Move> Board::find_best_moves(int depth, int alpha, int beta, bool prioritized, const std::vector<Move>& best, int& score)
{
    if (depth < 0)
    {
        score = evaluate();
        return {};
    }

    auto moves = get_legal_moves();

    if (moves.empty())
    {
        score = WorstScore;
        return {};
    }

    std::vector<Move> best_moves;

    if (prioritized && depth > 0)
    {
        assert(alpha == WorstScore);
        assert(beta == -WorstScore);

        Move move = best[depth - 1];

        if (not move.is_valid())
        {
            make_move(move);

            int current_score;
            auto current_moves = find_best_moves(depth - 1, -beta, -alpha, true, best, current_score);
            current_score = -current_score;

            undo_move();

            alpha = current_score;
            best_moves = current_moves;
            best_moves.push_back(move);
        }
    }

    for (Move move : moves)
    {
        make_move(move);

        int current_score;
        auto current_moves = find_best_moves(depth - 1, -beta, -alpha, false, best, current_score);
        current_score = -current_score;

        undo_move();

        if (current_score >= beta)
        {
            score = beta;
            return {}; //Should not be used by upper level
        }

        if (alpha < current_score)
        {
            alpha = current_score;
            best_moves = current_moves;
            best_moves.push_back(move);
        }
    }

    score = alpha;
    return best_moves;
}

// int Board::find_best_move(int depth, int alpha, int beta, bool prioritized, std::vector<Move>& best)
// {
//     if (depth < 0)
//     {
//         best = {};
//         return evaluate();
//     }

//     auto moves = get_legal_moves();
//     if (moves.empty()) return WorstScore;

//     if (prioritized && depth < best.size())
//     {
//         assert(alpha == WorstScore);
//         assert(beta == -WorstScore);
//         Move move = best[depth];

//         make_move(move);
//         int score = -find_best_move(depth - 1, WorstScore, -WorstScore, true, best);
//         undo_move();

//         best.push_back(move);
//         alpha = score;
//     }

//     for (Move move : moves)
//     {
//         std::vector<Move> current_best;

//         make_move(move);
//         int score = -find_best_move(depth - 1, -beta, -alpha, false, current_best);
//         undo_move();

//         if (score >= beta) return beta;

//         if (alpha < score)
//         {
//             best = current_best;
//             best.push_back(move);
//             alpha = score;
//         }
//     }

//     return alpha;
// }

void Board::fill_pawn_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;
    
    //Move forward
    move.end = position;
    move.end.y += black_turn ? -1 : 1;

    if (move.end.y >= 0 && move.end.y < Height) try_take(move, moves);

    bool crossed = (position.y <= 4) == black_turn;

    if (crossed)
    {
        //Move left
        if (position.x > 0)
        {
            move.end = position;
            move.end.x--;
            try_take(move, moves);
        }

        //Move right
        if (position.x < Width - 1)
        {
            move.end = position;
            move.end.x++;
            try_take(move, moves);
        }
    }
}


void Board::fill_cannon_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;
    
    for (Position direction : Directions)
    {
        move.end = position + direction;
        while (move.end.is_valid())
        {
            Piece piece = pieces[move.end.get_index()];
            if (not piece.is_empty()) break;
            moves.push_back(move);
            move.end = move.end + direction;
        }
        
        move.end = move.end + direction;
        while (move.end.is_valid())
        {
            Piece piece = pieces[move.end.get_index()];
            if (not piece.is_empty()) 
            {
                try_take(move, moves);
                break;
            }
            move.end = move.end + direction;
        }
    }
}

void Board::fill_rook_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;

    for (Position direction : Directions)
    {
        move.end = position + direction;

        while (move.end.is_valid())
        {
            try_take(move, moves);

            Piece piece = pieces[move.end.get_index()];
            if (not piece.is_empty()) break;
            move.end = move.end + direction;
        }
    }
}

void Board::fill_horse_moves(Position position, std::vector<Move>& moves) const
{
    static constexpr Position JumpOffsets[8] = { {1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1} };
    static constexpr Position LegOffsets[8] =  { {0, 1}, {1, 0}, {0, 1},  {-1, 0}, {0, -1}, {1, 0},  {0, -1},  {-1, 0} };

    Move move;
    move.begin = position;

    for (int i = 0; i < 8; i++)
    {
        move.end = position + JumpOffsets[i];

        if (not move.end.is_valid()) continue;

        Position leg_position = position + LegOffsets[i];
        if (pieces[leg_position.get_index()].is_empty())
        {
            try_take(move,moves);
        }
    }
}

void Board::fill_elephant_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;

    for (Position corner : Corners)
    {
        move.end = position + corner * 2;

        if (not move.end.is_valid()) continue;
        if ((move.end.y <= 4) == black_turn) continue;

        Position leg_position = position + corner;
        if (pieces[leg_position.get_index()].is_empty())
        {
            try_take(move,moves);
        }
    }
}

void Board::fill_advisor_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;

    int8_t center_y = black_turn?8:1;

    if (position.x != 4)
    {
        move.end = Position{ 4, center_y };
        try_take(move, moves);
    }
    else
    {
        for (Position corner : Corners)
        {
            move.end = Position{ 4, center_y } + corner;
            try_take(move, moves);
        }
    }
}

void Board::fill_king_moves(Position position, std::vector<Move>& moves) const
{
    Move move;
    move.begin = position;
    move.end = position;
    
    while (true)
    {
        move.end.y += black_turn ? -1 : 1;
        if (not move.end.is_valid()) break;

        Piece piece = pieces[move.end.get_index()];
        if (piece.is_empty()) continue;
        if (piece.equals_ignore_color(Piece::king())) try_take(move, moves);
        break;
    }

    for (Position direction : Directions)
    {
        move.end = position + direction;
        if (move.end.x < 3 || move.end.x > 5) continue;
        if (move.end.y < (black_turn ? 7 : 0)) continue;
        if (move.end.y >= (black_turn ? Height : 3)) continue;

        try_take(move,moves);
    }
}

