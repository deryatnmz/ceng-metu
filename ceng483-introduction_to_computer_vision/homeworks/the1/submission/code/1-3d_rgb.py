import cv2
import numpy as np

intervals  = [128,64,32,16]
instance_names = np.array(open("InstanceNames.txt").read().splitlines())


def calculate_3d_histogram(image, interval):
    num_bins = (256//interval)
    
    red_indices = (image[:, :, 0] // interval).clip(0, num_bins - 1)
    green_indices = (image[:, :, 1] // interval).clip(0, num_bins - 1)
    blue_indices = (image[:, :, 2] // interval).clip(0, num_bins - 1)

    histogram3d = np.zeros((num_bins, num_bins, num_bins), dtype=int)
    np.add.at(histogram3d, (red_indices, green_indices, blue_indices), 1)

    normalized_histogram3d = histogram3d / np.sum(histogram3d)
    return normalized_histogram3d

def calculate_query_set(input_interval):
    query_set_histograms = [[],[],[]]
    for i in range(3):
        for test_instance in instance_names:
            test_filename = 'query_' +str(i+1)+'/'+ test_instance
            test_image = cv2.imread(test_filename)
            test_image = cv2.cvtColor(test_image, cv2.COLOR_BGR2RGB)
            test_rgb_3d_histogram = calculate_3d_histogram(test_image, input_interval)
            query_set_histograms[i].append(test_rgb_3d_histogram)
    return np.array(query_set_histograms)

def per_channael(input_interval,query_set_histograms):
    correct_answers=0
    correct_answers_list = np.array([0,0,0])
    for instance in instance_names:
        correct_answers=0
        filename = 'support_96/' + instance
        image = cv2.imread(filename)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        rgb_3d_histogram = calculate_3d_histogram(image,input_interval)
        for i in range(3):
            max_similarity_value=-1
            max_similarity_instance=''
            for j in range(len(instance_names)):
                test_rgb_3d_histogram = query_set_histograms[i][j]
                similarity= np.sum(np.minimum(rgb_3d_histogram,test_rgb_3d_histogram))
                if(similarity>max_similarity_value):
                    max_similarity_value = similarity
                    max_similarity_instance = instance_names[j]
            if(instance == max_similarity_instance):
                correct_answers_list[i] +=1
                correct_answers +=1
    return correct_answers_list



# rgb 3d color histogram for intervals: [128,64,32,16]
print("3D Color Histogram (RGB)")
print("each triple represents top-1 accuracy scores based on query sets 1, 2, and 3")
for i in intervals:
    query_set_histograms = calculate_query_set(i)
    scores = per_channael(i,query_set_histograms)
    scores = scores /len(instance_names)
    print("interval: "+ str(i))
    print(scores)


