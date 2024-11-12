#pragma once

template <typename T>
class List
{
public:
	struct Node
	{
		T  _data;
		Node* _prev;
		Node* _next;
	};

	class iterator
	{
	private:
		Node* _node;
		friend class List;
	public:
		iterator(Node* node = nullptr)
		{
			//인자로 들어온 노드포인터를 저장
			if (node != nullptr)
				_node = node;
		}

		const iterator operator++(int)
		{
			//노드가 가리키는 값을 한칸 앞으로 밀고 과거값을 리턴
			iterator iter(_node);
			_node = _node->_next;
			return iter;
		}

		iterator& operator++()
		{
			//노드가 가리키는 값을 한칸 앞으로 밀고 리턴
			_node = _node->_next;
			return *this;
		}

		const iterator operator--(int)
		{
			//노드가 가리키는 값을 한칸 뒤로 당기고 과거값을 리턴
			iterator iter(_node);
			_node = _node->_prev;
			return iter;
		}

		iterator& operator--()
		{
			//노드가 가리키는 값을 한칸 뒤로 당기고 리턴
			_node = _node->_prev;
			return *this;
		}

		const T& operator*() const
		{
			//노드에 저장된 데이터를 리턴해줌
			return _node->_data;
		}

		T& operator*()
		{
			//노드에 저장된 데이터를 리턴해줌
			return _node->_data;
		}

		const T* operator->() const
		{
			//노드에 저장된 값이 객체일때 객체를 바로 지정하기 위해 사용
			return &(_node->_data);
		}

		T* operator->() 
		{
			//노드에 저장된 값이 객체일때 객체를 바로 지정하기 위해 사용
			return &(_node->_data);
		}

		bool operator==(const iterator& other)
		{
			//노드에 저장된 데이터 값을 비교한 결과를 리턴
			return _node == other._node;
		}
		bool operator!=(const iterator& other)
		{
			//노드에 저장된 데이터 값을 비교한 결과를 리턴
			return _node != other._node;
		}
	};

public:
	List() : _size(0)
	{
		_head._next = &_tail;
		_tail._prev = &_head;
	}
	~List()
	{
		if (!empty())
			clear();
	}

	iterator begin()
	{
		// 첫번째 데이터 노드를 가리키는 이터레이터 리턴
		iterator iter(_head._next);
		return iter;
	}
	iterator end()
	{
		// Tail 노드를 가리키는(데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
		// 	또는 끝으로 인지할 수 있는 이터레이터를 리턴
		iterator iter(&_tail);
		return iter;
	}
	iterator next(iterator iter)
	{
		//매개변수로 받은 이터레이터의 다음 노드 리턴
		return ++iter;
	}
	void push_front(T data)
	{
		//노드 생성 및 값 세팅
		Node* node = new Node;
		node->_data = data;
		node->_prev = &_head;
		// head가 리스트의 tail을 가리키고 있지 않으면 값이 이미 있기때문에 해당 값이 가리키는 위치도 수정
		// tail을 가리키고 있으면 tail또한 head를 가리키고 있을것 이기 때문에 노드를 가리키도록 해줌
		//if (_head._next != &_tail)
		//	_head._next->_prev = node;
		//else
		//	_tail._prev = node;
		if (_size != 0)
			_head._next->_prev = node;
		else
			_tail._prev = node;

		//기존 head가 가리키고 있던 값을 노드로 옮기고 head가 가리키는 위치에 노드를 넣어줌
		node->_next = _head._next;
		_head._next = node;

		_size++;
	}
	void push_back(T data)
	{
		Node* node = new Node;
		node->_data = data;
		node->_next = &_tail;

		//if (_tail._prev != &_head)
		//	_tail._prev->_next = node;
		//else
		//	_head._next = node;
		if (_size != 0)
			_tail._prev->_next = node;
		else
		{
			_head._next = node;
		}

		node->_prev = _tail._prev;
		_tail._prev = node;

		_size++;
	}
	void pop_front()
	{
		//head가 tail을 가리키고 있으면 값이 없는 상태임
		if (_head._next == &_tail)
			return;

		// 노드를 삭제하기 위해 head가 가리키는 값을 노드가 가리키던 값으로 변경
		Node* node = _head._next;
		_head._next = node->_next;

		//노드가 사라지면 노드의 앞에 있던 노드는 여전히 사라진 노드를 가리키기 때문에 사라질 노드의 앞을 미리 가리키도록 해줌
		node->_next->_prev = node->_prev;
		delete node;
		_size--;
	}
	void pop_back()
	{
		if (_tail._prev == &_head)
			return;

		Node* node = _tail._prev;
		_tail._prev = node->_prev;

		node->_prev->_next = node->_next;
		delete node;
		_size--;
	}
	void clear()
	{
		//첫번째부터 순차적으로 삭제하기 위해 첫번째 노드를 가져옴
		Node* current = _head._next;

		while (current != &_tail) {
			Node* nextNode = current->_next;
			delete current;
			current = nextNode;
		}

		// 모든 노드를 삭제한 후, head와 tail을 다시 연결
		_head._next = &_tail;
		_tail._prev = &_head;

		_size = 0;
	}
	int size() { return _size; };

	bool empty()
	{
		if (_size != 0)
			return false;
		else
			return true;
	};


	// 	- 이터레이터의 그 노드를 지움.
	// 	- 그리고 지운 노드의 다음 노드를 카리키는 이터레이터 리턴
	iterator erase(iterator iter)
	{
		//사라지는 node의 앞뒤를 연결해주기 위해 두 node의 iterator를 모두 가져옴
		iterator nextIter(iter);
		iterator prevIter(iter);
		nextIter++;
		prevIter--;
		//서로 연결되도록 설정
		prevIter._node->_next = nextIter._node;
		nextIter._node->_prev = prevIter._node;

		//대상 iterator삭제
		delete iter._node;
		_size--;

		return nextIter;
	}

	void remove(T Data)
	{
		iterator iter;
		for (iter = this->begin(); iter != this->end(); ++iter)
		{
			if (*iter == Data)
			{
				erase(iter);
				break;
			}
		}
	}

private:
	int _size = 0;
	Node _head;
	Node _tail;
};
