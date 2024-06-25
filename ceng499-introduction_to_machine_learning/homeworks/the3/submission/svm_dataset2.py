import pickle
from sklearn.preprocessing import StandardScaler
import statistics as s
from sklearn.pipeline import make_pipeline
import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import StratifiedKFold, GridSearchCV, cross_val_score
from sklearn.metrics import accuracy_score
dataset, labels = pickle.load(open("../data/part2_dataset2.data", "rb"))


parameter_grid = {"kernel": ["poly","rbf","sigmoid"],
                  "C": [10,100],
                  "degree": [5]
              }


parameters=[]
means = [[],[],[],[],[],[],[]]
final_means = [[],[],[],[],[],[]]
for i in range(5):
    cross_validation = StratifiedKFold(n_splits=10,shuffle=True)
    for train_indices, test_indices in cross_validation.split(dataset, labels):
        train_mean = np.mean(dataset[train_indices])
        train_std= np.std(dataset[train_indices])
        
        current_training_part =  StandardScaler().fit_transform(dataset[train_indices])
        current_training_part_label = labels[train_indices]

        current_test_part =(dataset[test_indices] - train_mean) /train_std
        current_test_part_label = labels[test_indices]
        
        gridsearch_cv = GridSearchCV(SVC(), parameter_grid, cv=cross_validation ,scoring="accuracy", refit=True)
        gridsearch_cv.fit(current_training_part, current_training_part_label)
        
        for i in range(6):
            means[i].append(gridsearch_cv.cv_results_['mean_test_score'][i])
            
        predicted = gridsearch_cv.best_estimator_.predict(current_test_part)
        accutacy_score_of_best_estimator=(accuracy_score(current_test_part_label, predicted)*100)
        
        parameters = gridsearch_cv.cv_results_['params']
        
    for i in range (6):
        final_means[i].append((sum(means[i])*10))
    means = [[],[],[],[],[],[],[]]

ind=0
for i in final_means:
    print(parameters[ind])
    mean=s.mean(i)
    std=s.stdev(i)
    c=1.96*std/(len(i)**0.5)
    print ("accuracy: %.3f" % mean)
    print("confidence iterval: (%.3f, %.3f)" % (mean-c , mean+c))
    ind+=1

