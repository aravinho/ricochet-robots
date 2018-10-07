
"""
A Queue class to use for BFS.
"""
class Queue:

	def __init__(self, max_capacity=10000000):
		"""
		Allocates space for MAX_CAPACITY nodes.
		"""
		self.front = 0
		self.back = 0
		self.capacity = 0
		self.max_capacity = max_capacity
		self.queue = [None for pos in range(max_capacity)]

	def enqueue(self, element):
		"""
		Adds this element to the back of the queue.
		Errors if this new element exceeds max capacity.
		"""
		assert self.capacity < self.max_capacity, "Capacity exceeded for queue"
		self.queue[self.back] = element
		self.back = (self.back + 1) % self.max_capacity
		self.capacity += 1

	def dequeue(self):
		"""
		Returns the element at the front of the queue.
		Errors if the queue is empty.
		"""
		assert not self.empty(), "Cannot dequeue from an empty queue"
		ret = self.queue[self.front]
		self.front = (self.front + 1) % self.max_capacity
		self.capacity -= 1
		return ret

	def empty(self):
		"""
		Returns whether this queue is empty.
		"""
		return self.capacity <= 0


