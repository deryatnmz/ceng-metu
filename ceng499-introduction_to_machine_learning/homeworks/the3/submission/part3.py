import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from DataLoader import DataLoader
from sklearn.svm import SVC
from sklearn.model_selection import StratifiedKFold, RepeatedStratifiedKFold, GridSearchCV
from sklearn.metrics import accuracy_score, f1_score
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import MinMaxScaler
from sklearn.pipeline import Pipeline
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
import statistics as s

data_path = "../data/credit.data"
dataset, labels = DataLoader.load_credit_with_onehot(data_path)


knn_parameter_grid = {"kneighborsclassifier__metric": ["cosine", "euclidean"],
                      "kneighborsclassifier__n_neighbors": [11, 51]
                      }

svm_parameter_grid = {"svc__C": [10, 100],
                      "svc__kernel": ["poly", "rbf"]
                      }

decision_tree_parameter_grid = { "decisiontreeclassifier__criterion": ['gini', 'entropy'],
                                 "decisiontreeclassifier__max_depth": [30,50]
                                 }

random_forest_parameter_grid = { "randomforestclassifier__n_estimators": [100, 200],
                                 "randomforestclassifier__max_depth": [30,50]
                                 }   


outer_cross_validation = RepeatedStratifiedKFold(n_splits=3, n_repeats=5, random_state=np.random.randint(1, 1000))
inner_cross_validation = RepeatedStratifiedKFold(n_splits=5, n_repeats=5, random_state=np.random.randint(1, 1000))

knn_performance = []
svm_performance = []
decision_tree_performance=[]
random_forest_performance=[]
random_forest_inner_performance_f1=[]
random_forest_inner_performance_acc=[]

knn_overall_performance_f1 = []
knn_overall_performance_acc = []
svm_overall_performance_f1 = []
svm_overall_performance_acc = []
decision_tree_overall_performance_f1=[]
decision_tree_overall_performance_acc=[]
random_forest_overall_performance_f1=[]
random_forest_overall_performance_acc=[]

knn_means=[[],[],[],[]]
svm_means=[[],[],[],[]]
decision_tree_means=[[],[],[],[]]
random_forest_means=[[],[],[],[]]


for train_indices, test_indices in outer_cross_validation.split(dataset, labels):
    current_training_part = dataset[train_indices]
    current_training_part_label = labels[train_indices]

    current_test_part = dataset[test_indices]
    current_test_part_label = labels[test_indices]
    
    knn_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), KNeighborsClassifier())
    knn_grid_search = GridSearchCV(knn_pipeline, param_grid=knn_parameter_grid, refit=True, cv=inner_cross_validation, scoring="f1_micro")
    knn_grid_search.fit(current_training_part, current_training_part_label)
    
    knn_predicted = knn_grid_search.predict(current_test_part)
    knn_overall_performance_f1.append(f1_score(current_test_part_label, knn_predicted)*100)
    knn_overall_performance_acc.append(accuracy_score(current_test_part_label, knn_predicted)*100)
    
    knn_parameters = knn_grid_search.cv_results_['params']
    for i in range(4):
        knn_means[i].append(knn_grid_search.cv_results_['mean_test_score'][i]*100)
        
    
    svm_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), SVC())
    svm_grid_search = GridSearchCV(svm_pipeline, param_grid=svm_parameter_grid, refit=True, cv=inner_cross_validation, scoring="f1_micro")
    svm_grid_search.fit(current_training_part, current_training_part_label)

    svm_predicted = svm_grid_search.predict(current_test_part)
    svm_overall_performance_f1.append(f1_score(current_test_part_label, svm_predicted)*100)
    svm_overall_performance_acc.append(accuracy_score(current_test_part_label, svm_predicted)*100)

    svm_parameters = svm_grid_search.cv_results_['params']
    for i in range(4):
        svm_means[i].append(svm_grid_search.cv_results_['mean_test_score'][i]*100)
    

    decision_tree_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), DecisionTreeClassifier())
    decision_tree_grid_search = GridSearchCV(decision_tree_pipeline, param_grid=decision_tree_parameter_grid, refit=True, cv=inner_cross_validation, scoring="f1_micro")
    decision_tree_grid_search.fit(current_training_part, current_training_part_label)

    decision_tree_predicted = decision_tree_grid_search.predict(current_test_part)
    decision_tree_overall_performance_f1.append(f1_score(current_test_part_label, decision_tree_predicted)*100)
    decision_tree_overall_performance_acc.append(accuracy_score(current_test_part_label, decision_tree_predicted)*100)

    decision_tree_parameters = decision_tree_grid_search.cv_results_['params']
    for i in range(4):
        decision_tree_means[i].append(decision_tree_grid_search.cv_results_['mean_test_score'][i]*100)
    
    for i in range(5):
        random_forest_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), RandomForestClassifier())
        random_forest_grid_search = GridSearchCV(random_forest_pipeline, param_grid=random_forest_parameter_grid, refit=True, cv=inner_cross_validation, scoring="f1_micro")
        random_forest_grid_search.fit(current_training_part, current_training_part_label)

        random_forest_predicted = random_forest_grid_search.predict(current_test_part)
        random_forest_inner_performance_f1.append(f1_score(current_test_part_label, random_forest_predicted))
        random_forest_inner_performance_acc.append(accuracy_score(current_test_part_label, random_forest_predicted))
        for i in range(4):
            random_forest_means[i].append(random_forest_grid_search.cv_results_['mean_test_score'][i]*100)
    random_forest_parameters = random_forest_grid_search.cv_results_['params']
        
    random_forest_overall_performance_f1.append(np.mean(random_forest_inner_performance_f1)*100)
    random_forest_overall_performance_acc.append(np.mean(random_forest_inner_performance_acc)*100)
    random_forest_inner_performance_f1=[]
    random_forest_inner_performance_acc=[]

def confidence_interval(liste):
    mean=s.mean(liste)
    std=s.stdev(liste)
    c=1.96*std/(len(liste)**0.5)
    print ("score: %.3f" % mean)
    print("confidence iterval: (%.3f, %.3f)" % (mean-c , mean+c))

print("hyperparameter search results:")
print("-knn")
for i in range(4):
    print(knn_parameters[i])
    confidence_interval(knn_means[i])
print("-svm")
for i in range(4):
    print(svm_parameters[i])
    confidence_interval(svm_means[i])
print("-decision tree")
for i in range(4):
    print(decision_tree_parameters[i])
    confidence_interval(decision_tree_means[i])
print("-random forest")
for i in range(4):
    print(random_forest_parameters[i])
    confidence_interval(random_forest_means[i])
print("  ")
print("f1 scores:")
print("-knn")
confidence_interval(knn_overall_performance_f1)
print("-svm")
confidence_interval(svm_overall_performance_f1)
print("-decision tree")
confidence_interval(decision_tree_overall_performance_f1)
print("-random forest")
confidence_interval(random_forest_overall_performance_f1)
print("  ")
print("accuracy scores:")
print("-knn")
confidence_interval(knn_overall_performance_acc)
print("-svm")
confidence_interval(svm_overall_performance_acc)
print("-decision tree")
confidence_interval(decision_tree_overall_performance_acc)
print("-random forest")
confidence_interval(random_forest_overall_performance_acc)



