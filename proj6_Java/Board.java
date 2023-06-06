/*
    Public Class Board
    Represents a game board for tic tac toe
    with positions
     1 | 2 | 3
     ---------
     4 | 5 | 6
     ---------
     7 | 8 | 9

    Winnig lines:
    1: 1, 2, 3
    2: 4, 5, 6
    3: 7, 8, 9
    4: 1, 5, 9
    5: 3, 5, 7
    6: 1, 4, 7
    7: 2, 5, 8
    8: 3, 6, 9 
 */


public class Board {

    private char [][] board;
    
    public Board(){
        board = new char [3][3];
    }

    // returns false if all positions are not empty
    // and there are not any winning lines
    // returns true otherwise
    public boolean isDraw(){
        char[][] testArr = new char[3][3];
        for (int i = 0; i < testArr.length; i++){
            for (int j = 0; j < testArr[i].length; j++){
                if (testArr[i][j] == board[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

    // returns true if any winning lines were filled with only X or only O
    // returns faslse otherwise
    public boolean isVictory(){
        if ((getCont(1) == getCont(2)) && (getCont(1) == getCont(3)) &&
             (getCont(2) == getCont(3)) &&
             (!isEmpty(1)) && (!isEmpty(2)) && (!isEmpty(3)) ||
             (getCont(4) == getCont(5)) && (getCont(4) == getCont(6)) && 
             (getCont(5) == getCont(6)) &&
             (!isEmpty(4)) && (!isEmpty(5)) && (!isEmpty(6)) ||
             (getCont(7) == getCont(8)) && (getCont(7) == getCont(9)) && 
             (getCont(8) == getCont(9)) &&
             (!isEmpty(7)) && (!isEmpty(8)) && (!isEmpty(9)) ||
             (getCont(1) == getCont(5)) && (getCont(1) == getCont(9)) && 
             (getCont(5) == getCont(9)) &&
             (!isEmpty(1)) && (!isEmpty(5)) && (!isEmpty(9)) ||
             (getCont(3) == getCont(5)) && (getCont(3) == getCont(7)) && 
             (getCont(5) == getCont(7)) &&
             (!isEmpty(3)) && (!isEmpty(5)) && (!isEmpty(7)) ||
             (getCont(1) == getCont(4)) && (getCont(1) == getCont(7)) && 
             (getCont(4) == getCont(7)) &&
             (!isEmpty(1)) && (!isEmpty(4)) && (!isEmpty(7)) ||
             (getCont(2) == getCont(5)) && (getCont(2) == getCont(8)) && 
             (getCont(5) == getCont(8)) &&
             (!isEmpty(2)) && (!isEmpty(5)) && (!isEmpty(8)) ||
             (getCont(3) == getCont(6)) && (getCont(3) == getCont(9)) && 
             (getCont(6) == getCont(9)) &&
             (!isEmpty(3)) && (!isEmpty(6)) && (!isEmpty(9))){
                return true;
             }
             else {
                 return false;
             }
    }

    // returns true if the content of the position is '\0'
    // return false otherwise
    public boolean isEmpty(int choice){
        if (getCont(choice) == '\0'){
            return true;
        }
        return false;
    }

    // returns a content of the position
    public char getCont(int choice){
        char content = '\0';
        switch (choice){
            case 1:
                content = board[0][0];
                break;
            case 2:
                content = board[0][1];
                break;
            case 3:
                content = board[0][2];
                break;
            case 4:
                content = board[1][0];
                break;
            case 5:
                content = board[1][1];
                break;
            case 6:
                content = board[1][2];
                break;
            case 7:
                content = board[2][0];
                break;
            case 8:
                content = board[2][1];
                break;
            case 9:
                content = board[2][2];
                break;
        }
        return content;
    }

    // sets a specified position to X or O
    public void setPosition(int position, char symbol){
        switch (position){
            case 1:
                board[0][0] = symbol;
                break;
            case 2:
                board[0][1] = symbol;
                break;
            case 3:
                board[0][2] = symbol;
                break;
            case 4:
                board[1][0] = symbol;
                break;
            case 5:
                board[1][1] = symbol;
                break;
            case 6:
                board[1][2] = symbol;
                break;
            case 7:
                board[2][0] = symbol;
                break;
            case 8:
                board[2][1] = symbol;
                break;
            case 9:
                board[2][2] = symbol;
                break;
        }
    }

    // prints out the game board with all possible position next to it
    public void printGameBoard(){
        int [][] positions = {{1, 2, 3},
                              {4, 5, 6},
                              {7, 8, 9}};
        
        System.out.println("Game Board:\t\tPositions:\n");
    

        for (int i = 0; i < board.length; i++){
            for (int k = 0; k < board[i].length; k++){
                if (k == board[i].length - 1){
                    if (board[i][k] == '\0'){
                        System.out.print("   ");
                    }
                    else {
                        System.out.print(board[i][k]);
                    }                    
                }                  
                else {
                    if (board[i][k] == '\0'){
                        System.out.print("  | ");
                    }
                    else {
                       System.out.print(board[i][k] + " | "); 
                    }
                    
                }
            }
        
            System.out.print("\t\t");

            for (int j = 0; j < positions[i].length; j++){
                if (j == positions[i].length - 1){
                    System.out.print(positions[i][j]);
                }                  
                else {
                    System.out.print(positions[i][j] + " | ");
                }
            }
            System.out.println();
            if (i != positions.length - 1){
                System.out.println("---------\t\t---------");
            }
        }
    }
   
    // returns the number of the winning line - of a Player
    // blocking line - for an opponent (opponent's symbol differs
    // from player's symbol)
    // returns 0 if there are not any winning or blocking lines
    public int getLine(char playerSymbol, boolean isOpponent){
        char symbol;
        if (isOpponent){
            if (playerSymbol == 'O'){
            symbol = 'X';
            }
            else {
                symbol = 'O';
            }
        }
        else {
            symbol = playerSymbol;
        }
        
        if (((getCont(1) == symbol) && getCont(2) == symbol) && (isEmpty(3)) ||
            ((getCont(1) == symbol) && getCont(3) == symbol) && (isEmpty(2)) ||
            ((getCont(2) == symbol) && getCont(3) == symbol) && (isEmpty(1))){
                return 1;
            }
        else if (((getCont(4) == symbol) && getCont(5) == symbol) && (isEmpty(6)) ||
                 ((getCont(4) == symbol) && getCont(6) == symbol) && (isEmpty(5)) ||
                 ((getCont(5) == symbol) && getCont(6) == symbol) && (isEmpty(4))){
                    return 2;
        }
        else if (((getCont(7) == symbol) && getCont(8) == symbol) && (isEmpty(9)) ||
                 ((getCont(7) == symbol) && getCont(9) == symbol) && (isEmpty(8)) ||
                 ((getCont(8) == symbol) && getCont(9) == symbol) && (isEmpty(7))){
                    return 3;
        }
        else if (((getCont(1) == symbol) && getCont(5) == symbol) && (isEmpty(9)) ||
                 ((getCont(1) == symbol) && getCont(9) == symbol) && (isEmpty(5)) ||
                 ((getCont(5) == symbol) && getCont(9) == symbol) && (isEmpty(1))){
                    return 4;
        }
        else if (((getCont(3) == symbol) && getCont(5) == symbol) && (isEmpty(7)) ||
                 ((getCont(3) == symbol) && getCont(7) == symbol) && (isEmpty(5)) ||
                 ((getCont(5) == symbol) && getCont(7) == symbol) && (isEmpty(3))){
                    return 5;
        }
        else if (((getCont(1) == symbol) && getCont(4) == symbol) && (isEmpty(7)) ||
                 ((getCont(1) == symbol) && getCont(7) == symbol) && (isEmpty(4)) ||
                 ((getCont(4) == symbol) && getCont(7) == symbol) && (isEmpty(1))){
                    return 6;
        }
        else if (((getCont(2) == symbol) && getCont(5) == symbol) && (isEmpty(8)) ||
                 ((getCont(2) == symbol) && getCont(8) == symbol) && (isEmpty(5)) ||
                 ((getCont(5) == symbol) && getCont(8) == symbol) && (isEmpty(2))){
                    return 7;
        }
        else if (((getCont(3) == symbol) && getCont(6) == symbol) && (isEmpty(9)) ||
                 ((getCont(3) == symbol) && getCont(9) == symbol) && (isEmpty(6)) ||
                 ((getCont(6) == symbol) && getCont(9) == symbol) && (isEmpty(3))){
                    return 8;
        }
        else {
            return 0;
        }
    }

    // returns a possible empty position which will allow a player to win
    // or to block the opponent's winning line
    // returts 0 if there are not any possible position to win or block
    public int blockOrWin(int line){
        switch (line){
            case 1:
                if (isEmpty(1)){
                    return 1;
                }
                else if (isEmpty(2)){
                    return 2;
                }
                else if (isEmpty(3)){
                    return 3;
                }
                break;
            case 2:
                if (isEmpty(4)){
                    return 4;
                }
                else if (isEmpty(5)){
                    return 5;
                }
                else if (isEmpty(6)){
                    return 6;
                }
                break;
            case 3:
                if (isEmpty(7)){
                    return 7;
                }
                else if (isEmpty(8)){
                    return 8;
                }
                else if (isEmpty(9)){
                    return 9;
                }
                break;
            case 4:
                if (isEmpty(1)){
                    return 1;
                }
                else if (isEmpty(5)){
                    return 5;
                }
                else if (isEmpty(9)){
                    return 9;
                }
                break;
            case 5:
                if (isEmpty(3)){
                    return 3;
                }
                else if (isEmpty(5)){
                    return 5;
                }
                else if (isEmpty(7)){
                    return 7;
                }
                break;
            case 6:
                if (isEmpty(1)){
                    return 1;
                }
                else if (isEmpty(4)){
                    return 4;
                }
                else if (isEmpty(7)){
                    return 7;
                }
                break;
            case 7:
                if (isEmpty(2)){
                    return 2;
                }
                else if (isEmpty(5)){
                    return 5;
                }
                else if (isEmpty(8)){
                    return 8;
                }
                break;
            case 8:
                if (isEmpty(3)){
                    return 3;
                }
                else if (isEmpty(6)){
                    return 6;
                }
                else if (isEmpty(9)){
                    return 9;
                }
                break;
        }
        return 0;
    }
    
}
