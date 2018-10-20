
# coding: utf-8

# <img src="http://oproject.org/tiki-download_file.php?fileId=8&display&x=450&y=128">
# <img src="http://files.oproject.org/tmvalogo.png" height="50%" width="50%">
# 
# # TMVA Classification Example Using a Convolutional Neural Network

# ## Declare Factory
# 
# Create the Factory class. Later you can choose the methods
# whose performance you'd like to investigate. 
# 
# The factory is the major TMVA object you have to interact with. Here is the list of parameters you need to pass
# 
#  - The first argument is the base of the name of all the output
# weightfiles in the directory weight/ that will be created with the 
# method parameters 
# 
#  - The second argument is the output file for the training results
#   
#  - The third argument is a string option defining some general configuration for the TMVA session. For example all TMVA output can be suppressed by removing the "!" (not) in front of the "Silent" argument in the option string

# In[1]:

import ROOT
from ROOT import TMVA
import os 


# In[2]:

ROOT.TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()

## For PYMVA methods
TMVA.PyMethodBase.PyInitialize();


outputFile = ROOT.TFile.Open("CaloImages_ClassificationOutput.root", "RECREATE")

factory = ROOT.TMVA.Factory("TMVA_CaloImages_Classification", outputFile,
                      "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification:!Correlations" )


# ## Declare DataLoader(s)
# 
# The next step is to declare the DataLoader class that deals with input variables 
# 
# Define the input variables that shall be used for the MVA training
# note that you may also use variable expressions, which can be parsed by TTree::Draw( "expression" )]
# 
# In this case the input data consists of an image of 16x16 pixels. Each single pixel is a branch in a ROOT TTree

# In[3]:

inputFileName = "CaloImages_data.root"

##inputFile = ROOT.TFile.Open( inputFileName )
###ROOT.TFile.SetCacheFileDir(".");
#inputWebFileName = "http://www.cern.ch/moneta/root/" + inputFileName
inputWebFileName = inputFileName
inputFile = ROOT.TFile.Open(inputWebFileName)
inputFile.ls()



# In[ ]:

# retrieve input trees

signalTree     = inputFile.Get("sig_tree")
backgroundTree = inputFile.Get("bkg_tree")

signalTree.Print()


# In[ ]:

loader = ROOT.TMVA.DataLoader("dataset")

### global event weights per tree (see below for setting event-wise weights)
signalWeight     = 1.0
backgroundWeight = 1.0
   
### You can add an arbitrary number of signal or background trees
loader.AddSignalTree    ( signalTree,     signalWeight     )
loader.AddBackgroundTree( backgroundTree, backgroundWeight )

imgSize = 32 * 32; 
for  i in range(0,imgSize):
    varName = "Energy"+str(i)
    loader.AddVariable(varName,'F');


# ## Setup Dataset(s)
# 
# Define input data file and signal and background trees

# In[ ]:

## Apply additional cuts on the signal and background samples (can be different)
mycuts = ROOT.TCut("")   ## for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
mycutb = ROOT.TCut("")   ## for example: TCut mycutb = "abs(var1)<0.5";


loader.PrepareTrainingAndTestTree( mycuts, mycutb,
                                  "nTrain_Signal=5000:nTrain_Background=5000:SplitMode=Random:"
                                   "NormMode=NumEvents:!V" )


# # Booking Methods
# 
# Here we book the TMVA methods. We book a DNN and a CNN

# #### Booking Deep Neural Network
# 
# Here we book the new DNN of TMVA. If using master version you can use the new DL method

# In[ ]:

inputLayoutString = "InputLayout=1|1|1024"; 
batchLayoutString= "BatchLayout=1|32|1024";
layoutString = ("Layout=DENSE|64|TANH,DENSE|64|TANH,DENSE|64|TANH,DENSE|64|TANH,DENSE|1|LINEAR")

training1  = "Optimizer=ADAM,LearningRate=1e-3,Momentum=0.,Regularization=None,WeightDecay=1e-4,"
training1 += "DropConfig=0.+0.+0.+0.,MaxEpochs=30,ConvergenceSteps=10,BatchSize=32,TestRepetitions=1"
trainingStrategyString = "TrainingStrategy=" + training1


dnnOptions = "!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=None:WeightInitialization=XAVIER::Architecture=CPU"

dnnOptions +=  ":" + inputLayoutString
dnnOptions +=  ":" + batchLayoutString
dnnOptions +=  ":" + layoutString
dnnOptions +=  ":" + trainingStrategyString

#we can now book the method
              
factory.BookMethod(loader, ROOT.TMVA.Types.kDL, "DL_DENSE", dnnOptions)


# ### Book Convolutional Neural Network in TMVA

# In[ ]:

#input layout 
inputLayoutString = "InputLayout=1|32|32"
                                                                                                
## Batch Layout                                                                                                                                     
batchLayoutString = "BatchLayout=32|1|1024"
                                                   

layoutString = ("Layout=CONV|10|3|3|1|1|1|1|RELU,CONV|10|3|3|1|1|1|1|RELU,MAXPOOL|2|2|1|1,"
            "RESHAPE|FLAT,DENSE|64|TANH,DENSE|1|LINEAR")
                                                                                                                                              


##Training strategies.                                                                                                                          
training1 = ("LearningRate=1e-3,Momentum=0.9,Repetitions=1,"
                     "ConvergenceSteps=10,BatchSize=32,TestRepetitions=1,"
                     "MaxEpochs=20,WeightDecay=1e-4,Regularization=None,"
                     "Optimizer=ADAM,DropConfig=0.0+0.0+0.0+0.0")

trainingStrategyString = "TrainingStrategy=" + training1
    
## General Options.                                                                                                                              
cnnOptions = ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=None:"
                       "WeightInitialization=XAVIERUNIFORM");

cnnOptions +=  ":" + inputLayoutString
cnnOptions +=  ":" + batchLayoutString
cnnOptions +=  ":" + layoutString
cnnOptions +=  ":" + trainingStrategyString
cnnOptions +=  ":Architecture=CPU"

##book CNN
factory.BookMethod(loader, ROOT.TMVA.Types.kDL, "DL_CNN", cnnOptions);


# ### Book Convolutional Neural Network in Keras using a generated model 

# In[ ]:

## to use tensorflow backend
import os
os.environ["KERAS_BACKEND"] = "tensorflow"


# In[ ]:

from keras.models import Sequential
from keras.optimizers import Adam, SGD
#from keras.initializers import TruncatedNormal
#from keras import initializations
from keras.layers import Input, Dense, Dropout, Flatten, Conv2D, MaxPooling2D, Reshape
#from keras.callbacks import ReduceLROnPlateau


# In[ ]:

model = Sequential()
model.add(Reshape((32,32, 1), input_shape=(1024,)))
model.add(Conv2D(10, kernel_size=(3,3), kernel_initializer='glorot_normal', activation='relu', padding='same' ) )
model.add(Conv2D(10, kernel_size=(3,3), kernel_initializer='glorot_normal', activation='relu', padding='same' ) )
#stride for maxpool is equal to pool size
model.add(MaxPooling2D(pool_size=(2, 2) ))
#model.add(Conv2D(10, activation='relu', kernel_size=(3,3), padding='same', kernel_initializer='glorot_normal'))
#model.add(Conv2D(10, activation='relu', kernel_size=(3,3), padding='same', kernel_initializer='glorot_normal'))
#model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(64, activation='relu'))
#model.add(Dropout(0.2))
model.add(Dense(2, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer=Adam(lr=0.001), metrics=['accuracy'])
model.save('model_cnn.h5')
model.summary()


# In[ ]:

kerasOption = ("H:!V:VarTransform=None:FilenameModel=model_cnn.h5:"
               "FilenameTrainedModel=trained_model_cnn.h5:NumEpochs=20:BatchSize=32")
factory.BookMethod(loader, ROOT.TMVA.Types.kPyKeras,"PyKeras",kerasOption)


# ## Train Methods

# In[ ]:

##retrieve data before to remove output during data retrieval
ROOT.TMVA.MsgLogger.InhibitOutput()
loader.GetDefaultDataSetInfo().GetDataSet()
ROOT.TMVA.MsgLogger.EnableOutput()


# In[ ]:

factory.TrainAllMethods()


# ## Test and Evaluate Methods

# In[ ]:

factory.TestAllMethods()


# In[ ]:

ROOT.TMVA.MsgLogger.InhibitOutput()
factory.EvaluateAllMethods()
ROOT.TMVA.MsgLogger.EnableOutput()


# ## Plot ROC Curve
# We enable JavaScript visualisation for the plots

# In[ ]:

###(/%jsroot, on)


# In[ ]:

#c1 = factory.GetROCCurve(loader);
#c1->Draw();


# In[ ]:

##(/, close, outputfile, to, save, output, file)
##outputFile->Close();

