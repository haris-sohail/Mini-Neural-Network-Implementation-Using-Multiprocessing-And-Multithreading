#include<iostream>
#include<vector>
#include<pthread.h>
#include<fstream>
#include<string>
#include<semaphore.h>

#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

using namespace std;

sem_t sem; 

int neuronNum = 0;

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
    double* outputWeights;
    Connection* weights;
    int size, weightNo, noNeurons_next;

    s_outputWeight(int numOutput)
    {
        size = numOutput;
        outputWeights = new double [numOutput];

        for(int i = 0; i < size; i++)
        {
            outputWeights[i] = 0;
        }
    }

    void setWeightsNo(int num)
    {
        weights = new Connection[num];

        weightNo = num;
    }

    int getWeightNo()
    {
        return weightNo;
    }

    void setWeights(string filename)
    {
        ifstream in(filename);

        if(in)
        {
            string line;

            for(int i = 0; getline(in, line); i++)
            {
                weights[i].weight = stod(line);
            }
        }

        else
        {
            cout << endl << "Weight file not opened successfully" << endl;
        }
    }

    void setOutputWeights(vector<double> vals)
    {
        for(int i = 0; i < vals.size(); i++)
        {
            outputWeights[i] = vals[i];
        }
    }

    int getSize()
    {
        return size;
    }
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

void setOutput_andWrite(s_outputWeight* output, vector<double>* inputVals)
{
    ofstream out("output.txt");

    for(int i = 0; i < inputVals->size(); i++)
    {
        output->outputWeights[i] = ((*inputVals).at(i));

        out << output->outputWeights[i] << endl;
    }
}

void* func (void* args)
{
    cout << endl << "Made a neuron !" << endl;

    sem_wait(&sem); // Acquire the semaphore

    s_outputWeight* input = (s_outputWeight*) args;

    s_outputWeight* output = new s_outputWeight(input->noNeurons_next);

    for(int j = 0; j < input->getSize(); j++)
    {
        cout << "input->outputWeights: " << input->outputWeights[j] << endl << "input->weights[i].weight: " << input->weights[neuronNum].weight << endl; 
        output->outputWeights[neuronNum] += input->outputWeights[j] * input->weights[neuronNum].weight;

        cout << output->outputWeights[neuronNum] << endl;
    }

    neuronNum++;
    sem_post(&sem); // Release the semaphore
    pthread_exit(output);
}

void readFromFile(vector<double>* input)
{
    ifstream in("output.txt");

    string line;
    while(getline(in, line))
    {
        (*input).push_back(stod(line));
    }
}

int main()
{
    vector<unsigned> topology;
    getTopology(&topology);
    int numberOfLayers = topology.size();

    // input the values for the input layer
    vector<double> inputVals;

    getInputVals(&inputVals);

    if(inputVals.size() != topology[0])
    {
        cout << endl << "Invalid input. Problem: Number of inputs do not match with number of input neurons" << endl;

        return 0;
    }
    

    // set the output values for the first layer

    s_outputWeight output(inputVals.size());

    setOutput_andWrite(&output, &inputVals);


    // make layers

    for(int layerNum = 1; layerNum < topology.size(); layerNum++, neuronNum = 0) // starting from 1 because input layer's work (to take input) has already been done
    {

        pthread_t tid[topology[layerNum] + 1]; // 1 extra for the bias neuron

        // create layers
        if(fork() == 0)
        {
            // create neurons

            //read from the file

            vector<double> inputFromFile; // holds the input 

            readFromFile(&inputFromFile);

            // make input object for the thread

            s_outputWeight input(inputFromFile.size());

            input.setOutputWeights(inputFromFile);

            input.noNeurons_next = topology[layerNum]; // +1 not done because we are already in the next layer

            // now we set the weights of the neurons

            input.setWeightsNo(input.noNeurons_next);

            // get the filename of the corresponding layer

            string weightFileName = "weight" + to_string(layerNum) + ".txt";

            input.setWeights(weightFileName);

            sem_init(&sem, 0, 1); // Initialize the semaphore

            for(int i = 0; i <= topology[layerNum]; i++)
            {
                if(i != topology[layerNum])
                {
                    pthread_create(&tid[i], NULL, func, &input);
                }                
            }
            
            for(int i = 0; i <= topology[layerNum]; i++)
            {
                if(i != topology[layerNum])
                {
                    pthread_join(tid[i], NULL);
                }
            }

            sem_destroy(&sem); // Destroy the semaphore

            exit(0);
        }
    }

    for(int layerNum = 1; layerNum < topology.size(); layerNum++)
    {
        wait(NULL);
    }
    
    
}