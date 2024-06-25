import numpy as np
import math 

class TreeNode:
    def __init__(self, attribute):
        self.attribute = attribute
        self.subtrees = {}

class TreeLeafNode:
    def __init__(self, data, label):
        self.data = data
        self.labels = label

class DecisionTree:
    def __init__(self, dataset: list, labels, features, criterion="information gain"):
        self.dataset = dataset
        self.labels = labels
        self.features = features
        self.criterion = criterion
        self.root = None

    def calculate_entropy__(self, dataset, labels):
        values=[]
        accurances=[]
        summ=len(labels)
        if(summ==0):
            return 0
        for i in labels:
            if (i not in values):
                values.append(i)
        for i in values:
            accurances.append(labels.count(i))
        entropy_value = 0.0
        for i in accurances:
            entropy_value += (-i/summ)*(math.log2(i/summ))
        return entropy_value

    def calculate_average_entropy__(self, dataset, labels, attribute):
        values=[]
        for i in labels:
            if (i not in values):
                values.append(i)
        num_of_labels=len(values)
        
        index = self.features.index(attribute)
        properties=[]
        prop_label=[]
        for i in dataset:
            if(i[index] not in properties):
                properties.append(i[index])
                prop_label.append(([0] * num_of_labels))
        for prop in properties:
            for i in range(len(dataset)):
                if(dataset[i][index]==prop):
                    lab=labels[i]
                    ind=values.index(lab)
                    prop_label[properties.index(prop)][ind]+=1
        average_entropy = 0.0
        for j in prop_label:
            ent=0
            summ = sum(j)
            if(summ==0):
                ent=0
            else:
                for i in j:
                    if(i==0):
                        ent=0
                    else:
                        ent+= (-i/summ)*(math.log2(i/summ))*sum(j)
            average_entropy += ent
        average_entropy/=len(labels)
        return average_entropy

    def calculate_information_gain__(self, dataset, labels, attribute):
        entropy_value=self.calculate_entropy__(dataset, labels)
        average_entropy=self.calculate_average_entropy__(dataset, labels, attribute)
        information_gain = entropy_value-average_entropy
        return information_gain

    def calculate_intrinsic_information__(self, dataset, labels, attribute):
        values=[]
        for i in labels:
            if (i not in values):
                values.append(i)
        num_of_labels=len(values)
        
        index = self.features.index(attribute)
        properties=[]
        prop_label=[]
        for i in dataset:
            if(i[index] not in properties):
                properties.append(i[index])
                prop_label.append(0)
        for prop in properties:
            for i in range(len(dataset)):
                if(dataset[i][index]==prop):
                    prop_label[properties.index(prop)]+=1
        
        intrinsic_info = 0.0
        summ=sum(prop_label)
        for i in prop_label:
            intrinsic_info += -(i/summ)*math.log2(i/summ)
        return intrinsic_info


    
    def calculate_gain_ratio__(self, dataset, labels, attribute):
        gain= self.calculate_information_gain__(dataset, labels, attribute)
        intrinsic_info=self.calculate_intrinsic_information__(dataset, labels, attribute)
        gain_ratio = gain/intrinsic_info
        return gain_ratio


    def ID3__(self, dataset, labels, used_attributes):
        #calculating max information gain or gain ratio respectively
        max_gain=-1
        for i in self.features:
            if (i not in used_attributes):
                if (self.criterion=="information gain"):
                    gain= self.calculate_information_gain__(dataset, labels, i)
                else:
                    gain = self.calculate_gain_ratio__(dataset, labels, i)
                if(gain>max_gain):
                    max_gain=gain
                    max_attribute =i
        #creating node to return for this attribute
        node= TreeNode(max_attribute)
        used_attributes.append(max_attribute)
        #determining how many labels and what are they
        values=[]
        for i in labels:
            if (i not in values):
                values.append(i)
        num_of_labels=len(values)
        index = self.features.index(max_attribute)   
        #for chosen attribute determining its branches and their corresponding labels
        properties=[]
        prop_label=[]
        for i in dataset:
            if(i[index] not in properties):
                properties.append(i[index])
                prop_label.append(([0] * num_of_labels))
        for prop in properties:
            for i in range(len(dataset)):
                if(dataset[i][index]==prop):
                    lab=labels[i]
                    ind=values.index(lab)
                    prop_label[properties.index(prop)][ind]+=1
        #for each branch of max_attribute do the same thing
        for i in range(len(prop_label)):
            pure=0
            att=properties[i]
            #if a branch is pure(consist of only one label) make it a leaf node
            for j in prop_label[i]:    
                if (j!=0):
                    pure+=1
                    lab=values[prop_label[i].index(j)]
            if(pure==1):
                pure=0
                leaf = TreeLeafNode(max_attribute,lab)
                node.subtrees[att] = leaf
            #if branch is not pure but all the attributes are used so no futher split can be done,
            #make it a leaf node with max number of label (I implemented this here, not in the predict part)
            elif((len(used_attributes))==len(self.features)):           
                max_lab=(values[prop_label[i].index(max(prop_label[i]))])
                leaf = TreeLeafNode(max_attribute,max_lab)
                node.subtrees[att] = leaf
            #else make recursive call for the branch attribute with its dataset and labels
            else:
                new_dataset=[]
                new_labels=[]
                for k in range(len(dataset)):
                    if (dataset[k][self.features.index(max_attribute)]==att):
                        new_dataset.append(dataset[k])
                        new_labels.append(labels[k])
                m=list(used_attributes)
                node.subtrees[att] = self.ID3__(new_dataset, new_labels, used_attributes)
                used_attributes=list(m)
            pure=0
        return node

    def predict_helper(self,x,tree):
        if(isinstance(tree, TreeNode)):
            return self.predict_helper(x,tree.subtrees[x[self.features.index(tree.attribute)]])
        else:
            return (tree.labels)
            
    def predict(self, x):
        predicted_label = self.predict_helper(x,self.root)

        return predicted_label

    def train(self):
        self.root = self.ID3__(self.dataset, self.labels, [])
        print("Training completed")
