import  numpy as np
from numpy import dot
from numpy.linalg import norm

class Distance:
    @staticmethod
    def calculateCosineDistance(x, y):
        return dot(x, y)/(norm(x)*norm(y))
    @staticmethod
    def calculateMinkowskiDistance(x, y, p=2):
        return np.sum(np.abs(x - y)**p)**(1/p)
    @staticmethod
    def calculateMahalanobisDistance(x,y, S_minus_1):
        delta = x - y
        m = np.dot(np.dot(delta, S_minus_1), delta)
        return np.sqrt(m)

