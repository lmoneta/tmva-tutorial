import keras

from keras.models import Sequential
from keras.optimizers import Adam, SGD
#from keras.initializers import TruncatedNormal
#from keras import initializations
from keras.layers import Input, Dense, Dropout, Flatten, Conv2D, MaxPooling2D, Reshape
#from keras.callbacks import ReduceLROnPlateau

model = Sequential()
model.add(Reshape((8,8, 1), input_shape=(64,)))
model.add(Conv2D(10, kernel_size=(3,3), kernel_initializer='glorot_normal', activation='relu', padding='same' ) )
model.add(Conv2D(10, kernel_size=(3,3), kernel_initializer='glorot_normal', activation='relu', padding='same' ) )
#stride for maxpool is equal to pool size
model.add(MaxPooling2D(pool_size=(2, 2) ))
model.add(Flatten())
model.add(Dense(64, activation='tanh'))
#model.add(Dropout(0.2))
model.add(Dense(2, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer=Adam(lr=0.001), metrics=['accuracy'])
model.save('model_cnn.h5')
model.summary()
