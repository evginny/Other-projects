/*
    Public HumanPlayer class
    Derived from Player class
*/
 
import java.util.InputMismatchException;
import java.util.Scanner;

public class HumanPlayer extends Player{

    Scanner input;

    public HumanPlayer(){
        super();
        input = new Scanner(System.in);
    }

    public HumanPlayer(boolean f){
        super(f);
        input = new Scanner(System.in);
        if (isFirst){
            name = new String("Player 1");
        }
        else {
            name = new String("Player 2");
        }
    }

    // ask a player what position they want to take, and
    // sets it to the player's symbol (X or O) if it is empty
    // if it is not, repeats the whole process
    public void move(Board board){
        board.printGameBoard();
        int playerChoice = Move();
        while (board.isEmpty(playerChoice) == false){
            System.out.println
            ("The position is occupied. Please choose another position.");
            playerChoice = Move();
        }
        board.setPosition(playerChoice, symbol);
    }

    // asks a player what position they want to take
    // checks if the input is an appropriate type and valid
    // repeats the whole process if it is not
    private int Move(){
        System.out.print(getName() + ", please eneter a move (1-9): ");
        int choice = 0;
        boolean contInp = true;
        do {
            try {
                choice = input.nextInt();
                System.out.println();
                contInp = false;
                while (!isLegal(choice)){
                    System.out.print
                    ("\nInvalid input, please eneter a move (1-9): ");
                    choice = input.nextInt();
                }
            }
            catch (InputMismatchException exc){
                System.out.print
                ("\nInvalid input, please eneter a move (1-9): ");
                input.nextLine();
                contInp = true;
            }
        }
        while(contInp);
        return choice;
    }

    // returns 1 if legal, 0 if not.
    private static boolean isLegal(int opt){
        return ((opt == 1) || (opt == 2) || (opt == 3) ||
                (opt == 4) || (opt == 5) || (opt == 6) || 
                (opt == 7) || (opt == 8) || (opt == 9));
    }
    
}
