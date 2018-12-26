#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>


namespace chess_namespace
{
  using CChessSquare = std::pair<int, int>;

  static CChessSquare
  operator+ (const CChessSquare &square, const std::pair<int, int> &movement)
  {
    return {
        square.first + movement.first,
        square.second + movement.second
    };
  }

  static CChessSquare
  operator* (int scale, const CChessSquare &square)
  {
    return {
        square.first * scale,
        square.second * scale
    };
  }

  static CChessSquare
  operator* (const CChessSquare &square, int scale)
  {
    return scale * square;
  }

  static bool
  operator== (const CChessSquare &lhs, const CChessSquare &rhs)
  {
    return lhs.first == rhs.first && lhs.second == rhs.second;
  }

  static bool
  operator!= (const CChessSquare &lhs, const CChessSquare &rhs)
  {
    return !(lhs == rhs);
  }

  static bool
  IsPositionCorrect (const CChessSquare &square)
  {
    return square.first >= 0 &&
           square.first < 8 &&
           square.second >= 0 &&
           square.second < 8;
  }

  static const
  std::pair<int, int> movements[]{
      {0,  1},
      {0,  -1},
      {1,  0},
      {-1, 0},
      {1,  1},
      {1,  -1},
      {-1, -1},
      {-1, 1}
  };

  static bool CanKingAttack (const CChessSquare &king_position,
                             const CChessSquare &enemy_position)
  {
    return abs(king_position.first - enemy_position.first) <= 1 &&
           abs(king_position.second - enemy_position.second) <= 1;
  }

  static bool CanQueenAttack (const CChessSquare &queen_position,
                              const CChessSquare &enemy_position)
  {
    return abs(queen_position.first - enemy_position.first) == 0 ||
           abs(queen_position.second - enemy_position.second) == 0 ||
           (abs(queen_position.first - enemy_position.first) ==
            abs(queen_position.second - enemy_position.second));
  }

  struct CChessState
  {
    CChessState () :
        white_queen({-1, -1}),
        black_king({-1, -1}),
        is_white_going(true),
        white_king({-1, -1})
    {}

    CChessState (CChessSquare wq_square, CChessSquare bk_square,
                 bool next_step_, CChessSquare wk_square) :
        white_queen(std::move(wq_square)),
        black_king(std::move(bk_square)),
        is_white_going(next_step_),
        white_king(std::move(wk_square))
    {}

    bool MoveQueenForCheck () const
    {
      if (white_queen == black_king)
      {
        return false;
      }
      for (auto movement: movements)
      {
        for (int scale = 1; scale <= 8; ++scale)
        {
          CChessSquare next_position(std::move(white_queen + (scale * movement)));
          if (!IsPositionCorrect(next_position))
          {
            continue;
          }
          if (next_position == white_king)
          {
            break;
          }
          if (next_position == black_king)
          {
            return true;
          }
        }
      }
      return false;
    }

    bool HandleMate () const
    {
      if (!MoveQueenForCheck())
      {
        return false;
      }
      for (auto movement: movements)
      {
        CChessSquare next_position(std::move(black_king + movement));
        if (!IsPositionCorrect(next_position))
        {
          continue;
        }
        CChessState next_state(white_queen, next_position, false, white_king);
        if (!next_state.MoveQueenForCheck() && !CanKingAttack(white_king, next_position))
        {
          return false;
        }
      }
      return true;
    }

    bool HandleCheck () const
    {
      return MoveQueenForCheck() || CanKingAttack(white_king, black_king);
    }

    bool is_white_going = true; // white - true, black - false
    CChessSquare white_queen;
    CChessSquare white_king = {2, 2}; // [C][3]
    CChessSquare black_king;
  };

  struct CChessStateHash
  {
    int operator() (const CChessState &state) const
    {
      // encoding: N_S|WQ_X|WQ_Y|WK_X|WK_Y|BK_X|BK_Y| = 19 bits
      // 0b000 ... 0b111 <-> 0 ... 7
      return state.is_white_going << 18 |
             state.white_queen.first << 15 |
             state.white_queen.second << 12 |
             state.white_king.first << 9 |
             state.white_king.second << 6 |
             state.black_king.first << 3 |
             state.black_king.second;
    }

    static CChessState decode (int hash)
    {
      CChessState state;
      state.black_king.second = hash & 7;
      state.black_king.first = (hash >> 3) & 7;
      state.white_king.second = (hash >> 6) & 7;
      state.white_king.first = (hash >> 9) & 7;
      state.white_queen.second = (hash >> 12) & 7;
      state.white_queen.first = (hash >> 15) & 7;
      state.is_white_going = static_cast<bool>(hash >> 18);
      return state;
    }
  };

  static bool operator== (const CChessState &lhs, const CChessState &rhs)
  {
    CChessStateHash hash;
    return hash(lhs) == hash(rhs);
  }

  class CChessGameGraph
  {
  public:
    CChessGameGraph () = default;

    explicit CChessGameGraph (const CChessState &init_state)
    {
      Rebuild(init_state);
    }

    void Rebuild (const CChessState &reinit_state)
    {
      start_state = reinit_state;
      for (int white_queen_x = 0; white_queen_x < 8; ++white_queen_x)
      {
        for (int white_queen_y = 0; white_queen_y < 8; ++white_queen_y)
        {
          for (int black_king_x = 0; black_king_x < 8; ++black_king_x)
          {
            for (int black_king_y = 0; black_king_y < 8; ++black_king_y)
            {
              CChessState state(
                  {white_queen_x, white_queen_y},
                  {black_king_x, black_king_y},
                  start_state.is_white_going,
                  start_state.white_king
              );
              SetStepsAmt(state, UNREACHABLE);
              state.is_white_going = !state.is_white_going;
              SetStepsAmt(state, UNREACHABLE);
            }
          }
        }
      }
    }

    int GetStepsAmt (const CChessState &state) const
    {
      return state_steps.find(state)->second;
    }

    void SetStepsAmt (const CChessState &state, int steps_amt)
    {
      state_steps[state] = steps_amt;
    }

    const CChessState& GetStartState () const
    {
      return start_state;
    }

    void ComputeMates (std::vector<CChessState> &checkmates)
    {
      checkmates.clear();
      for (int i = 0; i < 5; i++)
      {
        for (int x_axis = 0; x_axis < 8; ++x_axis)
        {
          for (int y_axis = 0; y_axis < 8; ++y_axis)
          {
            if (std::make_pair(x_axis, y_axis) == std::make_pair(i, 0))
            {
              continue;
            }
            if (std::make_pair(x_axis, y_axis) == std::make_pair(0, i))
            {
              continue;
            }
            if (std::make_pair(x_axis, y_axis) == start_state.white_king)
            {
              continue;
            }
            CChessState desk1(
                {x_axis, y_axis},
                {0, i},
                !start_state.is_white_going,
                start_state.white_king
            );
            if (desk1.HandleMate())
            {
              SetStepsAmt(desk1, 0);
              checkmates.emplace_back(std::move(desk1));
            }
            if (i > 0)
            {
              CChessState desk2(
                  {x_axis, y_axis},
                  {i, 0},
                  !start_state.is_white_going,
                  start_state.white_king
              );
              if (desk2.HandleMate())
              {
                SetStepsAmt(desk2, 0);
                checkmates.emplace_back(std::move(desk2));
              }
            }
          }
        }
      }
    }

  private:
    CChessState start_state;
    std::unordered_map<CChessState, int, CChessStateHash> state_steps{};

  public:
    static const int UNREACHABLE = std::numeric_limits<int>::max();
  };

  class CChessGameSolver
  {
  public:
    CChessGameSolver () = default;

    int operator() (CChessSquare wq_square, CChessSquare bk_square,
                    bool is_white_going, CChessSquare wk_square)
    {
      CChessState start_state(wq_square, bk_square, is_white_going, wk_square);
      game_graph.Rebuild(start_state);
      while (!states.empty())
      {
        states.pop();
      }
      std::vector<CChessState> checkmates;
      game_graph.ComputeMates(checkmates);
      for (const CChessState &mate_state: checkmates)
      {
        tryWhiteQueenToMove(mate_state);
      }
      auto game_is_over = [&] () -> bool
      {
        bool unreachable = game_graph.GetStepsAmt(start_state) ==
            CChessGameGraph::UNREACHABLE;
        return states.empty() || !unreachable;
      };
      while (!game_is_over())
      {
        CChessState state(states.front());
        states.pop();
        if (state.is_white_going)
        {
          int white_steps_amt = std::numeric_limits<int>::max();
          if (state.HandleCheck())
          {
            continue;
          }
          for (auto movement : movements)
          {
            for (int scale = 1; scale <= 8; ++scale)
            {
              CChessSquare position(std::move(state.white_queen + (scale * movement)));
              if (!IsPositionCorrect(position))
              {
                continue;
              }
              if (position == state.black_king || position == start_state.white_king)
              {
                break;
              }
              CChessState new_state(
                  position,
                  state.black_king,
                  !state.is_white_going,
                  game_graph.GetStartState().white_king
              );
              if (CanKingAttack(state.black_king, position) &&
                  !CanKingAttack(start_state.white_king, position))
              {
                continue;
              }
              white_steps_amt = std::min(
                  white_steps_amt,
                  game_graph.GetStepsAmt(new_state)
              );
            }
          }
          if (game_graph.GetStepsAmt(state) > white_steps_amt * 1LL + 1)
          {
            game_graph.SetStepsAmt(state, white_steps_amt + 1);
            tryBlackKingToMove(state);
          }
        }
        else
        {
          int black_steps_amt = std::numeric_limits<int>::min();
          for (auto movement: movements)
          {
            CChessSquare position(std::move(state.black_king + movement));
            CChessState new_state(
                state.white_queen,
                position,
                !state.is_white_going,
                game_graph.GetStartState().white_king
            );
            if (!IsPositionCorrect(position))
            {
              continue;
            }
            if (position == state.white_queen || position == state.white_king)
            {
              continue;
            }
            if (new_state.HandleCheck())
            {
              continue;
            }

            black_steps_amt = std::max(
                black_steps_amt,
                game_graph.GetStepsAmt(new_state)
            );
          }
          if (black_steps_amt != CChessGameGraph::UNREACHABLE)
          {
            if (game_graph.GetStepsAmt(state) == CChessGameGraph::UNREACHABLE ||
                game_graph.GetStepsAmt(state) < black_steps_amt * 1LL + 1)
            {
              game_graph.SetStepsAmt(state, black_steps_amt + 1);
              tryWhiteQueenToMove(state);
            }
          }
        }
      }
      return game_graph.GetStepsAmt(start_state) != CChessGameGraph::UNREACHABLE
             ? game_graph.GetStepsAmt(start_state)  // MIN STEPS FOR CHECK
             : -1;                                  // IMPOSSIBLE
    }

  private:
    bool tryWhiteQueenToMove (const CChessState &state)
    {
      if (state.is_white_going)
      {
        return false;
      }
      for (auto movement: movements)
      {
        for (int scale = 1; scale <= 8; ++scale)
        {
          CChessSquare next_square(std::move(state.white_queen + (scale * movement)));
          if (!IsPositionCorrect(next_square))
          {
            continue;
          }
          if (next_square == state.white_king || next_square == state.black_king)
          {
            break;
          }
          CChessState next_state(
              next_square,
              state.black_king,
              !state.is_white_going,
              game_graph.GetStartState().white_king
          );
          if (next_state.HandleCheck())
          {
            continue;
          }
          if (game_graph.GetStepsAmt(next_state) != CChessGameGraph::UNREACHABLE &&
              game_graph.GetStepsAmt(next_state) <= game_graph.GetStepsAmt(state) * 1LL + 1)
          {
            continue;
          }
          states.push(next_state);
        }
      }
      return true;
    }

    bool tryBlackKingToMove (const CChessState &state)
    {
      if (!state.is_white_going)
      {
        return false;
      }
      bool added = false;
      for (auto movement: movements)
      {
        CChessSquare next_square = state.black_king + movement;
        if (!IsPositionCorrect(next_square))
        {
          continue;
        }
        bool white_attack = (
            next_square == state.white_king ||
            next_square == state.white_queen
        );
        bool giveaway = CanKingAttack(state.white_king, next_square);
        if (white_attack || giveaway)
        {
          continue;
        }
        CChessState next_state(
            state.white_queen,
            next_square,
            !state.is_white_going,
            game_graph.GetStartState().white_king
        );
        if (game_graph.GetStepsAmt(next_state) != CChessGameGraph::UNREACHABLE &&
            game_graph.GetStepsAmt(next_state) >= game_graph.GetStepsAmt(state) + 1)
        {
          continue;
        }
        states.push(next_state);
        added = true;
      }
      return added;
    }

  private:
    std::queue<CChessState> states{};
    CChessGameGraph game_graph{};
  };

  extern CChessSquare
  ParsePosition (const std::string &);
};

using chess_namespace::CChessSquare;
using chess_namespace::CChessGameSolver;
using chess_namespace::ParsePosition;


int main ()
{
  std::string white_queen, black_king;
  std::cin >> white_queen >> black_king;
  auto wq_square = ParsePosition(white_queen);
  auto bk_square = ParsePosition(black_king);
  CChessGameSolver game_solver{};
  int game_result = game_solver(wq_square, bk_square, true, {2, 2});
  if (game_result == -1)
  {
    std::cout << "IMPOSSIBLE" << std::endl;
  }
  else
  {
    std::cout << game_result << std::endl;
  }
  return 0;
}


CChessSquare chess_namespace::
ParsePosition (const std::string &position_txt)
{
  int x_axis = static_cast<int>(position_txt[0] - 'a');
  int y_axis = static_cast<int>(position_txt[1] - '1');
  return {x_axis, y_axis};
}
