import os
import numpy as np
import tensorflow as tf
from tflite_model_maker import model_spec
from tflite_model_maker import audio_classifier
from tflite_model_maker.config import ExportFormat

# Load data
data_dir = 'C:\\Users\\kenbr\\OneDrive\\Documents\\Arduino\\Broden_Lab9_3412\\data'
data = audio_classifier.DataLoader.from_folder(data_dir)

# Split data into training and testing
train_data, test_data = data.split(0.8)

# Define model
spec = model_spec.get('yamnet')
model = audio_classifier.create(train_data, model_spec=spec)

# Evaluate model
loss, accuracy = model.evaluate(test_data)
print(f'Test accuracy: {accuracy:.2f}')

# Export the model
model.export(export_dir='C:\\Users\\kenbr\\OneDrive\\Documents\\Arduino\\Broden_Lab9_3412\\Broden_YesNoMonitor')