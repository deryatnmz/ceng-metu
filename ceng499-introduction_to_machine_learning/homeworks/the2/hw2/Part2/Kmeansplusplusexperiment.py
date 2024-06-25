from KMeansPlusPlus import KMeansPlusPlus
import pickle
import numpy as np
import statistics as s


dataset1 = pickle.load(open("../data/part2_dataset_1.data", "rb"))

dataset2 = pickle.load(open("../data/part2_dataset_2.data", "rb")) 


diff=1.0
final_list=[]
for j in range (10):
    loss_list=[]
    for i in range(10):
        model = KMeansPlusPlus(dataset1, 5) #final model for dataset 1
        #model = KMeansPlusPlus(dataset2, 4) #final model for dataset 2
        while diff>0.00000001: #if centers are not changed
            cluster_centers1, clusters, loss = model.run()
            cluster_centers2, clusters, loss = model.run()
            diff=np.sum(np.square(np.subtract(cluster_centers1,cluster_centers2)))
        loss_list.append(loss)
    print(loss_list)
    final_list.append(min(loss_list))
print(final_list)

#calculating confidence interval of loss values
mean=s.mean(final_list)
std=s.stdev(final_list)
c=1.96*std/(len(final_list)**0.5)
print ("loss value: %.3f" % mean)
print("confidence iterval: (%.3f, %.3f)" % (mean-c , mean+c))

