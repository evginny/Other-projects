/*
    Public SmartComputer class
    Derived from Player class
    Implemets all strategies that depends on the opponent's move
    in order to win or get a draw
*/

import java.util.Random;

public class SmartComputer extends Player {
    private Random rand;
    private boolean isFirstMove, isSecondMove, isThirdMove;
    // if opponent's symbol from the first move is next to 
    // the computer's symbol from the first move
    private boolean isNextTo = true;
    // a position in the corner - 1, 3, 7, and 9   
    private int corner;
    private int position;


    public SmartComputer(){
        super();
        rand = new Random();
        isFirstMove = true;
        isSecondMove = true;
        isThirdMove = true;
    }

    public SmartComputer(boolean f){
        super(f);
        if (isFirst){
            name = new String("Player 1 (computer)");
        }
        else {
            name = new String("Player 2 (computer)");
        }
        rand = new Random();
        isFirstMove = true;
        isSecondMove = true;
        isThirdMove = true;
    }
    
    // allows a computer to take an appropriate move depending on the
    // opponent's first move and the number of computer's move 
    public void move(Board board){
        int winLine = 0;
        board.printGameBoard();
        // if a computer is Player 1 and it is its first move
        if (isFirstMove && isFirst){
            // takes random position in the corner
            takeCorner(board);
            isFirstMove = false;
            System.out.println(name + " chooses position " + corner + "\n");
            return;
        }
        // if a computer is Player 2, this is its first move, 
        // and the center position is empty
        // sets a center position (5) to O
        if (isFirstMove && (!isFirst) && (board.isEmpty(5))){
            board.setPosition(5, symbol);
            position = 5;
            isFirstMove = false;
            System.out.println(name + " chooses position " + position + "\n");
            return;
        }
        // if a computer is Player 2, rhis is its first move, 
        // and the center position is already occupied,
        // sets O to random corner
        if (isFirstMove && (!isFirst) && (!board.isEmpty(5))){
            takeCorner(board);

            isFirstMove = false;
            System.out.println(name + " chooses position " + corner + "\n");
            return;
        }
        // if a computer is Player 1, this is its second move,
        // and a center position is empty,
        // allows a computer to make a move
        if (isSecondMove && (board.isEmpty(5)) && isFirst){
            secondMove(board);
            isSecondMove = false;
            System.out.println(name + " chooses position " + position + "\n");
            return;
        }
        // if a computer is Player 1, this is its second move, 
        // and a center possition is already occupied,
        // sets X to the opposite corner from the first move
        // skips the process of the third move for the next move
        if (isSecondMove && (!board.isEmpty(5)) && isFirst){
            if (corner == 7){
                board.setPosition(3, symbol);
                position = 3;
            }
            else if (corner == 1){
                board.setPosition(9, symbol);
                position = 9;
            }
            else if (corner == 3){
                board.setPosition(7, symbol);
                position = 7;
            }
            else if (corner == 9){
                board.setPosition(1, symbol);
                position = 1;
            }
            isSecondMove = false;
            isThirdMove = false;
            System.out.println(name + " chooses position " + position + "\n");
                return;
        }
        // if there is at least one winning line,
        // skips the process of the third move
        winLine = board.getLine(symbol, false);
        if (winLine != 0){
            isThirdMove = false;
        }
        // depending on the corner position taken by computer 
        // in the first move, and on the specified position taken by opponent
        // in the first move, skips the process of the third move
        if (((corner == 7) && (!board.isEmpty(6))) ||
            ((corner == 1) && (!board.isEmpty(6))) ||
            ((corner == 3) && (!board.isEmpty(4))) ||
            ((corner == 9) && (!board.isEmpty(4))) ||
            ((corner == 7) && (!board.isEmpty(2))) ||
            ((corner == 1) && (!board.isEmpty(8))) ||
            ((corner == 3) && (!board.isEmpty(8))) ||
            ((corner == 7) && (!board.isEmpty(2)))){
                    isThirdMove = false;
        }
        // if a computer is Player 1, it is its third move,
        // and the center position is empty,
        // allows a computer make an appropriate move
        if (isThirdMove && (board.isEmpty(5)) && isFirst){
            thirdMove(board);
            isThirdMove = false;
            System.out.println(name + " chooses position " + position + "\n");
            return;
        }
        // if there are any winning lines,
        // allows a computr to take an appropriate move to win
        winLine = board.getLine(symbol, false);
        if (winLine != 0){
            position = board.blockOrWin(winLine);
            if (position != 0){
                board.setPosition(position, symbol);
                System.out.println(name + " chooses position " + position + "\n");
                return;
            }
        }
        // if there are any opponent's winning lines that need to be blocked,
        // allows a computer to make an appropriate move to block
        int blockLine = board.getLine(symbol, true);
        if (blockLine != 0){
            position = board.blockOrWin(blockLine);
            if (position != 0){
                board.setPosition(position, symbol);
                System.out.println(name + " chooses position " + position + "\n");
                return;
            }
        }
        // if three first moved passed, and there is no any winning, or 
        // blocking lines, generates a random position and sets it 
        // to the computer symbol if it is empty
        int randSpot = rand.nextInt(9) + 1;
        while (!board.isEmpty(randSpot)){
            randSpot = rand.nextInt(9) + 1;
        }
        board.setPosition(randSpot, symbol);
        System.out.println(name + " chooses position " + randSpot + "\n");
    }

    // generates a random corner position,
    // and sets computer's symbol to it
    private void takeCorner(Board board){
        int randCorner = rand.nextInt(4) + 1;
        switch (randCorner){
            case 1:
                board.setPosition(7, symbol);
                corner = 7;
                break;
            case 2:
                board.setPosition(1, symbol);
                corner = 1;
                break;
            case 3:
                board.setPosition(3, symbol);
                corner = 3;
                break;
            case 4:
                board.setPosition(9, symbol);
                corner = 9;
                break;
        }
    }

    // allows a computer make an appropriate second move
    // depending on the opponent's position from their first move,
    // and on the computer's corner position from the first move
    private void secondMove(Board board){
        int randSecond = rand.nextInt(2) + 1;
        if (corner == 7){
            if (!board.isEmpty(8)){
                board.setPosition(1, symbol);
                position = 1;
            }
            else if ((!board.isEmpty(9)) || (!board.isEmpty(6))){
                board.setPosition(1, symbol);
                position = 1;
                isNextTo = false;
            }
            else if (!board.isEmpty(4)){
                board.setPosition(9, symbol);
                position = 9;
            }
            else if ((!board.isEmpty(1)) || (!board.isEmpty(2))){
                board.setPosition(9, symbol);
                position = 9;
                isNextTo = false;
            }
            else if (!board.isEmpty(3)){
                isNextTo = false;
                if (randSecond == 1){
                    board.setPosition(1, symbol);
                    position = 1;
                }
                else if (randSecond == 2){
                    board.setPosition(9, symbol);
                    position = 9;
                }
            }
        }
        else if (corner == 1){
            if (!board.isEmpty(2)){
                board.setPosition(7, symbol);
                position = 7;
            }
            else if ((!board.isEmpty(3)) || (!board.isEmpty(6))){
                board.setPosition(7, symbol);
                position = 7;
                isNextTo = false;
            }
            else if (!board.isEmpty(4)){
                board.setPosition(3, symbol);
                position = 3;
            }
            else if (!board.isEmpty(7) || !board.isEmpty(8)){
                board.setPosition(3, symbol);
                position = 3;
                isNextTo = false;
            }
            else if (!board.isEmpty(9)){
                isNextTo = false;
                if (randSecond == 1){
                    board.setPosition(3, symbol);
                    position = 3;
                }
                else if (randSecond == 2){
                    board.setPosition(7, symbol);
                    position = 7;
                }
            }
        }
        else if (corner == 3){
            if (!board.isEmpty(2)){
                board.setPosition(9, symbol);
                position = 9;
            }
            else if ((!board.isEmpty(1)) || (!board.isEmpty(4))){
                board.setPosition(9, symbol);
                position = 9;
                isNextTo = false;
            }
            else if (!board.isEmpty(6)){
                board.setPosition(1, symbol);
                position = 1;
            }
            else if (!board.isEmpty(9) || !board.isEmpty(8)){
                board.setPosition(1, symbol);
                position = 1;
                isNextTo = false;
            }
            else if (!board.isEmpty(7)){
                isNextTo = false;
                if (randSecond == 1){
                    board.setPosition(1, symbol);
                    position = 1;
                }
                else if (randSecond == 2){
                    board.setPosition(9, symbol);
                    position = 9;
                }
            }
        }
        else if (corner == 9){
            if (!board.isEmpty(8)){
                board.setPosition(3, symbol);
                position = 3;
            }
            else if ((!board.isEmpty(7)) || (!board.isEmpty(4))){
                board.setPosition(3, symbol);
                position = 3;
                isNextTo = false;
            }
            else if (!board.isEmpty(6)){
                board.setPosition(7, symbol);
                position = 7;
            }
            else if (!board.isEmpty(3) || !board.isEmpty(2)){
                board.setPosition(7, symbol);
                position = 7;
                isNextTo = false;
            }
            else if (!board.isEmpty(1)){
                isNextTo = false;
                if (randSecond == 1){
                    board.setPosition(3, symbol);
                    position = 3;
                }
                else if (randSecond == 2){
                    board.setPosition(7, symbol);
                    position = 7;
                }
            }
        }
    }

    // allows a computer make an appropriate third move (if needed)
    // depending on the opponent's position from their first move,
    // and on the computer's corner position from the first move
    private void thirdMove(Board board){
    // generates 2 random number because sometimes, depending on the 
    // opponents position from the first move, computer can make two 
    // appropriate moves for the third move 
    // 1 - center position
    // 2 - another specific position
    int thirdMove = rand.nextInt(2) + 1;
    // if the opponent took a position from their first move next to
    // the computer position from the first move, and the generated 
    // number is 1, sets the computer's symbol to the center position
    if ((thirdMove == 1) && (isNextTo)){
        if (((corner == 7) && (!board.isEmpty(8))) ||
            ((corner == 1) && (!board.isEmpty(2))) ||
            ((corner == 3) && (!board.isEmpty(2))) ||
            ((corner == 9) && (!board.isEmpty(8))) ||
            ((corner == 7) && (!board.isEmpty(4))) ||
            ((corner == 1) && (!board.isEmpty(4))) ||
            ((corner == 3) && (!board.isEmpty(6))) ||
            ((corner == 9) && (!board.isEmpty(6)))){
                board.setPosition(5, symbol);
                return;
        }
    }
    // if the generate number is 2,
    // depending on the computer's corner position from the first move,
    // and on the opponent's position from the first move,
    // sets the computer's symbol to the specific position
    if (corner == 7){
        if ((!board.isEmpty(3)) && (!board.isEmpty(1))){
            board.setPosition(9, symbol);
            position = 9;
        }
        else if ((!board.isEmpty(3)) && (!board.isEmpty(9))){
            board.setPosition(1, symbol);
            position = 1;
        }
        else if ((!board.isEmpty(8)) || (!board.isEmpty(9)) || 
            (!board.isEmpty(4)) || (!board.isEmpty(1))){
            board.setPosition(3, symbol);
            position = 3;
        }
    }
        else if (corner == 1){
            if ((!board.isEmpty(9)) && (!board.isEmpty(3))){
                board.setPosition(7, symbol);
                position = 7;
            }
            else if ((!board.isEmpty(9)) && (!board.isEmpty(7))){
                board.setPosition(3, symbol);
                position = 3;
            }
            else if ((!board.isEmpty(2)) || (!board.isEmpty(3)) ||
                (!board.isEmpty(4)) || (!board.isEmpty(7))){
                board.setPosition(9, symbol);
                position = 9;
            }
        }
        else if (corner == 3){
            if ((!board.isEmpty(7)) && (!board.isEmpty(1))){
                board.setPosition(9, symbol);
                position = 9;
            }
            else if ((!board.isEmpty(7)) && (!board.isEmpty(9))){
                board.setPosition(1, symbol);
                position = 1;
            }
            else if ((!board.isEmpty(2)) || (!board.isEmpty(1)) ||
                (!board.isEmpty(6)) || (!board.isEmpty(9))){
                board.setPosition(7, symbol);
                position = 7;
            }
        }
        else if (corner == 9){
            if ((!board.isEmpty(1)) && (!board.isEmpty(3))){
                board.setPosition(7, symbol);
                position = 7;
            }
            else if ((!board.isEmpty(1)) && (!board.isEmpty(7))){
                board.setPosition(3, symbol);
                position = 3;
            }
            else if ((!board.isEmpty(8)) || (!board.isEmpty(7)) ||
                (!board.isEmpty(6)) || (!board.isEmpty(3))){
                board.setPosition(1, symbol);
                position = 1;
            }
        }
    }
}

