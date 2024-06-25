import numpy as np

class KMeans:
    def __init__(self, dataset, K=2):
        self.K = K
        self.dataset = dataset
        self.clusters = [ [] for i in range(K)]
        self.cluster_centers = dataset[np.random.choice(dataset.shape[0], size=K, replace=False), :]
        self.cluster_table= [ -1 for i in range(len(dataset))]
        self.assign_clusters()

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

        
   
