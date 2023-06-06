/*
    Abstract class Player
    Parent to HumanPlayer Class, DumbComputer Class, and SmartComputer Class
 */

public abstract class Player {

    protected boolean isFirst;  // if it is Player 1
    protected String name;      // can be Player 1, Player 2
                                // Player 1 (computer), Player 2 (computer)
    protected char symbol;      // X or Y

    public abstract void move(Board board); // allows player to take a turn

    public Player(){

    }

    public Player(boolean first){
        if (first){
            isFirst = true;
            symbol = 'X';
        }
        else {
            isFirst = false;
            symbol = 'O';
        }
    }

    public String getName(){
        return name;
    }    
}


