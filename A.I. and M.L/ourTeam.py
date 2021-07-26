from pacai.util import reflection
from pacai.agents.capture.capture import CaptureAgent
from pacai.core.directions import Directions
from pacai.util import util
from pacai.util import counter
from pacai.agents.base import BaseAgent
from pacai.agents.search.multiagent import MultiAgentSearchAgent
from pacai.core.distance import euclidean
from pacai.core.distance import manhattan
import random

class OneStepDefenseBoi(CaptureAgent):

    def __init__(self, index, **kwargs):
        super().__init__(index)
        print("Using DefenseBoi")


    def registerInitialState(self, gameState):
        """
        This method handles the initial setup of the agent and populates useful fields.
        """
        
        super().registerInitialState(gameState)

        self.block = 0
        self.turns = 0
        self.circleIndex = 0
        self.fear = 0
        self.lastEaten = None
        if gameState.getAgentPosition(self.index)[0] < 15:
            self.side = 13
        else:
            self.side = 17

        self.visitedPositions = counter.Counter()
        self.circlePositions = counter.Counter()  # Snail trail to avoid backtrack

    def chooseAction(self, gameState):
        """
        Pick an action using AStarSearch
        """
        # Update visited positions
        self.visitedPositions[gameState.getAgentPosition(self.index)] += 1

        if self.circleIndex == 5:
            self.circleIndex = 0

        # storing the last 3 visited coords
        self.circlePositions[self.circleIndex] = gameState.getAgentPosition(self.index)

        legalMoves = gameState.getLegalActions(self.index)

        # Choose one of the best actions.
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [
            index for index in range(len(scores)) if scores[index] == bestScore
        ]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best.

        # Checks for the enemy eating a capsule
        if gameState.getLastCapsuleEaten() != self.lastEaten:
            self.lastEaten = gameState.getLastCapsuleEaten()
            if gameState.getLastCapsuleEaten()[0] > 15 and self.side == 17:  # Checking which side the cap was
                    self.fear = 15
            elif gameState.getLastCapsuleEaten()[0] <= 15 and self.side == 13:
                    self.fear = 15

        if self.fear > 0:  # Fear counter 
            self.fear -= 1

        if legalMoves[chosenIndex] == 'Stop':  # Detect/Build on block chain
            self.block += 1
        else:
            self.block = 0

        self.turns += 1
        self.circleIndex += 1

        return legalMoves[chosenIndex]


    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.
        """

        returnVal = 0

        # Get current Pacman position
        successorState = currentGameState.generateSuccessor(self.index, action)
        successorPosition = successorState.getAgentPosition(self.index)
        x, y = successorPosition

        # Get enemy positions
        opponents = self.getOpponents(successorState)
        currentGhostPositions = []
        for opponent in opponents:
            opponentState = successorState.getAgentState(opponent)
            currentGhostPositions.append(successorState.getAgentPosition(opponent))

        # Chase them pacbois
        for ghost in currentGhostPositions:
            proximity = self.getMazeDistance(successorPosition, ghost)
            if self.side == 13:  # higher weight for enemies on your side of the board
                if ghost[0] <= 15:
                    returnVal += abs(600 - proximity * 20)
                else:
                    returnVal += abs(100 - proximity * 2)
            elif self.side == 17:
                if ghost[0] > 15:
                    # print(action, " - Guard: ", abs(1000 - proximity * 20))
                    returnVal += abs(1000 - proximity * 20)
                else:
                    # print(action, " - Potential(", proximity, "): ", abs(500 - proximity * 3))
                    returnVal += abs(500 - proximity * 3)

            if proximity < 5:
                if self.fear > 0:  # If they have capsule, run away
                    # print("Run Away! ", -(10000 - (proximity * 1000)))
                    return -(10000 - (proximity * 1000))
                else:  # Chase them!
                    if (self.block > 1 and action == "Stop") or (proximity == 1 and action == "Stop"):  # Encourage Block Chain
                        # print("Blocking! ", returnVal + 1000)
                        returnVal += 1000
            else: # Discourage backtracking on the last 3 visited coords
                for x in range(len(self.circlePositions)):
                    if self.circlePositions[x] == successorPosition and proximity > 5:
                        # print("Circle Check! -5")
                        returnVal -= 5

        # Persuade agent to stay near his line - ?: can cause running into deadend issues
        #if abs(successorPosition[0] - self.side) > abs(currentGameState.getAgentPosition(self.index)[0] - self.side):
            #returnVal -= abs(successorPosition[0] - self.side)
            #print("line weight: ", -abs(successorPosition[0] - self.side))
        
        # Don't allow agent to go to opposing side
        if self.side == 13:
            if successorPosition[0] > 14:
                return -9999
        elif self.side == 17:
            if successorPosition[0] <= 16:
                return -9999

        return returnVal

class OneStepReflexAgent(CaptureAgent):

    def __init__(self, index, **kwargs):
        super().__init__(index)
        print("Using OneStep")


    def registerInitialState(self, gameState):
        """
        This method handles the initial setup of the agent and populates useful fields,
        such as the team the agent is on and the `pacai.core.distanceCalculator.Distancer`.
        IMPORTANT: If this method runs for more than 15 seconds, your agent will time out.
        """

        super().registerInitialState(gameState)
        self.visitedPositions = counter.Counter()

    def chooseAction(self, gameState):
        """
        Pick an action using AStarSearch
        """
        # Update visited positions
        self.visitedPositions[gameState.getAgentPosition(self.index)] += 1

        legalMoves = gameState.getLegalActions(self.index)

        # Choose one of the best actions.
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [
            index for index in range(len(scores)) if scores[index] == bestScore
        ]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best.


        return legalMoves[chosenIndex]


    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.
        """

        returnVal = 0

        # Get current Pacman position
        successorState = currentGameState.generateSuccessor(self.index, action)
        successorPosition = successorState.getAgentPosition(self.index)
        x, y = successorPosition

        # Get ghost and food pellet positions
        opponents = self.getOpponents(successorState)
        currentGhostPositions = []

        for opponent in opponents:
            opponentState = successorState.getAgentState(opponent)
            if opponentState.isGhost():
                currentGhostPositions.append(successorState.getAgentPosition(opponent))

        foodGrid = self.getFood(currentGameState)
        foodPositions = self.getFood(successorState).asList()


        # Basic ghost checking to make sure Pacman doesn't die
        for ghost in currentGhostPositions:
            proximity = self.getMazeDistance(successorPosition, ghost)

            if proximity < 2:
                # print('here')
                return -10

        # If there are no pellets remaining at this state, this is a good state
        numFood = len(foodPositions)

        if numFood == 0:
            return 20

        # Determines if there's food at this location
        if foodGrid[x][y] == True:
            returnVal += 10


        # Incentive for new position, disincentive for previous position
        if self.visitedPositions[(x, y)] == 0:
            returnVal += 1
        else:
            returnVal += -(self.visitedPositions.get((x, y)))

        # Return reciprocal of distance to nearest point, or return 1 if at point
        minDistance = None
        for food in foodPositions:
            distance = self.getMazeDistance(successorPosition, food)
            if minDistance is None or minDistance > distance:
                minDistance = distance
                returnVal += 10 / distance

        return returnVal

class MinimaxAgent(MultiAgentSearchAgent):

    def __init__(self, index, **kwargs):
        super().__init__(index)
        print("Using minimax")
        self.treeDepth = 2  # int(kwargs.get("depth"))  Use this? saw on piazza

    def getAction(self, gameState):
        return self.findScore(gameState, self.treeDepth, 0)[0]  # Only need the action here

    def findScore(self, gameState, depth, agentIndex = 0):
        if gameState.isWin() or gameState.isLose() or depth == 0:  # End or Reset
            # Does this force a stop sometimes?
            return('Stop', self.getEvaluationFunction()(gameState))
        if agentIndex == 0:  # Pacman's turn'
            return self.findMax(gameState, depth, agentIndex)
        else:  # Ghost's turn
            return self.findMin(gameState, depth, agentIndex)

    def findMin(self, gameState, depth, agentIndex):  # Ghost turn, minimize
        values = []
        for action in gameState.getLegalActions(agentIndex):  # Determine branching
            # If last agent, start new round at next depth
            if agentIndex == gameState.getNumAgents() - 1:  # Fancy mod
                nextDepth = depth - 1  # All agents done, onto next round!
                nextAgent = 0
            else:
                nextDepth = depth  # Same depth, next agent
                nextAgent = agentIndex + 1
            values.append(self.findScore(gameState.generateSuccessor(agentIndex, action),
            nextDepth, nextAgent)[1])  # Recursively call minimax function with new values->
        bestVal = min(values)  # ->But only append the returned score here for checking
        bestIndexes = [i for i in range(len(values)) if values[i] == bestVal]  # Include ties
        chosenIndex = random.choice(bestIndexes)  # Roll the dice for any ties
        return gameState.getLegalActions(agentIndex)[chosenIndex], bestVal  # Return Both

    def findMax(self, gameState, depth, agentIndex):  # Pacman's turn, maximize
        values = []
        for action in gameState.getLegalActions(agentIndex):  # Determine branching
            # If pacman is the only agent (Does this happen?)
            if agentIndex == gameState.getNumAgents() - 1:  # Fancy mod
                nextDepth = depth - 1  # All agents done, onto next round!
                nextAgent = 0
            else:
                nextDepth = depth  # Same depth, next agent
                nextAgent = agentIndex + 1
            values.append(self.findScore(gameState.generateSuccessor(agentIndex, action),
            nextDepth, nextAgent)[1])  # Recursively call minimax function with new values->
        bestVal = max(values)  # ->But only append the returned score here for checking
        bestIndexes = [i for i in range(len(values)) if values[i] == bestVal]  # Include ties
        chosenIndex = random.choice(bestIndexes)  # Roll the dice for any ties
        return gameState.getLegalActions(agentIndex)[chosenIndex], bestVal  # Return Both