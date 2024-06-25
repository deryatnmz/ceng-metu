import pickle
import numpy as np
from matplotlib import pyplot as plt
from sklearn.cluster import AgglomerativeClustering
from sklearn.metrics import silhouette_samples, silhouette_score
import matplotlib.cm as cm



dataset = pickle.load(open("../data/part3_dataset.data", "rb"))

#plotting dendrogram
#linkage_data = linkage(dataset, method='single', metric='euclidean')
#linkage_data = linkage(dataset, method='single', metric='cosine')
#linkage_data = linkage(dataset, method='complete', metric='euclidean')
"""linkage_data = linkage(dataset, method='complete', metric='cosine')
dendrogram = dendrogram(linkage_data)
plt.xlabel("instances")
plt.ylabel("distance")
plt.title("complete linkage with cosine distance")
#plt.show()"""

for n_clusters in range(2,6):
    clusterer = AgglomerativeClustering(n_clusters=n_clusters, affinity='cosine', linkage='complete')
    clusterer.fit(dataset)
    cluster_labels = clusterer.labels_
    silhouette_avg = silhouette_score(dataset, cluster_labels)
    print("For K =",n_clusters,"The average silhouette score is :",silhouette_avg)
    sample_silhouette_values = silhouette_samples(dataset, cluster_labels)
    fig, (ax1) = plt.subplots(1, 1)
    ax1.set_xlim([-0.1, 1])
    ax1.set_ylim([0, len(dataset) + (n_clusters + 1) * 10])
    y_lower = 10
    for i in range(n_clusters):
        ith_cluster_silhouette_values = sample_silhouette_values[cluster_labels == i]
        ith_cluster_silhouette_values.sort()
        size_cluster_i = ith_cluster_silhouette_values.shape[0]
        y_upper = y_lower + size_cluster_i
        color = cm.nipy_spectral(float(i) / n_clusters)
        ax1.fill_betweenx(np.arange(y_lower, y_upper),0,ith_cluster_silhouette_values,facecolor=color,edgecolor=color,alpha=0.7,)
        ax1.text(-0.05, y_lower + 0.5 * size_cluster_i, str(i))
        y_lower = y_upper + 10
    ax1.set_title("The silhouette plot for cosine distance with complete linkage")
    ax1.set_xlabel("The silhouette coefficient values")
    ax1.set_ylabel("Cluster label")
    plt.show()

