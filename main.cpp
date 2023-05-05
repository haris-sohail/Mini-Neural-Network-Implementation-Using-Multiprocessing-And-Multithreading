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
    
    // make layers

    for(int layerNum = 0; layerNum < topology.size(); layerNum++)
    {

        pthread_t tid[topology[layerNum] + 1]; // 1 extra for the bias neuron

        // create layers
        if(fork() == 0)
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

            exit(0);
        }
    }

    for(int layerNum = 0; layerNum < topology.size(); layerNum++)
    {
        wait(NULL);
    }
    
    
}