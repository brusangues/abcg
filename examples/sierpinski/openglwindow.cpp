#include <fmt/core.h>

#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>
#include <string>

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

    void restart(){
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
      if(turn>=8){
        winner = 'D';
        fmt::print("DRAW\n");
        return true;
      }
      return false;
    }

    bool checkEnd(){
      return checkWinner() || checkDraw();
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
      if(getWinner()==' '){
        if(getPosition(i,j) == ' '){
          fmt::print("Valid play: {} at {},{}\n", getPlayer(), i, j);
          setPosition(i,j);
          checkEnd();
          nextTurn();
          return true;
        }
        else{
          fmt::print("Invalid play: {} at {},{}\n", getPlayer(), i, j);
          return false;
        }
      }
      else{
        fmt::print("Game ended: {}\n", getWinner());
        return false;
      }
    }

};

void OpenGLWindow::initializeGL() {
  // called only once
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", 
             windowSettings.width, windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Called once per frame update

  // Set the clear color - used to clear the window every frame. RGBA
  abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
                     m_clearColor[3]);
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT); // uses constant
}

void OpenGLWindow::paintUI() {
  // Called every time paintGL is called

  // Our own ImGui widgets go below
  {
    // If this is the first frame, set initial position of our window
    static bool firstTime{true};
    static Game ttt{Game()};
    if (firstTime) {
      ImGui::SetNextWindowPos(ImVec2(0,0));
      ImGui::SetNextWindowSize(ImVec2(190,300));
      firstTime = false;
      ttt.printBoard();
    }

    // Window begin
    ImGui::Begin("Tit-Tac-Toe!", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );

    // Turn indicator
    char win = ttt.getWinner();
    if(win != ' '){
      if(win != 'D')
        ImGui::Text("%c is the winner!", ttt.getWinner());
      else
        ImGui::Text("Draw!");
    }
    else{
      ImGui::Text("%c's turn.", ttt.getPlayer());
    }
    ImGui::Text("Turn count %d", ttt.getTurn());

    // 100x50 button
    if (ImGui::Button("Print board", ImVec2(-1, 0))) {
      fmt::print("Board:\n");
      ttt.printBoard();
    }
    
    // Game Loop
    for(auto i{0u}; i<3; i++){
      ImGui::Columns(3);

      for(auto j{0u}; j<3; j++){
        ImGui::SetColumnWidth(j,62);
        char pos = ttt.getPosition(i, j);
        std::string label = fmt::format("{}##{}{}", pos, i, j);
        bool press = ImGui::Button(label.c_str(), ImVec2(50, 50));
        if( press ) ttt.play(i,j);

        ImGui::NextColumn();
      }
      
      ImGui::Separator();
    }

    ImGui::Columns(1);

    // Restart
    if(ImGui::Button("Restart Game", ImVec2(-1, 0))){
      ttt.restart();
    }

    // Window end
    ImGui::End();
  }
}