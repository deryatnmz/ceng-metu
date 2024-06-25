import cv2
import numpy as np

intervals  = [128,64,32,16,8]
instance_names = np.array(open("InstanceNames.txt").read().splitlines())



def rgb_to_hsv(image):
    image = image /255.
    r, g, b = image[:, :, 0], image[:, :, 1], image[:, :, 2]
    c_max = np.max(image, axis=2)
    c_min = np.min(image, axis=2)
    delta_c = c_max - c_min

    h = np.zeros_like(c_max, dtype=float)
    nonzero_delta_c = delta_c != 0
    h_values = np.zeros_like(c_max, dtype=float)

    h_values[nonzero_delta_c] = np.where( c_max[nonzero_delta_c] == r[nonzero_delta_c],
                                        (g[nonzero_delta_c] - b[nonzero_delta_c]) / delta_c[nonzero_delta_c],
                                        np.where(c_max[nonzero_delta_c] == g[nonzero_delta_c],
                                                (b[nonzero_delta_c] - r[nonzero_delta_c]) / delta_c[nonzero_delta_c] + 2.0,
                                                (r[nonzero_delta_c] - g[nonzero_delta_c]) / delta_c[nonzero_delta_c] + 4.0
                                        )                                 
    )
    h[nonzero_delta_c] = (h_values[nonzero_delta_c] / 6.0) % 1.0

    s = np.zeros_like(c_max, dtype=float)
    nonzero_c_max = c_max != 0
    s[nonzero_c_max] = delta_c[nonzero_c_max] / c_max[nonzero_c_max]
    v = c_max
    return np.stack((255*h, 255*s, 255*v), axis=2).astype(int)

def calculate_3d_histogram(image, interval):
    num_bins = (256//interval)
    image = rgb_to_hsv(image)
    red_indices = (image[:, :, 0] // interval).clip(0, num_bins - 1)
    green_indices = (image[:, :, 1] // interval).clip(0, num_bins - 1)
    blue_indices = (image[:, :, 2] // interval).clip(0, num_bins - 1)

    histogram3d = np.zeros((num_bins, num_bins, num_bins), dtype=int)
    np.add.at(histogram3d, (red_indices, green_indices, blue_indices), 1)

    normalized_histogram3d = histogram3d / np.sum(histogram3d)
    return normalized_histogram3d

def calculate_query_set(query_set_number,input_interval, grid_size):
    query_set_histograms = []
    for test_instance in instance_names:
        test_filename = 'query_' +str(query_set_number)+'/'+ test_instance
        test_image = cv2.imread(test_filename)
        test_image = cv2.cvtColor(test_image, cv2.COLOR_BGR2RGB)
        histagrams = compute_histograms_in_grid(test_image, input_interval, grid_size)
        query_set_histograms.append(histagrams)
    return np.array(query_set_histograms)

def per_channael(input_interval,query_set_histograms,grid_size):
    correct_answers=0
    
    for instance in instance_names:
        filename = 'support_96/' + instance
        image = cv2.imread(filename)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        histograms=compute_histograms_in_grid(image, input_interval, grid_size)
        max_similarity_value=-1
        max_similarity_instance=''

        for j in range(len(instance_names)):
            similarity = 0
            for k in range(grid_size**2):
                test_histogram = query_set_histograms[j][k]
                histogram = histograms[k]
                similarity += np.sum(np.minimum(histogram,test_histogram))
            if(similarity>max_similarity_value):
                max_similarity_value = similarity
                max_similarity_instance = instance_names[j]

        if(instance == max_similarity_instance):
            correct_answers +=1
    return correct_answers/len(instance_names)


def compute_histograms_in_grid(image, input_interval, grid_size):
    cell_size = image.shape[0] // grid_size
    histograms = []
    for i in range(grid_size):
        for j in range(grid_size):
            start_x = i * cell_size
            end_x = (i + 1) * cell_size
            start_y = j * cell_size
            end_y = (j + 1) * cell_size
            cell = image[start_x:end_x, start_y:end_y]
            hist = calculate_3d_histogram(cell, input_interval)    
            histograms.append(hist)
    return histograms


print("3d hsv color histogram with interval 64")
best_3d_interval = 64
grid_sizes = [2,4,6,8]
for set_num in range(3):
    query_set = set_num + 1
    print("Grid Based Feature Extraction - Query set: " + str(query_set))
    for i in grid_sizes:
        query_set_histograms = calculate_query_set(query_set,best_3d_interval,i)
        score = per_channael(best_3d_interval,query_set_histograms,i)
        print("for grid size: " + str(i) + "x" + str(i))
        print(score)











