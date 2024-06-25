import pickle
import numpy as np
from sklearn import svm
import matplotlib.pyplot as plt

dataset, labels = pickle.load(open("../data/part2_dataset1.data", "rb"))

X = dataset
y = labels

m1 = svm.SVC(kernel='rbf', gamma=0.7, C=1.0).fit(X, y)
m2 = svm.SVC(kernel='rbf', gamma=0.7, C=10.0).fit(X, y)
m3 = svm.SVC(kernel='poly', degree=5, C=1.0).fit(X, y)
m4 = svm.SVC(kernel='poly', degree=5, C=10.0).fit(X, y)
m5 = svm.SVC(kernel='poly', degree=10, C=1.0).fit(X, y)
m6 = svm.SVC(kernel='poly', degree=10, C=10.0).fit(X, y)

# create a mesh to plot in
x_min, x_max = X[:, 0].min() - 0.5, X[:, 0].max() + 0.5
y_min, y_max = X[:, 1].min() - 0.5, X[:, 1].max() + 0.5
xx, yy = np.meshgrid(np.arange(x_min, x_max, .02 ),
                     np.arange(y_min, y_max, .02 ))

# title for the plots
titles = ["radial basis function kernel with C=1.0","radial basis function kernel with C=10.0",
          "5th degree polynomial kernel with C=1.0", "5th degree polynomial kernel with C=10.0",
          "10th degree polynomial kernel with C=1.0", "10th degree polynomial kernel with C=10.0"]


for i, clf in enumerate((m1, m2, m3, m4,m5,m6)):
    plt.subplot(3, 2, i + 1)
    plt.subplots_adjust(wspace=0.4, hspace=0.4)
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.contourf(xx, yy, Z, cmap=plt.cm.PiYG, alpha=0.3)
    plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.PiYG,marker='.')
    plt.xlim(xx.min(), xx.max())
    plt.ylim(yy.min(), yy.max())
    plt.title(titles[i])

plt.show()
