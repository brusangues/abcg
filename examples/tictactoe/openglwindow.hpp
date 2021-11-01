#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <vector>
#include <fmt/core.h>

#include "abcg.hpp"

class Game {
  private:
    char player;
    int turn;
    std::vector<std::vector<char> > board;// = {
      // {' ',' ',' '},
      // {' ',' ',' '},
      // {' ',' ',' '}
      // };
    
  public:
    Game() {
      board = {{' ',' ',' '}, {' ',' ',' '}, {' ',' ',' '}};
      player = 'X';
      turn = 0;
    }

    char getPlayer(){
      return player;
    }
    void setPlayer(char p){
      player = p;
    }

    void printBoard(){
      for (auto i{0u}; i < board.size(); i++){
        for (auto j{0u}; j < board.at(i).size(); j++){
          fmt::print("{} ", board.at(i).at(j));
        }
        fmt::print("\n");
      }
    }

    char getPosition(int i, int j){
      return board.at(i).at(j);
    }

    void nextTurn(){
      if(player == 'X'){
        setPlayer('O');
      }
      else{
        setPlayer('X');
      }
      turn++;
    }

    bool play(int i, int j){
      if(board.at(i).at(j) == ' '){
        fmt::print("Valid play at {} {}\n", i, j);
        board.at(i).at(j) = player;
        nextTurn();
        return true;
      }
      else{
        fmt::print("Invalid play at {} {}\n", i, j);
        return false;
      }
    }

};

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override; // override optional but recommended
  void paintGL() override;
  void paintUI() override;

 private:
  Game ttt{Game()};
  std::array<float, 4> m_clearColor{0.906f, 0.910f, 0.918f, 1.0f};
  // 4 position array defining the background color
};

#endif