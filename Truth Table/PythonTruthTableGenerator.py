#Dennis Canar
#PyhtonTruthTableGenerator.py
#9-26-17
#CST-215 Luke Kanuchok
#This code gets the end result of a logical expression for all possible truth values.


from itertools import *
operators = ['->', '^', 'v', '(', ')', '~']
varList = []
Value = [True, False]
dataSet=[]

#Instructions for the calculator
print("Welcome to the True & False Table Calculator\nPlease enter a string using variables such as '( X ^ Y ) v Z' ")
print("Use the following operators when typing your string:\n[or = v, and = ^, then = ->, parenthesis = ( ), not = ~] ")
print("If a character entered isn't an operator it will be taken as a variable")
print("when typing your string enter a single space in between each character")
print("and use all uppercase letters or all lowercase letters")
problem = input('Enter String:')


#Here we split the input into a list and remove space from the left and right ends
problem = problem.strip()
problem = problem.split()

print('String',problem)


#Find the variables and put them in their own list
def findVar(problem):
    for letter in problem:
        if letter not in operators and letter not in varList:
            varList.append(letter)
findVar(problem)
print('Variables:',varList)

#Create Data for our truth table according to what the length of our variable list is
data = (product(Value, repeat=(len(varList))))
data=(list(data))
#data.insert(0,varList)
print("Data:",data)



#here is where we will simplify our problem depending on the true and false values and if the not operation is used
def notSimplify(temp):
    i = 0
    change =False#If no change is made the loop goes to the next function
    while i < (len(temp)):
        if temp[i] == '~': #if the not is found then it will look at what the values around it are
            if temp[i + 1] == True:
                temp[i] = False#if the value in front of not is true it will change it to false
                del temp[i + 1]
                change=True
            elif temp[i + 1] == False:
                temp[i] = True#if the value in front of not is false it will change it to True
                del temp[i + 1]
                change=True#True is here so that if we make a change the loop runs until there is no change
        i += 1
    return temp,change#Gives us two values the temp list and if change is true or false

#Here we define the simplification of a string if the operator and is used
def andSimplify(temp):
    i=0
    change=False
    while i<(len(temp)):
        if temp[i]=='^':#if the and operator is found we check the items around it in the list
            if temp[i+1]==True:
                if temp[i-1]==True:#if true are behind and in front of the and
                    temp[i]=True
                    del temp[i+1]#changes i to true and deletes items around it so we just have True
                    del temp[i-1]
                    change=True
                elif temp[i-1]==False:#if the value before the and is false and after is true
                    temp[i] =False
                    del temp[i + 1]#changes the i to False and deletes items around it
                    del temp[i - 1]
                    change=True
            elif temp[i+1]==False:
                if temp[i-1]==False:#if value before and after and is false
                    temp[i] = False
                    del temp[i + 1]#changes i to false and deletes items around it
                    del temp[i - 1]
                    change=True
                if temp[i-1]==True:#if value before i is false and after it is true
                    temp[i] = False
                    del temp[i + 1]#changes i to false and deletes items around it
                    del temp[i - 1]
                    change = True
        i+=1
    return temp,change#mentioned in previous function

#Here we define the simplification of a string if the operator or is used
def orSimplify(temp):
        i=0
        change=False
        while i<(len(temp)):
            if temp[i]=='v':#if the or operator is found in the temp list we check values around it
                if temp[i+1]==True:
                    if temp[i-1]==True:#if true is before and after the and operator
                        temp[i]=True
                        del temp[i+1]#changes the i to true and deletes elements next to it
                        del temp[i-1]
                        change=True
                    elif temp[i-1]==False:#if value before the or is false and after the or is true
                        temp[i]=True
                        del temp[i+1]#changes i into true and deletes the surrounding items
                        del temp[i-1]
                        change=True
                elif temp[i+1]==False:
                    if temp[i-1]==True:#if false is before and after the or operator
                        temp[i] = True
                        del temp[i + 1]#changes i to true and deletes the items before and after it
                        del temp[i - 1]
                        change=True
                    elif temp[i-1]==False:#if values before and after the or are false
                        temp[i]=False
                        del temp[i+1]#changes i to false and deltes the items around it
                        del temp[i-1]
                        change=True
            i+=1
        return temp,change#previously mentioned

#Here we define the simplification of a string if the operator implies is used
def impliesSimplify(temp):
    i=0
    change = False
    while i<(len(temp)):
        if temp[i]=='->':#Finds the implies in the temp list
            if temp[i-1]==True:
                if temp[i+1]==True:#if there is a true on both sides of the implies operator
                    temp[i]=True
                    del temp[i+1]#changes i to true and deletes the items before and after it
                    del temp[i-1]
                    change = True
                elif temp[i+1]==False:#if there is a true before the implies and a false after the
                    temp[i]=False
                    del temp[i+1]#change i to false and delete the items before and after it
                    del temp[i-1]
                    change=True
            elif temp[i-1]==False and temp[i+1] in [False,True]:
                #If the value before the -> in false and the next value is either true of false
                temp[i]=True
                del temp[i + 1]#change i to true and delete the items before and after it
                del temp[i - 1]
                change=True
        i+=1
    return temp,change#mentioned in the not simplify function

#Here we define the simplification of a string if the operator left parenthesis or right parenthesis is used
#this function will only get rid of a set of parenthesis if there is one true or false in between them
def parenthesisSimplify(temp):
    i = 0
    change=False
    while i < (len(temp)):
        if temp[i] == '(':
            if temp[i + 2] == ')':
                if temp[i + 1] == False:
                    del temp[i + 2]
                    del temp[i]
                    change =True
                elif temp[i+1]==True:
                    del temp[i+2]
                    del temp[i]
                    change = True
        i += 1
    return temp,change

#Here we created a loop call a simplify function until a change cannot be made. If one can't be made then we move on to
#the next simplification function. The whole while loop will run until we get our answer of one boolean value.
def simplify(temp):
    while len(temp)>1:
        temp,change=notSimplify(temp)
        if change:
            continue
        temp,change=andSimplify(temp)
        if change:
            continue
        temp,change=orSimplify(temp)
        if change:
            continue
        temp,change=impliesSimplify(temp)
        if change:
            continue
        temp,change=parenthesisSimplify(temp)
        if change:
            continue
    return temp[0]


#Data Headings are created and printed here
print(" | ".join(varList),end=' ')
print("| "," ".join(problem),)
heading= varList+problem
heading= (" | ".join(heading))
print('-' * len(heading))

#This chunk of code is a big loop that gets our data and prints them out as a table
#The first part of this code goes through the temp list and checks if it is a variable
#if the character is a variable it is set equal to the data set value (true or false)
for dataSet in data:
    temp = problem[:]
    for i in range(len(temp)):
        ch = temp[i]
        for j in range(len(varList)):
            if ch == varList[j]:
                temp[i] = dataSet[j]
#This part of the loop takes the temp list with the boolean values instead of the variables
#and uses the simplify function (which is all the simplification functions together) then
#puts the answer in a list called result and adds that to the data
    result=simplify(temp)
    dataSet=list(dataSet)
    dataSet.append(result)
    output=[]
#this is where a new list called output is created and it adds a T or F for the boolean value
#in data set then it prints it out with a seperation using .join
    for val in dataSet:
        if val == True:
            output.append('T')
        if val == False:
            output.append('F')
    print(" | ".join(output))

print("\nThe last column is the final boolean value once all the simplifications are made.(answer)")