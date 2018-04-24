//
// Created by paul on 24/04/18.
//

#include <cstdint>

#ifndef ROOT_CIRCULARBUFFER_H
#define ROOT_CIRCULARBUFFER_H


template <typename T, uint32_t BufferSize, typename CounterType = uint32_t>
class CircularBuffer {
public:
	CircularBuffer() : _head(0), _tail(0), _full(false) {}

	~CircularBuffer() {}

	/** Push the transaction to the buffer. This overwrites the buffer if it's
	 *  full
	 *
	 * @param data Data to be pushed to the buffer
	 */
	void push(const T& data) {
		if(full()) {
			_tail++;
			_tail %= BufferSize;
		}
		_pool[_head++] = data;
		_head %= BufferSize;
		if(_head == _tail) {
			_full = true;
		}
	}

	/** Pop the transaction from the buffer
	 *
	 * @param data Data to be pushed to the buffer
	 * @return True if the buffer is not empty and data contains a transaction, false otherwise
	 */
	bool pop(T& data) {
		bool data_popped = false;
		if(!empty()) {
			data = _pool[_tail++];
			_tail %= BufferSize;
			_full = false;
			data_popped = true;
		}
		return data_popped;
	}

	/** Check if the buffer is empty
	 *
	 * @return True if the buffer is empty, false if not
	 */
	bool empty() const {
		bool is_empty = (_head == _tail) && !_full;
		return is_empty;
	}

	/** Check if the buffer is full
	 *
	 * @return True if the buffer is full, false if not
	 */
	bool full() const {
		bool full = _full;
		return full;
	}

	/** Reset the buffer
	 *
	 */
	void reset() {
		_head = 0;
		_tail = 0;
		_full = false;
	}

private:
	T _pool[BufferSize];
	volatile CounterType _head;
	volatile CounterType _tail;
	volatile bool _full;
};

/**@}*/

/**@}*/

#endif // ROOT_CIRCULARBUFFER_H
