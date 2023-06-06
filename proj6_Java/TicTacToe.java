/*
    Public Class TicTacToe
    The program allows two players to play agains each other in tic tac toe
    Possible players combination:
    1. Not So Smart Computer vs. Not So Smart Computer
    2. Smart Computer vs. Smart Computer
    3. Human vs. Human
    4. Not So Smart Computer vs. Human
    5. Human vs. Not So Smart Computer
    6. Smart Computer vs. Human
    7. Human vs. Smart Computer
 */

public class TicTacToe {
    
    public static void main(String [] args){

        Board gameBoard = new Board();
        boolean validUsage = true; 

        Player pl1;
        Player pl2;

        pl1 = new HumanPlayer(true);    // true if it is Player 1
        pl2 = new HumanPlayer(false);   // false if it is Player 2

        if (args.length == 0){
            pl1 = new HumanPlayer(true);
            pl2 = new HumanPlayer(false);
        }
        else if ((args[0].equals("-c")) && (args.length == 1)){
            pl1 = new DumbComputer(true);
            pl2 = new DumbComputer(false);
        }
        else if ((args[0].equals("-c")) && (args[1].equals("-a")) && (args.length == 2)){
            pl1 = new SmartComputer(true);
            pl2 = new SmartComputer(false);
        }
        else if ((args[0].equals("-c")) && (args[1].equals("1")) && (args.length == 2)){
            pl1 = new DumbComputer(true);
            pl2 = new HumanPlayer(false);
        }
        else if ((args[0].equals("-c")) && (args[1].equals("1")) && 
                 (args[2].equals("-a")) && (args.length == 3)){
            pl1 = new SmartComputer(true);
            pl2 = new HumanPlayer(false);
        }
        else if ((args[0].equals("-c")) && (args[1].equals("2")) && (args.length == 2)){
            pl1 = new HumanPlayer(true);
            pl2 = new DumbComputer(false);
        }
        else if ((args[0].equals("-c")) && (args[1].equals("2")) && 
                 (args[2].equals("-a")) && (args.length == 3)){
            pl1 = new HumanPlayer(true);
            pl2 = new SmartComputer(false); 
        }
        else {
            validUsage = false;
            System.out.println("Usage:  java TicTacToe [-c [1|2] [-a]]");
        }

        // if usage is valid, Player 1 and Player 1 takes turns before 
        // there is a draw or a win
        if (validUsage){
            do {
                pl1.move(gameBoard);
                if (gameBoard.isVictory()){
                    gameBoard.printGameBoard();
                    System.out.println("\n" + pl1.getName() + " is the winner! Thank you for playing!");
                    break;
                }
                if (gameBoard.isDraw()){
                    gameBoard.printGameBoard();
                    System.out.println("\nIt is a draw! Thank you for playing!");
                    break;
                }
                pl2.move(gameBoard);
                if  ((gameBoard.isVictory()) || gameBoard.isDraw()){
                    gameBoard.printGameBoard();
                    System.out.println("\n" + pl2.getName() + " is the winner! Thank you for playing!");
                    break;
                }
                if (gameBoard.isDraw()){
                    gameBoard.printGameBoard();
                    System.out.println("\nIt is a draw! Thank you for playing!");
                    break;
                }
            }
            while ((!gameBoard.isVictory()) || (!gameBoard.isDraw()));
        }
      
    }

    
    
}
