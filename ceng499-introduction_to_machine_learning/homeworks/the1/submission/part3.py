import torch
import numpy as np
import pickle
import matplotlib.pyplot as plt
import torch.nn as nn
import statistics as s

if torch.cuda.is_available():
    device = torch.device('cuda') #to speed up my training
else:
    device = torch.device('cpu')


x_train, y_train = pickle.load(open("data/mnist_train.data", "rb"))
x_validation, y_validation = pickle.load(open("data/mnist_validation.data", "rb"))
x_test, y_test = pickle.load(open("data/mnist_test.data", "rb"))

x_train = x_train/255.0
x_train = x_train.astype(np.float32)

x_test = x_test / 255.0
x_test = x_test.astype(np.float32)

x_validation = x_validation/255.0
x_validation = x_validation.astype(np.float32)


x_train = torch.from_numpy(x_train).to(device)
y_train = torch.from_numpy(y_train).to(torch.long).to(device)

x_validation = torch.from_numpy(x_validation).to(device)
y_validation = torch.from_numpy(y_validation).to(torch.long).to(device)

x_test = torch.from_numpy(x_test).to(device)
y_test = torch.from_numpy(y_test).to(torch.long).to(device)

# I used cross entrophy loss function
loss_function = nn.CrossEntropyLoss()
soft_max_function = torch.nn.Softmax(dim=1)

#8 models for combinations (1 hidden layer, 2 hidden layers) x (50 neurons, 500 neurons) x (sigmoid activation func, tanh activation func), their names are chosen accordingly

class One50Sigmoid(nn.Module):
    def __init__(self):
        super(One50Sigmoid,self).__init__()
        self.input_layer = nn.Linear(784, 50)
        self.hidden_layer = nn.Linear(50, 10)
        self.activation_function = nn.Sigmoid()
        print("model 1") #to make sure that right model is used
    def forward(self, x):
        hidden_layer_output = self.activation_function(self.input_layer(x))
        output_layer = self.hidden_layer(hidden_layer_output)
        return output_layer

class One50Tanh(nn.Module):
    def __init__(self):
        super(One50Tanh,self).__init__()
        self.input_layer = nn.Linear(784, 50)
        self.hidden_layer = nn.Linear(50, 10)
        self.activation_function = nn.Tanh()
        print("model 2")
    def forward(self, x):
        hidden_layer_output = self.activation_function(self.input_layer(x))
        output_layer = self.hidden_layer(hidden_layer_output)
        return output_layer
    
class One500Sigmoid(nn.Module):
    def __init__(self):
        super(One500Sigmoid,self).__init__()
        self.input_layer = nn.Linear(784, 500)
        self.hidden_layer = nn.Linear(500, 10)
        self.activation_function = nn.Sigmoid()
        print("model 3")
    def forward(self, x):
        hidden_layer_output = self.activation_function(self.input_layer(x))
        output_layer = self.hidden_layer(hidden_layer_output)
        return output_layer

class One500Tanh(nn.Module): #this is my best model
    def __init__(self):
        super(One500Tanh,self).__init__()
        self.input_layer = nn.Linear(784, 500)
        self.hidden_layer = nn.Linear(500, 10)
        self.activation_function = nn.Tanh()
        print("model 4")
    def forward(self, x):
        hidden_layer_output = self.activation_function(self.input_layer(x))
        output_layer = self.hidden_layer(hidden_layer_output)
        return output_layer

class Two50Sigmoid(nn.Module):
    def __init__(self):
        super(Two50Sigmoid,self).__init__()
        self.input_layer = nn.Linear(784, 50)
        self.hidden_layer1 = nn.Linear(50, 50)
        self.hidden_layer2 = nn.Linear(50, 10)
        self.activation_function = nn.Sigmoid()
        print("model 5")
    def forward(self, x):
        hidden1_layer_output = self.activation_function(self.input_layer(x))
        hidden2_layer_output = self.activation_function(self.hidden_layer1(hidden1_layer_output))
        output_layer = self.hidden_layer2(hidden2_layer_output)
        return output_layer

class Two50Tanh(nn.Module):
    def __init__(self):
        super(Two50Tanh,self).__init__()
        self.input_layer = nn.Linear(784, 50)
        self.hidden_layer1 = nn.Linear(50, 50)
        self.hidden_layer2 = nn.Linear(50, 10)
        self.activation_function = nn.Tanh()
        print("model 6")
    def forward(self, x):
        hidden1_layer_output = self.activation_function(self.input_layer(x))
        hidden2_layer_output = self.activation_function(self.hidden_layer1(hidden1_layer_output))
        output_layer = self.hidden_layer2(hidden2_layer_output)
        return output_layer

class Two500Sigmoid(nn.Module):
    def __init__(self):
        super(Two500Sigmoid,self).__init__()
        self.input_layer = nn.Linear(784, 500)
        self.hidden_layer1 = nn.Linear(500, 500)
        self.hidden_layer2 = nn.Linear(500, 10)
        self.activation_function = nn.Sigmoid()
        print("model 7")
    def forward(self, x):
        hidden1_layer_output = self.activation_function(self.input_layer(x))
        hidden2_layer_output = self.activation_function(self.hidden_layer1(hidden1_layer_output))
        output_layer = self.hidden_layer2(hidden2_layer_output)
        return output_layer
    
class Two500Tanh(nn.Module):
    def __init__(self):
        super(Two500Tanh,self).__init__()
        self.input_layer = nn.Linear(784, 500)
        self.hidden_layer1 = nn.Linear(500, 500)
        self.hidden_layer2 = nn.Linear(500, 10)
        self.activation_function = nn.Tanh()
        print("model 8")
    def forward(self, x):
        hidden1_layer_output = self.activation_function(self.input_layer(x))
        hidden2_layer_output = self.activation_function(self.hidden_layer1(hidden1_layer_output))
        output_layer = self.hidden_layer2(hidden2_layer_output)
        return output_layer

#accuracy canculation
def accuracy(predictions, label):
    summ= torch.sum(torch.argmax(predictions,dim=1)==label)
    num= torch.argmax(predictions,dim=1).size()[0]
    return 100*summ/num
 

def my_func(hidden_layer, neuron_num, activation_fun, learning_rate, epochs):

    if(hidden_layer==1 and activation_fun=="sigmoid" and neuron_num==50):
        model = One50Sigmoid().to(device)
    elif(hidden_layer==1 and activation_fun=="tanh" and neuron_num==50):
        model = One50Tanh().to(device)
    elif(hidden_layer==1 and activation_fun=="sigmoid" and neuron_num==500):
        model = One500Sigmoid().to(device)
    elif(hidden_layer==1 and activation_fun=="tanh" and neuron_num==500):
        model = One500Tanh().to(device)  
    elif(hidden_layer==2 and activation_fun=="sigmoid" and neuron_num==50):
        model = Two50Sigmoid().to(device)
    elif(hidden_layer==2 and activation_fun=="tanh" and neuron_num==50):
        model = Two50Tanh().to(device)
    elif(hidden_layer==2 and activation_fun=="sigmoid" and neuron_num==500):
        model = Two500Sigmoid().to(device)
    elif(hidden_layer==2 and activation_fun=="tanh" and neuron_num==500):
        model = Two500Tanh().to(device)
        
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)        
    for iteration in range(1, epochs+1):
        optimizer.zero_grad()
#        train_predictions = model(x_train) #this dataset is used for training during hyperparameter tuning
#        train_mean_crossentropy_loss = loss_function(train_predictions, y_train) # this label set is used for loss function during the hyperparameter tuning
        train_predictions = model(torch.cat((x_train,x_validation))) #this dataset is used for training the best hyperparameter settings 
        train_mean_crossentropy_loss = loss_function(train_predictions, torch.cat((y_train,y_validation))) # this label set is used for loss function with best hyperparameter setting
        train_mean_crossentropy_loss.backward()
        optimizer.step()
    with torch.no_grad():
#        test_predictions = model(x_validation) #this dataset is used for testing during the hyperparameter tuning
#        test_accuracy = accuracy(soft_max_function(test_predictions),y_validation) #this dataset is used for calculating accuracy score during the hyperparameter tuning
        test_predictions = model(x_test) #this dataset is used for testing of the best hyperparameter setting
        test_accuracy = accuracy(soft_max_function(test_predictions),y_test) #this dataset is used for calculating accuracy score after best hyperparameter combination is set
        print("Accuracy : %.2f" % (test_accuracy.item())) #to see the accuracy of each epochs 
    return test_accuracy


accuracies = []
for i in range(10): # 10 times run for each hyperparameter combination
    x= my_func(1, 500, "tanh", 0.1, 1500) # This is my final/ best hyperparameter configuration, I changed these paramaters during the hyperparameter search according to my grid search table
    accuracies.append(x.item())
    
mean=s.mean(accuracies) # average of accuracies
std=s.stdev(accuracies) # standard deviation of the accuracies
c=1.96*std/(10**0.5) # for confidence interval
print ("average accuracy:")
print(mean)
print("confidence iterval:")
print((mean-c, mean+c))
