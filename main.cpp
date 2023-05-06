#include<iostream>
#include<vector>
#include<pthread.h>

#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

using namespace std;

void getTopology(vector<unsigned>* topology)
{
    unsigned countOfNeurons;

    // make layers i.e. processes

    cout << "--- Input the topology ---" << endl << endl;

    cout << "0 to exit input" << endl << endl;
    for(int i = 0; ; i++)
    {
        cout << "Enter the number of neurons in the " << i << "th layer: ";
        cin >> countOfNeurons;

        if(countOfNeurons != 0)
        {
            (*topology).push_back(countOfNeurons);
        }
        else
        {
            break;
        }
    }   
}

struct Connection 
{
    double weight;
    double deltaWeight;
};

struct s_outputWeight
{
    vector<Connection> outputWeights;
};

void getInputVals(vector<double>* inputVals)
{
    cout << endl << "--- Taking input ---" << endl << endl;
    cout << "0 to exit." << endl << endl;

    int inputVal = 1;

    while(1)
    {
        cout << "Enter the input value: ";
        cin >> inputVal;

        if(inputVal == 0)
        {
            break;
        }

        else
        {
            (*inputVals).push_back(inputVal);
        }
    }
}

void* func (void* args)
{
    cout << "Made a neuron !" << endl;

    pthread_exit(0);
}

int main()
{
    vector<unsigned> topology;
    getTopology(&topology);
    int numberOfLayers;

    // input the values for the input layer
    vector<double> inputVals;

    getInputVals(&inputVals);
    
    // make layers

    for(int layerNum = 0; layerNum < topology.size(); layerNum++)
    {

        pthread_t tid[topology[layerNum] + 1]; // 1 extra for the bias neuron

        // create layers
        if(fork() == 0)
        {
            // initially we want to give the first layer (input layer) the inputs

            if(layerNum == 0)
            {
                // create neurons
                for(int i = 0; i <= topology[layerNum]; i++)
                {
                    pthread_create(&tid[i], NULL, func, NULL);
                }
                
                for(int i = 0; i <= topology[layerNum]; i++)
                {
                    pthread_join(tid[i], NULL);
                }
            }

            // create neurons
            for(int i = 0; i <= topology[layerNum]; i++)
            {
                pthread_create(&tid[i], NULL, func, NULL);
            }
            
            for(int i = 0; i <= topology[layerNum]; i++)
            {
                pthread_join(tid[i], NULL);
            }

            exit(0);
        }
    }

    for(int layerNum = 0; layerNum < topology.size(); layerNum++)
    {
        wait(NULL);
    }
    
    
}