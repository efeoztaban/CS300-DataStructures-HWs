
#ifndef STACK_GUARD
#define STACK_GUARD

template <class T>
struct Node {
    
    Node* next;
    T data;
	
	Node(T data, Node<T>* next) 
        : data(data), next(next) {}
};

template <class T>
class Stack
{
	public:

		Stack()
		{
			top = nullptr;
		};

		~Stack() 
		{
			clear();
		};

		void push(T data) 
		{
			Node<T>* newNode = new Node<T>(data, top);
			top = newNode;
		};

		void pop() {
			if ( !isEmpty() ) 
			{
				Node<T>* popped = top;
				T poppedData = popped -> data;
				top = popped -> next;

				delete popped;
			}

		};

		T getpop() {
			if ( !isEmpty() ) 
			{
				Node<T>* popped = top;
				T poppedData = popped -> data;
				top = popped -> next;
				
				delete popped;

				return poppedData;
			}

		};

		T getTop()
		{
			if ( !isEmpty() ) 
			{
				return top->data;
			}
		};
		bool isEmpty() 
		{
			if( top == nullptr)
				return true;
			else
				return false;
		};
  

	private:
		Node<T>* top;

		

		void clear()
		{
			while( !isEmpty() )
			{
				pop();
			}
		};
};
#endif


