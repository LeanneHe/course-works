/* Heap (Priority Queue Representation) */

//The following code provides a demo for the BST class:

#include<iostream>
#include"priority_queue.hpp"

int main() {
	//Used to set queue size:
	int queue_size;
	
	//flags for demo
	char continue_flag;
	bool valid_choice = false;
	char choice;
	
	std::cout << "Please select a queue size: ";
	std::cin >> queue_size;
	//initialize binary search tree structure
	PriorityQueue queue(queue_size);
	
	//Main loop where users can interact with BST object
	while(true) {
	
		std::cout << "q - queue\nd - remove\ns - get size\np - print queue to console\nx - exit\n";
			
		// Menu
		do {
			std::cin >> choice;
			switch (choice) {
			case 'q':
				//insert
				int insert_value;
				std::cout << "Enter a value to insert: ";
				std::cin >> insert_value;
				if(queue.enqueue(insert_value)) std::cout << "\nValue successfully queued.\n";
				else std::cout << "\nValue failed to queue. Try dequeueing.\n";
				valid_choice = true;
				break;
			case 'd':
				if(queue.dequeue()) std::cout << "\nQueue successfully dequeued.\n";
				else std::cout << "\nQueue failed to dequeue. Try enqueuing.\n";
				valid_choice = true;
				break;
			case 's':
				//return size
				std::cout << "Size of Queue: ";
				std::cout << queue.size() << std::endl;
				valid_choice = true;
				break;
			case 'p':
				queue.print();
				std::cout << std::endl;
				valid_choice = true;
				break;
			case 'x':
				std::cout << "\nQuiting Heap Demo...\n\n";
				return(0);
			default:
				std::cout << "You did not enter a valid choice! Please try again: ";
				valid_choice = false;
				break;
			} 
		} while(valid_choice == false); //prompt if input is incorrect
	}
}
