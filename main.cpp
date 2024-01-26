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
