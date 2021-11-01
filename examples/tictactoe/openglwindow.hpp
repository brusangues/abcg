#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <vector>
#include <fmt/core.h>

#include "abcg.hpp"

class Game {
  private:
    char player;
    char winner;
    int turn;
    std::vector<std::vector<char> > board;

  public:
    Game() {
      player = 'X';
      winner = ' ';
      turn = 0;
      board = {{' ',' ',' '}, {' ',' ',' '}, {' ',' ',' '}};
    }

    char getPlayer(){
      return player;
    }
    void setPlayer(char p){
      player = p;
    }
    char getWinner(){
      return winner;
    }
    int getTurn(){
      return turn;
    }
    char getPosition(int i, int j){
      return board.at(i).at(j);
    }
    void setPosition(int i, int j){
      board.at(i).at(j) = getPlayer();
    }

    void printBoard(){
      for (auto i{0u}; i < board.size(); i++){
        for (auto j{0u}; j < board.at(i).size(); j++){
          fmt::print("{} ", board.at(i).at(j));
        }
        fmt::print("\n");
      }
    }

    bool checkWinner(){
      int win = 0;
      // Rows check
      for (auto i{0u}; i < board.size(); i++){
        for (auto j{0u}; j < board.at(i).size(); j++){
          if(getPosition(i, j) == getPlayer())
            win++;
        }
        if(win == 3){
          winner = getPlayer();
          fmt::print("WINNER:{}\n", getWinner());
          return true;
        }
        win = 0;
      }

      // Columns check
      for (auto i{0u}; i < board.size(); i++){
        for (auto j{0u}; j < board.at(i).size(); j++){
          if(getPosition(j, i) == getPlayer())
            win++;
        }
        if(win == 3){
          winner = getPlayer();
          fmt::print("WINNER:{}\n", getWinner());
          return true;
        }
        win = 0;
      }

      // First diagonal check
      for (auto i{0u}; i < board.size(); i++){
        if(getPosition(i, i) == getPlayer())
            win++;
      }
      if(win == 3){
        winner = getPlayer();
        fmt::print("WINNER:{}\n", getWinner());
        return true;
      }
      win = 0;

      // Second diagonal check
      for (auto i{0u}; i < board.size(); i++){
        if(getPosition(i, 2-i) == getPlayer())
            win++;
      }
      if(win == 3){
        winner = getPlayer();
        fmt::print("WINNER:{}\n", getWinner());
        return true;
      }
      win = 0;

      return false;
    }

    bool checkDraw(){
      if(turn>9){
        winner = 'D';
        fmt::print("DRAW\n");
        return true;
      }
      return false;
    }

    void nextTurn(){
      if( checkWinner() || checkDraw() ){
        return;
      }
      if(player == 'X'){
        setPlayer('O');
      }
      else{
        setPlayer('X');
      }
      turn++;
    }

    bool play(int i, int j){
      if(getPosition(i,j) == ' '){
        fmt::print("Valid play at {} {}\n", i, j);
        if( !(checkWinner() || checkDraw()) ){
          setPosition(i,j);
          nextTurn();
        }
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