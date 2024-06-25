import torch
import numpy as np
import pickle
import matplotlib.pyplot as plt


def forward_pass(w1, b1, w2, b2, input_data):
    product1 = torch.matmul(input_data, w1)
    output1 = product1 + b1
    product2 =torch.matmul(torch.sigmoid(output1),w2)
    output2 = product2 + b2
    return torch.softmax(output2,dim=1)

train_dataset, train_label = pickle.load(open("data/part2_classification_train.data", "rb"))
validation_dataset, validation_label = pickle.load(open("data/part2_classification_validation.data", "rb"))
test_dataset, test_label = pickle.load(open("data/part2_classification_test.data", "rb"))

indices = list(range(len(train_dataset)))
np.random.shuffle(indices)
train_dataset = np.array([train_dataset[i] for i in indices], dtype=np.float32)
train_label = np.array([train_label[i] for i in indices], dtype=np.float32)

train_dataset = torch.from_numpy(train_dataset)
train_label = torch.from_numpy(train_label)

validation_dataset = torch.from_numpy(validation_dataset)
validation_label = torch.from_numpy(validation_label)

test_dataset = torch.from_numpy(test_dataset)
test_label = torch.from_numpy(test_label)


w1= torch.from_numpy(np.random.normal(0, 1, 48).astype(np.float32).reshape((3,16))).requires_grad_(True)
b1= torch.from_numpy(np.random.normal(0, 1, 16).astype(np.float32).reshape((1,16))).requires_grad_(True)
w2= torch.from_numpy(np.random.normal(0, 1, 48).astype(np.float32).reshape((16,3))).requires_grad_(True)
b2= torch.from_numpy(np.random.normal(0, 1, 3).astype(np.float32).reshape((1,3))).requires_grad_(True)

iteration_array = []
train_loss_array = []
validation_loss_array = []

optimizer = torch.optim.SGD([w1, b1, w2, b2], lr=0.001)

def loss_function(predictions,label):
    ll=-torch.log(predictions)
    kk=torch.mul(ll,label)
    num = label.size()[0]
    summ = torch.sum(kk)
    return summ/num

def accuracy(predictions, label):
    summ= torch.sum(torch.argmax(predictions,dim=1)==torch.argmax(label,dim=1))
    num= torch.argmax(predictions,dim=1).size()[0]
    return 100*summ/num

ITERATION = 15000
for iteration in range(1, ITERATION+1):
    iteration_array.append(iteration)
    optimizer.zero_grad()
    train_predictions = forward_pass(w1, b1, w2, b2, train_dataset)
    train_mean_crossentropy_loss = loss_function(train_predictions, train_label)
    train_loss_array.append(train_mean_crossentropy_loss.item())
    train_mean_crossentropy_loss.backward()
    optimizer.step()
    with torch.no_grad():
        train_accuracy =accuracy(train_predictions,train_label)
        validation_predictions = forward_pass(w1, b1, w2, b2, validation_dataset)
        validation_mean_crossentropy_loss = loss_function(validation_predictions, validation_label.to(torch.double))
        validation_loss_array.append(validation_mean_crossentropy_loss.item())
        validation_accuracy =accuracy(validation_predictions,validation_label)
    print("Iteration : %d - Train Loss %.4f - Train Accuracy : %.2f - Validation Loss : %.4f Validation Accuracy : %.2f" % (iteration+1, train_mean_crossentropy_loss.item(), train_accuracy, validation_mean_crossentropy_loss.item(), validation_accuracy))

with torch.no_grad():
    test_predictions = forward_pass(w1, b1, w2, b2, test_dataset)
    test_accuracy=accuracy(test_predictions,test_label)
    print("Test accuracy : %.2f" % (test_accuracy.item()))

plt.plot(iteration_array, train_loss_array, label="Train Loss")
plt.plot(iteration_array, validation_loss_array, label="Validation Loss")
plt.legend()
plt.show()





