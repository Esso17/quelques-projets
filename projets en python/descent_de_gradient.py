
import os
import numpy as np
from sklearn.datasets import make_blobs
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt

X,Y=make_blobs(n_samples=600,centers=2,n_features=2,random_state=0)
Y=Y.reshape(Y.shape[0],1)
plt.scatter(X[:,0],X[:,1],c=Y)
plt.show()

def initialisation(X):
    w=np.random.randn(X.shape[1],1)
    b=np.random.randn(1)
    return (w,b)

w,b=initialisation(X)

def model(X,w,b):
    Z = X.dot(w)+b
    A = 1./(1+np.exp(-Z))
    return A
A=model(X,w,b)

def log_loss(A,Y):
    loss = np.sum(-Y*np.log(A)-(1-Y)*np.log(1-A))
    return 1./len(Y)*loss

logloss=log_loss(A,Y)

def gradients(A,X,Y):
    dw=1/len(Y)*(X.T)@(A-Y)
    db=1/len(Y)*np.sum(A-Y)
    return (dw,db)


dw,db=gradients(A,X,Y)


def update(dw,db,w,b,learning_rate):
    w-=learning_rate*dw
    b-=learning_rate*db
    return (w,b)
def predict(X,w,b):
    A=model(X,w,b)
    return A>=0.5

def artificial_neurone(X,Y,learning_rate=0.2,max_iter=100):
    w, b = initialisation(X)
    i=0
    logloss=list()
    while  i<max_iter:
        A=model(X,w,b)
        logloss.append(log_loss(A,Y))
        dw, db = gradients(A, X, Y)
        w,b=update(dw, db, w, b, learning_rate)
        i+=1
    y_predict=predict(X,w,b)
    print("la performance du modèle est:",accuracy_score(y_predict,Y))
    plt.plot(logloss)
    plt.title("Evolution de la perte en fonction du nombre d'iterations")
    plt.show()
    return (w,b,A)

if __name__=='__main__':
    w,b,a=artificial_neurone(X,Y,learning_rate=0.2,max_iter=50)
    print("la valeur de w est :",w)
    print("la valeur de b est : ",b)




#putain

