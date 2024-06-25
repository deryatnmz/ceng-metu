import numpy as np
import math
from collections import Counter

class KNN:
    def __init__(self, dataset, data_label, similarity_function, similarity_function_parameters=None, K=1):
        self.K = K
        self.dataset = dataset
        self.dataset_label = data_label
        self.similarity_function = similarity_function
        self.similarity_function_parameters = similarity_function_parameters

    def predict(self, instance):
        distances = []
        labels = []
        if(self.similarity_function.__name__ == "calculateCosineDistance"):
            for i in range(len(self.dataset)):
                dist = self.similarity_function(self.dataset[i], instance)
                distances.append([dist, i])
            distances.sort(reverse=True) #since in cosine distance bigger values represent more similarity
        else:
            for i in range(len(self.dataset)):
                dist = self.similarity_function(self.dataset[i], instance, self.similarity_function_parameters)
                distances.append([dist, i])
            distances.sort() #sorting according to their distance
        distances = distances[0:self.K] #taking k nearest(most similar) values
        for j in range(len(distances)):
            labels.append(self.dataset_label[distances[j][1]])
        label = Counter(labels).most_common(1)[0][0] #majority voting
        return label


