import numpy as np
import math
import random

class KMeansPlusPlus:
    def __init__(self, dataset, K=2):
        self.K = K
        self.dataset = dataset
        self.clusters = [ [] for i in range(K)]    
        self.cluster_centers = np.array(self.initialize_centers())
        self.cluster_table= [ -1 for i in range(len(dataset))]
        self.assign_clusters()
        
    def initialize_centers(self):
        centers=[]
        c_1=self.dataset[np.random.choice(self.dataset.shape[0], size=1, replace=False), :][0]
        centers.append(c_1.tolist())
        for i in range(self.K-1):
            center = centers[i]     
            distances = []
            for point in self.dataset:
                distances.append(np.sum((point - center)**2))
            new_center= (random.choices(self.dataset, distances, k=1)[0]).tolist()
            centers.append(new_center)
            distances=[]
        return centers
        
    def calculateLoss(self):
        summ=0
        i=0
        for datas in self.dataset:
            summ += (np.sum((datas - self.cluster_centers[self.cluster_table[i]])**2))
            i+=1
        return summ

    def run(self):
        self.calculate_cluster_centers()
        self.assign_clusters()
        return self.cluster_centers, self.clusters, self.calculateLoss()

    def assign_clusters(self):
        i=0
        self.cluster_table= [ -1 for i in range(len(self.dataset))]
        for point in self.dataset:
            index=0
            dist = float("inf")
            for cluster in self.cluster_centers:
                distance = np.sqrt(np.sum((point - cluster)**2))
                if distance < dist:
                    dist=distance
                    location = index
                index=index+1
            self.cluster_table[i]=location
            i=i+1
        self.clusters = [ [] for i in range(self.K)]
        for i in range(len(self.cluster_table)):
            (self.clusters[self.cluster_table[i]]).append((self.dataset[i]).tolist())
    
    def calculate_cluster_centers(self):
        new_centers = [ [] for i in range(self.K)]
        for i in range(self.K):
            if(len(self.clusters[i])==0):
                new_centers[i] = self.cluster_centers[i]
            else:
                new_centers[i] = (np.mean(self.clusters[i],axis=0)).tolist()
        self.cluster_centers=new_centers
