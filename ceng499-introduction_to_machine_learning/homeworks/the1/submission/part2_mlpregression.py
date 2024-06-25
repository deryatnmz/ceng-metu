import torch
import numpy as np
import pickle
import matplotlib.pyplot as plt

def forward_pass(w1, b1, w2, b2, input_data):
    product1 = torch.matmul(input_data, w1)
    output1 = product1 + b1
    product2 =torch.matmul(torch.sigmoid(output1),w2)
    output2 = product2 + b2
    return output2


train_dataset, train_label = pickle.load(open("data/part2_regression_train.data", "rb"))
validation_dataset, validation_label = pickle.load(open("data/part2_regression_validation.data", "rb"))
test_dataset, test_label = pickle.load(open("data/part2_regression_test.data", "rb"))

train_dataset = torch.from_numpy(train_dataset)
train_label = torch.from_numpy(train_label)

validation_dataset = torch.from_numpy(validation_dataset)
validation_label = torch.from_numpy(validation_label)

test_dataset = torch.from_numpy(test_dataset)
test_label = torch.from_numpy(test_label)

w1= torch.from_numpy(np.random.normal(0, 1,64).astype(np.float32).reshape((2,32))).requires_grad_(True)
b1= torch.from_numpy(np.random.normal(0, 1, 32).astype(np.float32).reshape((1,32))).requires_grad_(True)
w2= torch.from_numpy(np.random.normal(0, 1, 32).astype(np.float32).reshape((32,1))).requires_grad_(True)
b2= torch.from_numpy(np.random.normal(0, 1, 1).astype(np.float32).reshape((1,1))).requires_grad_(True)


iteration_array = []
train_loss_array = []
validation_loss_array = []

optimizer = torch.optim.SGD([w1, b1, w2, b2], lr=0.0001)

def loss_function(prediction,label):
    summ=torch.sum(torch.square(torch.flatten(prediction) - label))
    num=label.size()[0]
    return summ/num
    

ITERATION = 1500
for iteration in range( ITERATION+1):
    iteration_array.append(iteration+1)
    optimizer.zero_grad() 
    train_predictions = forward_pass(w1, b1, w2, b2, train_dataset)
    train_mse_loss = loss_function(torch.flatten(train_predictions), train_label)
    train_loss_array.append(train_mse_loss.item())
    train_mse_loss.backward()
    optimizer.step()
    with torch.no_grad():
        validation_predictions = forward_pass(w1, b1, w2, b2, validation_dataset)
        validation_mse_loss = loss_function(torch.flatten(validation_predictions), validation_label)
        validation_loss_array.append(validation_mse_loss.item())
    print("Iteration : %d - Train MSE Loss %.4f - Validation MSE Loss : %.2f" % (iteration+1, train_mse_loss.item(), validation_mse_loss.item()))

with torch.no_grad():
    test_predictions = forward_pass(w1, b1, w2, b2, test_dataset)
    test_loss = loss_function(torch.flatten(test_predictions), test_label)
    print("Test MSE loss : %.4f" % test_loss.item())

plt.plot(iteration_array, train_loss_array, label="Train Loss")
plt.plot(iteration_array, validation_loss_array, label="Validation Loss")
plt.legend()
plt.show()





