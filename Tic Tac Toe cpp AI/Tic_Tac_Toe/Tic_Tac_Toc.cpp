// Tic_Tac_Toe.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "BP.cpp"
#define M_PI_2 3.141592653
int board[N*N] = {0};                //chessboard
int chess = 0;                       //The total num of chess on chessboard
typedef enum{A=1, B=-1}piece;        //Two types of chess used by player1 and player2

void init_board();                   //initialize the chessboard
void print_board(int * );            //print the chessboard
void print_chess_board(int * );
void place_piece_on_board(int * ,int ,int ,piece type);   //set the chess on board
int win(int *);                                           //calculate if any player is wining
int test_node_type(int * );                               //test the type of node
int test_if_chess_boards_equal(int * ,int * );          //test if the two chessboard are at the same state
int evaluation_function(int * );                        //evaluation fuction for award
int full(int *);                                        //check if the borad is full
void init_ancestor(int * ,int );                        //initialize ancestor
int get_backed_up_value_of_node(int * ,int ,int ,int * ,int);     //search the back-up value of the node. The input is the board state, the depth(from 0), the MAX depth and ancestor function and the type of node.
void copy_chess_board(int *, int *);

int main(int argc, char* argv[])
{
    printf("%d\n",N);      //Using N change the game as "N in row". N is figurable in macro.h. As N goes lagger, drawn game happens more offen
    FILE * fp;
    fp = fopen("BP_net_weight.txt","r");

    int i,j,k;

    for(i=0;i<N*N;i++)
    {
        fscanf(fp,"%lf",&w_ji[i]);
    }
    for(i=0;i<N*N;i++)
    {
        for(j=0;j<N*N;j++)
        {
            fscanf(fp,"%lf ",&w_kj[i][j]);
        }
    }
    char ch[3];
    int n;
    bool isplay = true;
    init_board();
    int flag;             //choose offensive position or defensive position
    printf("Please choose who goes first!\n");
    printf("0:AI          1:player\n");
    scanf("%d",&flag);
    if(flag)               //humen choose offensice position
    {
        print_board(board);
        while(1){
          while(1){
              printf("Please enter the number of the square\n");
              printf("where you want to place your chess?\n ");
              scanf("%s",ch);
              n = (ch[0]-'0')*N+(ch[1]-'0');
              printf("%d\n",n);
              if(board[n]==0){
                  place_piece_on_board(board,n/N,n%N,A);
                  print_board(board);
                  break;
              }
          }
          if( full(board) && !win(board)){
              isplay = false;
              printf("It ends in a draw\n");
              break;
          }
            
          if(win(board))  {
              isplay = false;
              printf("you win!\n");
              break;
          }
            int min = MAX+1;
            int step;
            for(int i=0;i<N*N;i++){
                if(board[i]!=0)  continue;
                      int temp_chess_board[N*N];
                      copy_chess_board(temp_chess_board,board);
                      int ancestor[N*N];
                      init_ancestor(ancestor,test_node_type(temp_chess_board));
                      place_piece_on_board(temp_chess_board,i/N,i%N,B);
                      int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
                      printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
                      if(temp<min){
                          min = temp;
                          step = i;
                      }
            }
                  if(!win(board)&&!full(board))     place_piece_on_board(board,step/N,step%N,B);
                  print_board(board);
                  if(full(board) && !win(board)) {
                      isplay = false;
                      printf("It ends in a draw\n");
                      break;
                  }
                  if(win(board)==-1) {
                      isplay = false;
                      printf("you lose!\n");
                      break;
                  }
      }
      } //flag
      else  {
          int max = MIN-1;
          int step;
      
          for(i=0;i<N*N;i++)  {
                  int temp_chess_board[N*N];
                  copy_chess_board(temp_chess_board,board);
                  int ancestor[N*N];
                  init_ancestor(ancestor,test_node_type(temp_chess_board));
                  place_piece_on_board(temp_chess_board,i/N,i%N,A);
                  int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
                  printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
                  if(temp>max) {
                      max = temp;
                      step = i;
                  }
         }
      place_piece_on_board(board,step/N,step%N,B);
      print_board(board);
      while(1) {
      while(1) {
          printf("Please enter the number of the square\n");
          printf("where you want to place your chess?\n ");
          scanf("%s",ch);
          n = (ch[0]-'0')*N+(ch[1]-'0');
          printf("%d\n",n);
          
          if(board[n]==0) {
                  place_piece_on_board(board,n/N,n%N,A);
                  print_board(board);
                  break;
          }
      }

         if(full(board) && !win(board)) {
             isplay = false;
             printf("It ends in a draw\n");
             break;
         }
          
      if(win(board)==-1)  {
          isplay = false;
          printf("you win!\n");
          break;
      }
          
                  int max = MIN-1;
                  int step = 1;
                  for(i=0;i<N*N;i++) {
                      if(board[i]!=0)
                          continue;
                      int temp_chess_board[N*N];
                      copy_chess_board(temp_chess_board,board);
                      int ancestor[N*N];
                      init_ancestor(ancestor,test_node_type(temp_chess_board));
                      place_piece_on_board(temp_chess_board,i/N,i%N,B);
                      int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
                      printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
                      if(temp>max)
                      {
                          max = temp;
                          step = i;
                      }
                  }
                  if(win(board)==0&&!full(board))
                      place_piece_on_board(board,step/N,step%N,B);
      print_board(board);
                  if(full(board) && !win(board))
                  {
      isplay = false;
                      printf("It ends in a draw\n");
             break;
      }
      if(win(board)==1)
                  {
                      isplay = false;
                      printf("you lose!\n");
      break;
                  }
      }
      }
      // using back propagation (BP) neural network algorithm to adapt the weights
          /*FILE * fp;
          fp = fopen("BP_net_weight.txt","w");

          int learn_times = 10;
          int temp_learn_times = 0;
          int i,j,k;

          init_weight();
          srand((int)time(0));
          while(1)
          {
              int count = 0;
              init_board();
              while(count!=N*N && win(board)==0)
              {
                  if(count%2==0)
                  {
                      while(1)
                      {
                          int temp = rand()%(N*N);
                          if(board[temp]==0)
                          {
                              place_piece_on_board(board,temp/N,temp%N,A);
                              break;
                          }
                      }
                  }
                  else
                  {
                      while(1)
                      {
                          int temp =rand()%(N*N);
                          if(board[temp]==0)
                          {
                              place_piece_on_board(board,temp/N,temp%N,B);
                              break;
                          }
                      }
                  }
                  count ++;
                  if(count>=8)
                  {
                      int ancestor[N*N];
                      init_ancestor(ancestor,test_node_type(board));
                      int backed_up_value = get_backed_up_value_of_node(board,0,6,ancestor,test_node_type(board));
                      adapt_BP_net_weight(board,backed_up_value);
                      temp_learn_times ++;
                      printf("learn %d times.\n",temp_learn_times);
                  }
              }
              if(temp_learn_times>=learn_times)
                  break;
          }

          for(i=0;i<N*N;i++)
          {
              fprintf(fp,"%lf ",w_ji[i]);
          }
          fprintf(fp,"\n");
          for(i=0;i<N*N;i++)
          {
              for(j=0;j<N*N;j++)
              {
                  fprintf(fp,"%lf ",w_kj[i][j]);
              }
          }
          fprintf(fp,"\n");*/
        return 0;
      }


      void init_board()
      {
      int i;
      chess= 0;
      for(i = 0; i < N*N; i++) {
          board[i] = 0;
      }
      }

      void print_board(int * chess_board)     // print the chess_board
      {
      int i,j,k;
      char temp[N][2];
      printf("\n\n");
      
      for(i = 0;i < (N+1)*(N+1); i++)    printf("-");
      printf("\n");
          
      for(i = 0;i < N; i++) {
          for(j = 0;j < N; j++) {
              if(chess_board[i*N+j] == 1) {
                  temp[j][0] = '#';
                  temp[j][1] = '#';
              }
              else if(chess_board[i*N+j] == -1) {
                      temp[j][0] = 'X';
                      temp[j][1] = 'X';
              }
              else {
                    temp[j][0] = i + '0';
                    temp[j][1] = j + '0';
              }
          }
      
          for(j = 0; j < N-1; j++)
              printf(" %c%c |",temp[j][0],temp[j][1]);
              printf(" %c%c\n",temp[j][0],temp[j][1]);
              if(i!=N-1) {
                  for(j = 0; j < N-1; j++)
                      printf("----+");
                  printf("----\n");
              }
      }
      
      for(i = 0;i < (N+1)*(N+1); i++)
      printf("-");

      }

      void print_chess_board(int * chess_board)      //test function
      {
          for(int i=0;i<N;i++)
          {
              for(int j=0;j<N;j++)
              {
                  if(j<N-1)
                      printf("%d ",chess_board[i*N+j]);
                  else
                      printf("%d\n",chess_board[i*N+j]);
              }
          }
          return ;
      }

      void place_piece_on_board(int * board,int x,int y,piece type)
      {
          board[x*N+y] = type;
          return ;
      }

      int win(int * chess_board)
      {

          // test one row, one colomn and X state
          int i, j;
          for(i=0;i<N;i++)
          {
              bool check_row_flag = true;
              for(j=0;j<N-1;j++)
              {
                  if(chess_board[i*N+j]!=chess_board[i*N+j+1])
                  {
                      check_row_flag = false;
                      break;
                  }
              }
              if(check_row_flag&&chess_board[i*N+j]!=0)
                  return chess_board[i*N+j];
          }
          for(j=0;j<N;j++)
          {
              bool check_line_flag = true;
              for(i=0;i<N-1;i++)
              {
                  if(chess_board[i*N+j]!=chess_board[(i+1)*N+j])
                  {
                      check_line_flag = false;
                      break;
                  }
              }
              if(check_line_flag&&chess_board[i*N+j]!=0)
                  return chess_board[i*N+j];
          }
          bool check_diagonal_flag = true;
          for(i=0;i<N-1;i++)
          {
              if(chess_board[i*N+i]!=chess_board[(i+1)*N+i+1])
              {
                  check_diagonal_flag = false;
                  break;
              }
          }
          if(check_diagonal_flag&&chess_board[i*N+i]!=0)
              return chess_board[i*N+i];
          check_diagonal_flag = true;
          for(i=0;i<N-1;i++)
          {
              if(chess_board[i*N+(N-1-i)]!=chess_board[(i+1)*N+(N-1-(i+1))])
              {
                  check_diagonal_flag = false;
                  break;
              }
          }
          if(check_diagonal_flag&&chess_board[i*N+(N-1-i)]!=0)
              return chess_board[i*N+(N-1-i)];
          return 0;
      }

      int test_node_type(int * chess_board)   // test the node is MAX node or min node
      {
          // using the num of chess to make sure the node state
          int count_of_A = 0, count_of_B = 0;
          for(int i=0;i<N*N;i++)
          {
              if(chess_board[i]==A)
                  count_of_A ++;
              else if(chess_board[i]==B)
                  count_of_B ++;
          }
          if(count_of_A>count_of_B)
              return -1;
          else
              return 1;
      }

      int test_if_chess_boards_equal(int * chess_board1,int * chess_board2)    //判断棋盘是否等价，用于减少人工神经网络的输入
      {
          int temp_chess_board[N*N];
          bool same_flag = true;
          int i,j,k,h;

          for(i=0;i<4;i++)
          {
              int rotation_operator[2][2] = {{(int)cos(M_PI_2*i), (int)-sin(M_PI_2*i)},
                                             {(int)sin(M_PI_2*i), (int)cos(M_PI_2*i)}};
              for(k=0;k<N;k++)
              {
                  for(h=0;h<N;h++)
                  {
                      int temp_k = (k*2-(N-1)) * rotation_operator[0][0] + (h*2-(N-1)) * rotation_operator[1][0];
                      int temp_h = (k*2-(N-1)) * rotation_operator[0][1] + (h*2-(N-1)) * rotation_operator[1][1];
                      temp_k = (temp_k + (N-1)) / 2;
                      temp_h = (temp_h + (N-1)) / 2;
                      temp_chess_board[temp_k*N+temp_h] = chess_board2[k*N+h];
                  }
              }
              same_flag = true;
              for(k=0;k<N*N;k++)
              {
                  if(chess_board1[k]!=temp_chess_board[k])
                  {
                      same_flag = false;
                      break;
                  }
              }
              if(same_flag)
                  return 1;
          }

          same_flag = true;
          for(i=0;i<N;i++)
          {
              for(j=0;j<N;j++)
              {
                  if(chess_board1[i*N+j]!=chess_board2[(N-1-i)*N+j])
                  {
                      same_flag = false;
                      break;
                  }
              }
          }
          if(same_flag)
              return 1;

          same_flag = true;
          for(i=0;i<N;i++)
          {
              for(int j=0;j<N;j++)
              {
                  if(chess_board1[i*N+j]!=chess_board2[i*N+(N-1-j)])
                  {
                      same_flag = false;
                      break;
                  }
              }
          }
          if(same_flag)
              return 1;

          same_flag = true;
          for(i=0;i<N;i++)
          {
              for(int j=0;j<N;j++)
              {
                  if(chess_board1[i*N+j]!=chess_board2[j*N+i])
                  {
                      same_flag = false;
                      break;
                  }
              }
          }
          if(same_flag)
              return 1;

          same_flag = true;
          for(i=0;i<N;i++)
          {
              for(int j=0;j<N;j++)
              {
                  if(chess_board1[i*N+j]!=chess_board2[(N-1-j)*N+(N-1-i)])
                  {
                      same_flag = false;
                      break;
                  }
              }
          }
          if(same_flag)
              return 1;

          return 0;
      }

      void init_ancestor(int * ancestor,int node_type)       //alpha-beta pruning
      {

          if(node_type==1)
          {
              for(int i=0;i<N*N;i++)
              {
                  if(i%2==0)
                      ancestor[i] = MIN;
                  else
                      ancestor[i] = MAX;
              }
          }
          else
          {
              for(int i=0;i<N*N;i++)
              {
                  if(i%2==0)
                      ancestor[i] = MAX;
                  else
                      ancestor[i] = MIN;
              }
          }
      }

      int get_backed_up_value_of_node(int * chess_board,int h,int depth,int * ancestor,int root_type)
      {
          if(h==depth||full(chess_board)||win(chess_board))
          {
              // If the node is left node, return award value
              return evaluation_function(chess_board);
          }
          else
          {
              int temp_backed_up_value;
              int max_node_value = MIN, min_node_value = MAX;
              for(int i=0;i<N*N;i++)
              {
                  if(chess_board[i]==0)
                  {
                      int temp_chess_board[N*N];
                      copy_chess_board(temp_chess_board,chess_board);
                      place_piece_on_board(temp_chess_board,i/N,i%N,(piece)test_node_type(chess_board));

                      temp_backed_up_value = get_backed_up_value_of_node(temp_chess_board,h+1,depth,ancestor,root_type);

                      if(test_node_type(chess_board)==1)
                      {
                          if(temp_backed_up_value>max_node_value)
                              max_node_value = temp_backed_up_value;
                      }
                      else
                      {
                          if(temp_backed_up_value<min_node_value)
                              min_node_value = temp_backed_up_value;
                      }

                      // choose alpha or beta pruning
                      bool continue_search = true;
                      if(test_node_type(chess_board)==1)
                      {
                          for(int j=0;j<h;j++)
                          {
                              if(max_node_value>ancestor[j]&&((j%2!=0&&root_type==1)||(j%2==0&&root_type==-1)))
                              {
                                  continue_search = false;
                                  break;
                              }
                          }
                      }
                      else
                      {
                          for(int j=0;j<h;j++)
                          {
                              if(min_node_value<ancestor[j]&&((j%2==0&&root_type==1)||(j%2!=0&&root_type==-1)))
                              {
                                  continue_search = false;
                                  break;
                              }
                          }
                      }
                      if(continue_search==false)
                          break;
                  }
                  else
                      ;
              }
              // when finish searching, update the value of alpha and beta
              if(test_node_type(chess_board)==1)
              {
                  if(h-1>=0)
                  {
                      if(root_type==1)
                      {
                          if((h-1)%2==0)
                          {
                              if(max_node_value>ancestor[h-1])
                                  ancestor[h-1] = max_node_value;
                          }
                          else
                          {
                              if(max_node_value<ancestor[h-1])
                                  ancestor[h-1] = max_node_value;
                          }
                      }
                      else
                      {
                          if((h-1)%2==0)
                          {
                              if(max_node_value<ancestor[h-1])
                                  ancestor[h-1] = max_node_value;
                          }
                          else
                          {
                              if(max_node_value>ancestor[h-1])
                                  ancestor[h-1] = max_node_value;
                          }
                      }
                  }
                  return max_node_value;
              }
              else
              {
                  if(h-1>=0)
                  {
                      if(root_type==-1)
                      {
                          if((h-1)%2==0)
                          {
                              if(min_node_value<ancestor[h-1])
                                  ancestor[h-1] = min_node_value;
                          }
                          else
                          {
                              if(min_node_value>ancestor[h-1])
                                  ancestor[h-1] = min_node_value;
                          }
                      }
                      else
                      {
                          if((h-1)%2==0)
                          {
                              if(min_node_value>ancestor[h-1])
                                  ancestor[h-1] = min_node_value;
                          }
                          else
                          {
                              if(min_node_value<ancestor[h-1])
                                  ancestor[h-1] = min_node_value;
                          }
                      }
                  }
                  return min_node_value;
              }
          }
      }

      int evaluation_function(int * chess_board)
      {
          if(win(chess_board))
              return MAX;
          else if(win(chess_board)==-1)
              return MIN;
          else
          {
              int temp = BP_net_value(chess_board)*MAX;
              return temp;
              
              // Using evaluation function to replace BP neuron network
              /*int i, j;
              int A_win = 0, B_win = 0;
              int temp_chess_board[N*N];
              for(i=0;i<N;i++)
              {
                  for(j=0;j<N;j++)
                  {
                      temp_chess_board[i*N+j] = chess_board[i*N+j];
                      if(temp_chess_board[i*N+j]!=B)
                          place_piece_on_board(temp_chess_board,i,j,A);
                  }
              }
              for(i=0;i<N;i++)
              {
                  bool check_row_flag = true;
                  for(j=0;j<N-1;j++)
                  {
                      if(temp_chess_board[i*N+j]!=temp_chess_board[i*N+j+1])
                      {
                          check_row_flag = false;
                          break;
                      }
                  }
                  if(check_row_flag&&temp_chess_board[i*N+j]==A)
                      A_win ++;
              }
              for(j=0;j<N;j++)
              {
                  bool check_line_flag = true;
                  for(i=0;i<N-1;i++)
                  {
                      if(temp_chess_board[i*N+j]!=temp_chess_board[(i+1)*N+j])
                      {
                          check_line_flag = false;
                          break;
                      }
                  }
                  if(check_line_flag&&temp_chess_board[i*N+j]==A)
                      A_win ++;
              }
              bool check_diagonal_flag = true;
              for(i=0;i<N-1;i++)
              {
                  if(temp_chess_board[i*N+i]!=temp_chess_board[(i+1)*N+i+1])
                  {
                      check_diagonal_flag = false;
                      break;
                  }
              }
              if(check_diagonal_flag&&temp_chess_board[i*N+i]==A)
                  A_win ++;
              check_diagonal_flag = true;
              for(i=0;i<N-1;i++)
              {
                  if(temp_chess_board[i*N+(N-1-i)]!=temp_chess_board[(i+1)*N+(N-1-(i+1))])
                  {
                      check_diagonal_flag = false;
                      break;
                  }
              }
              if(check_diagonal_flag && temp_chess_board[i*N+(N-1-i)]==A)
                  A_win ++;

              for(i=0;i<N;i++)
              {
                  for(j=0;j<N;j++)
                  {
                      temp_chess_board[i*N+j] = chess_board[i*N+j];
                      if(temp_chess_board[i*N+j]!=A)
                          place_piece_on_board(temp_chess_board,i,j,B);
                  }
              }
              for(i=0;i<N;i++)
              {
                  bool check_row_flag = true;
                  for(j=0;j<N-1;j++)
                  {
                      if(temp_chess_board[i*N+j]!=temp_chess_board[i*N+j+1])
                      {
                          check_row_flag = false;
                          break;
                      }
                  }
                  if(check_row_flag&&temp_chess_board[i*N+j]==B)
                      B_win ++;
              }
              for(j=0;j<N;j++)
              {
                  bool check_line_flag = true;
                  for(i=0;i<N-1;i++)
                  {
                      if(temp_chess_board[i*N+j]!=temp_chess_board[(i+1)*N+j])
                      {
                          check_line_flag = false;
                          break;
                      }
                  }
                  if(check_line_flag&&temp_chess_board[i*N+j]==B)
                      B_win ++;
              }
              check_diagonal_flag = true;
              for(i=0;i<N-1;i++)
              {
                  if(temp_chess_board[i*N+i]!=temp_chess_board[(i+1)*N+i+1])
                  {
                      check_diagonal_flag = false;
                      break;
                  }
              }
              if(check_diagonal_flag&&temp_chess_board[i*N+i]==B)
                  B_win ++;
              check_diagonal_flag = true;
              for(i=0;i<N-1;i++)
              {
                  if(temp_chess_board[i*N+(N-1-i)]!=temp_chess_board[(i+1)*N+(N-1-(i+1))])
                  {
                      check_diagonal_flag = false;
                      break;
                  }
              }
              if(check_diagonal_flag&&temp_chess_board[i*N+(N-1-i)]==B)
                  B_win ++;
              return A_win - B_win;*/
          }
      }
      int full(int * chess_board) {
      int i,flag = 0;
      for(i=0;i<N*N;i++) {
          if(chess_board[i]==0) {
              flag = 1;
              break;
          }
      }
      if(flag) return 0;
      return 1;
      }

      void copy_chess_board(int * chess_board1, int * chess_board2) {
      int i=0;
      for(i= 0 ;i<N*N ;i++)
          chess_board1[i] = chess_board2[i];
      }
