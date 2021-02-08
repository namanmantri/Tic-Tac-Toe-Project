                                                     //  TIC TAC TOE PROJECT
#include <iostream>
#include<bits/stdc++.h>
#define mp make_pair
using namespace std;

// These variables defines who(ai vs player) won the match
#define WIN 100  // if player has this value at end then he won the match
#define	DRAW 0   // if player has this value at the end then match in draw
#define LOSS -100 // if player has this value at the end then he lost the match

#define AI_MARKER 'X'   // X represents marker for AI
#define PLAYER_MARKER 'O'  // O represents marker for player

#define DEPTH 0 // this variable is used in minimax algorithm which AI will use to play its next move
// depth of node increase as algrithm move down the tree

char matrix[3][3]; // this character array represents board for the game in which each character is either X or O or blank

// Print the current board state
void print_matrix(char matrix[3][3])
{
	cout << "\n";
	cout << matrix[0][0] << " | " << matrix[0][1] << " | " << matrix[0][2] << "\n";
	cout << "----------" << "\n";
	cout << matrix[1][0] << " | " << matrix[1][1] << " | " << matrix[1][2] << "\n";
	cout << "----------" << "\n";
	cout << matrix[2][0] << " | " << matrix[2][1] << " | " << matrix[2][2] << "\n\n";
}

vector<vector<pair<int, int> > > win_state  // This function gives all possible winning states in the match
{
	
	{ mp(0, 0), mp(0, 1),mp(0, 2) },
	{ mp(1, 0), mp(1, 1),mp(1, 2) },  // 3 win states for row
	{ mp(2, 0), mp(2, 1),mp(2, 2) },
	
	{ mp(0, 0), mp(1, 0), mp(2, 0) },
	{ mp(0, 1), mp(1, 1), mp(2, 1) },  // 3 win states for column
	{ mp(0, 2), mp(1, 2), mp(2, 2) },
	

	{ mp(0, 0), mp(1, 1), mp(2, 2) },  // 2 win states for diagonal
	{ mp(2, 0), mp(1, 1), mp(0, 2) }

};

vector<pair<int, int>> legal_move(char matrix[3][3])  // This function gives all legal moves available in match 
{
	vector<pair<int, int>> move;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (matrix[i][j] != AI_MARKER && matrix[i][j] != PLAYER_MARKER)
			{
				move.push_back(mp(i, j));
			}
		}
	}

	return move;
}

bool position_occupied(char matrix[3][3], pair<int, int> pos)  // This function checks if a given position is occupied in the board or not
{
	vector<pair<int, int>> move = legal_move(matrix);

	for (int i = 0; i < move.size(); i++)
	{
		if (pos.first == move[i].first && pos.second == move[i].second)
		{
			return false;
		}
	}

	return true;
}

vector<pair<int, int>> occupied_position(char matrix[3][3], char marker)  // This function gives all board positions which are occupied by given marker(X/O)
{
	vector<pair<int, int>> positions;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (marker == matrix[i][j])
			{
				positions.push_back(std::mp(i, j));
			}
		}
	}

	return positions;
}

bool full(char matrix[3][3])  // This function checks if any blank position is present or not in board
{
	vector<pair<int, int>> move = legal_move(matrix);

	if (move.size()==0)
	{
		return true;  // no legal move is present
	}
	else
	{
		return false;  // atleast one legal move is present
	}
}

bool won(vector<pair<int, int>> position) // This function check if the game has been won by AI / Player
{
	bool won_game;

	for (int i = 0; i < win_state.size(); i++)
	{
		won_game = true;
		vector<pair<int, int>> curr_win_state = win_state[i];
		for (int j = 0; j < 3; j++)
		{
			if (!(find(begin(position), end(position), curr_win_state[j]) != end(position)))
			{
				won_game = false;
				break;
			}
		}

		if (won_game)
		{
			break;
		}
	}
	return won_game;
}

char opponent_marker(char marker) // This function gives opponent's marker of input
{
	char o_marker;
	if (marker == PLAYER_MARKER)
		o_marker = AI_MARKER; // sets o_marker to X
	else
		o_marker = PLAYER_MARKER; // sets o_marker to O

	return o_marker;
}

int matrix_state(char matrix[3][3], char marker) //This function checks that whether any winning state is achieved or not thus specify outcome of match
{

	char o_marker = opponent_marker(marker);

	vector<pair<int, int>> positions = occupied_position(matrix, marker);

	bool won_game = won(positions);  // checks if player won the match
	positions = occupied_position(matrix, o_marker);
	bool lost_game = won(positions); // checks whether player lost the match
	bool full_matrix = full(matrix);  // checks whether match is draw
	
	if (won_game)
	{
		return WIN; // Player won the game
	}
	if (lost_game)
	{
		return LOSS;  // Player lost the game
	}

	if (full_matrix)
	{
		return DRAW; // game is draw
	}

	return DRAW;
}

// Apply the minimax game optimization algorithm using Depth First Search
// AI uses this algorithm to find its next move 
pair<int, pair<int, int>> minimax_optimization(char matrix[3][3], char marker, int depth, int alpha, int beta)
{
	pair<int, int> best_move = mp(-1, -1);	// Initializing the best move
	int best_score = (marker == AI_MARKER) ? LOSS : WIN;
	
	// If algorithm reaches a terminal state, return the best score and move
	if (  DRAW != matrix_state(matrix, AI_MARKER)||full(matrix))
	{
		best_score = matrix_state(matrix, AI_MARKER);
		return mp(best_score, best_move);
	}

	vector<pair<int, int>> moves = legal_move(matrix); // finding all available legal moves 

	for (int i = 0; i < moves.size(); i++)
	{
		pair<int, int> curr_move = moves[i];
		matrix[curr_move.first][curr_move.second] = marker;

		if (marker == AI_MARKER)  // Maximizing player's turn
		{
			int score = minimax_optimization(matrix, PLAYER_MARKER, depth + 1, alpha, beta).first;// calculating score for a particular move

			if (best_score < score)  // if score obtained above is greater than best_score then update best_score
			{
				best_score = score - depth * 10;
				best_move = curr_move;
				alpha = max(alpha, best_score);
				matrix[curr_move.first][curr_move.second] = ' ';
				if (beta <= alpha) 
				{ 
					break; 
				}
			}
		} 
		else  // Minimizing opponent's turn
		{
			int score = minimax_optimization(matrix, AI_MARKER, depth + 1, alpha, beta).first;
			if (best_score > score)
			{
				best_score = score + depth * 10;
				best_move = curr_move;
				beta = min(beta, best_score);
				matrix[curr_move.first][curr_move.second] = ' ';
				if (beta <= alpha) 
				{ 
					break; 
				}
			}
		}
		matrix[curr_move.first][curr_move.second] = ' '; // Undo move
	}
	return mp(best_score, best_move);  // returning best move along with its score
}

bool done(char matrix[3][3]) // This function checks if game is finished and return corresponding output
{
	if (full(matrix))
	{
		return true;
	}

	if (DRAW != matrix_state(matrix, AI_MARKER))
	{
		return true;
	}

	return false;
}

// This function implements all the above functions to start the game
void process(){
	for(int i=0;i<3;i++)
	for(int j=0;j<3;j++)
	matrix[i][j]=' ';
             
	cout << "================================\n\n\tTIC TAC TOE \n\n================================" << "\n\n";
	cout <<"Rules of game are:\n1. Valid Row play is between 1 and 3.\n2. Valid Col play is between 1 and 3.\n";
	cout <<"3. Player can choose only blank valid position.\n\n ";

	cout << "AI Computer = X\t\tPlayer = O\t " << "\n\n";
	print_matrix(matrix);

	int row,col;
	
	while (!done(matrix))
	{
		cout<<"Computer turn:\n";
		pair<int, pair<int, int>> ai_move = minimax_optimization(matrix, AI_MARKER, DEPTH, LOSS, WIN); // input given by ai
		matrix[ai_move.second.first][ai_move.second.second] = AI_MARKER;
		print_matrix(matrix);
		
		if(done(matrix))  // if ai's move has finished the game
	    break;
		while(1){
			cout<<"Player turn:\n";
			cout << "Row index: ";
			cin >> row;
			cout << "Col index: ";
			cin >> col;
			row--;col--; // Player inputs the row and col using 1 based index thus it is reduced by 1 to make 0 based index
			cout << endl << endl;
			if(row>2||col>2||row<0||col<0)
			{
				cout<<"The position (" << row+1 << ", " << col+1 << ") is invalid. Enter a different position.\n";
			}
			else if (position_occupied(matrix, mp(row, col)))
			{
				cout << "The position (" << row+1 << ", " << col+1 << ") is occupied. Enter a different  position." << "\n";
				continue;
			}
			else
			{
				matrix[row][col] = PLAYER_MARKER;
				print_matrix(matrix);
				break;
			}
			
		}
		if(done(matrix)) // if player's move has finished the game 
		break;
	}

	cout << "======================= GAME OVER =======================" << "\n\n";

	int player_state = matrix_state(matrix, PLAYER_MARKER);
	int ai_state = matrix_state(matrix, AI_MARKER);
	cout<<"Player has "<<player_state<<" score."<<"\n";
	cout<<"AI has "<<ai_state<<" score."<<"\n\n";
	if (player_state==WIN)
	    cout << "PLAYER WIN THE MATCH." << "\n\n"; 
	else if (player_state==DRAW)
		cout << "MATCH IS DRAW." << "\n\n";
	else
        cout << "COMPUTER WON THE MATCH." << "\n\n";
}

int main()
{
	process();// driver function of whole project
	char c;
	while(1){
		cout<<"Would you like to play again? Press y/n \n\n";
		cin>>c;
		if(c=='y') process(); // if player wants to play again run this function
		else break;		
	}

	return 0;
}
