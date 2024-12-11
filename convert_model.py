import tensorflow as tf

# Load the trained TensorFlow Lite model
model_path = 'yes_no_model.tflite'
model = open(model_path, 'rb').read()

# Convert the model to a C array
c_array = ','.join(['0x%02x' % b for b in model])

# Write the C array to a header file
with open('yes_no_model.h', 'w') as f:
    f.write(f'const unsigned char yes_no_model[] = {{{c_array}}};\n')
    f.write(f'const int yes_no_model_len = {len(model)};\n')