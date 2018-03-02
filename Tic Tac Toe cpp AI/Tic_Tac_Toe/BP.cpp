#include <math.h>

#include "macro.h"
#include "time.h"
#include <stdlib.h>

#define ETA 0.7
#define MAX_ITERATION_TIME 1000000

double input_layer[N*N];
double competition_layer[N*N];
double output_layer;

double y_i;
double theta_i;
double x_ji[N*N];
double w_ji[N*N];
double y_j[N*N];
double theta_j[N*N];
double x_kj[N*N][N*N];
double w_kj[N*N][N*N];
double d_i;

int i,j,k;

// Initialize neuron
void init_neuron()
{
    for(i=0;i<N*N;i++)
    {
        input_layer[i] = 0.0;
        competition_layer[i] = 0.0;
    }
    output_layer = 0.0;
}

//Initialize the init_weight(). Every weight is float type and it close to 0
void init_weight()
{
    srand((int)time(0));
    for(i=0;i<N*N;i++)
    {
        w_ji[i] = (double)rand()/RAND_MAX;
    }
    for(i=0;i<N*N;i++)
    {
        for(j=0;j<N*N;j++)
        {
            w_kj[i][j] = (double)rand()/RAND_MAX;
        }
    }
    return ;
}


// Activation fuction using Hyperbolic tangent function, finial step for each layer of the network

double logistic_S(double theta)
{
    return (1.0-exp(-theta))/(1.0+exp(-theta));
}

// adapt BP network weights. The iteration time will be no more than MAX_ITERATION_TIME
// Input: The chess_board state and the back-up value of the chess_board.
void adapt_BP_net_weight(int * chess_board,int backed_up_value)
{
    int iteration_times = MAX_ITERATION_TIME;
    int temp_iteration_times = 0;
    while(1)
    {
        init_neuron();
        // network goes forward
        for(i=0;i<N*N;i++)
        {
            input_layer[i] = (double)chess_board[i];
        }
        for(k=0;k<N*N;k++)
        {
            for(j=0;j<N*N;j++)
            {
                x_kj[k][j] = input_layer[k];
                competition_layer[j] += w_kj[k][j]*x_kj[k][j];
                theta_j[j] = competition_layer[j];
            }
        }
        for(j=0;j<N*N;j++)
        {
            y_j[j] = logistic_S(competition_layer[j]);
            x_ji[j] = y_j[j];
            output_layer += w_ji[j]*x_ji[j];
            theta_i = output_layer;
        }
        y_i = logistic_S(theta_i);
        
        
        // Stop if the loss is good enough or iteration time is up to MAX_ITERATION_TIME
        if(fabs(y_i-(double)backed_up_value/(double)MAX)<0.000001||temp_iteration_times>=iteration_times)
            break;
        
        // network goes backward. start back propagation
        for(j=0;j<N*N;j++)
        {
            w_ji[j] = w_ji[j] - ETA*x_ji[j]*(1-y_i*y_i)*(y_i-(double)backed_up_value/(double)MAX);
        }
        for(k=0;k<N*N;k++)
        {
            for(j=0;j<N;j++)
            {
                w_kj[k][j] = w_kj[k][j] - ETA*x_kj[k][j]*(1-y_j[j]*y_j[j])*w_ji[j]*(1-y_i*y_i)*(y_i-(double)backed_up_value/(double)MAX);
            }
        }
        temp_iteration_times ++;
    }
    return ;
}


// BP neuron network.
// Input: the state of chess_board
// Output: the value we got go through the whole network
double BP_net_value(int * chess_board)
{
    init_neuron();
    for(i=0;i<N*N;i++)
    {
        input_layer[i] = (double)chess_board[i];
    }
    for(k=0;k<N*N;k++)
    {
        for(int j=0;j<N*N;j++)
        {
            x_kj[k][j] = input_layer[k];
            competition_layer[j] += w_kj[k][j]*x_kj[k][j];
            theta_j[j] = competition_layer[j];
        }
    }
    for(j=0;j<N*N;j++)
    {
        y_j[j] = logistic_S(competition_layer[j]);
        x_ji[j] = y_j[j];
        output_layer += w_ji[j]*x_ji[j];
        theta_i = output_layer;
    }
    y_i = logistic_S(theta_i);
    
    return y_i;
}
