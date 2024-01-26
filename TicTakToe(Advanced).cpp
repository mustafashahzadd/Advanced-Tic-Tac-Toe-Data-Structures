#include <iostream>  // For everything
#include <algorithm> // For std::fill
#include <cstdlib>   // For std::rand and std::srand
#include <ctime>     // For std::time
#include <chrono>    // For std::sleep
#include <thread>    // For std::sleep
#include <windows.h> // For color
using namespace std; // For everything

// Define constants for players and empty spots on the board
const int PLAYER = 1;
const int OPPONENT = -1;
const int EMPTY = 0;

/*-------------------------------------------------------------------------------------------------------------------
The Stack class to store the moves made
-------------------------------------------------------------------------------------------------------------------*/
class MoveStack {
private:
    int stack[100]; // Array to store moves, assuming a maximum size of 100
    int top; // Index for the top element

public:
    MoveStack() : top(-1) {}

    void push(int move) {
        if (top < 99) { // Check for stack overflow
            stack[++top] = move;
        }
    }

    int pop() {
        if (top >= 0) {
            return stack[top--];
        }
        return -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    void clear() {
        top = -1;
    }
};


/*-------------------------------------------------------------------------------------------------------------------
The Queue class to store the values/moves of each computer that will play in function playCvC()
-------------------------------------------------------------------------------------------------------------------*/
class Queue {
private:
    int data[10]; // Array to store items (1-9 possibilities)
    int front, rear; // Indexes for front and rear

public:
    Queue() : front(0), rear(-1) {}

    //standard enqueue function
    void enqueue(int item) {
        if (rear < 99) { // Check for queue overflow
            data[++rear] = item;
        }
    }

    //standard dequeue function
    int dequeue() {
        if (front <= rear) {
            return data[front++];
        }
        return -1;
    }

    //standard function to check if queue is empty
    bool isEmpty() const {
        return front > rear;
    }

    //resets queue
    void clear() {
        front = 0;
        rear = -1;
    }

    /**************************************************************************************
    Fills the queue with random values (1-9), no duplicates

    Called in: constructor, playCvC()
    Calls: random_shuffle(), enqueue()
    **************************************************************************************/
    void fillRandom() {
        clear();
        int positions[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        random_shuffle(positions, positions + 9); // Randomize positions
        for (int i = 0; i < 9; ++i) {
            enqueue(positions[i]);
        }
    }
};


/*-------------------------------------------------------------------------------------------------------------------
The Stack class to store and display the total instances of wins, loss or ties across all game modes (except CvC)
-------------------------------------------------------------------------------------------------------------------*/
class ResultStack {
private:
    //element in a stack
    struct Result {
        string mode;
        string winner;
    };

    Result stack[100];  // Assuming a maximum of 100 results

    int top;

public:
    ResultStack() : top(-1) {}



    /**************************************************************************************
    Standard push function, if not full, add to stack.

    Called in: object requirment
    Calls:
    Return:
    **************************************************************************************/
    void push(string mode, string winner) {
        if (top < 99) {
            stack[++top] = { mode, winner };
        }
    }



    /**************************************************************************************
    Standard check if empty, return true.

    Called in: object requirment
    Calls:
    Return: bool
    **************************************************************************************/
    bool isEmpty() {
        return top == -1;
    }



    /**************************************************************************************
    Standard pop operation. If not empty, reduce top.

    Called in: object requirment
    Calls:
    Return: stackState (or last element, if not empty)
    **************************************************************************************/
    Result pop() {
        if (!isEmpty()) {
            return stack[top--];
        }
        return { "", "" };  // Return an empty result if stack is empty
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Node class for linked list
-----------------------------------------------------------------------------------------------------------*/
class LLNode {
public:
    int board[9]; //Per move, storing that instance's board
    string mover; //Who moved?
    int move; //Where was the move placed?
    LLNode* next; //Next board
    LLNode* prev; //Prev board
    string winner; //Who won?

    LLNode() {
        mover = "No-one";
        move = NULL;
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Class for linked list (that will be inside each 2D linked list node)
-----------------------------------------------------------------------------------------------------------*/
class LL {
public:
    LLNode* head;

    LL() {
        head = nullptr;
    }



    /**************************************************************************************
    Add a move to the Linked List, for the ongoing session's i-th game

    Called in: playPvC()
    Calls:
    Return:
    **************************************************************************************/
    void addMove(int b[9], string mov, int movee) {
        if (head == nullptr) {
            head = new LLNode();

            for (int i = 0; i < 9; i++)
                head->board[i] = b[i];

            head->mover = mov;
            head->move = movee;

            return;
        }

        LLNode* ptr = head;

        if (ptr->next == nullptr) {
            ptr->next = new LLNode();

            for (int i = 0; i < 9; i++)
                ptr->next->board[i] = b[i];

            ptr->next->mover = mov;
            ptr->next->move = movee;
            ptr->next->prev = ptr;

            return;
        }

        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }

        ptr->next = new LLNode();

        for (int i = 0; i < 9; i++)
            ptr->next->board[i] = b[i];

        ptr->next->mover = mov;
        ptr->next->move = movee;
        ptr->next->prev = ptr;

        return;
    }



    /********************************************************************************************************
    Display's a linked list completely. Meaning one session was played and all of it's moves were printed

    Called in: displayGameMoves()
    Calls:
    Return:
    ********************************************************************************************************/
    void displayAllMoves() {
        LLNode* temp = head;

        while (temp != nullptr) {
            for (int i = 0; i < 9; i++) {
                char mark = (temp->board[i] == PLAYER) ? 'X' : (temp->board[i] == OPPONENT ? 'O' : '1' + i);
                cout << mark;
                if ((i + 1) % 3 == 0) cout << "\n";
                if (i % 3 != 2) cout << "|";
            }

            if (temp->mover == "Player") {
                temp->move++;
            }

            cout << "(Mover: " << temp->mover << " || Placed On: " << temp->move << ")" << endl << endl;

            temp = temp->next;
        }

        cout << "All moves done!\n";
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Node class for 2D linked list
-----------------------------------------------------------------------------------------------------------*/
class LL2DNode {
public:
    LL* current;
    LL2DNode* next;
    LL2DNode* prev;

    LL2DNode() {
        next = nullptr;
        prev = nullptr;
        current = nullptr;
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Class that will have the 2D linked list to store the whole session's moves
-----------------------------------------------------------------------------------------------------------*/
class LL2D {
private:
    LL2DNode* head;

public:
    LL2D() {
        head = nullptr;
    }



    /**************************************************************************************
    Add a new Linked List to the 2D linked List

    Called in: playPvC()
    Calls:
    Return:
    **************************************************************************************/
    void addToList(LL* newList) {
        if (head == nullptr) {
            head = new LL2DNode();
            head->current = newList;
            return;
        }

        LL2DNode* ptr = head;
        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }

        LL2DNode* newNode = new LL2DNode();
        newNode->current = newList;
        ptr->next = newNode;
        newNode->prev = ptr;
    }



    /**************************************************************************************
    Getter for private variable LL2DNode pointer

    Called in:
    Calls:
    Return: LL2DNode pointer
    **************************************************************************************/
    LL2DNode* getHead() {
        return head;
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Class that do the PvP and PvC and CvC games, most important
-----------------------------------------------------------------------------------------------------------*/
class TicTacToeGame {
private:
    LL2D gamemoves; // Will contain all the moves that will be played in a session

    string currentGameMode; // PvP? PvC? CvC?

    ResultStack gameResults; // Stack for move result

    int board[9]; // Game board

    int winningConditions[9][9]; // Winning conditions for the game

    MoveStack moves; // Stack for player moves

    Queue computer1Moves; // Queue for the first computer player

    Queue computer2Moves; // Queue for the second computer player



    /**************************************************************************************
    Constructor. Initilizes the winnable conditions (row, column, or diagonal) has same elements or not.

    Called in: playPvP(), playCvC()
    Calls: QueueObject.fillRandom()
    Return:
    **************************************************************************************/
    void initializeWinningConditions() {
        // Rows
        winningConditions[0][0] = 0; winningConditions[0][1] = 1; winningConditions[0][2] = 2;
        winningConditions[1][0] = 3; winningConditions[1][1] = 4; winningConditions[1][2] = 5;
        winningConditions[2][0] = 6; winningConditions[2][1] = 7; winningConditions[2][2] = 8;
        // Columns
        winningConditions[3][0] = 0; winningConditions[3][1] = 3; winningConditions[3][2] = 6;
        winningConditions[4][0] = 1; winningConditions[4][1] = 4; winningConditions[4][2] = 7;
        winningConditions[5][0] = 2; winningConditions[5][1] = 5; winningConditions[5][2] = 8;
        // Diagonals
        winningConditions[6][0] = 0; winningConditions[6][1] = 4; winningConditions[6][2] = 8;
        winningConditions[7][0] = 2; winningConditions[7][1] = 4; winningConditions[7][2] = 6;
    }



    /**************************************************************************************
    Reset's the board, to 'empty' state. No inputs.

    Called in: playPvP(), playCvC()
    Calls: QueueObject.fillRandom()
    Return:
    **************************************************************************************/
    void resetBoard() {
        for (int i = 0; i < 9; i++) {
            board[i] = EMPTY;
        }
        computer1Moves.fillRandom();
        computer2Moves.fillRandom();
    }



    /**************************************************************************************
    Check's if game has finished, if either there are no moves left or someone won

    Called in: playPvP(), playCvC()
    Calls: checkWinner(), isMovesLeft()
    Returns: bool
    **************************************************************************************/
    bool gameOver() {
        return checkWinner() != EMPTY || !isMovesLeft();
    }



    /**************************************************************************************
    Check's if any cell is empty, hence moves left

    Called in: gameOver(), playCvC()
    Calls:
    Returns: bool
    **************************************************************************************/
    bool isMovesLeft() {
        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY) return true;
        }
        return false;
    }



    /*************************************************************************************************
    Check's who won by comparing winning conditions

    Called in: gameOver(), playPvP(), playCvC()
    Calls:
    Returns: Winner (if 1 or -1 then either player 1, if 0 then game no finished or game tied)
    *************************************************************************************************/
    int checkWinner() {
        for (int i = 0; i < 8; i++) {
            if (board[winningConditions[i][0]] != EMPTY &&
                board[winningConditions[i][0]] == board[winningConditions[i][1]] &&
                board[winningConditions[i][1]] == board[winningConditions[i][2]]) {
                return board[winningConditions[i][0]];
            }
        }
        return EMPTY; // ret 0
    }



    /**************************************************************************************
    Display's the tic tac toe board

    Called in: playPvP(), playCvC()
    Calls:
    returns:
    **************************************************************************************/
    void drawBoard() {
        for (int i = 0; i < 9; i++) {
            char mark = (board[i] == PLAYER) ? 'X' : (board[i] == OPPONENT ? 'O' : '1' + i);
            cout << mark;
            if ((i + 1) % 3 == 0) cout << "\n";
            if (i % 3 != 2) cout << "|";
        }
        cout << "\n";
    }



    /**************************************************************************************
    Asks user for input and (if that block is empty/valid) the move is played

    Called in: playPvP()
    Calls:
    Returns: Move (The position where player play's the move)
    **************************************************************************************/
    int playerMove(int currentPlayer) {
        int move;
        string playerLabel = (currentPlayer == PLAYER) ? "Player 1" : "Player 2";
        cout << playerLabel << ", enter your move (1-9): ";
        while (true) {
            cin >> move;
            move--; // Adjust for 0-indexed board
            if (move >= 0 && move < 9 && board[move] == EMPTY) {
                board[move] = currentPlayer;
                break;
            }
            else {
                cout << "Invalid move. Try again: ";
            }
        }

        return move;
    }



    /**************************************************************************************
    Displays who won

    Called in: playPvP(), playCvC()
    Calls:
    Returns:
    **************************************************************************************/
    void announceResult() {
        string winner;
        int result = checkWinner();

        if (currentGameMode == "Player vs Player") {
            if (result == PLAYER) {
                winner = "Player 1";
            }
            else if (result == OPPONENT) {
                winner = "Player 2";
            }
            else {
                winner = "Draw";
            }
        }
        else {
            if (result == PLAYER) {
                winner = "Player";
            }
            else if (result == OPPONENT) {
                winner = "Computer";
            }
            else {
                winner = "Draw";
            }
        }

        cout << winner << " wins!" << endl;

        gameResults.push(currentGameMode, winner);

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }



    /***********************************************************************************************************************
    Gives the state of the board a value (if user won, -10; if computer won, 10; else game is not complete or tie, 0)

    Called in: minimax()
    Calls:
    Returns: score (checking current board's state and giving it a value)
    ***********************************************************************************************************************/
    int evaluateBoard() {
        for (int i = 0; i < 8; i++) {
            if (board[winningConditions[i][0]] == board[winningConditions[i][1]] &&
                board[winningConditions[i][1]] == board[winningConditions[i][2]]) {
                if (board[winningConditions[i][0]] == OPPONENT) return 10;
                else if (board[winningConditions[i][0]] == PLAYER) return -10;
            }
        }
        return 0;
    }



    /**************************************************************************************
    Recursively works to find the 'best' score that would be advantagious to computer

    Called in: findBestMove()
    Calls: evaluateBoard(), minimax(), max(), min()
    Returns: bestScore (after reaching a state where someone won or game tied)
    **************************************************************************************/
    int minimax(int depth, bool isMaximizingPlayer) {
        int score = evaluateBoard();

        if (score == 10) return score - depth;
        if (score == -10) return score + depth;
        if (!isMovesLeft()) return 0;

        if (isMaximizingPlayer) {
            int bestScore = -INT_MAX;
            for (int i = 0; i < 9; ++i) {  // Use 9 instead of board.size()
                if (board[i] == EMPTY) {
                    board[i] = OPPONENT;
                    bestScore = max(bestScore, minimax(depth + 1, false));
                    board[i] = EMPTY;
                }
            }
            return bestScore;
        }
        else {
            int bestScore = INT_MAX;
            for (int i = 0; i < 9; ++i) {  // Use 9 instead of board.size()
                if (board[i] == EMPTY) {
                    board[i] = PLAYER;
                    bestScore = min(bestScore, minimax(depth + 1, true));
                    board[i] = EMPTY;
                }
            }
            return bestScore;
        }
    }



    /**************************************************************************************
    Finds the best move and returns it

    Called in: playPvC()
    Calls: minimax()
    Returns: bestMove (the index, upon which if computer plays, the user will NOT win)
    **************************************************************************************/
    int findBestMove() {
        int bestVal = -INT_MAX;
        int bestMove = -1;

        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY) {
                board[i] = OPPONENT; // Computer's move
                int moveVal = minimax(0, false);
                board[i] = EMPTY;

                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestMove = i;
                }
            }
        }
        return bestMove;
    }



    /*******************************************************************************************************
    If numberOfMovesOfPlayer <= numberOfMovesOfComputer, then return true meaning it is player's turn

    Called in: playPvC()
    Calls:
    Returns: bool
    *******************************************************************************************************/
    bool isPlayerTurn() {
        int playerMoves = 0, opponentMoves = 0;
        for (int i = 0; i < 9; i++) {
            if (board[i] == PLAYER) playerMoves++;
            if (board[i] == OPPONENT) opponentMoves++;
        }
        return playerMoves <= opponentMoves;
    }

public:

    /**************************************************************************************
    Getter of private variable

    Called in: object creation
    Calls: displayGameMoves()
    Returns: 2D Linked List Object
    **************************************************************************************/
    LL2D getGameMoves() {
        return gamemoves;
    }



    /**************************************************************************************
    Constructor

    Called in: object creation
    Calls: initializeWinningConditions(), resetBoard()
    Returns:
    **************************************************************************************/
    TicTacToeGame() {
        initializeWinningConditions();
        resetBoard(); // Initialize the board in the resetBoard method
    }



    /*****************************************************************************************************************************
    Prints results from stack ([*] Stack Implementation)

    Called in: printGameResults() of class TicTacPro
    Calls: stack.isEmpty(), stack.pop()
    Returns:
    *****************************************************************************************************************************/
    void printResults() {
        while (!gameResults.isEmpty()) {
            auto result = gameResults.pop();
            cout << "Game Mode: " << result.mode << ", Winner: " << result.winner << endl;
        }
    }



    /*****************************************************************************************************************************
    Player vs Computer. Player's move is user's move, computer's move is the 'best possible move' in response to that

    Called in: handleStartGame() of class TicTacPro
    Calls: gameOver(), resetBoard(), drawBoard(), playerMove(), isPlayerTurn(), announceResult(), checkWinner(), findBestMove()
    Returns: Bool (True means function finished correctly and is over)
    *****************************************************************************************************************************/
    bool playPvC() {
        LL* currentGameLL = new LL();
        gamemoves.addToList(currentGameLL);

        currentGameMode = "Player vs Computer";
        resetBoard();

        while (!gameOver()) {
            drawBoard();
            if (isPlayerTurn()) {
                int move = playerMove(PLAYER);

                currentGameLL->addMove(board, "Player", move);
            }
            else {
                cout << "Computer's turn:\n";
                int move = findBestMove();
                board[move] = OPPONENT;
                cout << "Computer placed at " << move + 1 << "\n";
                currentGameLL->addMove(board, "Computer", move + 1);
            }

            if (gameOver()) {
                drawBoard();
                string winner = (checkWinner() == PLAYER) ? "Player" : "Computer";
                announceResult();
                break;
            }
        }

        return true;  // Indicate the game was played
    }



    /*********************************************************************************************
    Does Player vs Player Tic Tac Toe. Each player plays a move until one wins or tie

    Called in: handleStartGame() of class TicTacPro
    Calls: gameOver(), resetBoard(), drawBoard(), playerMove(), announceResult(), checkWinner()
    Returns: Bool (True means function finished correctly and is over)
    *********************************************************************************************/
    bool playPvP() {

        currentGameMode = "Player vs Player";
        resetBoard();
        int currentPlayer = PLAYER;

        while (!gameOver()) {
            drawBoard();
            playerMove(currentPlayer);
            currentPlayer = -currentPlayer; // Switch player
            if (gameOver()) {
                drawBoard();
                string winner = (checkWinner() == PLAYER) ? "Computer 1" : "Computer 2";
                announceResult();
            }
        }

        return true;
    }



    /*************************************************************************************************************************************************************
    Fills Two Queue with random values, then dequeue() to place 'X' or 'O' on those random blocks ([*] Queue Implementation)

    Called in: run() of class TicTacPro
    Calls: gameOver(), resetBoard(), drawBoard(), checkWinner(), QueueObject.isEmpty(), QueueObject.fillRandom(), QueueObject.dequeue()
    Returns: Bool (No reason)
    *************************************************************************************************************************************************************/
    bool playCvC() {
        currentGameMode = "Computer vs Computer";
        resetBoard();
        int turn = 0; // Track turns to alternate between players

        while (!gameOver()) {
            drawBoard();
            std::this_thread::sleep_for(std::chrono::seconds(2));

            int currentPlayer = (turn % 2 == 0) ? PLAYER : OPPONENT;
            Queue& currentQueue = (currentPlayer == PLAYER) ? computer1Moves : computer2Moves;

            int move = -1;

            // If the queue is empty, refill it
            if (currentQueue.isEmpty()) {
                currentQueue.fillRandom();
            }

            // Try to find a valid move from the queue
            do {
                move = currentQueue.dequeue() - 1; // Adjust for 0-indexed board
                if (move < 0 || move >= 9) {
                    // Invalid move, refill the queue and try again
                    currentQueue.fillRandom();
                    move = currentQueue.dequeue() - 1;
                }
            } while (board[move] != EMPTY);

            // Make the move
            board[move] = currentPlayer;
            cout << "Computer " << (currentPlayer == PLAYER ? "1" : "2") << " plays at position " << move + 1 << "\n";

            turn++; // Increment to switch players
        }
        if (gameOver()) {
            drawBoard();
            string winner = (checkWinner() == PLAYER) ? "Computer 1" : "Computer 2";

            cout << "Winner: " << winner << endl;
        }

        system("pause");
        system("cls");
        return true;
    }
};


/*-----------------------------------------------------------------------------------------------------------
The Class that will house the main menue and work until user decides to stop playing
-----------------------------------------------------------------------------------------------------------*/
class TicTacPro {
private:
    TicTacToeGame game;



    /*************************************************************************************************************
    Display's each 2D's Linked List's i-th Doubly Linked List ([*] 2D Doubly Linked List Implementation)

    Called in: run()
    Calls: 2DLL.getGameMoves(), 2DLL.getHead(), LL.displayAllMoves(), system()
    Returns:
    *************************************************************************************************************/
    void displayGameMoves() {
        LL2DNode* gameNode = game.getGameMoves().getHead();

        int count = 1;

        if (gameNode != nullptr) {
            char c;
            while (1) {
                cout << "Game " << count << " Moves:\n";
                gameNode->current->displayAllMoves();

                cout << "Enter 'N' if you want to view next game's result\nEnter 'P' if you want to see previous game's result\nEnter 'E' if you want to not see any more games\n";
                cout << "Enter choice: ";
                cin >> c;

                if (c == 'N') {
                    if (gameNode->next == nullptr) {
                        cout << "No next game in record!\n";
                        system("pause");
                        system("cls");
                    }
                    else {
                        gameNode = gameNode->next;
                        count++;
                        system("cls");
                    }
                }
                else if (c == 'P') {
                    if (gameNode->prev == nullptr) {
                        cout << "No previous game in record!\n";
                        system("pause");
                        system("cls");
                    }
                    else {
                        gameNode = gameNode->prev;
                        count--;
                        system("cls");
                    }
                }
                else if (c == 'E') {
                    break;
                }
                else {
                    cout << "Enter again, invalid input!\n";
                    system("pause");
                    system("cls");
                }
            }
        }
        else {
            cout << "No moves stored yet. Please play and enjoy!\n";
        }

        return;
    }



    /*************************************************************************************************************
    Display's Instructions of how the game is played

    Called in: run()
    Calls:
    Returns:
    *************************************************************************************************************/
    void displayInstructions() {
        cout << "Instructions for Tic Tac Toe:\n";
        cout << "- The game is played on a 3x3 grid.\n";
        cout << "- Players take turns to mark a space in the grid.\n";
        cout << "- The first player to align three of their marks ('X' or 'O') vertically, horizontally, or diagonally wins.\n";
        cout << "- If all spaces are marked and no player has won, the game is a draw.\n\n";
    }



    /**************************************************************************************
    Displays 'Start' Menu ([*] keeps recalling itself if a 'wrong' input is detected)

    Called in: run()
    Calls: game.playPvP(), game.playPvC()
    **************************************************************************************/
    void handleStartGame() {
        int choice;
        do {
            cout << "Start Game Menu:\n";
            cout << "1) Player vs Player\n";
            cout << "2) Player vs Computer\n";
            cout << "3) Back\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                system("cls");
                game.playPvP();
                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");
                game.playPvC();
                system("pause");
                system("cls");
                break;
            case 3:
                system("cls");
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                system("cls");
            }
        } while (choice != 3);
    }

public:
    // Constructor
    TicTacPro() {}



    /**************************************************************************************
    Displays 'Main' Menu ([*] keeps recalling itself if a 'wrong' input is detected)

    Called in: main()
    Calls: TicTacToeGameObject.printResults()
    Returns:
    **************************************************************************************/
    void printGameResults() {
        cout << "Printing the results of the game that you played in order of latest played to last played:\n";
        game.printResults();
    }



    /**************************************************************************************
    Displays 'Main' Menu ([*] keeps recalling itself if a 'wrong' input is detected)

    Called in: main()
    Calls: handleStartGame(), game.playCvC(), displayInstructions(), displayGameMoves()
    **************************************************************************************/
    void run() {
        int choice;
        do {
            cout << "Main Menu\n";
            cout << "1) Start Game\n";
            cout << "2) Tutorial\n";
            cout << "3) Instructions\n";
            cout << "4) All The Moves That Were Played Per Gaming Session\n";
            cout << "5) Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            system("cls");

            switch (choice) {
            case 1:
                // Handles starting a new game, new menue
                handleStartGame();
                break;
            case 2:
                // Tutorial
                game.playCvC();
                break;
            case 3:
                // Displays the game instructions
                displayInstructions();
                break;
            case 4:
                //display all previous moves for PvC
                displayGameMoves();
                system("pause");
                system("cls");
                break;
            case 5:
                //exit condition
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                system("cls");
                break;
            }
        } while (choice != 5);
    }

};


/**************************************************************************************
Main Driver Code

Called in:
Calls: TicTacProObject.run(), TicTacProObject.printGameResults()
**************************************************************************************/
int main() {
    //8 = grey back ground, 0 =  black text color
    system("color 80");

    //seed assigned so that every time new randomized values
    srand(time(0));

    TicTacPro ticTacPro;
    ticTacPro.run(); // This plays the games

    // Print the results using the same TicTacPro instance
    ticTacPro.printGameResults();
}
