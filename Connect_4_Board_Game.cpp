#include <iostream>
#include <stdlib.h> // needed for rand function
#include <Windows.h>

#define HEIGHT 6
#define WIDTH 7

using namespace std;

class Game
{
 int board_info[HEIGHT][WIDTH]={{0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0}};
 public:

 int LastMoveX, LastMoveY;

 void draw_board()
 {
  cout<<endl;
  for (int y=0;y<HEIGHT;y++)
  {
    for (int x=0;x<WIDTH;x++)
    {
     cout<<"| ";
     if(board_info[y][x] == 0)
      cout<<" ";
     else if(board_info[y][x] == 1)
      cout<<"X";
     else if(board_info[y][x] == 2)
      cout<<"O";
    }
    cout<<"|"<<"\n---------------------"<<endl;
  }
 }

void player_movement(int player)
{
 int choice;
 cout<<"\nPLAYER "<<player<<", Please select a number from 1 - 7: ";
 cin>>choice;

 // Error Checking
 if(cin.fail())
 {
  cout<<"Error!";
  exit(1);
 }

 while(choice > WIDTH || choice <=0)
 {
  cout<<"\nInvalid Input ";
  cout<<"\nPlease select a number from 1 - 7 again: ";
  cin>>choice;
 }

 int number=0;
 while(board_info[(HEIGHT-1)-number][(choice-1)] != 0)
 {
  number++;
  if(number>(HEIGHT-1))
  {
   cout<<"\nPlease select again: ";
   cin>>choice;
   number=0;  // You must reset the number else it will start at the beginning of the array
  }
 }

 board_info[(HEIGHT-1)-number][choice-1] = player;
 LastMoveY = (HEIGHT-1)-number;
 LastMoveX = choice-1;
}

bool check_for_winner(int x, int y, int player)
{
 bool winner;

 if(check_diagonal_combo_SW_NE(x,y,player))
  return true;
 else if(check_diagonal_combo_NW_SE(x,y,player))
  return true;
 else if(check_vertical_combo(x,y,player))
  return true;
 else if(check_horizontal_combo(x,y,player))
  return true;
 else
  return false;
}

bool check_diagonal_combo_SW_NE(int x, int y, int player)
{
 int score=1;
 int count=1;

 while((y-count >= 0) && (x+count < WIDTH))
 {
  if(board_info[y-count][x+count] == player)  // Check SW to NE
  {
   score++;
   count++;
  }
  else
   break;  // If no combo is detected break from the loop
 }

 count=1;

 while((y+count < HEIGHT) && (x-count >=0))
 {
  if(board_info[y+count][x-count] == player)  // Check NE to SW
  {
   score++;
   count++;
  }
  else
    break;  // If no combo is detected break from the loop
 }

 if(score == 4)
  return true;
 else
  return false;
}

bool check_diagonal_combo_NW_SE(int x, int y, int player)
{
 int score=1;
 int count=1;
 while((y+count >= 0) && (x+count < WIDTH))
 {
  if(board_info[y+count][x+count] == player)  // Check NW to SE
  {
   score++;
   count++;
  }
  else
   break;  // If no combo is detected break from the loop
 }

 count=1;

 while((y-count < HEIGHT) && (x-count >=0))
 {
  if(board_info[y-count][x-count] == player)  // Check SE to NW
  {
   score++;
   count++;
  }
  else
    break;  // If no combo is detected break from the loop
   }

 if(score == 4)
  return true;
 else
  return false;
}

bool check_vertical_combo(int x, int y, int player)
{
 int score=1;
 int count=1;

 while(y+count >= 0 && y+count < HEIGHT)
 {
  if(board_info[y+count][x] == player)  // Check Down
  {
   score++;
   count++;
  }
  else
   break;  // If no combo is detected break from the loop
 }

 if(score == 4)
  return true;
 else
  return false;
}

bool check_horizontal_combo(int x, int y, int player)
{
 int score=1;
 int count=1;

 while((x+count >= 0) && (x+count < WIDTH))
 {
  if (board_info[y][x+count] == player)  // Check Left
  {
   score++;
   count++;
  }
  else
   break;  // If no combo is detected break from the loop
 }

 count = 1;
 while((x-count < WIDTH) && (x-count >=0))
 {
  if(board_info[y][x-count] == player)  // Check Right
  {
   score++;
   count++;
  }
  else
    break;  // If no combo is detected break from the loop
 }

 if(score == 4)
  return true;
 else
  return false;
}

void ClearConsoleToColors(int ForgC, int BackC)
{
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
  // This is used to get the handle to current output buffer.
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  // This is used to reset the carat/cursor to the top left.
  COORD coord = {0, 0};
  // This is a return value indicating how many characterss were written
  // It is not used but we need to capture this since it will be
  // written anyway (passing NULL causes an access violation).
  DWORD count;
  // This is a structure containing all the console info
  // It is used here to find the size of the console.

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  // Now the current color will be set by this handle
  SetConsoleTextAttribute(hStdOut, wColor);
  if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
  {
    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    // This fills the buffer with a given character (in this case 32=space).
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
    // This will set our cursor position for the next print statement
  }
  return;
}
};

// This is a simple 'Connect 4 Board Game'
// Two player game , no computer player

int main()
{
 int counter=0;
 bool winner=false;
 int option;
 srand(GetTickCount()); // sets randomizer as time *unique*
 int m,n;
 Game g;

 cout<<"\nWELCOME TO CONNECT 4 BOARD GAME"<<endl;

 do
 {
  cout<<"\n\n1.Rules"<<endl;
  cout<<"\n2.Start"<<endl;
  cout<<"\n3.To change the foreground and background"<<endl;
  cout<<"\n4.Exit"<<endl;
  cout<<"\nEnter an option number from 1 to 4: ";
  cin>>option;

  switch(option)
  {

   case 1:

   cout<<"\nThe board game consists of 7 columns and 6 rows where 2 players fit tiles of symbol X or 0"<<endl;
   cout<<"\nIt is a two player connection game where Player 1 gets tiles of symbol X and Player 2 gets tiles of symbol 0."<<endl;
   cout<<"\nThe tiles always fall from top to bottom i.e. in the last row as a result of force of gravity."<<endl;
   cout<<"\nThe players have to take turns and drop tiles which fall straight down, ";
   cout<<"occupying the lowest available space within the column by selecting a column number from 1 to 7."<<endl;
   cout<<"\nThe objective of the game is to connect 4 tiles of symbol (either X or 0)";
   cout<<" in the same line(vertical,horizontal or diagonal)."<<endl;
   cout<<"\nThe first player to do so wins the game."<<endl;
   break;

   case 2:
   cout<<"\nLet's Begin"<<endl;
   cout<<"\nPlease select a number from 1-7"<<endl;
   cout<<"| 1| 2| 3| 4| 5| 6| 7|"<<endl;
   cout<<"---------------------";
   g.draw_board();

   for(int i = 0; i < 21; i++)
   {
    int board_info[HEIGHT][WIDTH]={{0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,0}};

    g.player_movement(1);
    g.draw_board();
    winner = g.check_for_winner(g.LastMoveX,g.LastMoveY,1);
    if(winner)
    {
     cout<<"\nPlayer 1 Wins"<<endl;
     break;
    }

    g.player_movement(2);
    g.draw_board();
    winner = g.check_for_winner(g.LastMoveX,g.LastMoveY,2);
    if(winner)
    {
     cout<<"\nPlayer 2 Wins"<<endl;
     break;
    }

   }

   break;

   case 3:
   cout<<"\nEnter the color ( in range 0 - 255) for foreground: ";
   cin>>m;
   cout<<"\nEnter the color ( in range 0 - 255) for background: ";
   cin>>n;
   //Try m=16 and n=7
   g.ClearConsoleToColors(m,n);
   break;

   case 4:
   system("PAUSE");
   exit(1);
   break;

   default:
   cout<<"\nPlease enter a valid option number."<<endl;
   break;
  }
 }while(1);

 return 0;

}
