from utils import Queue 

"""

class Board:
- 4 quadrants
Board(quadrant1, quadrant2, quadrant3, quadrant4)
	- copies the 4 quadrants into one larger board
Board.solve(yellow_pos, red_pos, green_pos, blue_pos, dest_color, dest_piece, num_sols=3)
returns an array of NUM_SOLS Solution objects

Board.validMoves(robot_positions)
returns an array of 4-tuples.  each 4-tuple contains 4 (x, y) ordered pairs (yellow, red, green, blue)

Board.nextBoard(solution)
advances the state to the next board

"""

def diagDirection(diag):
	""" Returns whether this diag is forward or backward (from the perspective of looking at the board). """
	assert len(diag) == 3, "Diag must have 3 elements"
	return diag[2]

"""
Represents the position of a single robot.
Includes row, column and if there is a diag in this spot, whether the robot is above or below it.
"""
class Position:

	def __init__(self, row, col, above_diag=True):
		assert 0 <= row < 16 and 0 <= col < 16, "row and col out of bounds for Position: (" + str((row, col))
		self.row = row 
		self.col = col
		self.above_diag = above_diag

	def position(self):
		return (self.row, self.col)

	def __eq__(self, other):
		assert other is not None, "Cannot compare equality to a None Position"
		return self.row == other.row and self.col == other.col

	def __ne__(self, other):
		return not self.__eq__(other)

	def __repr__(self):
		if self.above_diag:
			return str((self.row, self.col))
		else:
			return str((self.row, self.col))

"""
Represents a solution that may or may not be complete.
A particular instance represents one step, and has a parent pointer to the previous Solution step
"""
class Solution:

	def __init__(self, robots, prev_move=None, depth=0, moving_robot_color="", direction=""):
		""" 
		Creates a solution step in which the robots are at the given positions
		"""
		assert depth >= 0, "Cannot have negative depth for a solution"
		assert len(robots) == 4, "Must have 4 robots in every solution"
		assert "yellow" in robots and "red" in robots and "green" in robots and "blue" in robots, "Must have all 4 robot colors"
		self.depth = depth
		self.robots = robots 
		self.prev_move = prev_move
		self.moving_robot_color = moving_robot_color
		self.direction = direction

	def addMove(self, robot_color, robot_pos, moving_robot_color="", direction=""):
		"""
		Creates a new Solution in which the the given robot is at the given position.
		Sets the previous move of the new step to this Solution instance.
		Direction signifies what direction the piece moved to get here
		"""
		new_robot_positions = dict(self.robots)
		new_robot_positions[robot_color] = robot_pos
		return Solution(new_robot_positions, self, self.depth + 1, moving_robot_color, direction)

	def __repr__(self):
		return str(self.robots)

	def printPath(self):
		moves = []
		curr_move = self
		while curr_move != None:
			moves.append(curr_move)
			curr_move = curr_move.prev_move
		moves.reverse()

		num_moves = len(moves)
		for move in moves[1:]:
			print "Move", move.moving_robot_color, move.direction

def solve(board, robots, dest, dest_color, num_sols=3, max_depth=20):
	"""
	Robots denotes the position and orientation (above or below a diag) of the robots.
	The robot of color DEST_COLOR must get to the square given by DEST (an (x, y) tuple).
	A list of Solution objects are returned, NUM_SOLS many of them (the best, second best, etc.)
	If we haven't found a solution less than MAX_DEPTH, give up
	"""

	assert len(robots) == 4, "there must be 4 robots"
	assert dest_color in ["yellow", "red", "green", "blue"]
	assert num_sols >= 0 and max_depth >= 0, "Cannot have a negative number of solutions or max depth"

	print "Trying to move", dest_color, "robot to", dest

	solutions = []
	if num_sols == 0:
		return solutions

	# initialize the BFS fringe
	initial_sol = Solution(robots)
	bfs_fringe = Queue()
	bfs_fringe.enqueue(initial_sol)

	# in loop: dequeue, check if solution, add all next positions to queue
	curr_depth = 0
	num_sols_at_depth = 0
	while True:
		assert not bfs_fringe.empty(), "BFS fringe is empty"

		# dequeue the Solution at the front of the stack
		curr_sol = bfs_fringe.dequeue()

		# if it is a valid solution for the board, add it to the list of solutions to return
		if board.isSolution(curr_sol, dest, dest_color):
			solutions.append(curr_sol)
			if len(solutions) == num_sols:
				return solutions

		if curr_sol.depth > curr_depth:
			print "Examined", num_sols_at_depth, "solutions at depth", curr_depth
			curr_depth = curr_sol.depth
			num_sols_at_depth = 0

		# check that the depth of this solution does not exceed max depth
		num_sols_at_depth += 1
		if num_sols_at_depth % 100 == 0:
			print "Examined", num_sols_at_depth, "solutions at depth", curr_depth

		if curr_sol.depth > max_depth:
			break

		

		# for each of the possible robot moves, make the move, grab the new Solution and enqueue it
		next_moves = board.allNextMoves(curr_sol)
		for next_move in next_moves:
			bfs_fringe.enqueue(next_move)


	return solutions


def robotAt(row, col, robot_positions, exclude=[]):
	"""
	The positions of the robots are given by the dictionary (string --> Position) ROBOT_POSITIONS
	Returns whether one of the robots is at (row, col).
	Do not examine the robots whose colors are given in the EXCLUDE list.
	"""
	assert robot_positions is not None and type(robot_positions) == dict, "incorrect robot positions in robotAt"
	for robot_color in ["yellow", "green", "red", "blue"]:
		if robot_color in exclude:
			continue
		robot_pos = robot_positions[robot_color]
		if robot_pos.row == row and robot_pos.col == col:
			return True

	return False


"""
Represents a Diagonal barrier.
Has a color, row, column and direction (Forward like / or Back like a backslash)
"""
class Diag:

	def __init__(self, row, col, color, direction):
		self.color = color 
		self.row = row 
		self.col = col 
		self.direction = direction 



class Board:

	def __init__(self):
		pass 


	def isSolution(self, solution, dest, dest_color):
		"""
		Returns whether the given Solution is a solution to this board,
		if the goal is to get the DEST_COLOR robot to position DEST.

		"""
		assert solution is not None, "solution is None in Board.isSolution"
		assert dest is not None, "dest is None in Board.isSolution"
		assert dest_color in ["yellow", "red", "green", "blue"]
		return solution.robots[dest_color] == dest

	def hasRowBarrier(self, row, col):
		"""
		Return whether there is a row barrier (vertical barrier) at the given position.
		"""
		return col in self.row_barriers[row]

	def hasColBarrier(self, row, col):
		"""
		Return whether there is a col barrier (horizontal barrier) at the given position.
		"""
		return row in self.col_barriers[col]

	def hasDiag(self, row, col):
		"""
		Returns whether there is a Diag at this position.
		"""
		for diag in self.row_diags[row]:
			if diag.col == col:
				return True

		return False

	def getDiag(self, row, col):
		"""
		Return the diag at the given position, otherwise None
		"""

		for diag in self.row_diags[row]:
			if diag.col == col:
				return diag 

		return None


	def allNextMoves(self, sol):
		"""
		Returns a list of solutions representing all the solution steps that can be reached on this board
		from the given SOL.
		"""
		assert sol is not None, "Cannot have a None solution in Board.allNextMoves"
		next_moves = []

		# try all yellow moves
		yellow_pos = sol.robots["yellow"]
		yellow_moves = self.allMovesForRobot("yellow", sol)


		# try all red moves
		red_pos = sol.robots["red"]
		red_moves = self.allMovesForRobot( "red", sol)


		# try all green moves
		green_pos = sol.robots["green"]
		green_moves = self.allMovesForRobot("green", sol)


		# try all blue moves
		blue_pos = sol.robots["blue"]
		blue_moves = self.allMovesForRobot("blue", sol)


		all_moves = yellow_moves + red_moves + green_moves + blue_moves
		return all_moves

	def allMovesForRobot(self, robot_color, curr_sol):
		"""
		Returns a list of Solution objects, each corresponding to a new position the given robot could take.
		The robots are currently at the positions specified in CURR_SOL.
		Each of the solutions have a parent pointer to the given CURR_SOL.
		"""
		assert curr_sol is not None, "curr_sol is None in Board.allMovesForRobot"
		assert robot_color in ["yellow", "red", "green", "blue"], "Invalid robot color"

		robot_pos = curr_sol.robots[robot_color]
		robot_row = robot_pos.row
		robot_col = robot_pos.col

		all_new_solutions = []

		temp_positions_east = dict(curr_sol.robots)
		east_move_position = self.eastMove(robot_color, temp_positions_east)
		# if the robot has actually moved, add the new solution
		if east_move_position != robot_pos:
			east_move_solution = curr_sol.addMove(robot_color, east_move_position, moving_robot_color=robot_color, direction="east")
			all_new_solutions.append(east_move_solution)


		temp_positions_west = dict(curr_sol.robots)
		west_move_position = self.westMove(robot_color, temp_positions_east)
		# if the robot actually moved, add the new solution
		if west_move_position != robot_pos:
			west_move_solution = curr_sol.addMove(robot_color, west_move_position, moving_robot_color=robot_color, direction="west")
			all_new_solutions.append(west_move_solution)

		temp_positions_north = dict(curr_sol.robots)
		north_move_position = self.northMove(robot_color, temp_positions_north)
		# if the robot actually moved, add the new solution
		if north_move_position != robot_pos:
			north_move_solution = curr_sol.addMove(robot_color, north_move_position, moving_robot_color=robot_color, direction="north")
			all_new_solutions.append(north_move_solution)

		temp_positions_south = dict(curr_sol.robots)
		south_move_position = self.southMove(robot_color, temp_positions_south)
		# if the robot actually moved, add the new solution
		if south_move_position != robot_pos:
			south_move_solution = curr_sol.addMove(robot_color, south_move_position, moving_robot_color=robot_color, direction="south")
			all_new_solutions.append(south_move_solution)

		return all_new_solutions


	def southMove(self, moving_robot_color, robot_positions):
		"""
		Returns the Position in which the given robot ends up if it moves north.
		The robots' initial positions are given by robot_positions.
		Assumes that the robot_positions dict can be modified.
		"""
		assert robot_positions is not None and type(robot_positions) == dict, "incorrect robot_positions in Board.southMove"
		assert moving_robot_color in ["yellow", "red", "green", "blue"], "Invalid robot color"


		moving_robot_pos = robot_positions[moving_robot_color]
		moving_robot_row = moving_robot_pos.row
		moving_robot_col = moving_robot_pos.col
		moving_robot_above_diag = moving_robot_pos.above_diag

		# look for the first barrier north
		# it could be a col barrier (horizontal barrier), a diag or another robot
		first_barrier_south = 16
		first_barrier_type = "barrier"

		for row in range(moving_robot_row, 16):


			# check if there is a robot already on that square
			is_blocker_robot = robotAt(row, moving_robot_col, robot_positions, exclude=[moving_robot_color])
			if is_blocker_robot:
				first_barrier_south = row 
				first_barrier_type = "robot"
				break

			# check if there is a diag
			if self.hasDiag(row, moving_robot_col):
				diag = self.getDiag(row, moving_robot_col)

				# if the diag is of the same color, it passes through
				# important to change the above/below diag, in case there is a same-color diag followed immediately by barrier
				if diag.color == moving_robot_color:
					if diag.direction == "forward":
						moving_robot_above_diag = False
					elif diag.direction == "back":
						moving_robot_above_diag = False
				
				# if the diag is of another color it will deflect
				if diag.color != moving_robot_color:

					if diag.direction == "forward":
						# if the robot is initially on a forward diag square, and the robot is already below that diag,
						# nothing will happen
						if row == moving_robot_row and not moving_robot_above_diag: 
							pass
						else:
							# otherwise, the robot will start going west
							first_barrier_south = row
							first_barrier_type = "forward"
							moving_robot_above_diag = True
							break

					if diag.direction == "back":
						# if the robot is initially on a back diag square, and the robot is already above that diag,
						# there is no barrier here
						if row == moving_robot_row and not moving_robot_above_diag:
							pass
						else:
						# otherwise, the robot will start going east
							first_barrier_south = row 
							first_barrier_type = "back"
							moving_robot_above_diag = True
							break

			# if neither a robot nor a diag blocked, check if there is a barrier
			if self.hasColBarrier(row + 1, moving_robot_col):
				first_barrier_south = row + 1
				first_barrier_type = "barrier"
				break

		# Based on the barrier type, decide what to do
		
		# if barrier or blocker robot, the moving robot stops
		if first_barrier_type == "barrier":
			new_position = Position(first_barrier_south - 1, moving_robot_col, moving_robot_above_diag)
			return new_position

		elif first_barrier_type == "robot":
			new_position = Position(first_barrier_south - 1, moving_robot_col, moving_robot_above_diag)
			return new_position

		elif first_barrier_type == "forward":
			new_position = Position(first_barrier_south, moving_robot_col, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.westMove(moving_robot_color, robot_positions)

		elif first_barrier_type == "back":
			new_position = Position(first_barrier_south, moving_robot_col, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.eastMove(moving_robot_color, robot_positions)

		assert False, "Barrier type is invalid"	





	def northMove(self, moving_robot_color, robot_positions):
		"""
		Returns the Position in which the given robot ends up if it moves north.
		The robots' initial positions are given by robot_positions.
		Assumes that the robot_positions dict can be modified.
		"""
		assert robot_positions is not None and type(robot_positions) == dict, "incorrect robot_positions in Board.northMove"
		assert moving_robot_color in ["yellow", "red", "green", "blue"], "Invalid robot color"


		moving_robot_pos = robot_positions[moving_robot_color]
		moving_robot_row = moving_robot_pos.row
		moving_robot_col = moving_robot_pos.col
		moving_robot_above_diag = moving_robot_pos.above_diag

		# look for the first barrier north
		# it could be a col barrier (horizontal barrier), a diag or another robot
		first_barrier_north = 0
		first_barrier_type = "barrier"

		for row in range(moving_robot_row, -1, -1):


			# check if there is a robot already on that square
			is_blocker_robot = robotAt(row, moving_robot_col, robot_positions, exclude=[moving_robot_color])
			if is_blocker_robot:
				first_barrier_north = row 
				first_barrier_type = "robot"
				break

			# check if there is a diag
			if self.hasDiag(row, moving_robot_col):
				diag = self.getDiag(row, moving_robot_col)

				# if the diag is of the same color, it passes through
				# important to change the above/below diag, in case there is a same-color diag followed immediately by barrier
				if diag.color == moving_robot_color:
					if diag.direction == "forward":
						moving_robot_above_diag = True
					elif diag.direction == "back":
						moving_robot_above_diag = True
				
				# if the diag is of another color it will deflect
				if diag.color != moving_robot_color:

					if diag.direction == "forward":
						# if the robot is initially on a forward diag square, and the robot is already below that diag,
						# nothing will happen
						if row == moving_robot_row and moving_robot_above_diag: 
							pass
						else:
							# otherwise, the robot will start going east
							first_barrier_north = row
							first_barrier_type = "forward"
							moving_robot_above_diag = False
							break

					if diag.direction == "back":
						# if the robot is initially on a back diag square, and the robot is already above that diag,
						# there is no barrier here
						if row == moving_robot_row and moving_robot_above_diag:
							pass
						else:
						# otherwise, the robot will start going west
							first_barrier_north = row 
							first_barrier_type = "back"
							moving_robot_above_diag = False
							break

			# if neither a robot nor a diag blocked, check if there is a barrier
			if self.hasColBarrier(row, moving_robot_col):
				first_barrier_north = row
				first_barrier_type = "barrier"
				break

		# Based on the barrier type, decide what to do
		
		# if barrier or blocker robot, the moving robot stops
		if first_barrier_type == "barrier":
			new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag)
			return new_position

		elif first_barrier_type == "robot":
			new_position = Position(first_barrier_north + 1, moving_robot_col, moving_robot_above_diag)
			return new_position

		elif first_barrier_type == "forward":
			new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.eastMove(moving_robot_color, robot_positions)

		elif first_barrier_type == "back":
			new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.westMove(moving_robot_color, robot_positions)

		assert False, "Barrier type is invalid"	



	def westMove(self, moving_robot_color, robot_positions):
		"""
		Returns the Position in which the given robot ends up if it moves west.
		The robots' initial positions are given by robot_positions.
		Assumes that the robot_positions dict can be modified.
		"""
		assert robot_positions is not None and type(robot_positions) == dict, "incorrect robot_positions in Board.westMove"
		assert moving_robot_color in ["yellow", "red", "green", "blue"], "Invalid robot color"



		moving_robot_pos = robot_positions[moving_robot_color]
		moving_robot_row = moving_robot_pos.row
		moving_robot_col = moving_robot_pos.col
		moving_robot_above_diag = moving_robot_pos.above_diag



		# look for the first barrier west
		# it could be a row barrier (vertical barrier), a diag or another robot
		first_barrier_west = 0
		first_barrier_type = "barrier"

		for col in range(moving_robot_col, -1, -1):


			# check if there is a robot already on that square
			is_blocker_robot = robotAt(moving_robot_row, col, robot_positions, exclude=[moving_robot_color])
			if is_blocker_robot:
				first_barrier_west = col 
				first_barrier_type = "robot"
				break

			# check if there is a diag
			if self.hasDiag(moving_robot_row, col):
				diag = self.getDiag(moving_robot_row, col)


				# if the diag is of the same color, it passes through
				# important to change the above/below diag, in case there is a same-color diag followed immediately by barrier
				if diag.color == moving_robot_color:
					if diag.direction == "forward":
						moving_robot_above_diag = True
					elif diag.direction == "back":
						moving_robot_above_diag = False
				
				# if the diag is of another color it will deflect
				if diag.color != moving_robot_color:

					if diag.direction == "forward":
						# if the robot is initially on a forward diag square, and the robot is already below that diag,
						# nothing will happen
						if col == moving_robot_col and moving_robot_above_diag: 
							pass
						else:
							# otherwise, the robot will start going south
							first_barrier_west = col
							first_barrier_type = "forward"
							moving_robot_above_diag = False
							break

					if diag.direction == "back":
						# if the robot is initially on a back diag square, and the robot is already above that diag,
						# there is no barrier here
						if col == moving_robot_col and not moving_robot_above_diag:
							pass
						else:
						# otherwise, the robot will start going north
							first_barrier_west = col 
							first_barrier_type = "back"
							moving_robot_above_diag = True
							break

			# if neither a robot nor a diag blocked, check if there is a barrier
			if self.hasRowBarrier(moving_robot_row, col):
				first_barrier_west = col
				first_barrier_type = "barrier"
				break

		# Based on the barrier type, decide what to do
		
		# if barrier or blocker robot, the moving robot stops
		if first_barrier_type == "barrier":
			new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag)
			return new_position
		elif first_barrier_type == "robot":
			new_position = Position(moving_robot_row, first_barrier_west + 1, moving_robot_above_diag)
			return new_position
		elif first_barrier_type == "forward":
			new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.southMove(moving_robot_color, robot_positions)
		elif first_barrier_type == "back":
			new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.northMove(moving_robot_color, robot_positions)

		assert False, "Barrier type is invalid"	


		
	def eastMove(self, moving_robot_color, robot_positions):
		"""
		Returns the Position in which the given robot ends up if it moves east.
		The robots' initial position is given by robot_positions.
		Assumes that the robot_positions dict can be modified.
		"""
		assert robot_positions is not None and type(robot_positions) == dict, "incorrect robot_positions in Board.eastMove"
		assert moving_robot_color in ["yellow", "red", "green", "blue"], "Invalid robot color"


		moving_robot_pos = robot_positions[moving_robot_color]
		moving_robot_row = moving_robot_pos.row
		moving_robot_col = moving_robot_pos.col
		moving_robot_above_diag = moving_robot_pos.above_diag

		# look for the first barrier east
		# it could be a row barrier (vertical barrier), a diag or another robot
		first_barrier_east = 16
		first_barrier_type = "barrier"

		for col in range(moving_robot_col, 16):


			# check if there is a robot already on that square
			is_blocker_robot = robotAt(moving_robot_row, col, robot_positions, exclude=[moving_robot_color])
			if is_blocker_robot:
				first_barrier_east = col 
				first_barrier_type = "robot"
				break

			# check if there is a diag
			if self.hasDiag(moving_robot_row, col):
				diag = self.getDiag(moving_robot_row, col)


				# if the diag is of the same color, it passes through
				# important to change the above/below diag, in case there is a same-color diag followed immediately by barrier
				if diag.color == moving_robot_color:
					if diag.direction == "forward":
						moving_robot_above_diag = False
					elif diag.direction == "back":
						moving_robot_above_diag = True
				
				# if the diag is of another color it will deflect
				if diag.color != moving_robot_color:

					if diag.direction == "forward":
						# if the robot is initially on a forward diag square, and the robot is already below that diag,
						# nothing will happen
						if col == moving_robot_col and not moving_robot_above_diag: 
							pass
						else:
							# otherwise, the robot will start going north
							first_barrier_east = col
							first_barrier_type = "forward"
							moving_robot_above_diag = True
							break

					if diag.direction == "back":
						# if the robot is initially on a back diag square, and the robot is already above that diag,
						# there is no barrier here
						if col == moving_robot_col and moving_robot_above_diag:
							pass
						else:
						# otherwise, the robot will start going south
							first_barrier_east = col 
							first_barrier_type = "back"
							moving_robot_above_diag = False
							break

			# if neither a robot nor a diag blocked, check if there is a barrier
			if self.hasRowBarrier(moving_robot_row, col + 1):
				first_barrier_east = col + 1
				first_barrier_type = "barrier"
				break

		# Based on the barrier type, decide what to do
		if first_barrier_type in ["barrier", "robot"]:
			# if barrier or blocker robot, the moving robot stops
			new_position = Position(moving_robot_row, first_barrier_east - 1, moving_robot_above_diag)
			return new_position
		elif first_barrier_type == "forward":
			new_position = Position(moving_robot_row, first_barrier_east, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.northMove(moving_robot_color, robot_positions)
		elif first_barrier_type == "back":
			new_position = Position(moving_robot_row, first_barrier_east, moving_robot_above_diag)
			robot_positions[moving_robot_color] = new_position
			return self.southMove(moving_robot_color, robot_positions)

		assert False, "Barrier type is invalid"	




	def display(self, robots={}, dest=None):
		"""
		Displays a human-readable representation of this board.
		Pieces is a dictionary of robot colors to Positions.
		Above/below denotes whether the robot is sitting above or below a diag.  If no diag, will be "above"
		"""
		assert 0 <= len(robots) <= 4, "there are either 0 or 4 robots"

		print

		# print top wall
		top_wall =  "."
		for col in range(16):
			top_wall += "___."
		print top_wall

		# prepare to print horizontal barriers in each column
		num_col_barriers = [len(self.col_barriers[col]) for col in range(16)]
		next_col_barrier_index = [1 for col in range(16)]
		next_col_barrier = [self.col_barriers[col][1] for col in range(16)]

		# print each row
		for row in range(16):

			# begin row with left wall
			row_str = "|"

			# prepare to print vertical barriers and diags in this row
			num_barriers = len(self.row_barriers[row])
			has_barriers = len(self.row_barriers[row]) > 0
			next_barrier_index = 1
			next_barrier_pos = self.row_barriers[row][1] if has_barriers else -1

			num_diags = len(self.row_diags[row])
			has_diags = len(self.row_diags[row]) > 0
			next_diag_index = 0 if has_diags else -1
			next_diag_pos = self.row_diags[row][0].col if has_diags else -1
			

			for col in range(16):

				# determine whether there is a robot at this spot, or the destination
				has_robot = False
				robot_above = False
				robot_below = False
				robot_str = ""
				robot_colors = {"yellow": "Y", "red":"R", "green":"G", "blue":"B"}
				for robot_color in robots:
					robot = robots[robot_color]
					if robot.row == row and robot.col == col:
						has_robot = True
						robot_above = robot.above_diag
						robot_below = not robot.above_diag
						robot_str = robot_colors[robot_color]

				if dest and dest.row == row and dest.col == col:
					has_robot = True
					robot_above = True
					robot_below = False
					robot_str = "*"

				# determine whether there is a diag at this spot
				forward_diag = next_diag_pos == col and \
					self.row_diags[row][next_diag_index].direction == "forward"
				back_diag = next_diag_pos == col and \
					self.row_diags[row][next_diag_index].direction == "back"

				# determine whether there is a col barrier at this spot
				col_barrier = (row + 1) == next_col_barrier[col]


				# Print the appropriate robot/diag/col barrier, if necessary
				
				if forward_diag and col_barrier:
					if has_robot:
						if robot_above:
							row_str += robot_str + "/_"
						else:
							row_str += "/" + robot_str + "_"
					else:
						row_str += "/" + robot_str + "_"

				elif back_diag and col_barrier:
					if has_robot:
						if robot_above:
							row_str += "\\" + robot_str + "_"
						else:
							row_str += robot_str + "\\" + "_"
					else:
						row_str += robot_str + "_\\" 

				elif forward_diag:
					if has_robot:
						if robot_above:
							row_str += robot_str + "/ "
						else:
							row_str += " /" + robot_str
					else:
						row_str += " / "

				elif back_diag:
					if has_robot:
						if robot_above:
							row_str += " \\" + robot_str
						else:
							row_str += robot_str + "\\ "
					else:
						row_str += " \\ "

				elif col_barrier:
					if has_robot:
						row_str += "_" + robot_str + "_"
					else:
						row_str += "___"

				else:
					if has_robot:
						row_str += " " + robot_str + " "
					else:
						row_str += "   "

				# prepare for the next column barrier
				if col_barrier:
					next_col_barrier_index[col] += 1
					next_col_barrier[col] = self.col_barriers[col][next_col_barrier_index[col]] \
						if next_col_barrier_index[col] < num_col_barriers[col] else -1

				
				# print dot or a vertical barrier
				if (col + 1) == next_barrier_pos:
					row_str += "|"
					next_barrier_index += 1
					next_barrier_pos = self.row_barriers[row][next_barrier_index] \
						if next_barrier_index < num_barriers else -1
				else:
					row_str += "."

			print row_str
		print






board = Board()
board.row_barriers = [

[0, 5, 10, 16],
[0, 13, 16],
[0, 6, 10, 16],
[0, 3, 9, 16],

[0, 16],
[0, 2, 16],
[0, 11, 16],
[0, 6, 7, 9, 16],

[0, 7, 9, 16],
[0, 2, 11, 16],
[0, 3, 16],
[0, 7, 13, 16],

[0, 16],
[0, 9, 16],
[0, 6, 16],
[0, 7, 11, 16],

]



board.col_barriers = [

[0, 7, 13, 16],
[0, 6, 16],
[0, 3, 10, 16],
[0, 4, 16],

[0, 16],
[0, 8, 15, 16],
[0, 2, 16],
[0, 7, 9, 11, 16],

[0, 7, 9, 16],
[0, 3, 13, 16],
[0, 10, 16],
[0, 7, 16],

[0, 1, 11, 16],
[0, 12, 16],
[0, 16],
[0, 6, 13, 16],

]

board.row_diags = [

[],
[Diag(1, 2, "red", "back")],
[],
[],

[Diag(4, 14, "green", "back")],
[Diag(5, 8, "yellow", "forward")],
[Diag(6, 3, "green", "back")],
[],

[Diag(8, 4, "red", "forward")],
[],
[],
[],

[Diag(12, 9, "yellow", "back")],
[Diag(13, 1, "blue", "back")],
[Diag(14, 11, "blue", "back")],
[],

]

board.col_diags = [

[],
[Diag(13, 1, "blue", "back")],
[Diag(1, 2, "red", "back")],
[Diag(6, 3, "green", "back")],

[Diag(8, 4, "red", "forward")],
[],
[],
[],

[Diag(5, 8, "yellow", "forward")],
[Diag(12, 9, "yellow", "back")],
[],
[Diag(14, 11, "blue", "back")],

[],
[],
[Diag(4, 14, "green", "back")],
[],

]


def main():
	

	yellow_robot = Position(9, 2)
	red_robot = Position(5, 1)
	green_robot = Position(2, 9)
	blue_robot = Position(0, 0)
	robots = {"yellow": yellow_robot, "red": red_robot, "green": green_robot, "blue": blue_robot}

	dest = Position(11, 13)
	dest_color = "green"
	max_depth=20

	board.display(robots, dest)

	solutions = solve(board, robots, dest, dest_color, num_sols=1, max_depth=max_depth)
	top_solution = solutions[0]
	print
	print "SOLUTION"
	top_solution.printPath()
	print

	board.display(top_solution.robots)






if __name__ == '__main__':
	main()
