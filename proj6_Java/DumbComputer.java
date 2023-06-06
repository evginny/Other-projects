/*
    Public DumbComputer class
    Derived from Player class
*/

import java.util.Random;

public class DumbComputer extends Player {

    private Random rand;
    
    public DumbComputer(){
        super();
        rand = new Random();
    }

    public DumbComputer(boolean f){
        super(f);
        if (isFirst){
            name = new String("Player 1 (computer)");
        }
        else {
            name = new String("Player 2 (computer)");
        }
        rand = new Random();
    }

    // allows a player to make an appropriate move
    public void move(Board board){
        board.printGameBoard();
        // checks if there are any winning lines
        int winLine = board.getLine(symbol, false);
        // if there is a winning line takes a position so the player will win
        if (winLine != 0){
            int wSpot = board.blockOrWin(winLine);
            // if there is a possible empty winning position
            if (wSpot != 0){
                board.setPosition(wSpot, symbol);
                System.out.println(name + " chooses position " + wSpot + "\n");
                return;
            }
        }
        // checks if there are any opponent's winning lines that need to 
        // be blockes
        int blockLine = board.getLine(symbol, true);
        // if there is at least one opponent's winning line
        // Player moves to the position that will block it
        if (blockLine != 0){
            int bSpot = board.blockOrWin(blockLine);
            //if there are any empty blocking positions
            if (bSpot != 0){
                board.setPosition(bSpot, symbol);
                System.out.println(name + " chooses position " + bSpot + "\n");
                return;
            }
        }
        // if the middle position is free, the player takes it
        if (board.isEmpty(5)){
            board.setPosition(5, symbol);
            System.out.println(name + " chooses position 5 \n");
            return;
        }
        // generates a random  possition, and if it is free, takes it 
        int randSpot = rand.nextInt(9) + 1;
        while (!board.isEmpty(randSpot)){
            randSpot = rand.nextInt(9) + 1;
        }
        board.setPosition(randSpot, symbol);
        System.out.println(name + " chooses position " + randSpot + "\n");
    }

}
