# Mini-Neural-Network-Implementation-Using-Multiprocessing-And-Multithreading

## Description
☞ This is an implementation of a mini neural network developed in C++
☞ Different layers are represented by different processes and each neuron in a layer is represented by a new thread
☞ Synchronization is implemented using semaphores
☞ Output of one process (layer) becomes the input of the second process (layer). This continues until we reach the output layer.
☞ The number of layers and threads are dynamic  
