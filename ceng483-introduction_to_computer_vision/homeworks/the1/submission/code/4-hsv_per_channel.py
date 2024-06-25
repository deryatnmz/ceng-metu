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

def per_channel_color_histogram(image, interval):
    num_bins = (256//interval)
    image = rgb_to_hsv(image)
    h_histogram = np.zeros(num_bins,dtype=int)
    s_histogram = np.zeros(num_bins,dtype=int)
    v_histogram = np.zeros(num_bins,dtype=int)
    
    h = image[:, :, 0].reshape(-1)
    s = image[:, :, 1].reshape(-1)
    v = image[:, :, 2].reshape(-1)

    h_histogram = np.bincount(h // interval, minlength=num_bins)
    s_histogram = np.bincount(s // interval, minlength=num_bins)
    v_histogram = np.bincount(v // interval, minlength=num_bins)

    return h_histogram/np.sum(h_histogram),s_histogram/np.sum(s_histogram),v_histogram/np.sum(v_histogram)

def calculate_query_set(input_interval):
    query_set_histograms = [[],[],[]]
    for i in range(3):
        for test_instance in instance_names:
            test_filename = 'query_' +str(i+1)+'/'+ test_instance
            test_image = cv2.imread(test_filename)
            test_image = cv2.cvtColor(test_image, cv2.COLOR_BGR2RGB)
            red_histogram, green_histogram, blue_histogram = per_channel_color_histogram(test_image, input_interval)
            query_set_histograms[i].append((red_histogram, green_histogram, blue_histogram))
    return np.array(query_set_histograms)

def per_channael(input_interval,query_set_histograms):
    correct_answers=0
    correct_answers_list = np.array([0,0,0])
    for instance in instance_names:
        correct_answers=0
        filename = 'support_96/' + instance
        image = cv2.imread(filename)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        red_histogram, green_histogram, blue_histogram = per_channel_color_histogram(image,input_interval)
        for i in range(3):
            max_similarity_value=-1
            max_similarity_instance=''
            for j in range(len(instance_names)):
                test_red_histogram, test_green_histogram, test_blue_histogram = query_set_histograms[i][j]
                red_similarity = np.sum(np.minimum(red_histogram,test_red_histogram))
                green_similarity = np.sum(np.minimum(green_histogram,test_green_histogram))
                blue_similarity = np.sum(np.minimum(blue_histogram,test_blue_histogram))
                
                average_similarity = (red_similarity + green_similarity + blue_similarity)/3.0
                if(average_similarity>max_similarity_value):
                    max_similarity_value = average_similarity
                    max_similarity_instance = instance_names[j]

            if(instance == max_similarity_instance):
                correct_answers_list[i] +=1
                correct_answers +=1
    return correct_answers_list



# hsv per channel color histogram for intervals: [128,64,32,16,8]
print("Per-Channel Color Histogram (HSV)")
print("each triple represents top-1 accuracy scores based on query sets 1, 2, and 3")

for i in intervals:
    query_set_histograms = calculate_query_set(i)
    scores = per_channael(i,query_set_histograms)
    scores = scores /len(instance_names)
    print("interval: "+ str(i))
    print(scores)











