import random

from pacai.agents.base import BaseAgent
from pacai.agents.search.multiagent import MultiAgentSearchAgent
from pacai.core.distance import euclidean
from pacai.core.distance import manhattan

class ReflexAgent(BaseAgent):

    def __init__(self, index, **kwargs):
        super().__init__(index)

    def getAction(self, gameState):
        """
        Basic PROVIDED code (Not Mine)
        """

        # Collect legal moves.
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions.
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best.

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):

        def findFood(foodGrid):  # returns x,y of closest food
            closest = 9999
            closestFood = (0, 0)
            for x in range(foodGrid.getWidth()):
                for y in range(foodGrid.getHeight()):
                    if foodGrid[x][y] is True:
                        checkDist = euclidean(currentGameState.getPacmanPosition(), (x, y))
                        if checkDist < closest:
                            closest = checkDist
                            closestFood = (x, y)

            return closestFood

        # Pull important data from the game state.
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        pacPos = currentGameState.getPacmanPosition()
        closestFood = findFood(currentGameState.getFood())
        currentFoodDist = manhattan(currentGameState.getPacmanPosition(), closestFood)
        newFoodDist = manhattan(successorGameState.getPacmanPosition(), closestFood)
        score = successorGameState.getScore()

        if action == 'Stop':
            score -= 5
        if newFoodDist < currentFoodDist:
            score += 10

        # Factor in ghost distances into potential move's value
        for i in successorGameState.getGhostStates():
            ghostDist = euclidean(pacPos, i.getPosition())
            if ghostDist < 5:
                score -= (5 - ghostDist)
                if ghostDist <= 1:
                    score -= 99
        return score

class MinimaxAgent(MultiAgentSearchAgent):
    """
    A minimax agent.
    """
    # Read an online article and talked to Andrew (tutor) to help understand the overall structure.
    # Article did include some code
    # Wrote my code after getting the basic outline, don't think it's super similar
    # but just incase/full disclosure!
    # Article: https://davideliu.com/2020/02/13/playing-pacman-with-multi-agents-adversarial-search/
    def __init__(self, index, **kwargs):
        super().__init__(index)
        self.treeDepth = int(kwargs.get("depth"))  # Use this? saw on piazza

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

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    A minimax agent with alpha-beta pruning.
    """
    # Read an online article and talked to Andrew (tutor) to help understand the overall structure.
    # Article did include some code
    # Wrote my code after getting the basic outline, don't think it's super similar
    # but just incase/full disclosure!
    # Article: https://davideliu.com/2020/02/13/playing-pacman-with-multi-agents-adversarial-search/
    def __init__(self, index, **kwargs):
        super().__init__(index)
        self.treeDepth = int(kwargs.get("depth"))  # Use this? saw on piazza

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

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
    An expectimax agent.
    """

    # Read an online article and talked to Andrew (tutor) to help understand the overall structure.
    # Article did include some code
    # Wrote my code after getting the basic outline, don't think it's super similar
    # but just incase/full disclosure!
    # Article: https://davideliu.com/2020/02/13/playing-pacman-with-multi-agents-adversarial-search/
    def __init__(self, index, **kwargs):
        super().__init__(index)
        self.treeDepth = int(kwargs.get("depth"))  # Use this? saw on piazza

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

def betterEvaluationFunction(currentGameState):
    return currentGameState.getScore()

class ContestAgent(MultiAgentSearchAgent):
    """
    Your agent for the mini-contest.

    You can use any method you want and search to any depth you want.
    Just remember that the mini-contest is timed, so you have to trade off speed and computation.

    Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
    just make a beeline straight towards Pacman (or away if they're scared!)

    Method to Implement:

    `pacai.agents.base.BaseAgent.getAction`
    """
    def __init__(self, index, **kwargs):
        super().__init__(index)
