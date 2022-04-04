### Define Dense Model ###

from keras.models import Sequential
from keras.optimizers import Adam, SGD
from keras.initializers import TruncatedNormal
#from keras import initializations
from keras.layers import Input, Dense, Dropout, Flatten, Conv2D, MaxPooling2D, Reshape, BatchNormalization
#from keras.callbacks import ReduceLROnPlateau

# Define initialization
#def normal(shape, name=None):
#    return initializations.normal(shape, scale=0.05, name=name)

model = Sequential()

model.add(Dense(64, activation='tanh',input_dim=7))
model.add(BatchNormalization() )
#model.add(Dropout(0.2))
model.add(Dense(64, activation='tanh'))
model.add(BatchNormalization() )
#model.add(Dropout(0.2))
model.add(Dense(64, activation='tanh'))
model.add(BatchNormalization() )
model.add(Dense(64, activation='tanh'))
model.add(BatchNormalization() )
model.add(Dense(2, activation='softmax'))
model.compile(loss='binary_crossentropy', optimizer=Adam(lr=0.001), metrics=['accuracy'])
#model.compile(loss='binary_crossentropy', optimizer=SGD(lr=0.01), metrics=['accuracy',])
model.save('model_dense.h5')
model.summary()
