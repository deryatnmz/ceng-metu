import numpy as np
class HMM:
    def __init__(self, A, B, Pi):
        self.A = A
        self.B = B
        self.Pi = Pi

    def forward_log(self, O: list):
        len_O = len(O) #length of observation sequence
        len_states = self.A.shape[0] #number of states
        table= np.zeros((len_O,len_states)) #initialize the table with zeros
        c_t=0 # to avoid numerical precision limitations
        for i in range (len_O):
            for j in range(len_states):
                if(i==0): #first probabilities are computed from initial state distribution
                    table[i][j]= self.Pi[j] * self.B[j][O[0]]
                else:
                    for k in range(len_states):
                        table[i][j] += table[i-1][k] * self.A[k][j]
                    table[i][j] *= self.B[j][O[i]]
            c_t += np.log(1.0/np.sum(table[i]))
            table[i] /= np.sum(table[i])
        return(-c_t)
    


    def viterbi_log(self, O: list):
        len_O = len(O) #length of observation sequence
        len_states = self.A.shape[0] #number of states
        probability=[]
        previous=[]

        for i in range (len_O):
            temp_prev = []
            temp_prob = []
            for j in range(len_states):
                if(i==0):
                    temp_prev.append(None) #Prev for first ones are None
                    temp_prob.append(np.log(self.Pi[j]) + np.log(self.B[j, O[0]])) #for the fist step, initial (B) is taken
                else:
                    max_prob = -100000
                    max_prev = -1
                    #fiding max probable next state
                    for prev in range(len_states):
                        prob = probability[i-1][prev] + np.log(self.A[prev,j])
                        if prob > max_prob:
                            max_prob = prob
                            max_prev = prev
                    temp_prob.append(max_prob + np.log(self.B[j, O[i]]))
                    temp_prev.append(max_prev)
            previous.append(temp_prev)
            probability.append(temp_prob)
            temp_prev=[]
            temp_prob=[]
        #reversing the list to make it easy to trace back
        rev_probability = probability[::-1]
        rev_previous= previous[::-1]

        #final max probability value
        result=max(rev_probability[0])
        max_index= rev_probability[0].index(max(rev_probability[0]))
        sequence=[]
        sequence.append(max_index)
        #tracing back
        for i in range(len_O-1):
            max_index = rev_previous[i][max_index]
            sequence.append(max_index)
        #reversing for the correct order
        sequence=sequence[::-1]
        
        return (result, sequence)


    

