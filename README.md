# Mini-Neural-Network-Implementation-Using-Multiprocessing-And-Multithreading
## SETUP
g++ -o [object name] [.cpp file] 

## DEMO

Go to the drive link for the video demo: https://drive.google.com/drive/folders/1NFwF6Z-VGZzXzR6khGxVknUHu65-k6QU?usp=sharing


## Description
☞ This is an implementation of a mini neural network developed in C++ <br>
☞ Different layers are represented by different processes and each neuron in a layer is represented by a new thread <br>
☞ Synchronization is implemented using semaphores <br>
☞ Output of one process (layer) becomes the input of the second process (layer). This continues until we reach the output layer<br>
☞ The number of layers and threads are dynamic  <br>

## TO-DO
📝 Implement IPC <br>
📝 Back Propagation
