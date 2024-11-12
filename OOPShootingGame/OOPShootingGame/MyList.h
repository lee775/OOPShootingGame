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
			//���ڷ� ���� ��������͸� ����
			if (node != nullptr)
				_node = node;
		}

		const iterator operator++(int)
		{
			//��尡 ����Ű�� ���� ��ĭ ������ �а� ���Ű��� ����
			iterator iter(_node);
			_node = _node->_next;
			return iter;
		}

		iterator& operator++()
		{
			//��尡 ����Ű�� ���� ��ĭ ������ �а� ����
			_node = _node->_next;
			return *this;
		}

		const iterator operator--(int)
		{
			//��尡 ����Ű�� ���� ��ĭ �ڷ� ���� ���Ű��� ����
			iterator iter(_node);
			_node = _node->_prev;
			return iter;
		}

		iterator& operator--()
		{
			//��尡 ����Ű�� ���� ��ĭ �ڷ� ���� ����
			_node = _node->_prev;
			return *this;
		}

		const T& operator*() const
		{
			//��忡 ����� �����͸� ��������
			return _node->_data;
		}

		T& operator*()
		{
			//��忡 ����� �����͸� ��������
			return _node->_data;
		}

		const T* operator->() const
		{
			//��忡 ����� ���� ��ü�϶� ��ü�� �ٷ� �����ϱ� ���� ���
			return &(_node->_data);
		}

		T* operator->() 
		{
			//��忡 ����� ���� ��ü�϶� ��ü�� �ٷ� �����ϱ� ���� ���
			return &(_node->_data);
		}

		bool operator==(const iterator& other)
		{
			//��忡 ����� ������ ���� ���� ����� ����
			return _node == other._node;
		}
		bool operator!=(const iterator& other)
		{
			//��忡 ����� ������ ���� ���� ����� ����
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
		// ù��° ������ ��带 ����Ű�� ���ͷ����� ����
		iterator iter(_head._next);
		return iter;
	}
	iterator end()
	{
		// Tail ��带 ����Ű��(�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
		// 	�Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����
		iterator iter(&_tail);
		return iter;
	}
	iterator next(iterator iter)
	{
		//�Ű������� ���� ���ͷ������� ���� ��� ����
		return ++iter;
	}
	void push_front(T data)
	{
		//��� ���� �� �� ����
		Node* node = new Node;
		node->_data = data;
		node->_prev = &_head;
		// head�� ����Ʈ�� tail�� ����Ű�� ���� ������ ���� �̹� �ֱ⶧���� �ش� ���� ����Ű�� ��ġ�� ����
		// tail�� ����Ű�� ������ tail���� head�� ����Ű�� ������ �̱� ������ ��带 ����Ű���� ����
		//if (_head._next != &_tail)
		//	_head._next->_prev = node;
		//else
		//	_tail._prev = node;
		if (_size != 0)
			_head._next->_prev = node;
		else
			_tail._prev = node;

		//���� head�� ����Ű�� �ִ� ���� ���� �ű�� head�� ����Ű�� ��ġ�� ��带 �־���
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
		//head�� tail�� ����Ű�� ������ ���� ���� ������
		if (_head._next == &_tail)
			return;

		// ��带 �����ϱ� ���� head�� ����Ű�� ���� ��尡 ����Ű�� ������ ����
		Node* node = _head._next;
		_head._next = node->_next;

		//��尡 ������� ����� �տ� �ִ� ���� ������ ����� ��带 ����Ű�� ������ ����� ����� ���� �̸� ����Ű���� ����
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
		//ù��°���� ���������� �����ϱ� ���� ù��° ��带 ������
		Node* current = _head._next;

		while (current != &_tail) {
			Node* nextNode = current->_next;
			delete current;
			current = nextNode;
		}

		// ��� ��带 ������ ��, head�� tail�� �ٽ� ����
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


	// 	- ���ͷ������� �� ��带 ����.
	// 	- �׸��� ���� ����� ���� ��带 ī��Ű�� ���ͷ����� ����
	iterator erase(iterator iter)
	{
		//������� node�� �յڸ� �������ֱ� ���� �� node�� iterator�� ��� ������
		iterator nextIter(iter);
		iterator prevIter(iter);
		nextIter++;
		prevIter--;
		//���� ����ǵ��� ����
		prevIter._node->_next = nextIter._node;
		nextIter._node->_prev = prevIter._node;

		//��� iterator����
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
