import pickle
from Distance import Distance
from Knn import KNN
from sklearn.model_selection import StratifiedKFold
import statistics as s



dataset, labels = pickle.load(open("../data/part1_dataset.data", "rb"))
total=[]

for i in range(5):
        acc_list = []
        kfold = StratifiedKFold(n_splits=10,shuffle=True)
        for train_ix, test_ix in kfold.split(dataset, labels):
                X_train, X_test = dataset[train_ix, :], dataset[test_ix, :]
                y_train, y_test = labels[train_ix], labels[test_ix]
                #these are the combinations that I used while grid searching
                #model = KNN(X_train,y_train,Distance.calculateCosineDistance,None,13)
                model = KNN(X_train,y_train,Distance.calculateMinkowskiDistance,2,13)
                #S_minus_1=np.linalg.inv(np.cov(X_train.T))
                #model = KNN(X_train,y_train,Distance.calculateMahalanobisDistance,S_minus_1,13)
                true_label = 0
                for i in range(len(X_test)):
                        true_label += (model.predict(X_test[i])==y_test[i])
                (acc_list.append(true_label/len(X_test)))
        total.append(s.mean(acc_list))

#pconfidence interval for accuracy score
mean=s.mean(total)
std=s.stdev(total)
c=1.96*std/(len(total)**0.5)
print ("accuracy: %.3f" % mean)
print("confidence iterval: (%.3f, %.3f)" % (mean-c , mean+c))

#final model 
model = KNN(dataset,labels,Distance.calculateMinkowskiDistance,2,13)




